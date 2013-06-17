/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_FACTORY_H_
#define COMMONAPI_FACTORY_H_

#include <cassert>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "MiddlewareInfo.h"
#include "Proxy.h"
#include "Stub.h"
#include "types.h"


namespace CommonAPI {


class Factory;
class ServicePublisher;


template<template<typename ...> class _ProxyType, template<typename> class _AttributeExtension>
struct DefaultAttributeProxyFactoryHelper;


template<template<typename ...> class _ProxyClass, template<typename> class _AttributeExtension>
std::shared_ptr<typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t> createProxyWithDefaultAttributeExtension(Factory* specificFactory, const std::string& participantId, const std::string& domain);


/**
 * \brief The main CommonAPI access class. A factory is responsible for creation and destruction of service objects.
 *
 * The main CommonAPI access class. A factory is responsible for creation and destruction of service objects.
 * This includes proxies and stubs. It also provides service discovery methods.
 */
class Factory {
 public:
    typedef std::function<void(std::vector<std::string>&) > GetAvailableServiceInstancesCallback;
    typedef std::function<void(bool)> IsServiceInstanceAliveCallback;

    /**
     * \brief Creates factory. Don't call manually.
     *
     * Creates factory. Don't call manually.
     */
    Factory(const std::shared_ptr<Runtime> runtime,
            const MiddlewareInfo* middlewareInfo):
                runtime_(runtime),
                middlewareInfo_(middlewareInfo) {
    }

    /**
     * \brief Creates factory. Don't call manually.
     *
     * Creates factory. Don't call manually.
     */
    Factory(const std::shared_ptr<Runtime> runtime,
            const MiddlewareInfo* middlewareInfo,
            const std::string factoryName,
            const bool nullOnInvalidName) :
                    runtime_(runtime),
                    middlewareInfo_(middlewareInfo) {
    }

    virtual ~Factory() {}

    /**
     * \brief Build a proxy for the specified address
     *
     * Build a proxy for the specified address.
     * Template this method call for the desired proxy type and attribute extension.
     *
     * @param participantId The participant ID of the common API address (last part)
     * @param serviceName The service name of the common API address (middle part)
     * @param domain The domain of the common API address (first part)
     * @return a shared pointer to the constructed proxy
     */
    template<template<typename ...> class _ProxyClass, typename ... _AttributeExtensions>
    std::shared_ptr<_ProxyClass<_AttributeExtensions...> >
    buildProxy(const std::string& participantId,
               const std::string& serviceName,
               const std::string& domain);

    /**
     * \brief Build a proxy for the specified address
     *
     * Build a proxy for the specified address.
     * Template this method call for the desired proxy type and attribute extensions.
     *
     * @param serviceAddress The common API address
     * @return a shared pointer to the constructed proxy
     */
    template<template<typename ...> class _ProxyClass, typename ... _AttributeExtensions >
    std::shared_ptr<_ProxyClass<_AttributeExtensions...> >
    buildProxy(const std::string& serviceAddress);

    /**
     * \brief Build a proxy for the specified address with one extension for all attributes
     *
     * Build a proxy for the specified address with one extension for all attributes
     * Template this method call for the desired proxy type and attribute extensions.
     *
     * @param participantId The participant ID of the common API address (last part)
     * @param serviceName The service name of the common API address (middle part)
     * @param domain The domain of the common API address (first part)
     * @return a shared pointer to the constructed proxy
     */
    template <template<typename ...> class _ProxyClass, template<typename> class _AttributeExtension>
    std::shared_ptr<typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t>
    buildProxyWithDefaultAttributeExtension(const std::string& participantId,
                                            const std::string& serviceName,
                                            const std::string& domain);

    /**
     * \brief Build a proxy for the specified address with one extension for all attributes
     *
     * Build a proxy for the specified address with one extension for all attributes
     * Template this method call for the desired proxy type attribute extension.
     *
     * @param serviceAddress The common API address
     * @return a shared pointer to the constructed proxy
     */
    template <template<typename ...> class _ProxyClass, template<typename> class _AttributeExtension>
    std::shared_ptr<typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t>
    buildProxyWithDefaultAttributeExtension(const std::string& serviceAddress);

    /**
     * \brief Get a pointer to the runtime of this factory.
     *
     * Get a pointer to the runtime of this factory.
     *
     * @return the Runtime
     */
    inline std::shared_ptr<Runtime> getRuntime();

    /**
     * \brief Register a service stub under a specified address
     *
     * Register a service stub under a specified address. The service will be registered
     * with the ServicePublisher that is provided by the runtime which you also retrieved
     * this factory from. It is recommended to use the ServicePublisher for registering
     * and unregistering purposes.
     *
     * @param stub The stub pointer
     * @param participantId The participant ID of the common API address (last part)
     * @param serviceName The service name of the common API address (middle part)
     * @param domain The domain of the common API address (first part)
     * @return Was the registration successful
     *
     * @deprecated Use CommonAPI::Runtime->getServicePublisher()->registerService() instead.
     * Purpose for this change is to make service management (esp. deregistering) independent
     * from factory instances.
     */
    template<typename _Stub>
    COMMONAPI_DEPRECATED bool registerService(std::shared_ptr<_Stub> stub,
                         const std::string& participantId,
                         const std::string& serviceName,
                         const std::string& domain);

    /**
     * \brief Register a service stub under a specified address
     *
     * Register a service stub under a specified address. The service will be registered
     * with the ServicePublisher that is provided by the runtime which you also retrieved
     * this factory from. It is recommended to use the ServicePublisher for registering
     * and unregistering purposes.
     *
     * @param stub The stub pointer
     * @param serviceAddress The common API address
     * @return Was the registration successful
     *
     * @deprecated Use CommonAPI::Runtime->getServicePublisher()->registerService() instead.
     * Purpose for this change is to make service management (esp. deregistering) independent
     * from factory instances.
     */
    template<typename _Stub>
    COMMONAPI_DEPRECATED bool registerService(std::shared_ptr<_Stub> stub, const std::string& serviceAddress);

    /**
     * \brief Unregister a service stub associated with a specified address
     *
     * Unregister a service stub associated with a specified address.
     *
     * @param participantId The participant ID of the common API address (last part)
     * @param serviceName The service name of the common API address (middle part)
     * @param domain The domain of the common API address (first part)
     * @return Was the deregistration successful
     *
     * @deprecated Use CommonAPI::Runtime->getServicePublisher()->unregisterService() instead.
     * Purpose for this change is to make service management (esp. deregistering) independent
     * from factory instances.
     */
    COMMONAPI_DEPRECATED virtual bool unregisterService(const std::string& participantId, const std::string& serviceName, const std::string& domain) = 0;

    /**
     * \brief Unregister a service stub associated with a specified address
     *
     * Unregister a service stub associated with a specified address
     *
     * @param serviceAddress The common API address
     * @return Was the deregistration successful
     *
     * @deprecated Use CommonAPI::Runtime->getServicePublisher()->unregisterService() instead.
     * Purpose for this change is to make service management (esp. deregistering) independent
     * from factory instances.
     */
    COMMONAPI_DEPRECATED bool unregisterService(const std::string& serviceAddress);

    /**
     * \brief Get all instances of a specific service name available. Synchronous call.
     *
     * Get all instances of a specific service name available. Synchronous call.
     *
     * @param serviceName The service name of the common API address (middle part)
     * @param serviceDomainName The domain of the common API address (first part)
     * @return A vector of strings containing the available complete common api addresses.
     */
    virtual std::vector<std::string> getAvailableServiceInstances(const std::string& serviceName, const std::string& serviceDomainName = "local") = 0;

    /**
     * \brief Is a particular complete common api address available. Synchronous call.
     *
     * Is a particular complete common api address available. Synchronous call.
     *
     * @param serviceAddress The common API address
     * @return Is alive
     */
    virtual bool isServiceInstanceAlive(const std::string& serviceAddress) = 0;

    /**
     * \brief Is a particular complete common api address available. Synchronous call.
     *
     * Is a particular complete common api address available. Synchronous call.
     *
     * @param serviceInstanceID The participant ID of the common API address (last part)
     * @param serviceName The service name of the common API address (middle part)
     * @param serviceDomainName The domain of the common API address (first part)
     * @return Is alive
     */
    virtual bool isServiceInstanceAlive(const std::string& serviceInstanceID, const std::string& serviceName, const std::string& serviceDomainName = "local") = 0;

    /**
     * \brief Get all instances of a specific service name available. Asynchronous call.
     *
     * Get all instances of a specific service name available. Asynchronous call.
     *
     * @param serviceName The service name of the common API address (middle part)
     * @param serviceDomainName The domain of the common API address (first part)
     */
    virtual void getAvailableServiceInstancesAsync(GetAvailableServiceInstancesCallback callback, const std::string& serviceName, const std::string& serviceDomainName = "local") = 0;

    /**
     * \brief Tells whether a particular service instance is available. Asynchronous call.
     *
     * Tells whether a particular service instance is available. Asynchronous call.
     *
     * @param serviceAddress The common API address of the service
     */
    virtual void isServiceInstanceAliveAsync(IsServiceInstanceAliveCallback callback, const std::string& serviceAddress) = 0;

    /**
     * \brief Tells whether a particular service instance is available. Asynchronous call.
     *
     * Tells whether a particular service instance is available. Asynchronous call.
     *
     * @param serviceInstanceID The participant ID of the common API address (last part) of the service
     * @param serviceName The service name of the common API address (middle part) of the service
     * @param serviceDomainName The domain of the common API address (first part) of the service
     */
    virtual void isServiceInstanceAliveAsync(IsServiceInstanceAliveCallback callback, const std::string& serviceInstanceID, const std::string& serviceName, const std::string& serviceDomainName = "local") = 0;

 protected:
    virtual std::shared_ptr<Proxy> createProxy(const char* interfaceId, const std::string& participantId, const std::string& serviceName, const std::string& domain) = 0;
    virtual bool registerAdapter(std::shared_ptr<StubBase> stubBase, const char* interfaceId, const std::string& participantId, const std::string& serviceName, const std::string& domain) = 0;

 private:
    std::shared_ptr<Runtime> runtime_;

    const MiddlewareInfo* middlewareInfo_;

    inline bool splitValidAddress(const std::string& serviceAddress, std::string& domain, std::string& serviceName, std::string& participantId) {
        std::istringstream addressStream(serviceAddress);
        if(!std::getline(addressStream, domain, ':')) {
            return false;
        }
        if(!std::getline(addressStream, serviceName, ':')) {
            return false;
        }
        if(!std::getline(addressStream, participantId, ':')) {
            return false;
        }
        if(std::getline(addressStream, participantId)) {
            return false;
        }
        return true;
    }

    friend class ServicePublisher;
};


} // namespace CommonAPI

#include "Factory.hpp"

#endif // COMMONAPI_FACTORY_H_
