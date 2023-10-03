// Copyright (C) 2013-2020 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.hpp> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_RUNTIME_HPP_
#define COMMONAPI_RUNTIME_HPP_

#include <map>
#include <memory>
#include <mutex>
#include <set>

#include <CommonAPI/AttributeExtension.hpp>
#include <CommonAPI/Export.hpp>
#include <CommonAPI/Factory.hpp>
#include <CommonAPI/Types.hpp>

namespace CommonAPI {

static const ConnectionId_t DEFAULT_CONNECTION_ID = "";

class MainLoopContext;
class Proxy;
class ProxyManager;
class StubBase;

class COMMONAPI_EXPORT_CLASS_EXPLICIT Runtime {
public:
    COMMONAPI_METHOD_EXPORT static std::string getProperty(const std::string &_name);
    COMMONAPI_METHOD_EXPORT static void setProperty(const std::string &_name, const std::string &_value);

    COMMONAPI_METHOD_EXPORT static std::shared_ptr<Runtime> get();

    COMMONAPI_METHOD_EXPORT Runtime();
    COMMONAPI_METHOD_EXPORT virtual ~Runtime();

    template<template<typename ...> class ProxyClass_, typename ... AttributeExtensions_>
    COMMONAPI_METHOD_EXPORT std::shared_ptr<
        ProxyClass_<AttributeExtensions_...>
    >
    buildProxy(const std::string &_domain,
               const std::string &_instance,
               const ConnectionId_t &_connectionId = DEFAULT_CONNECTION_ID) {
        std::shared_ptr<Proxy> proxy
            = createProxy(_domain,
                          ProxyClass_<AttributeExtensions_...>::getInterface(),
                          _instance,
                          _connectionId);

        if (proxy) {
            return std::make_shared<ProxyClass_<AttributeExtensions_...>>(proxy);
        }
        return nullptr;
    }

    template<template<typename ...> class ProxyClass_, typename ... AttributeExtensions_>
    COMMONAPI_METHOD_EXPORT std::shared_ptr<
        ProxyClass_<AttributeExtensions_...>
    >
    buildProxy(const std::string &_domain,
               const std::string &_instance,
               std::shared_ptr<MainLoopContext> _context) {
        std::shared_ptr<Proxy> proxy
            = createProxy(_domain,
                          ProxyClass_<AttributeExtensions_...>::getInterface(),
                          _instance,
                          _context);
        if (proxy) {
            return std::make_shared<ProxyClass_<AttributeExtensions_...>>(proxy);
        }
        return nullptr;
    }

    template <template<typename ...> class ProxyClass_, template<typename> class AttributeExtension_>
    COMMONAPI_METHOD_EXPORT std::shared_ptr<typename DefaultAttributeProxyHelper<ProxyClass_, AttributeExtension_>::class_t>
    buildProxyWithDefaultAttributeExtension(const std::string &_domain,
                                            const std::string &_instance,
                                            const ConnectionId_t &_connectionId = DEFAULT_CONNECTION_ID) {
        std::shared_ptr<Proxy> proxy
            = createProxy(_domain,
                           DefaultAttributeProxyHelper<ProxyClass_, AttributeExtension_>::class_t::getInterface(),
                          _instance,
                          _connectionId);
        if (proxy) {
            return std::make_shared<typename DefaultAttributeProxyHelper<ProxyClass_, AttributeExtension_>::class_t>(proxy);
        }
        return nullptr;
    }

    template <template<typename ...> class ProxyClass_, template<typename> class AttributeExtension_>
    COMMONAPI_METHOD_EXPORT std::shared_ptr<typename DefaultAttributeProxyHelper<ProxyClass_, AttributeExtension_>::class_t>
    buildProxyWithDefaultAttributeExtension(const std::string &_domain,
                                            const std::string &_instance,
                                            std::shared_ptr<MainLoopContext> _context) {
        std::shared_ptr<Proxy> proxy
            = createProxy(_domain,
                           DefaultAttributeProxyHelper<ProxyClass_, AttributeExtension_>::class_t::getInterface(),
                          _instance,
                          _context);
        if (proxy) {
            return std::make_shared<typename DefaultAttributeProxyHelper<ProxyClass_, AttributeExtension_>::class_t>(proxy);
        }
        return nullptr;
    }

    template<typename Stub_>
    COMMONAPI_METHOD_EXPORT bool registerService(const std::string &_domain,
                         const std::string &_instance,
                         std::shared_ptr<Stub_> _service,
                         const ConnectionId_t &_connectionId = DEFAULT_CONNECTION_ID) {
        return registerStub(_domain, Stub_::StubInterface::getInterface(), _instance, _service, _connectionId);
    }

    template<typename Stub_>
    COMMONAPI_METHOD_EXPORT bool registerService(const std::string &_domain,
                         const std::string &_instance,
                         std::shared_ptr<Stub_> _service,
                         std::shared_ptr<MainLoopContext> _context) {
        return registerStub(_domain, Stub_::StubInterface::getInterface(), _instance, _service, _context);
    }

    COMMONAPI_METHOD_EXPORT bool unregisterService(const std::string &_domain,
                            const std::string &_interface,
                            const std::string &_instance) {
        return unregisterStub(_domain, _interface, _instance);
    }

    COMMONAPI_METHOD_EXPORT bool registerFactory(const std::string &_ipc, std::shared_ptr<Factory> _factory);
    COMMONAPI_METHOD_EXPORT bool unregisterFactory(const std::string &_ipc);

    inline const std::string &getDefaultBinding() const { return defaultBinding_; }

    COMMONAPI_METHOD_EXPORT void initFactories();
    COMMONAPI_METHOD_EXPORT Timeout_t getDefaultCallTimeout() const;

private:
    COMMONAPI_METHOD_EXPORT void init();
    COMMONAPI_METHOD_EXPORT bool readConfiguration();
    COMMONAPI_METHOD_EXPORT bool splitAddress(const std::string &, std::string &, std::string &, std::string &);

    COMMONAPI_METHOD_EXPORT std::shared_ptr<Proxy> createProxy(const std::string &, const std::string &, const std::string &,
                                       const ConnectionId_t &);
    COMMONAPI_METHOD_EXPORT std::shared_ptr<Proxy> createProxy(const std::string &, const std::string &, const std::string &,
                                       std::shared_ptr<MainLoopContext>);

    COMMONAPI_METHOD_EXPORT std::shared_ptr<Proxy> createProxyHelper(const std::string &, const std::string &, const std::string &,
                                             const ConnectionId_t &, bool);
    COMMONAPI_METHOD_EXPORT std::shared_ptr<Proxy> createProxyHelper(const std::string &, const std::string &, const std::string &,
                                             std::shared_ptr<MainLoopContext>, bool);


    COMMONAPI_METHOD_EXPORT bool registerStub(const std::string &, const std::string &, const std::string &,
                      std::shared_ptr<StubBase>, const ConnectionId_t &);
    COMMONAPI_METHOD_EXPORT bool registerStub(const std::string &, const std::string &, const std::string &,
                      std::shared_ptr<StubBase>, std::shared_ptr<MainLoopContext>);
    COMMONAPI_METHOD_EXPORT bool registerStubHelper(const std::string &, const std::string &, const std::string &,
                            std::shared_ptr<StubBase>, const ConnectionId_t &, bool);
    COMMONAPI_METHOD_EXPORT bool registerStubHelper(const std::string &, const std::string &, const std::string &,
                            std::shared_ptr<StubBase>, std::shared_ptr<MainLoopContext>, bool);

    COMMONAPI_METHOD_EXPORT bool unregisterStub(const std::string &, const std::string &, const std::string &);

    COMMONAPI_METHOD_EXPORT std::string getLibrary(const std::string &, const std::string &, const std::string &, bool);
    COMMONAPI_METHOD_EXPORT bool loadLibrary(const std::string &);

private:
    std::string usedConfig_;

    std::string defaultBinding_;
    std::string defaultFolder_;
    std::string defaultConfig_;
    Timeout_t defaultCallTimeout_;

    std::map<std::string, std::shared_ptr<Factory>> factories_;
    std::shared_ptr<Factory> defaultFactory_;
    std::map<std::string, std::map<bool, std::string>> libraries_;
    std::set<std::string> loadedLibraries_; // Library name

    std::mutex mutex_;
    std::mutex factoriesMutex_;
    std::mutex loadMutex_;

    bool isConfigured_;
    bool isInitialized_;

    static std::map<std::string, std::string> properties_;

friend class ProxyManager;
};

} // namespace CommonAPI

#endif // COMMONAPI_RUNTIME_HPP_
