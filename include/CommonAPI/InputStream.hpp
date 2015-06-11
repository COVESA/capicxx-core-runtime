// Copyright (C) 2013-2015 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_INPUT_STREAM_HPP_
#define COMMONAPI_INPUT_STREAM_HPP_

#include <unordered_map>

#include <CommonAPI/ByteBuffer.hpp>
#include <CommonAPI/Deployable.hpp>
#include <CommonAPI/Deployment.hpp>
#include <CommonAPI/Enumeration.hpp>
#include <CommonAPI/Struct.hpp>
#include <CommonAPI/Variant.hpp>
#include <CommonAPI/Version.hpp>

namespace CommonAPI {

template<class _Derived>
class InputStream {
public:
    template<class _Deployment>
    InputStream &readValue(bool &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment>
    InputStream &readValue(int8_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment>
    InputStream &readValue(int16_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment>
    InputStream &readValue(int32_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment>
    InputStream &readValue(int64_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment>
    InputStream &readValue(uint8_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment>
    InputStream &readValue(uint16_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment>
    InputStream &readValue(uint32_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment>
    InputStream &readValue(uint64_t &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment>
    InputStream &readValue(float &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment>
    InputStream &readValue(double &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment>
    InputStream &readValue(std::string &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment, typename _Base>
    InputStream &readValue(Enumeration<_Base> &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment, typename... _Types>
    InputStream &readValue(Struct<_Types...> &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment, class _PolymorphicStruct>
    InputStream &readValue(std::shared_ptr<_PolymorphicStruct> &_value,
    					   const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment, typename... _Types>
    InputStream &readValue(Variant<_Types...> &_value, const _Deployment *_depl = nullptr) {
        return get()->readValue(_value, _depl);
    }

    template<class _Deployment, typename _ElementType>
    InputStream &readValue(std::vector<_ElementType> &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment, typename _KeyType, typename _ValueType, typename _HasherType>
    InputStream &readValue(std::unordered_map<_KeyType, _ValueType, _HasherType> &_value,
    					   const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    template<class _Deployment>
    InputStream &readValue(Version &_value, const _Deployment *_depl = nullptr) {
    	return get()->readValue(_value, _depl);
    }

    bool hasError() const {
    	return get()->hasError();
    }

private:
    inline _Derived *get() {
    	return static_cast<_Derived *>(this);
    }

    inline const _Derived *get() const {
    	return static_cast<const _Derived *>(this);
    }
};

template<class _Derived>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, bool &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, int8_t &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, int16_t &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, int32_t &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, int64_t &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, uint8_t &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, uint16_t &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, uint32_t &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, uint64_t &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, float &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, double &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, std::string &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, Version &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived, typename _Base>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, Enumeration<_Base> &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived, typename... _Types>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, Struct<_Types...> &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived, class _PolymorphicStruct>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, std::shared_ptr<_PolymorphicStruct> &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived, typename... _Types>
InputStream<_Derived> & operator>>(InputStream<_Derived> &_input, Variant<_Types...> &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived, typename _ElementType>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, std::vector<_ElementType> &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived, typename _KeyType, typename _ValueType, typename _HasherType>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, std::unordered_map<_KeyType, _ValueType, _HasherType> &_value) {
	return _input.template readValue<EmptyDeployment>(_value);
}

template<class _Derived, typename _Type, typename _TypeDeployment>
InputStream<_Derived> &operator>>(InputStream<_Derived> &_input, Deployable<_Type, _TypeDeployment> &_value) {
	return _input.template readValue<_TypeDeployment>(_value.getValue(), _value.getDepl());
}

} // namespace CommonAPI

#endif // COMMONAPI_INPUT_STREAM_HPP_
