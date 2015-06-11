// Copyright (C) 2014-2015 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_DEPLOYABLE_HPP_
#define COMMONAPI_DEPLOYABLE_HPP_

namespace CommonAPI {

struct DeployableBase {
};

template<typename _Type, typename _TypeDepl>
struct Deployable : DeployableBase
{
	Deployable(const _TypeDepl *_depl = nullptr)
		: depl_(const_cast<_TypeDepl *>(_depl)) {
	}

	Deployable(const _Type &_value, const _TypeDepl *_depl)
		: value_(_value),
		  depl_(const_cast<_TypeDepl *>(_depl)) {
    };

    Deployable<_Type, _TypeDepl>& operator=(const Deployable<_Type, _TypeDepl> &_source) {
        value_ = _source.value_;
        depl_ = _source.depl_;
        return (*this);
    }

    operator _Type() const {
    	return value_;
    }

    const _Type &getValue() const {
    	return value_;
    }

	_Type &getValue() {
        return value_;
    }

	const _TypeDepl *getDepl() const {
		return depl_;
	}

 protected:
	_Type value_;
	_TypeDepl *depl_;
};

} // namespace CommonAPI

#endif // COMMONAPI_DEPLOYABLE_HPP_
