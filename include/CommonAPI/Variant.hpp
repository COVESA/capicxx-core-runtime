// Copyright (C) 2013-2015 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#include <CommonAPI/Deployable.hpp>
#include <CommonAPI/Deployment.hpp>

#ifndef COMMONAPI_VARIANT_HPP_
#define COMMONAPI_VARIANT_HPP_

namespace CommonAPI {

template<class _Derived>
class InputStream;

template<class _Derived>
class OutputStream;

template<typename... _Types>
struct MaxSize;

template<>
struct MaxSize<> {
    static const unsigned int value = 0;
};

template<typename _Type, typename... _Types>
struct MaxSize<_Type, _Types...> {
    static const unsigned int current_type_size = sizeof(_Type);
    static const unsigned int next_type_size = MaxSize<_Types...>::value;
    static const unsigned int value =
                    current_type_size > next_type_size ?
                                    current_type_size : next_type_size;
};

template<typename _SearchType, typename... _RestTypes>
struct VariantTypeSelector;

template<typename _SearchType, typename... _RestTypes>
struct VariantTypeSelector<_SearchType, _SearchType, _RestTypes...> {
    typedef _SearchType type;
};

/**
 * \brief A templated generic variant class which provides type safe access and operators
 *
 * A templated generic variant class which provides type safe access and operators
 */
template<typename... _Types>
class Variant {
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
     * @param _source Variant to copy
     */
    Variant(const Variant &_source);

    /**
     * \brief Copy constructor. Must have identical templates.
     *
     * Copy constructor. Must have identical templates.
     *
     * @param _source Variant to copy
     */
    Variant(Variant &&_source);

    ~Variant();

    /**
      * \brief Assignment of another variant. Must have identical templates.
      *
      * Assignment of another variant. Must have identical templates.
      *
      * @param _source Variant to assign
      */
    Variant &operator=(const Variant &_source);
    /**
     * \brief Assignment of another variant. Must have identical templates.
     *
     * Assignment of another variant. Must have identical templates.
     *
     * @param _source Variant to assign
     */
    Variant &operator=(Variant &&_source);

    /**
     * \brief Assignment of a contained type. Must be one of the valid templated types.
     *
     * Assignment of a contained type. Must be one of the valid templated types.
     *
     * @param _value Value to assign
     */
    template<typename _Type>
    typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value, Variant<_Types...>&>::type
    operator=(const _Type &_value);

    /**
     * \brief Equality of another variant. Must have identical template list and content.
     *
     * Equality of another variant. Must have identical template list and content.
     *
     * @param _other Variant to compare
     */
    bool operator==(const Variant<_Types...> &_other) const;

    /**
      * \brief Not-Equality of another variant. Must have identical template list and content.
      *
      * Not-Equality of another variant. Must have identical template list and content.
      *
      * @param _other Variant to compare
      */
    bool operator!=(const Variant<_Types...> &_other) const;

    /**
      * \brief Testif the contained type is the same as the template on this method.
      *
      * Testif the contained type is the same as the template on this method.
      *
      * @return Is same type
      */
    template<typename _Type>
    const bool isType() const;

    /**
     * \brief Construct variant with content type set to value.
     *
     * Construct variant with content type set to value.
     *
     * @param _value Value to place
     */
    template<typename _Type>
    Variant(const _Type &_value,
            typename std::enable_if<!std::is_const<_Type>::value>::type* = 0,
            typename std::enable_if<!std::is_reference<_Type>::value>::type* = 0,
            typename std::enable_if<!std::is_same<_Type, Variant>::value>::type* = 0);

    /**
     * \brief Construct variant with content type set to value.
     *
     * Construct variant with content type set to value.
     *
     * @param _value Value to place
     */
    template<typename _Type>
    Variant(_Type &&_value,
            typename std::enable_if<!std::is_const<_Type>::value>::type* = 0,
            typename std::enable_if<!std::is_reference<_Type>::value>::type* = 0,
            typename std::enable_if<!std::is_same<_Type, Variant>::value>::type* = 0);

    /**
     * \brief Get value of variant, template to content type. Throws exception if type is not contained.
     *
     * Get value of variant, template to content type. Throws exception if type is not contained.
     */
    template<typename _Type>
    const _Type &get() const;

    /**
     * \brief Get index in template list of type actually contained, starting at 1 at the end of the template list
     *
     * Get index in template list of type actually contained, starting at 1 at the end of the template list
     *
     * @return Index of contained type
     */
    uint8_t getValueType() const {
        return valueType_;
    }

private:
    template<typename _Type>
    void set(const _Type &_value, const bool clear);

    template<typename _Type>
    void set(_Type &&_value, const bool clear);

    template<typename _FriendType>
    friend struct TypeWriter;
    template<typename ... _FriendTypes>
    friend struct AssignmentVisitor;
    template<typename _FriendType>
    friend struct TypeEqualsVisitor;
    template<typename ... _FriendTypes>
    friend struct PartialEqualsVisitor;
    template<class _Derived, typename ... _FriendTypes>
    friend struct InputStreamReadVisitor;
    template<class Variant, typename ... _FTypes>
    friend struct ApplyVoidIndexVisitor;

public:
    inline bool hasValue() const {
        return (valueType_ < TypesTupleSize::value);
    }
    typename std::aligned_storage<maxSize>::type valueStorage_;

    uint8_t valueType_;
};

template<class _Variant, typename... _Types>
struct ApplyVoidIndexVisitor;

template<class _Variant>
struct ApplyVoidIndexVisitor<_Variant> {
    static const uint8_t index = 0;

    static
    void visit(_Variant &, uint8_t &) {
        assert(false);
    }
};

template<class _Variant, typename _Type, typename... _Types>
struct ApplyVoidIndexVisitor<_Variant, _Type, _Types...> {
    static const uint8_t index
    	= ApplyVoidIndexVisitor<_Variant, _Types...>::index + 1;

    static void visit(_Variant &_variant, uint8_t &_index) {
        if (index == _index) {
            new (&_variant.valueStorage_) _Type();
            _variant.valueType_ = index;
        } else {
            ApplyVoidIndexVisitor<
            	_Variant, _Types...
            >::visit(_variant, _index);
        }
    }
};

template<class _Visitor, class _Variant, typename... _Types>
struct ApplyVoidVisitor;

template<class _Visitor, class _Variant>
struct ApplyVoidVisitor<_Visitor, _Variant> {
    static const uint8_t index = 0;

    static
    void visit(_Visitor &, _Variant &) {
        assert(false);
    }

    static
    void visit(_Visitor &, const _Variant &) {
        assert(false);
    }
};

template<class _Visitor, class _Variant, typename _Type, typename ... _Types>
struct ApplyVoidVisitor<_Visitor, _Variant, _Type, _Types...> {
    static const uint8_t index
    	= ApplyVoidVisitor<_Visitor, _Variant, _Types...>::index + 1;

    static void visit(_Visitor &_visitor, _Variant &_variant) {
        if (_variant.getValueType() == index) {
            _visitor(_variant.template get<_Type>());
        } else {
            ApplyVoidVisitor<
            	_Visitor, _Variant, _Types...
            >::visit(_visitor, _variant);
        }
    }

    static void visit(_Visitor &_visitor, const _Variant &_variant) {
        if (_variant.getValueType() == index) {
            _visitor(_variant.template get<_Type>());
        } else {
            ApplyVoidVisitor<
            	_Visitor, _Variant, _Types...
            >::visit(_visitor, _variant);
        }
    }
};

template<class _Visitor, class _Variant, typename ... _Types>
struct ApplyBoolVisitor;

template<class _Visitor, class _Variant>
struct ApplyBoolVisitor<_Visitor, _Variant> {
    static const uint8_t index = 0;

    static bool visit(_Visitor &, _Variant &) {
        assert(false);
        return false;
    }
};

template<class _Visitor, class _Variant, typename _Type, typename ... _Types>
struct ApplyBoolVisitor<_Visitor, _Variant, _Type, _Types...> {
    static const uint8_t index
    	= ApplyBoolVisitor<_Visitor, _Variant, _Types...>::index + 1;

    static bool visit(_Visitor &_visitor, _Variant &_variant) {
        if (_variant.getValueType() == index) {
            return _visitor(_variant.template get<_Type>());
        } else {
            return ApplyBoolVisitor<
            			_Visitor, _Variant, _Types...
            	   >::visit(_visitor, _variant);
        }
    }
};

template<class _Visitor, class _Variant, class _Deployment, typename ... _Types>
struct ApplyStreamVisitor;

template<class _Visitor, class _Variant, class _Deployment>
struct ApplyStreamVisitor<_Visitor, _Variant, _Deployment> {
    static const uint8_t index = 0;

    static
    void visit(_Visitor &, _Variant &, const _Deployment *) {
        assert(false);
    }

    static
    void visit(_Visitor &, const _Variant &, const _Deployment *) {
        assert(false);
    }
};

template<class _Visitor, class _Variant, class _Deployment, typename _Type, typename ... _Types>
struct ApplyStreamVisitor<_Visitor, _Variant, _Deployment, _Type, _Types...> {
    static const uint8_t index
    	= ApplyStreamVisitor<_Visitor, _Variant, _Deployment, _Types...>::index + 1;

    static void visit(_Visitor &_visitor, _Variant &_variant, const _Deployment *_depl) {
        if (_variant.getValueType() == index) {
        	_visitor(_variant.template get<_Type>(),
        			 (_depl ? std::get<std::tuple_size<decltype(_depl->values_)>::value-index>(_depl->values_)
        					: nullptr));
        } else {
            ApplyStreamVisitor<
            	_Visitor, _Variant, _Deployment, _Types...
            >::visit(_visitor, _variant, _depl);
        }
    }

    static void visit(_Visitor &_visitor, const _Variant &_variant, const _Deployment *_depl) {
        if (_variant.getValueType() == index) {
            _visitor(_variant.template get<_Type>(),
            		 (_depl ? std::get<std::tuple_size<decltype(_depl->values_)>::value-index>(_depl->values_)
            				: nullptr));
        } else {
            ApplyStreamVisitor<
            	_Visitor, _Variant, _Deployment, _Types...
            >::visit(_visitor, _variant, _depl);
        }
    }
};

template<uint32_t _Size>
struct DeleteVisitor {
public:
    DeleteVisitor(typename std::aligned_storage<_Size>::type &_storage)
		: storage_(_storage) {
    }

    template<typename _Type>
    void operator()(const _Type &) const {
        (reinterpret_cast<const _Type *>(&storage_))->~_Type();
    }

private:
    typename std::aligned_storage<_Size>::type &storage_;
};

template<class _Derived>
struct OutputStreamWriteVisitor {
public:
    OutputStreamWriteVisitor(OutputStream<_Derived> &_output)
		: output_(_output) {
    }

    template<typename _Type, typename _Deployment = EmptyDeployment>
    void operator()(const _Type &_value, const _Deployment *_depl = nullptr) const {
        Deployable<_Type, _Deployment> itsValue(_value, _depl);
        output_ << itsValue;
    }

private:
    OutputStream<_Derived> &output_;
};

template<class _Derived, typename ... _Types>
struct InputStreamReadVisitor {
public:
    InputStreamReadVisitor(InputStream<_Derived> &_input, Variant<_Types...> &_target)
		: input_(_input), target_(_target) {
    }

    template<typename _Type, typename _Deployment = EmptyDeployment>
    void operator()(const _Type &_value, const _Deployment *_depl = nullptr) {
        Deployable<_Type, _Deployment> itsValue(_depl);
        input_ >> itsValue;
        target_.Variant<_Types...>::template set<_Type>(std::move(itsValue.getValue()), false);
    }

private:
    InputStream<_Derived> &input_;
    Variant<_Types...> &target_;
};

template<class _Derived>
struct TypeOutputStreamWriteVisitor {
public:
    TypeOutputStreamWriteVisitor(_Derived &_output)
		: output_(_output) {
    }

    template<typename _Type>
    void operator()(const _Type &_value) const {
        output_ << _value;
    }

private:
    _Derived &output_;
};

template<typename _Type>
struct TypeEqualsVisitor
{
public:
    TypeEqualsVisitor(const _Type &_me)
		: me_(_me) {
    }

    bool operator()(const _Type &_other) const {
        return (me_ == _other);
    }

    template<typename _OtherType>
    bool operator()(const _OtherType &) const {
        return false;
    }

private:
    const _Type& me_;
};

template<typename ... _Types>
struct PartialEqualsVisitor
{
public:
    PartialEqualsVisitor(const Variant<_Types...> &_me)
		: me_(_me) {
    }

    template<typename _Type>
    bool operator()(const _Type &_other) const {
        TypeEqualsVisitor<_Type> visitor(_other);
        return ApplyBoolVisitor<
        			TypeEqualsVisitor<_Type>, const Variant<_Types...>, _Types...
        	   >::visit(visitor, me_);
    }

private:
    const Variant<_Types...> &me_;
};

template<typename ... _Types>
struct AssignmentVisitor {
public:
    AssignmentVisitor(Variant<_Types...> &_me, const bool _clear = true)
		: me_(_me), clear_(_clear) {
    }

    template<typename _Type>
    void operator()(const _Type &_value) const {
        me_.Variant<_Types...>::template set<_Type>(_value, clear_);
    }

    template<typename _Type>
    void operator()(_Type &_value) const {
        me_.Variant<_Types...>::template set<_Type>(_value, clear_);
    }

private:
    Variant<_Types...> &me_;
    const bool clear_;
};

template<typename... _Types>
struct TypeSelector;

template<typename _Type>
struct TypeSelector<_Type> {
};

template<typename _Type, typename... _Types>
struct TypeSelector<_Type, _Type, _Types...> {
    typedef _Type type;
};

template<typename _Type, typename... _Types>
struct TypeSelector<_Type, _Type &, _Types...> {
    typedef _Type& type;
};

template<typename _Type, typename... _Types>
struct TypeSelector<_Type &, _Type, _Types...> {
    typedef _Type type;
};

template<typename _Type, typename... _Types>
struct TypeSelector<_Type, const _Type &, _Types...> {
    typedef const _Type &type;
};

template<typename _Type, typename... _Types>
struct TypeSelector<const _Type&, _Type, _Types...> {
    typedef _Type type;
};

template<typename _Type, typename... _Types>
struct TypeSelector<_Type*, const _Type*, _Types...> {
    typedef const _Type *type;
};

template<typename _Type, typename... _Types>
struct TypeSelector<_Type &, const _Type &, _Types...> {
    typedef const _Type &type;
};

template<typename _U, typename _Type, typename... _Types>
struct TypeSelector<_U, _Type, _Types...> {
    typedef typename TypeSelector<_U, _Types...>::type type;
};

template<typename... _Types>
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
    static uint8_t get(typename std::enable_if<std::is_same<_Type, _U>::value>::type* = 0) {
        return index;
    }

    template<typename _U>
    static uint8_t get(typename std::enable_if<!std::is_same<_Type, _U>::value>::type* = 0) {
        return TypeIndex<_Types...>::template get<_U>();
    }
};

template<typename... _Types>
Variant<_Types...>::Variant()
	: valueType_(TypesTupleSize::value) {
    ApplyVoidIndexVisitor<Variant<_Types...>, _Types...>::visit(*this, valueType_);
}

template<typename... _Types>
Variant<_Types...>::Variant(const Variant &_source) {
    AssignmentVisitor<_Types...> visitor(*this, false);
    ApplyVoidVisitor<
    	AssignmentVisitor<_Types...> , Variant<_Types...>, _Types...
    >::visit(visitor, _source);
}

template<typename... _Types>
Variant<_Types...>::Variant(Variant &&_source)
{
    AssignmentVisitor<_Types...> visitor(*this, false);
    ApplyVoidVisitor<
    	AssignmentVisitor<_Types...> , Variant<_Types...>, _Types...
    >::visit(visitor, _source);
}

template<typename... _Types>
Variant<_Types...>::~Variant() {
    if (hasValue()) {
        DeleteVisitor<maxSize> visitor(valueStorage_);
        ApplyVoidVisitor<
        	DeleteVisitor<maxSize>, Variant<_Types...>, _Types...
        >::visit(visitor, *this);
    }
}

template<typename... _Types>
Variant<_Types...>& Variant<_Types...>::operator=(const Variant<_Types...> &_source) {
    AssignmentVisitor<_Types...> visitor(*this, hasValue());
    ApplyVoidVisitor<
    	AssignmentVisitor<_Types...>, Variant<_Types...>, _Types...
    >::visit(visitor, _source);
    return *this;
}

template<typename... _Types>
Variant<_Types...>& Variant<_Types...>::operator=(Variant<_Types...> &&_source) {
    AssignmentVisitor<_Types...> visitor(*this, hasValue());
    ApplyVoidVisitor<
    	AssignmentVisitor<_Types...>, Variant<_Types...>, _Types...
    >::visit(visitor, _source);
    return *this;
}

template<typename ... _Types>
template<typename _Type>
typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value, Variant<_Types...>&>::type
Variant<_Types...>::operator=(const _Type &_value) {
    set<typename TypeSelector<_Type, _Types...>::type>(_value, hasValue());
    return *this;
}

template<typename ... _Types>
template<typename _Type>
const bool Variant<_Types...>::isType() const {
    typedef typename TypeSelector<_Type, _Types...>::type selected_type_t;
    uint8_t itsType = TypeIndex<_Types...>::template get<selected_type_t>();
    if (itsType == valueType_) {
        return true;
    } else {
        return false;
    }
}

template<typename ... _Types>
template<typename _Type>
Variant<_Types...>::Variant(const _Type &_value,
                            typename std::enable_if<!std::is_const<_Type>::value>::type*,
                            typename std::enable_if<!std::is_reference<_Type>::value>::type*,
                            typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value>::type*) {
    set<typename TypeSelector<_Type, _Types...>::type>(_value, false);
}

template<typename ... _Types>
template<typename _Type>
Variant<_Types...>::Variant(_Type &&_value,
typename std::enable_if<!std::is_const<_Type>::value>::type*,
typename std::enable_if<!std::is_reference<_Type>::value>::type*,
typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value>::type*) {
    set<typename TypeSelector<_Type, _Types...>::type>(std::move(_value), false);
}


template<typename ... _Types>
template<typename _Type>
const _Type & Variant<_Types...>::get() const {
    typedef typename TypeSelector<_Type, _Types...>::type selected_type_t;
    uint8_t itsType = TypeIndex<_Types...>::template get<selected_type_t>();
    if (itsType == valueType_) {
        return *(reinterpret_cast<const _Type *>(&valueStorage_));
    } else {
#ifdef __EXCEPTIONS
        std::bad_cast toThrow;
        throw toThrow;
#else
        printf("SerializableVariant.hpp:%i %s: Incorrect access to variant; attempting to get type not currently contained", __LINE__, __FUNCTION__);
        abort();
#endif
    }
}


template<typename ... _Types>
template<typename _U>
void Variant<_Types...>::set(const _U &_value, const bool _clear) {
    typedef typename TypeSelector<_U, _Types...>::type selected_type_t;

    if (_clear) {
		DeleteVisitor<maxSize> visitor(valueStorage_);
        ApplyVoidVisitor<
        	DeleteVisitor<maxSize>, Variant<_Types...>, _Types...
        >::visit(visitor, *this);
    }
    new (&valueStorage_) selected_type_t(std::move(_value));
    valueType_ = TypeIndex<_Types...>::template get<selected_type_t>();
}

template<typename ... _Types>
template<typename _U>
void Variant<_Types...>::set(_U &&_value, const bool _clear) {
    typedef typename TypeSelector<_U, _Types...>::type selected_type_t;

    selected_type_t&& any_container_value = std::move(_value);
    if(_clear)
    {
		DeleteVisitor<maxSize> visitor(valueStorage_);
        ApplyVoidVisitor<
        	DeleteVisitor<maxSize>, Variant<_Types...>, _Types...
        >::visit(visitor, *this);
    } else {
        new (&valueStorage_) selected_type_t(std::move(any_container_value));
    }

    valueType_ = TypeIndex<_Types...>::template get<selected_type_t>();
}

template<typename ... _Types>
bool Variant<_Types...>::operator==(const Variant<_Types...> &_other) const
{
    PartialEqualsVisitor<_Types...> visitor(*this);
    return ApplyBoolVisitor<
    			PartialEqualsVisitor<_Types...>, const Variant<_Types...>, _Types...
    	   >::visit(visitor, _other);
}

template<typename ... _Types>
bool Variant<_Types...>::operator!=(const Variant<_Types...> &_other) const
{
    return !(*this == _other);
}

} // namespace CommonAPI

#endif // COMMONAPI_VARIANT_HPP_
