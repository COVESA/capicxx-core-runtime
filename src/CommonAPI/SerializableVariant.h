/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_SERIALIZABLE_VARIANT_H_
#define COMMONAPI_SERIALIZABLE_VARIANT_H_

#include <memory>
#include <cstdint>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <cassert>

namespace CommonAPI {

class InputStream;
class OutputStream;

class TypeOutputStream;

template<typename _Type>
struct TypeWriter;

class SerializableVariant {
public:
    virtual ~SerializableVariant() {
    }

    virtual uint8_t getValueType() const = 0;

    virtual void readFromInputStream(const uint8_t typeIndex, InputStream& inputStream) = 0;
    virtual void writeToOutputStream(OutputStream& outputStream) const = 0;

    virtual void writeToTypeOutputStream(TypeOutputStream& typeOutputStream) const = 0;
};

template<typename ... _Types>
struct MaxSize;

template<>
struct MaxSize<> {
    static const unsigned int value = 0;
};

template<typename _Type, typename ... _Types>
struct MaxSize<_Type, _Types...> {
    static const unsigned int current_type_size = sizeof(_Type);
    static const unsigned int next_type_size = MaxSize<_Types...>::value;
    static const unsigned int value =
                    current_type_size > next_type_size ?
                                    current_type_size : next_type_size;
};

template<typename _SearchType, typename _CurrentType, typename ... _RestTypes>
struct VariantTypeSelector: VariantTypeSelector<_SearchType, _RestTypes...> {
};

template<typename _SearchType, typename ... _RestTypes>
struct VariantTypeSelector<_SearchType, _SearchType, _RestTypes...> {
    typedef _SearchType type;
};

template<typename ... _Types>
class Variant: public SerializableVariant {
private:
    typedef std::tuple_size<std::tuple<_Types...>> TypesTupleSize;

public:

    static const unsigned int maxSize = MaxSize<_Types...>::value;

    Variant();

    Variant(const Variant& fromVariant);

    Variant(Variant&& fromVariant);

    ~Variant();

    virtual void readFromInputStream(const uint8_t typeIndex, InputStream& inputStream);

    virtual void writeToOutputStream(OutputStream& outputStream) const;

    virtual void writeToTypeOutputStream(TypeOutputStream& typeOutputStream) const;

    Variant& operator=(const Variant& rhs);

    Variant& operator=(Variant&& rhs);

    template<typename _Type>
    typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value, Variant<_Types...>&>::type
    operator=(const _Type& value);

    bool operator==(const Variant<_Types...>& rhs) const;

    bool operator!=(const Variant<_Types...>& rhs) const;

    template <typename _Type>
    const bool isType() const;

    template <typename _Type>
    Variant(const _Type& value,
                    typename std::enable_if<!std::is_const<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_reference<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_same<_Type, Variant>::value>::type* = 0);

    template <typename _Type>
    Variant(_Type && value,
                    typename std::enable_if<!std::is_const<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_reference<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_same<_Type, Variant>::value>::type* = 0);

    template <typename _Type>
    const typename VariantTypeSelector<_Type, _Types...>::type & get(bool& success) const;

    inline uint8_t getValueType() const {
    	return valueType_;
    }

    template<typename _U>
    void set( const _U& value, const bool clear);

    template<typename _U>
    void set2( _U&& value, const bool clear);

private:
    inline bool hasValue() const {
        return valueType_ < TypesTupleSize::value;
    }

    uint8_t valueType_;
    typename std::aligned_storage<maxSize>::type valueStorage_;

};
} // namespace CommonAPI

#include "SerializableVariant.hpp"

#endif // COMMONAPI_SERIALIZABLE_VARIANT_H_
