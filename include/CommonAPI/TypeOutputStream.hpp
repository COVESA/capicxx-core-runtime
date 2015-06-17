// Copyright (C) 2014-2015 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_TYPEOUTPUTSTREAM_HPP_
#define COMMONAPI_TYPEOUTPUTSTREAM_HPP_

#include <unordered_map>

#include <CommonAPI/Struct.hpp>
#include <CommonAPI/Variant.hpp>
#include <CommonAPI/Types.hpp>

namespace CommonAPI {

template<class _Derived>
class TypeOutputStream {
public:
	inline TypeOutputStream &writeType(const bool &_value) {
		return get()->writeType(_value);
	}

	inline TypeOutputStream &writeType(const int8_t &_value) {
		return get()->writeType(_value);
	}

	inline TypeOutputStream &writeType(const int16_t &_value) {
		return get()->writeType(_value);
	}

	inline TypeOutputStream &writeType(const int32_t &_value) {
		return get()->writeType(_value);
	}

	inline TypeOutputStream &writeType(const int64_t &_value) {
		return get()->writeType(_value);
	}

	inline TypeOutputStream &writeType(const uint8_t &_value) {
		return get()->writeType(_value);
	}

	inline TypeOutputStream &writeType(const uint16_t &_value) {
		return get()->writeType(_value);
	}

	inline TypeOutputStream &writeType(const uint32_t &_value) {
		return get()->writeType(_value);
	}

	inline TypeOutputStream &writeType(const uint64_t &_value) {
		return get()->writeType(_value);
	}

	inline TypeOutputStream &writeType(const float &_value) {
		return get()->writeType(_value);
	}

	inline TypeOutputStream &writeType(const double &_value) {
		return get()->writeType(_value);
	}

	inline TypeOutputStream &writeType(const std::string &_value) {
		return get()->writeType(_value);
	}

	inline TypeOutputStream &writeType(const Version &_value) {
		return get()->writeType(_value);
	}

	template<typename _Type>
	TypeOutputStream &writeType(const Enumeration<_Type> &_value) {
		_Type tmpValue;
		return get()->writeType(tmpValue);
	}

	template<typename... _Types>
	TypeOutputStream &writeType(const Struct<_Types...> &_value) {
		return get()->writeType(_value);
	}

    template<class _PolymorphicStruct>
    TypeOutputStream &writeType(const std::shared_ptr<_PolymorphicStruct> &_value) {
        return get()->writeType(_value);
    }

	template<typename... _Types>
	TypeOutputStream &writeType(const Variant<_Types...> &_value) {
		return get()->writeType(_value);
	}

	template<typename _ElementType>
	TypeOutputStream &writeType(const std::vector<_ElementType> &_value) {
		return get()->writeType(_value);
	}

	template<typename _KeyType, typename _ValueType, typename _HasherType>
	TypeOutputStream &writeType(const std::unordered_map<_KeyType, _ValueType, _HasherType> &_value) {
		return get()->writeType(_value);
	}

private:
	inline _Derived *get() {
		return static_cast<_Derived *>(this);
	}
};

template<class _Derived>
inline TypeOutputStream<_Derived> &operator<<(TypeOutputStream<_Derived> &_output, const bool &_value) {
    return _output.writeType(_value);
}

template<class _Derived>
inline TypeOutputStream<_Derived>& operator<<(TypeOutputStream<_Derived> &_output, const int8_t &_value) {
    return _output.writeType(_value);
}

template<class _Derived>
inline TypeOutputStream<_Derived>& operator<<(TypeOutputStream<_Derived> &_output, const int16_t &_value) {
    return _output.writeType(_value);
}

template<class _Derived>
inline TypeOutputStream<_Derived>& operator<<(TypeOutputStream<_Derived> &_output, const int32_t &_value) {
    return _output.writeType(_value);
}

template<class _Derived>
inline TypeOutputStream<_Derived>& operator<<(TypeOutputStream<_Derived> &_output, const int64_t &_value) {
    return _output.writeType(_value);
}

template<class _Derived>
inline TypeOutputStream<_Derived>& operator<<(TypeOutputStream<_Derived> &_output, const uint8_t &_value) {
    return _output.writeType(_value);
}

template<class _Derived>
inline TypeOutputStream<_Derived>& operator<<(TypeOutputStream<_Derived> &_output, const uint16_t &_value) {
    return _output.writeType(_value);
}

template<class _Derived>
inline TypeOutputStream<_Derived>& operator<<(TypeOutputStream<_Derived> &_output, const uint32_t &_value) {
    return _output.writeType(_value);
}

template<class _Derived>
inline TypeOutputStream<_Derived>& operator<<(TypeOutputStream<_Derived> &_output, const uint64_t &_value) {
    return _output.writeType(_value);
}

template<class _Derived>
inline TypeOutputStream<_Derived>& operator<<(TypeOutputStream<_Derived> &_output, const float &_value) {
    return _output.writeType(_value);
}

template<class _Derived>
inline TypeOutputStream<_Derived>& operator<<(TypeOutputStream<_Derived> &_output, const double &_value) {
    return _output.writeType(_value);
}

template<class _Derived>
inline TypeOutputStream<_Derived>& operator<<(TypeOutputStream<_Derived> &_output, const std::string &_value) {
    return _output.writeType(_value);
}

template<class _Derived, typename _Type, typename _TypeDepl>
inline TypeOutputStream<_Derived> &operator<<(TypeOutputStream<_Derived> &_output, const Deployable<_Type, _TypeDepl> &_value) {
	return _output.writeType(_value.getValue());
}

template<class _Derived>
inline TypeOutputStream<_Derived>& operator<<(TypeOutputStream<_Derived> &_output, const Version &_value) {
    return _output.writeType(_value);
}

template<class _Derived, typename... _Types>
TypeOutputStream<_Derived> &operator<<(TypeOutputStream<_Derived> &_output, const Struct<_Types...> &_value) {
	return _output.writeType(_value);
}

template<class _Derived, class _PolymorphicStruct>
TypeOutputStream<_Derived> &operator<<(TypeOutputStream<_Derived> &_output, const std::shared_ptr<_PolymorphicStruct> &_value) {
    return _output.writeType(_value);
}

template<class _Derived, typename... _Types>
TypeOutputStream<_Derived> &operator<<(TypeOutputStream<_Derived> &_output, const Variant<_Types...> &_value) {
	return _output.writeType(_value);
}

template<class _Derived, typename _ElementType>
TypeOutputStream<_Derived> &operator<<(TypeOutputStream<_Derived> &_output, const std::vector<_ElementType> &_value) {
	return _output.writeType(_value);
}

template<class _Derived, typename _KeyType, typename _ValueType, typename _HasherType>
TypeOutputStream<_Derived> &operator<<(TypeOutputStream<_Derived> &_output,
                         	 	 	   const std::unordered_map<_KeyType, _ValueType, _HasherType> &_value) {
	return _output.writeType(_value);
}

} // namespace CommonAPI

#endif // COMMONAPI_TYPEOUTPUTSTREAM_HPP_
