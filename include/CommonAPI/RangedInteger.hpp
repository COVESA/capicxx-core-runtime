// Copyright (C) 2015-2020 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.hpp> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_RANGEDINTEGER_HPP
#define COMMONAPI_RANGEDINTEGER_HPP

namespace CommonAPI {
    template <int minimum, int maximum>
    struct RangedInteger {

    	int value_;

    	RangedInteger(int _value):
    		value_(_value) {
    		// assert(_value >= minimum && _value <= maximum);
    	}
    	RangedInteger():
    		value_(minimum) {
    	}
    	~RangedInteger() {}

    	inline RangedInteger& operator= (const int _value) {
    		// assert(_value >= minimum && _value <= maximum);
    		value_ = _value;
    		return (*this);
    	}
        inline operator int() const {
            return value_;
    	}

        inline bool operator==(const RangedInteger &_other) const {
            return (value_ == _other.value_);
        }

        inline bool operator!=(const RangedInteger &_other) const {
            return (value_ != _other.value_);
        }

        inline bool operator<(const RangedInteger &_other) const {
            return (value_ < _other.value_);
        }

        inline bool operator<=(const RangedInteger &_other) const {
            return (value_ <= _other.value_);
        }

        inline bool operator>(const RangedInteger &_other) const {
            return (value_ > _other.value_);
        }

        inline bool operator>=(const RangedInteger &_other) const {
            return (value_ >= _other.value_);
        }

        inline bool operator==(const int &_other) const {
            return (value_ == _other);
        }

        inline bool operator!=(const int &_other) const {
            return (value_ != _other);
        }

        inline bool operator<(const int &_other) const {
            return (value_ < _other);
        }

        inline bool operator<=(const int &_other) const {
            return (value_ <= _other);
        }

        inline bool operator>(const int &_other) const {
            return (value_ > _other);
        }

        inline bool operator>=(const int &_other) const {
            return (value_ >= _other);
        }

        bool validate() const {
    		return value_ <= maximum && value_ >= minimum;
    	}
    };
}

#endif
