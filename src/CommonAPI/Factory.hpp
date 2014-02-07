/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_FACTORY_IMPL_H_
#define COMMONAPI_FACTORY_IMPL_H_

#include "Runtime.h"
#include "ServicePublisher.h"

namespace CommonAPI {

template <template<typename ...> class _ProxyClass, template<typename> class _AttributeExtension>
std::shared_ptr<typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t>
Factory::buildProxyWithDefaultAttributeExtension(const std::string& participantId,
                                        const std::string& serviceName,
                                        const std::string& domain) {

    std::shared_ptr<Proxy> abstractMiddlewareProxy = createProxy(DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t::getInterfaceId(), participantId, serviceName, domain);
    if (abstractMiddlewareProxy) {
        return std::make_shared<typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t>(abstractMiddlewareProxy);
    }
    return NULL;
}

template <template<typename ...> class _ProxyClass, template<typename> class _AttributeExtension>
std::shared_ptr<typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t>
Factory::buildProxyWithDefaultAttributeExtension(const std::string& serviceAddress) {

    std::string domain;
    std::string serviceName;
    std::string participantId;
    if(!splitValidAddress(serviceAddress, domain, serviceName, participantId)) {
        return std::shared_ptr<typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t>();
    }

    return buildProxyWithDefaultAttributeExtension<_ProxyClass, _AttributeExtension>(participantId, serviceName, domain);
}

template<typename _Stub>
COMMONAPI_DEPRECATED bool Factory::registerService(std::shared_ptr<_Stub> stub,
                     const std::string& participantId,
                     const std::string& serviceName,
                     const std::string& domain) {

    std::shared_ptr<StubBase> stubBase = std::dynamic_pointer_cast<StubBase>(stub);
    return registerAdapter(stubBase, _Stub::StubAdapterType::getInterfaceId(), participantId, serviceName, domain);
}

template<typename _Stub>
COMMONAPI_DEPRECATED bool Factory::registerService(std::shared_ptr<_Stub> stub, const std::string& serviceAddress) {
    std::string domain;
    std::string serviceName;
    std::string participantId;
    if(!splitValidAddress(serviceAddress, domain, serviceName, participantId)) {
        return false;
    }

    std::shared_ptr<StubBase> stubBase = std::dynamic_pointer_cast<StubBase>(stub);
    return registerAdapter(stubBase, _Stub::StubAdapterType::getInterfaceId(), participantId, serviceName, domain);
}

COMMONAPI_DEPRECATED inline bool Factory::unregisterService(const std::string& serviceAddress) {
    return runtime_->getServicePublisher()->unregisterService(serviceAddress);
}

} //namespace CommonAPI

#endif // COMMONAPI_FACTORY_IMPL_H_
