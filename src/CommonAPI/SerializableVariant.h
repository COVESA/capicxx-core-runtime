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

/**
 * \brief A variant class which can be serialised by bindings.
 *
 * A variant class which can be serialised by bindings.
 */
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

template<typename _SearchType, typename ... _RestTypes>
struct VariantTypeSelector;

template<typename _SearchType, typename ... _RestTypes>
struct VariantTypeSelector<_SearchType, _SearchType, _RestTypes...> {
    typedef _SearchType type;
};

/**
 * \brief A templated generic variant class which provides type safe access and operators
 *
 * A templated generic variant class which provides type safe access and operators
 */
template<typename ... _Types>
class Variant: public SerializableVariant {
private:
    typedef std::tuple_size<std::tuple<_Types...>> TypesTupleSize;

public:

    static const unsigned int maxSize = MaxSize<_Types...>::value;

    /**
     * \brief Construct an empty variant
     *
     * Construct an empty variant
     */
    Variant();


    /**
     * \brief Copy constructor. Must have identical templates.
     *
     * Copy constructor. Must have identical templates.
     *
     * @param fromVariant Variant to copy
     */
    Variant(const Variant& fromVariant);

    /**
     * \brief Copy constructor. Must have identical templates.
     *
     * Copy constructor. Must have identical templates.
     *
     * @param fromVariant Variant to copy
     */
    Variant(Variant&& fromVariant);

    ~Variant();

    virtual void readFromInputStream(const uint8_t typeIndex, InputStream& inputStream);

    virtual void writeToOutputStream(OutputStream& outputStream) const;

    virtual void writeToTypeOutputStream(TypeOutputStream& typeOutputStream) const;

    /**
      * \brief Assignment of another variant. Must have identical templates.
      *
      * Assignment of another variant. Must have identical templates.
      *
      * @param rhs Variant to assign
      */
    Variant& operator=(const Variant& rhs);
    /**
     * \brief Assignment of another variant. Must have identical templates.
     *
     * Assignment of another variant. Must have identical templates.
     *
     * @param rhs Variant to assign
     */
    Variant& operator=(Variant&& rhs);

    /**
     * \brief Assignment of a contained type. Must be one of the valid templated types.
     *
     * Assignment of a contained type. Must be one of the valid templated types.
     *
     * @param value Value to assign
     */
    template<typename _Type>
    typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value, Variant<_Types...>&>::type
    operator=(const _Type& value);

    /**
     * \brief Equality of another variant. Must have identical template list and content.
     *
     * Equality of another variant. Must have identical template list and content.
     *
     * @param rhs Variant to compare
     */
    bool operator==(const Variant<_Types...>& rhs) const;

    /**
      * \brief Not-Equality of another variant. Must have identical template list and content.
      *
      * Not-Equality of another variant. Must have identical template list and content.
      *
      * @param rhs Variant to compare
      */
    bool operator!=(const Variant<_Types...>& rhs) const;

    /**
      * \brief Testif the contained type is the same as the template on this method.
      *
      * Testif the contained type is the same as the template on this method.
      *
      * @return Is same type
      */
    template <typename _Type>
    const bool isType() const;

    /**
     * \brief Construct variant with content type set to value.
     *
     * Construct variant with content type set to value.
     *
     * @param value Value to place
     */
    template <typename _Type>
    Variant(const _Type& value,
                    typename std::enable_if<!std::is_const<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_reference<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_same<_Type, Variant>::value>::type* = 0);

    /**
     * \brief Construct variant with content type set to value.
     *
     * Construct variant with content type set to value.
     *
     * @param value Value to place
     */
    template <typename _Type>
    Variant(_Type && value,
                    typename std::enable_if<!std::is_const<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_reference<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_same<_Type, Variant>::value>::type* = 0);

    /**
     * \brief Get value of variant, template to content type. Throws exception if type is not contained.
     *
     * Get value of variant, template to content type. Throws exception if type is not contained.
     */
    template <typename _Type>
    const _Type& get() const;

    /**
     * \brief Get index in template list of type actually contained, starting at 1 at the end of the template list
     *
     * Get index in template list of type actually contained, starting at 1 at the end of the template list
     *
     * @return Index of contained type
     */
    inline uint8_t getValueType() const {
    	return valueType_;
    }

private:
    inline bool hasValue() const {
        return valueType_ <= TypesTupleSize::value;
    }

    template<typename _U>
    void set( const _U& value, const bool clear);

    template<typename _U>
    void set( _U&& value, const bool clear);

    template<typename >
    friend struct TypeWriter;
    template<typename ... _FriendTypes>
    friend struct AssignmentVisitor;
    template<typename _FriendType>
    friend struct TypeEqualsVisitor;
    template<typename ... _FriendTypes>
    friend struct PartialEqualsVisitor;
    template<typename ... _FriendTypes>
    friend struct InputStreamReadVisitor;
    template<class Variant, typename ... _FTypes>
    friend struct ApplyVoidIndexVisitor;

    uint8_t valueType_;
    typename std::aligned_storage<maxSize>::type valueStorage_;
};
} // namespace CommonAPI

#include "SerializableVariant.hpp"

#endif // COMMONAPI_SERIALIZABLE_VARIANT_H_
