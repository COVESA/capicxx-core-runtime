/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_SERIALIZABLE_VARIANT_H_
#define COMMONAPI_SERIALIZABLE_VARIANT_H_

#include "Stream.h"

#include <memory>
#include <cstdint>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <cassert>

#ifndef __COMMON_API_EXPERIMENTAL
#error "Use -D__COMMON_API_EXPERIMENTAL to include experimental variant support"
#endif

namespace CommonAPI {

class SerializableVariant {
 public:
	virtual ~SerializableVariant() { }

 	inline uint32_t getType() const;

 	virtual void readFromInputStream(InputStream& inputStream) = 0;
	virtual void writeToOutputStream(OutputStream& outputStream) = 0;

 protected:
	uint32_t type_;
	std::shared_ptr<void> value_;
};

uint32_t SerializableVariant::getType() const {
	return type_;
}

template<typename ... Ts>
struct assign_visitor;

template<typename T>
struct complete_equals_visitor;

template<typename... Ts>
struct partial_equals_visitor;

template<class Visitor, class Variant, typename ... Ts>
struct apply_void_visitor;

template<class Visitor, class Variant>
struct apply_void_visitor<Visitor, Variant> {
	static const unsigned int index = 0;

	static
	void visit(Visitor&, Variant&) {
		//won't be called
		assert(false);
		throw "";
	}
};

template<class Visitor, class Variant, typename T, typename ... Ts>
struct apply_void_visitor<Visitor, Variant, T, Ts...> {
	static const unsigned int index = apply_void_visitor<Visitor, Variant,
			Ts...>::index + 1;

	static
	void visit(Visitor& visitor, Variant& var) {
		if (var.getValueType() == index) {
			bool b;
			visitor(var.template get<T>(b));
		} else {
			apply_void_visitor<Visitor, Variant, Ts...>::visit(visitor,	var);
		}
	}
};

template<class Visitor, class Variant, typename ... Ts>
struct apply_return_visitor
;

template<class Visitor, class Variant>
struct apply_return_visitor<Visitor, Variant> {
	static const unsigned int index = 0;

	static bool visit(Visitor&, Variant&) {
		//won't be called
		assert(false);
	}
};

template<class Visitor, class Variant, typename T, typename ... Ts>
struct apply_return_visitor<Visitor, Variant, T, Ts...> {
	static const unsigned int index = apply_return_visitor<Visitor, Variant,
			Ts...>::index + 1;

	static bool visit(Visitor& visitor, Variant& var) {
		if (var.getValueType() == index) {
			bool b;
			return visitor(var.template get<T>(b));
		} else {
			return apply_return_visitor<Visitor, Variant, Ts...>::visit(visitor,
					var);
		}
	}
};

template<unsigned int size>
struct clear_visitor {
public:
	clear_visitor(typename std::aligned_storage<size>::type& storage) :
			storage_(storage) {
	}

	template<typename _Type>
	void operator()(const _Type&) const {
		(reinterpret_cast<const _Type *>(&storage_))->~_Type();
	}

	private:
	typename std::aligned_storage<size>::type& storage_;
};

template<typename U, typename ... Ts>
struct select_type;

template<typename U>
struct select_type<U> {
};

//U == T
template<typename T, typename ... Ts>
struct select_type<T, T, Ts...> {
	typedef T type;
};

//U& == T
template<typename T, typename ... Ts>
struct select_type<T, T&, Ts...> {
	typedef T& type;
};

//U == T&
template<typename T, typename ... Ts>
struct select_type<T&, T, Ts...> {
	typedef T type;
};

//const U& == T
template<typename T, typename ... Ts>
struct select_type<T, const T&, Ts...> {
	typedef const T& type;
};

//U == const T&
template<typename T, typename ... Ts>
struct select_type<const T&, T, Ts...> {
	typedef T type;
};

//U == X*
//T == const X*
template<typename T, typename ... Ts>
struct select_type<T*, const T*, Ts...> {
	typedef const T* type;
};

//U == X&
//T == const X&
template<typename T, typename ... Ts>
struct select_type<T&, const T&, Ts...> {
	typedef const T& type;
};

//U != T, let's try to find U among Ts
template<typename U, typename T, typename ... Ts>
struct select_type<U, T, Ts...> {
	typedef typename select_type<U, Ts...>::type type;
};

template<typename ... Ts>
struct type_index_getter;

template<>
struct type_index_getter<> {
	static const unsigned int index = 0;

	template<typename U>
	static
	unsigned int get() {
		return 0;
	}
};

template<typename T, typename ... Ts>
struct type_index_getter<T, Ts...> {
	static const unsigned int index = type_index_getter<Ts...>::index + 1;

	template<typename U>
	static
	unsigned int get(
			typename std::enable_if<std::is_same<T, U>::value >::type* = 0) {
		return index;
	}

	template<typename U>
	static
	unsigned int get(typename std::enable_if<!std::is_same<T, U>::value >::type* = 0) {
		return type_index_getter<Ts...>::template get<U>();
	}
};

template<typename ... Ts>
struct max_size;

template<>
struct max_size<> {
	static const unsigned int value = 0;
};

template<typename T, typename ... Ts>
struct max_size<T, Ts...> {
	static const unsigned int current_type_size = sizeof(T);
	static const unsigned int next_type_size = max_size<Ts...>::value;
	static const unsigned int value =
			current_type_size > next_type_size ?
					current_type_size : next_type_size;
};

template<typename _SearchType, typename _CurrentType, typename ... _RestTypes>
struct VariantTypeSelector: VariantTypeSelector<_SearchType, _RestTypes...> {
};

template <typename _SearchType, typename... _RestTypes>
struct VariantTypeSelector<_SearchType, _SearchType, _RestTypes...> {
    typedef _SearchType type;
};

template <typename... _Types>
class Variant {
 private:
    typedef std::tuple_size<std::tuple<_Types...>> TypesTupleSize;

 public:

    static const unsigned int maxSize = max_size<_Types...>::value;


    Variant(): valueType_(TypesTupleSize::value) {
    }

    Variant(const Variant& fromVariant):
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
            clear_visitor<maxSize> visitor(valueStorage_);
            apply_void_visitor<clear_visitor<maxSize>, Variant<_Types...>, _Types...>::visit(visitor, *this);
        }
    }

    Variant& operator=(const Variant& rhs)
    {
    	assign_visitor<_Types...> visitor(*this);
    	apply_void_visitor<assign_visitor<_Types...>, Variant<_Types...>, _Types...>::visit(visitor, rhs);
    	return *this;
    }

    Variant& operator=(Variant&& rhs) {
    	assign_visitor<_Types...> visitor(*this);
    	apply_void_visitor<assign_visitor<_Types...>, Variant<_Types...>, _Types...>::visit(visitor, rhs);
    	return *this;
    }

    template<typename _Type>
    typename std::enable_if<!std::is_same<_Type, Variant<_Types...>>::value, Variant<_Types...>&>::type
    operator=(const _Type& value)
    {
    	set<typename select_type<_Type, _Types...>::type>(value);
    	return *this;
    }

    template <typename _Type>
	const bool isType() const {
		typedef typename select_type<_Type, _Types...>::type selected_type_t;
		unsigned int cType = type_index_getter<_Types...>::template get<selected_type_t>();
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
    	set<typename select_type<_Type, _Types...>::type>(value, false);
    }

    template <typename _Type>
    Variant(_Type && value,
    			typename std::enable_if<!std::is_const<_Type>::value>::type* = 0,
    		    typename std::enable_if<!std::is_reference<_Type>::value>::type* = 0,
    		    typename std::enable_if<!std::is_same<_Type, Variant>::value>::type* = 0) {
    	set2<typename select_type<_Type, _Types...>::type>(std::move(value), false);
    }

    //TODO: Return type???
	template <typename _Type>
	const typename VariantTypeSelector<_Type, _Types...>::type & get(bool& success) const {
		typedef typename select_type<_Type, _Types...>::type selected_type_t;
		unsigned int cType = type_index_getter<_Types...>::template get<selected_type_t>();
		if(cType == valueType_) {
			success = true;
			return *(reinterpret_cast<const _Type *>(&valueStorage_));
		} else {
			success = false;
			return *(reinterpret_cast<const _Type *>(&valueStorage_));
		}
	}

	inline size_t getValueType() const {
		return valueType_;
	}

	template<typename U>
	void set( const U& value, const bool clear)	{
		typedef typename select_type<U, _Types...>::type selected_type_t;

		const selected_type_t& type_value = value;
		if(clear) {
			clear_visitor<maxSize> visitor(valueStorage_);
			apply_void_visitor<clear_visitor<maxSize>, Variant<_Types...>, _Types...>::visit(visitor, *this);
		}
		new (&valueStorage_) selected_type_t(std::move(value));
		valueType_ = type_index_getter<_Types...>::template get<selected_type_t>();
	}

	template<typename U>
	void set2( U&& value, const bool clear)	{
		typedef typename select_type<U, _Types...>::type selected_type_t;

		selected_type_t&& any_container_value = std::move(value);
		if(clear)
		{
			clear_visitor<maxSize> visitor(valueStorage_);
			apply_void_visitor<clear_visitor<maxSize>, Variant<_Types...>, _Types...>::visit(visitor, *this);
		} else {
			new (&valueStorage_) selected_type_t(std::move(any_container_value));
		}

		valueType_ = type_index_getter<_Types...>::template get<selected_type_t>();
	}

 private:
    inline bool hasValue() const {
        return valueType_ < TypesTupleSize::value;
    }

    size_t valueType_;
    typename std::aligned_storage<maxSize>::type valueStorage_;

};

template<typename... _Types>
bool operator==(const Variant<_Types...>& lhs, const Variant<_Types...>& rhs)
{
	partial_equals_visitor<_Types...> visitor(lhs);
	return apply_return_visitor<partial_equals_visitor<_Types...>, const Variant<_Types...>, _Types...>::visit(visitor, rhs);
}

template<typename... _Types>
bool operator!=(const Variant<_Types...>& lhs, const Variant<_Types...>& rhs)
{
	return !(lhs == rhs);
}

template<typename T>
struct complete_equals_visitor
{
	public:
		complete_equals_visitor(const T& rhs):
			rhs_(rhs)
		{
		}

		bool
		operator()(const T& lhs) const
		{
			return lhs == rhs_;
		}

		template<typename U>
		bool
		operator()(const U&) const
		{
			return false;
		}

	private:
		const T& rhs_;
};

template<typename... Ts>
struct partial_equals_visitor
{
	public:
		partial_equals_visitor(const Variant<Ts...>& lhs):
			lhs_(lhs)
		{
		}

		template<typename T>
		bool
		operator()(const T& rhs) const
		{
			complete_equals_visitor<T> visitor(rhs);
			return apply_return_visitor<complete_equals_visitor<T>, const Variant<Ts...>, Ts...>::visit(visitor, lhs_);
		}

	private:
		const Variant<Ts...>& lhs_;
};

template<typename ... Ts>
struct assign_visitor {
public:
	assign_visitor(Variant<Ts...>& lhs, const bool clear = true) :
			lhs_(lhs), clear_(clear) {
	}

	template<typename T>
	void operator()(const T& value) const {
		lhs_.template set<T>(value, clear_);
	}

	template<typename T>
	void operator()(T& value) const {
		lhs_.template set<T>(value, clear_);
	}

private:
	Variant<Ts...>& lhs_;
	const bool clear_;
};

} // namespace CommonAPI

#endif // COMMONAPI_SERIALIZABLE_VARIANT_H_
