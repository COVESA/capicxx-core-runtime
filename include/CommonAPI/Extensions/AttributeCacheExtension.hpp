// Copyright (C) 2015 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef COMMONAPI_EXTENSIONS_ATTRIBUTE_CACHE_EXTENSION_HPP_
#define COMMONAPI_EXTENSIONS_ATTRIBUTE_CACHE_EXTENSION_HPP_

#include <CommonAPI/CommonAPI.hpp>

#include <cassert>
#include <memory>
#include <type_traits>

namespace CommonAPI {
namespace Extensions {

template<typename _AttributeType,
        bool = (std::is_base_of<
                        CommonAPI::ObservableAttribute<
                                typename _AttributeType::ValueType>,
                        _AttributeType>::value
                        || std::is_base_of<
                                CommonAPI::ObservableReadonlyAttribute<
                                        typename _AttributeType::ValueType>,
                                _AttributeType>::value)>
class AttributeCacheExtension {
};

template<typename _AttributeType>
class AttributeCacheExtension<_AttributeType, false> : public CommonAPI::AttributeExtension<
        _AttributeType> {
protected:
    typedef CommonAPI::AttributeExtension<_AttributeType> __baseClass_t;

    typedef typename _AttributeType::ValueType value_t;

    AttributeCacheExtension(_AttributeType& baseAttribute)
            : CommonAPI::AttributeExtension<_AttributeType>(baseAttribute) {
    }

    ~AttributeCacheExtension() {
    }
};

template<typename _AttributeType>
class AttributeCacheExtension<_AttributeType, true> : public CommonAPI::AttributeExtension<
        _AttributeType> {
    typedef CommonAPI::AttributeExtension<_AttributeType> __baseClass_t;

protected:
    typedef typename _AttributeType::ValueType value_t;
    typedef std::shared_ptr<const value_t> valueptr_t;

    AttributeCacheExtension(_AttributeType& baseAttribute)
            : CommonAPI::AttributeExtension<_AttributeType>(baseAttribute) {
        auto &event = __baseClass_t::getBaseAttribute().getChangedEvent();
        subscription_ =
                event.subscribe(
                        std::bind(
                                &AttributeCacheExtension<_AttributeType, true>::onValueUpdate,
                                this, std::placeholders::_1));
    }

    ~AttributeCacheExtension() {
        auto &event = __baseClass_t::getBaseAttribute().getChangedEvent();
        event.unsubscribe(subscription_);
    }

public:
    /**
     * @brief getCachedValue Retrieve attribute value from the cache
     * @return The value of the attribute or a null pointer if the value is not
     *         yet available. Retrieving a non-cached value will trigger
     *         retrieval of the value. Changes to the cached value are emitted
     *         via the getChangedEvent.
     */
    valueptr_t getCachedValue() {
        if (cachedValue_) {
            return cachedValue_;
        }

        // This may get called more than once if a previous retrieval is still
        // on-going (saving the current state would just take up extra resources)
        __baseClass_t::getBaseAttribute().getValueAsync(
                std::bind(&AttributeCacheExtension<_AttributeType, true>::valueRetrieved,
                          this, std::placeholders::_1, std::placeholders::_2));

        return nullptr;
    }

    /**
     * @brief getCachedValue Retrieve attribute value from the cache returning a
     *                       default value if the cache was empty.
     * @param errorValue The value to return if the value could not be found in
     *                   the cache.
     * @return The value of the attribute or errorValue.
     */
    valueptr_t getCachedValue(const value_t &errorValue) {
        valueptr_t result = getCachedValue();

        if (!result)
            result = std::make_shared<const value_t>(errorValue);

        return result;
    }

private:

    void valueRetrieved(const CommonAPI::CallStatus &callStatus, value_t t) {
        if (callStatus == CommonAPI::CallStatus::SUCCESS) {
            assert(!cachedValue_ || *cachedValue_ == t);
            onValueUpdate(t);
        }
    }

    void onValueUpdate(const value_t& t) {
        cachedValue_ = std::make_shared<const value_t>(t);
    }

    valueptr_t cachedValue_;

    typename _AttributeType::ChangedEvent::Subscription subscription_;
};

} // namespace Extensions
} // namespace CommonAPI

#endif // COMMONAPI_EXTENSIONS_ATTRIBUTE_CACHE_EXTENSION_HPP_
