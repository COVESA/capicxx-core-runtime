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

class SerializableVariant {
public:
    virtual ~SerializableVariant() {
    }

    virtual uint8_t getValueType() const = 0;

    virtual void readFromInputStream(InputStream& inputStream) = 0;
    virtual void writeToOutputStream(OutputStream& outputStream) = 0;
};

template<typename ... _Types>
struct AssignmentVisitor;

template<typename _Type>
struct TypeEqualsVisitor;

template<typename ... _Types>
struct PartialEqualsVisitor;

template<class Visitor, class Variant, typename ... _Types>
struct ApplyVoidVisitor;

template<class Visitor, class Variant>
struct ApplyVoidVisitor<Visitor, Variant> {
    static const uint8_t index = 0;

    static
    void visit(Visitor&, Variant&) {
        //won't be called
        assert(false);
    }
};

template<class Visitor, class Variant, typename _Type, typename ... _Types>
struct ApplyVoidVisitor<Visitor, Variant, _Type, _Types...> {
    static const uint8_t index = ApplyVoidVisitor<Visitor, Variant,
                    _Types...>::index + 1;

    static
    void visit(Visitor& visitor, Variant& var) {
        if (var.getValueType() == index) {
            bool b;
            visitor(var.template get<_Type>(b));
        } else {
            ApplyVoidVisitor<Visitor, Variant, _Types...>::visit(visitor, var);
        }
    }
};

template<class Visitor, class Variant, typename ... _Types>
struct ApplyBoolVisitor
;

template<class Visitor, class Variant>
struct ApplyBoolVisitor<Visitor, Variant> {
    static const uint8_t index = 0;

    static bool visit(Visitor&, Variant&) {
        //won't be called
        assert(false);
    }
};

template<class Visitor, class Variant, typename _Type, typename ... _Types>
struct ApplyBoolVisitor<Visitor, Variant, _Type, _Types...> {
    static const uint8_t index = ApplyBoolVisitor<Visitor, Variant,
                    _Types...>::index + 1;

    static bool visit(Visitor& visitor, Variant& var) {
        if (var.getValueType() == index) {
            bool b;
            return visitor(var.template get<_Type>(b));
        } else {
            return ApplyBoolVisitor<Visitor, Variant, _Types...>::visit(visitor,
                            var);
        }
    }
};

template<uint8_t size>
struct DeleteVisitor {
public:
    DeleteVisitor(typename std::aligned_storage<size>::type& storage) :
                    storage_(storage) {
    }

    template<typename _Type>
    void operator()(const _Type&) const {
        (reinterpret_cast<const _Type *>(&storage_))->~_Type();
    }

private:
    typename std::aligned_storage<size>::type& storage_;
};

template<typename _U, typename ... _Types>
struct TypeSelector;

template<typename _U>
struct TypeSelector<_U> {
};

//_U == _Type
template<typename _Type, typename ... _Types>
struct TypeSelector<_Type, _Type, _Types...> {
    typedef _Type type;
};

//_U& == _Type
template<typename _Type, typename ... _Types>
struct TypeSelector<_Type, _Type&, _Types...> {
    typedef _Type& type;
};

//_U == _Type&
template<typename _Type, typename ... _Types>
struct TypeSelector<_Type&, _Type, _Types...> {
    typedef _Type type;
};

//const _U& == _Type
template<typename _Type, typename ... _Types>
struct TypeSelector<_Type, const _Type&, _Types...> {
    typedef const _Type& type;
};

//_U == const _Type&
template<typename _Type, typename ... _Types>
struct TypeSelector<const _Type&, _Type, _Types...> {
    typedef _Type type;
};

//_U == X*
//_Type == const X*
template<typename _Type, typename ... _Types>
struct TypeSelector<_Type*, const _Type*, _Types...> {
    typedef const _Type* type;
};

//_U == X&
//_Type == const X&
template<typename _Type, typename ... _Types>
struct TypeSelector<_Type&, const _Type&, _Types...> {
    typedef const _Type& type;
};

//_U != _Type, let's try to find _U among _Types
template<typename _U, typename _Type, typename ... _Types>
struct TypeSelector<_U, _Type, _Types...> {
    typedef typename TypeSelector<_U, _Types...>::type type;
};

template<typename ... _Types>
struct TypeIndex;

template<>
struct TypeIndex<> {
    static const uint8_t index = 0;

    template<typename _U>
    static
    uint8_t get() {
        return 0;
    }
};

template<typename _Type, typename ... _Types>
struct TypeIndex<_Type, _Types...> {
    static const uint8_t index = TypeIndex<_Types...>::index + 1;

    template<typename _U>
    static
    uint8_t get(
                     typename std::enable_if<std::is_same<_Type, _U>::value>::type* = 0) {
        return index;
    }

    template<typename _U>
    static
    uint8_t get(typename std::enable_if<!std::is_same<_Type, _U>::value>::type* = 0) {
        return TypeIndex<_Types...>::template get<_U>();
    }
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

    Variant() :
                    valueType_(TypesTupleSize::value) {
    }

    Variant(const Variant& fromVariant) :
                    valueType_(fromVariant.valueType_),
                    valueStorage_(fromVariant.valueStorage_) {
    }

    Variant(Variant&& fromVariant):
    valueType_(std::move(fromVariant.valueType_)),
    valueStorage_(std::move(fromVariant.valueStorage_)) {
        fromVariant.valueType_ = TypesTupleSize::value;
    }

    ~Variant() {
        if (hasValue()) {
            DeleteVisitor<maxSize> visitor(valueStorage_);
            ApplyVoidVisitor<DeleteVisitor<maxSize>, Variant<_Types...>, _Types...>::visit(visitor, *this);
        }
    }

    virtual void readFromInputStream(InputStream& inputStream) {

    }

    virtual void writeToOutputStream(OutputStream& outputStream) {

    }

    Variant& operator=(const Variant& rhs)
    {
        AssignmentVisitor<_Types...> visitor(*this);
        ApplyVoidVisitor<AssignmentVisitor<_Types...>, Variant<_Types...>, _Types...>::visit(visitor, rhs);
        return *this;
    }

    Variant& operator=(Variant&& rhs) {
        AssignmentVisitor<_Types...> visitor(*this);
        ApplyVoidVisitor<AssignmentVisitor<_Types...>, Variant<_Types...>, _Types...>::visit(visitor, rhs);
        return *this;
    }

    template<typename _Type>
    typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value, Variant<_Types...>&>::type
    operator=(const _Type& value)
    {
        set<typename TypeSelector<_Type, _Types...>::type>(value);
        return *this;
    }

    template <typename _Type>
    const bool isType() const {
        typedef typename TypeSelector<_Type, _Types...>::type selected_type_t;
        uint8_t cType = TypeIndex<_Types...>::template get<selected_type_t>();
        if(cType == valueType_) {
            return true;
        } else {
            return false;
        }
    }

    template <typename _Type>
    Variant(const _Type& value,
                    typename std::enable_if<!std::is_const<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_reference<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_same<_Type, Variant>::value>::type* = 0) {
        set<typename TypeSelector<_Type, _Types...>::type>(value, false);
    }

    template <typename _Type>
    Variant(_Type && value,
                    typename std::enable_if<!std::is_const<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_reference<_Type>::value>::type* = 0,
                    typename std::enable_if<!std::is_same<_Type, Variant>::value>::type* = 0) {
        set2<typename TypeSelector<_Type, _Types...>::type>(std::move(value), false);
    }

    //TODO: Return type???
    template <typename _Type>
    const typename VariantTypeSelector<_Type, _Types...>::type & get(bool& success) const {
        typedef typename TypeSelector<_Type, _Types...>::type selected_type_t;
        uint8_t cType = TypeIndex<_Types...>::template get<selected_type_t>();
        if(cType == valueType_) {
            success = true;
            return *(reinterpret_cast<const _Type *>(&valueStorage_));
        } else {
            success = false;
            return *(reinterpret_cast<const _Type *>(&valueStorage_));
        }
    }

    inline uint8_t getValueType() const {
        return valueType_;
    }

    template<typename _U>
    void set( const _U& value, const bool clear) {
        typedef typename TypeSelector<_U, _Types...>::type selected_type_t;

        const selected_type_t& type_value = value;
        if(clear) {
            DeleteVisitor<maxSize> visitor(valueStorage_);
            ApplyVoidVisitor<DeleteVisitor<maxSize>, Variant<_Types...>, _Types...>::visit(visitor, *this);
        }
        new (&valueStorage_) selected_type_t(std::move(value));
        valueType_ = TypeIndex<_Types...>::template get<selected_type_t>();
    }

    template<typename _U>
    void set2( _U&& value, const bool clear) {
        typedef typename TypeSelector<_U, _Types...>::type selected_type_t;

        selected_type_t&& any_container_value = std::move(value);
        if(clear)
        {
            DeleteVisitor<maxSize> visitor(valueStorage_);
            ApplyVoidVisitor<DeleteVisitor<maxSize>, Variant<_Types...>, _Types...>::visit(visitor, *this);
        } else {
            new (&valueStorage_) selected_type_t(std::move(any_container_value));
        }

        valueType_ = TypeIndex<_Types...>::template get<selected_type_t>();
    }

private:
    inline bool hasValue() const {
        return valueType_ < TypesTupleSize::value;
    }

    uint8_t valueType_;
    typename std::aligned_storage<maxSize>::type valueStorage_;

};

template<typename ... _Types>
bool operator==(const Variant<_Types...>& lhs, const Variant<_Types...>& rhs)
                {
    PartialEqualsVisitor<_Types...> visitor(lhs);
    return ApplyBoolVisitor<PartialEqualsVisitor<_Types...>, const Variant<_Types...>, _Types...>::visit(
                    visitor,
                    rhs);
}

template<typename ... _Types>
bool operator!=(const Variant<_Types...>& lhs, const Variant<_Types...>& rhs)
                {
    return !(lhs == rhs);
}

template<typename _Type>
struct TypeEqualsVisitor
{
public:
    TypeEqualsVisitor(const _Type& rhs) :
                    rhs_(rhs)
    {
    }

    bool
    operator()(const _Type& lhs) const
                      {
        return lhs == rhs_;
    }

    template<typename _U>
    bool
    operator()(const _U&) const
                      {
        return false;
    }

private:
    const _Type& rhs_;
};

template<typename ... _Types>
struct PartialEqualsVisitor
{
public:
    PartialEqualsVisitor(const Variant<_Types...>& lhs) :
                    lhs_(lhs) {
    }

    template<typename _Type>
    bool
    operator()(const _Type& rhs) const
                      {
        TypeEqualsVisitor<_Type> visitor(rhs);
        return ApplyBoolVisitor<TypeEqualsVisitor<_Type>, const Variant<_Types...>, _Types...>::visit(visitor, lhs_);
    }

private:
    const Variant<_Types...>& lhs_;
};

template<typename ... _Types>
struct AssignmentVisitor {
public:
    AssignmentVisitor(Variant<_Types...>& lhs, const bool clear = true) :
                    lhs_(lhs), clear_(clear) {
    }

    template<typename _Type>
    void operator()(const _Type& value) const {
        lhs_.template set<_Type>(value, clear_);
    }

    template<typename _Type>
    void operator()(_Type& value) const {
        lhs_.template set<_Type>(value, clear_);
    }

private:
    Variant<_Types...>& lhs_;
    const bool clear_;
};

} // namespace CommonAPI

#endif // COMMONAPI_SERIALIZABLE_VARIANT_H_
