/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
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
#include "Runtime.h"
#include "Stub.h"


namespace CommonAPI {


class Factory;
class Runtime;
class MiddlewareInfo;


template<template<typename ...> class _ProxyType, template<typename> class _AttributeExtension>
struct DefaultAttributeProxyFactoryHelper;


template<template<typename ...> class _ProxyClass, template<typename> class _AttributeExtension>
std::shared_ptr<typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t> createProxyWithDefaultAttributeExtension(Factory* specificFactory, const std::string& participantId, const std::string& domain);


class Factory {
 public:
    Factory(const std::shared_ptr<Runtime> runtime,
            const MiddlewareInfo* middlewareInfo):
                runtime_(runtime),
                middlewareInfo_(middlewareInfo) {
    }

    virtual ~Factory() {}

    template<template<typename ...> class _ProxyClass, typename ... _AttributeExtensions>
    std::shared_ptr<_ProxyClass<_AttributeExtensions...> >
    buildProxy(const std::string& participantId,
               const std::string& domain) {

    	std::shared_ptr<Proxy> abstractMiddlewareProxy = createProxy(_ProxyClass<_AttributeExtensions...>::getInterfaceName(), participantId, domain);
    	return std::make_shared<_ProxyClass<_AttributeExtensions...>>(abstractMiddlewareProxy);
    }

    template<template<typename ...> class _ProxyClass, typename ... _AttributeExtensions >
    std::shared_ptr<_ProxyClass<_AttributeExtensions...> >
    buildProxy(const std::string& serviceAddress) {

		std::string domain;
		std::string serviceName;
		std::string participantId;
		if(!splitAddress(serviceAddress, domain, serviceName, participantId)) {
			return false;
		}

		return buildProxy<_ProxyClass, _AttributeExtensions...>(participantId, domain);
    }

    template <template<typename ...> class _ProxyClass, template<typename> class _AttributeExtension>
    std::shared_ptr<typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t>
    buildProxyWithDefaultAttributeExtension(const std::string& participantId,
                                       const std::string& domain) {

    	std::shared_ptr<Proxy> abstractMiddlewareProxy = createProxy(DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t::getInterfaceName(), participantId, domain);
    	return std::make_shared<typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t>(abstractMiddlewareProxy);
    }

    template <template<typename ...> class _ProxyClass, template<typename> class _AttributeExtension>
    std::shared_ptr<typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t>
    buildProxyWithDefaultAttributeExtension(const std::string& serviceAddress) {

		std::string domain;
		std::string serviceName;
		std::string participantId;
		if(!splitAddress(serviceAddress, domain, serviceName, participantId)) {
			return false;
		}

		return buildProxyWithDefaultAttributeExtension<_ProxyClass, _AttributeExtension>(participantId, domain);
    }

    inline std::shared_ptr<Runtime> getRuntime() {
        return runtime_;
    }

    template<typename _Stub>
    bool registerService(std::shared_ptr<_Stub> stub,
    				     const std::string& participantId,
            			 const std::string& domain) {

    	std::shared_ptr<StubBase> stubBase = std::dynamic_pointer_cast<StubBase>(stub);
		std::shared_ptr<CommonAPI::StubAdapter> stubAdapter = createAdapter(stubBase, _Stub::StubAdapterType::getInterfaceName(), participantId, domain);
		if(!stubAdapter) {
			return false;
		}

		std::string address = domain + ":" + _Stub::StubAdapterType::getInterfaceName() + ":" + participantId;
		registeredServices_.insert( {std::move(address), stubAdapter} );

		return true;
    }

    template<typename _Stub>
    bool registerService(std::shared_ptr<_Stub> stub, const std::string& serviceAddress) {

    	if(registeredServices_.find(serviceAddress) != registeredServices_.end()) {
    		return false;
    	}

		std::string domain;
		std::string serviceName;
		std::string participantId;
		if(!splitAddress(serviceAddress, domain, serviceName, participantId)) {
			return false;
		}

		return registerService<_Stub>(stub, participantId, domain);
    }

    inline bool unregisterService(const std::string& serviceAddress) {
    	return registeredServices_.erase(serviceAddress);
    }

    virtual std::vector<std::string> getAvailableServiceInstances(const std::string& serviceName, const std::string& serviceDomainName = "local") = 0;

    virtual bool isServiceInstanceAlive(const std::string& serviceInstanceID, const std::string& serviceName, const std::string& serviceDomainName = "local") = 0;

 protected:
    virtual std::shared_ptr<Proxy> createProxy(const char* interfaceName, const std::string& participantId, const std::string& domain) = 0;
    virtual std::shared_ptr<StubAdapter> createAdapter(std::shared_ptr<StubBase> stubBase, const char* interfaceName, const std::string& participantId, const std::string& domain) = 0;

 private:
    std::shared_ptr<Runtime> runtime_;
    std::unordered_map<std::string, std::shared_ptr<CommonAPI::StubAdapter>> registeredServices_;

    const MiddlewareInfo* middlewareInfo_;

    inline bool splitAddress(const std::string& serviceAddress, std::string& domain, std::string& serviceName, std::string& participantId) {
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
};


} // namespace CommonAPI

#endif // COMMONAPI_FACTORY_H_
