// Copyright (C) 2015 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef COMMONAPI_ENUMERATION_HPP_
#define COMMONAPI_ENUMERATION_HPP_

namespace CommonAPI {

template <typename _Base>
struct Enumeration {
	Enumeration() = default;
	Enumeration(const _Base &_value)
		: value_(_value) {
	}

	inline Enumeration &operator=(const _Base &_value) {
		value_ = _value;
		return (*this);
	}

	inline operator const _Base() const {
		return value_;
	}

	inline bool operator == (const Enumeration<_Base> &_other) const {
		return value_ == _other.value_;
	}

	inline bool operator != (const Enumeration<_Base> &_other) const {
		return value_ != _other.value_;
	}

	_Base value_;
};

} // namespace CommonAPI

#endif // COMMONAPI_ENUMERATION_HPP_
