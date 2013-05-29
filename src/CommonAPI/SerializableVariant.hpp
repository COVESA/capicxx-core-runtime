/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_SERIALIZABLE_VARIANT_IMPL_
#define COMMONAPI_SERIALIZABLE_VARIANT_IMPL_

#include "OutputStream.h"
#include "InputStream.h"

#include <exception>


namespace CommonAPI {

template<class Variant, typename ... _Types>
struct ApplyVoidIndexVisitor;

template<class Variant>
struct ApplyVoidIndexVisitor<Variant> {
    static const uint8_t index = 0;

    static
    void visit(Variant&, uint8_t&) {
        //won't be called
        assert(false);
    }
};

template<class Variant, typename _Type, typename ... _Types>
struct ApplyVoidIndexVisitor<Variant, _Type, _Types...> {
    static const uint8_t index = ApplyVoidIndexVisitor<Variant,
                    _Types...>::index + 1;

    static void visit(Variant& var, uint8_t& ind) {
        if (ind == index) {
            new (&var.valueStorage_) _Type();
            var.valueType_ = index;
        } else {
            ApplyVoidIndexVisitor<Variant, _Types...>::visit(var, ind);
        }
    }
};

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

    static
    void visit(Visitor&, const Variant&) {
        //won't be called
        assert(false);
    }
};

template<class Visitor, class Variant, typename _Type, typename ... _Types>
struct ApplyVoidVisitor<Visitor, Variant, _Type, _Types...> {
    static const uint8_t index = ApplyVoidVisitor<Visitor, Variant,
                    _Types...>::index + 1;

    static void visit(Visitor& visitor, Variant& var) {
        if (var.getValueType() == index) {
            visitor(var.template get<_Type>());
        } else {
            ApplyVoidVisitor<Visitor, Variant, _Types...>::visit(visitor, var);
        }
    }

    static void visit(Visitor& visitor, const Variant& var) {
        if (var.getValueType() == index) {
            visitor(var.template get<_Type>());
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
        return false;
    }
};

template<class Visitor, class Variant, typename _Type, typename ... _Types>
struct ApplyBoolVisitor<Visitor, Variant, _Type, _Types...> {
    static const uint8_t index = ApplyBoolVisitor<Visitor, Variant,
                    _Types...>::index + 1;

    static bool visit(Visitor& visitor, Variant& var) {
        if (var.getValueType() == index) {
            return visitor(var.template get<_Type>());
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

struct TypeOutputStreamWriteVisitor {
public:
    TypeOutputStreamWriteVisitor(TypeOutputStream& typeStream) :
                    typeStream_(typeStream) {
    }

    template<typename _Type>
    void operator()(const _Type&) const {
        TypeWriter<_Type>::writeType(typeStream_);
    }

private:
    TypeOutputStream& typeStream_;
};

struct OutputStreamWriteVisitor {
public:
    OutputStreamWriteVisitor(OutputStream& outputStream) :
                    outputStream_(outputStream) {
    }

    template<typename _Type>
    void operator()(const _Type& value) const {
        outputStream_ << value;
    }

private:
    OutputStream& outputStream_;
};


template<typename ... _Types>
struct InputStreamReadVisitor {
public:
    InputStreamReadVisitor(Variant<_Types...>& lhs, InputStream& inputStream) :
                    lhs_(lhs),
                    inputStream_(inputStream) {
    }

    template<typename _Type>
    void operator()(const _Type&) {
        _Type value;
        inputStream_ >> value;
        lhs_.Variant<_Types...>::template set<_Type>(std::move(value), false);
    }

private:
    Variant<_Types...>& lhs_;
    InputStream& inputStream_;
};


template<typename _Type>
struct TypeEqualsVisitor
{
public:
    TypeEqualsVisitor(const _Type& rhs): rhs_(rhs) {
    }

    bool operator()(const _Type& lhs) const {
        return lhs == rhs_;
    }

    template<typename _U>
    bool operator()(const _U&) const {
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
        lhs_.Variant<_Types...>::template set<_Type>(value, clear_);
    }

    template<typename _Type>
    void operator()(_Type& value) const {
        lhs_.Variant<_Types...>::template set<_Type>(value, clear_);
    }

private:
    Variant<_Types...>& lhs_;
    const bool clear_;
};

template<typename ... _Types>
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
    static uint8_t get() {
        return 0;
    }
};

template<typename _Type, typename ... _Types>
struct TypeIndex<_Type, _Types...> {
    static const uint8_t index = TypeIndex<_Types...>::index + 1;

    template<typename _U>
    static uint8_t get(
                       typename std::enable_if<std::is_same<_Type, _U>::value>::type* = 0) {
        return index;
    }

    template<typename _U>
    static uint8_t get(typename std::enable_if<!std::is_same<_Type, _U>::value>::type* = 0) {
        return TypeIndex<_Types...>::template get<_U>();
    }
};

template<typename ... _Types>
Variant<_Types...>::Variant() :
                valueType_(TypesTupleSize::value) {
    ApplyVoidIndexVisitor<Variant<_Types...>, _Types...>::visit(*this, valueType_);
}

template<typename ... _Types>
Variant<_Types...>::Variant(const Variant& fromVariant) {
    AssignmentVisitor<_Types...> visitor(*this, false);
    ApplyVoidVisitor<AssignmentVisitor<_Types...> , Variant<_Types...>, _Types...>::visit(visitor, fromVariant);
}

template<typename ... _Types>
Variant<_Types...>::Variant(Variant&& fromVariant)
{
    AssignmentVisitor<_Types...> visitor(*this, false);
    ApplyVoidVisitor<AssignmentVisitor<_Types...> , Variant<_Types...>, _Types...>::visit(visitor, fromVariant);
}

/*template<typename ... _Types>
Variant<_Types...>::Variant(Variant&& fromVariant) :
    valueType_(std::move(fromVariant.valueType_)),
    valueStorage_(std::move(fromVariant.valueStorage_))
{
}*/

template<typename ... _Types>
Variant<_Types...>::~Variant() {
    if (hasValue()) {
        DeleteVisitor<maxSize> visitor(valueStorage_);
        ApplyVoidVisitor<DeleteVisitor<maxSize>, Variant<_Types...>, _Types...>::visit(visitor, *this);
    }
}

template<typename ... _Types>
void Variant<_Types...>::readFromInputStream(const uint8_t typeIndex, InputStream& inputStream) {
    if(hasValue()) {
		DeleteVisitor<maxSize> visitor(valueStorage_);
        ApplyVoidVisitor<DeleteVisitor<maxSize>, Variant<_Types...>, _Types...>::visit(visitor, *this);
    }
    valueType_ = typeIndex;
    InputStreamReadVisitor<_Types...> visitor(*this, inputStream);
    ApplyVoidVisitor<InputStreamReadVisitor<_Types...>, Variant<_Types...>, _Types...>::visit(visitor, *this);
}

template<typename ... _Types>
void Variant<_Types...>::writeToOutputStream(OutputStream& outputStream) const {
    OutputStreamWriteVisitor visitor(outputStream);
    ApplyVoidVisitor<OutputStreamWriteVisitor, Variant<_Types...>, _Types...>::visit(
                    visitor, *this);
}

template<typename ... _Types>
void Variant<_Types...>::writeToTypeOutputStream(TypeOutputStream& typeOutputStream) const {
    TypeOutputStreamWriteVisitor visitor(typeOutputStream);
    ApplyVoidVisitor<TypeOutputStreamWriteVisitor, Variant<_Types...>, _Types...>::visit(
                    visitor, *this);
}

template<typename ... _Types>
Variant<_Types...>& Variant<_Types...>::operator=(const Variant<_Types...>& rhs) {
    AssignmentVisitor<_Types...> visitor(*this, hasValue());
    ApplyVoidVisitor<AssignmentVisitor<_Types...>, Variant<_Types...>, _Types...>::visit(
                    visitor, rhs);
    return *this;
}

template<typename ... _Types>
Variant<_Types...>& Variant<_Types...>::operator=(Variant<_Types...>&& rhs) {
    AssignmentVisitor<_Types...> visitor(*this, hasValue());
    ApplyVoidVisitor<AssignmentVisitor<_Types...>, Variant<_Types...>, _Types...>::visit(visitor, rhs);
    return *this;
}

template<typename ... _Types>
template<typename _Type>
typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value, Variant<_Types...>&>::type
Variant<_Types...>::operator=(const _Type& value) {
    set<typename TypeSelector<_Type, _Types...>::type>(value, hasValue());
    return *this;
}

template<typename ... _Types>
template<typename _Type>
const bool Variant<_Types...>::isType() const {
    typedef typename TypeSelector<_Type, _Types...>::type selected_type_t;
    uint8_t cType = TypeIndex<_Types...>::template get<selected_type_t>();
    if (cType == valueType_) {
        return true;
    } else {
        return false;
    }
}

template<typename ... _Types>
template<typename _Type>
Variant<_Types...>::Variant(const _Type& value,
                            typename std::enable_if<!std::is_const<_Type>::value>::type*,
                            typename std::enable_if<!std::is_reference<_Type>::value>::type*,
                            typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value>::type*) {
    set<typename TypeSelector<_Type, _Types...>::type>(value, false);
}

template<typename ... _Types>
template<typename _Type>
Variant<_Types...>::Variant(_Type && value,
typename std::enable_if<!std::is_const<_Type>::value>::type*,
typename std::enable_if<!std::is_reference<_Type>::value>::type*,
typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value>::type*) {
    set<typename TypeSelector<_Type, _Types...>::type>(std::move(value), false);
}


template<typename ... _Types>
template<typename _Type>
const _Type & Variant<_Types...>::get() const {
    typedef typename TypeSelector<_Type, _Types...>::type selected_type_t;
    uint8_t cType = TypeIndex<_Types...>::template get<selected_type_t>();
    if (cType == valueType_) {
        return *(reinterpret_cast<const _Type *>(&valueStorage_));
    } else {
        std::bad_cast toThrow;
        throw toThrow;
    }
}


template<typename ... _Types>
template<typename _U>
void Variant<_Types...>::set(const _U& value, const bool clear) {
    typedef typename TypeSelector<_U, _Types...>::type selected_type_t;

    if (clear) {
		DeleteVisitor<maxSize> visitor(valueStorage_);
        ApplyVoidVisitor<DeleteVisitor<maxSize>, Variant<_Types...>, _Types...>::visit(visitor, *this);
    }
    new (&valueStorage_) selected_type_t(std::move(value));
    valueType_ = TypeIndex<_Types...>::template get<selected_type_t>();
}

template<typename ... _Types>
template<typename _U>
void Variant<_Types...>::set(_U&& value, const bool clear) {
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

template<typename ... _Types>
bool Variant<_Types...>::operator==(const Variant<_Types...>& rhs) const
                {
    PartialEqualsVisitor<_Types...> visitor(*this);
    return ApplyBoolVisitor<PartialEqualsVisitor<_Types...>, const Variant<_Types...>, _Types...>::visit(
                    visitor,
                    rhs);
}

template<typename ... _Types>
bool Variant<_Types...>::operator!=(const Variant<_Types...>& rhs) const
                {
    return !(*this == rhs);
}

}

#endif //COMMONAPI_SERIALIZABLE_VARIANT_IMPL_
