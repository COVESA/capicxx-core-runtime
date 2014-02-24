/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_PROXY_MANAGER_H_
#define COMMONAPI_PROXY_MANAGER_H_

#include "types.h"
#include "Event.h"
#include "Proxy.h"
#include "Factory.h"

#include <functional>
#include <future>
#include <string>
#include <vector>


namespace CommonAPI {

class ProxyManager {
 public:
    typedef std::function<void(const CallStatus&, const std::vector<std::string>&)> GetAvailableInstancesCallback;
    typedef std::function<void(const CallStatus&, const AvailabilityStatus&)> GetInstanceAvailabilityStatusCallback;

    typedef Event<std::string, AvailabilityStatus> InstanceAvailabilityStatusChangedEvent;

    ProxyManager() { };
    ProxyManager(ProxyManager&&) = delete;
    ProxyManager(const ProxyManager&) = delete;

    virtual ~ProxyManager() { }

    virtual void getAvailableInstances(CommonAPI::CallStatus&, std::vector<std::string>& availableInstances) = 0;
	virtual std::future<CallStatus> getAvailableInstancesAsync(GetAvailableInstancesCallback callback) = 0;

    virtual void getInstanceAvailabilityStatus(const std::string& instanceAddress,
                                               CallStatus& callStatus,
                                               AvailabilityStatus& availabilityStatus) = 0;

    virtual std::future<CallStatus> getInstanceAvailabilityStatusAsync(const std::string&,
                                                                       GetInstanceAvailabilityStatusCallback callback) = 0;

    virtual InstanceAvailabilityStatusChangedEvent& getInstanceAvailabilityStatusChangedEvent() = 0;

    template<template<typename ...> class _ProxyClass, typename ... _AttributeExtensions>
    std::shared_ptr<
    _ProxyClass<
#ifdef WIN32
        CommonAPI::WINDummyAttributeExtension<WINDummyAttribute>,
#endif
        _AttributeExtensions...>
    >
    buildProxy(const std::string& instanceName) {
        std::shared_ptr<Proxy> abstractMiddlewareProxy = createProxy(instanceName);
        if (abstractMiddlewareProxy) {
            auto returnProxy = std::make_shared<
                _ProxyClass<
#ifdef WIN32
                CommonAPI::WINDummyAttributeExtension<WINDummyAttribute>,
#endif
                _AttributeExtensions...>
            >(abstractMiddlewareProxy);
            return returnProxy;
        }
        return NULL;
    }

    template <template<typename ...> class _ProxyClass, template<typename> class _AttributeExtension>
    std::shared_ptr<typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t>
    buildProxyWithDefaultAttributeExtension(const std::string& instanceName) {
        std::shared_ptr<Proxy> abstractMiddlewareProxy = createProxy(instanceName);
        if (abstractMiddlewareProxy) {
            return std::make_shared<typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t>(abstractMiddlewareProxy);
        }
        return NULL;
    }

 protected:
    virtual std::shared_ptr<Proxy> createProxy(const std::string& instanceName) = 0;
};

} // namespace CommonAPI

#endif // COMMONAPI_PROXY_MANAGER_H_
