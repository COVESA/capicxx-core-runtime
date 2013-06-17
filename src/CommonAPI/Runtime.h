/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_RUNTIME_H_
#define COMMONAPI_RUNTIME_H_


#include "MiddlewareInfo.h"
#include "MainLoopContext.h"

#include <memory>
#include <fstream>
#include <unordered_map>
#include <dlfcn.h>
#include <string>
#include <cassert>
#include <cstring>


namespace CommonAPI {


class Factory;
class Runtime;
class MainLoopContext;
class ServicePublisher;

/**
 * \brief Represents the CommonAPI runtime bindings available.
 *
 * Represents the CommonAPI runtime bindings available.
 */
class Runtime {
 public:
    /**
     * \brief Loads the default runtime.
     *
     * Loads the runtime for the default middleware binding. This either is the only binding available,
     * or the one defined as default in the configuration.
     *
     * @return The runtime object for the default binding
     */
    static std::shared_ptr<Runtime> load();

    /**
     * \brief Loads specified runtime.
     *
     * Loads the runtime for the specified middleware binding. The given middleware ID can be either
     * the well known name defined by a binding, or a configured alias for a binding.
     *
     * @return The runtime object for specified binding
     */
    static std::shared_ptr<Runtime> load(const std::string& middlewareId);

    /**
     * \brief Called by bindings to register their runtime loaders. Do not call from applications.
     *
     * Called by bindings to register their runtime loaders. Do not call from applications.
     */
    static void registerRuntimeLoader(std::string middlewareName, MiddlewareRuntimeLoadFunction middlewareRuntimeLoadFunction);

    virtual ~Runtime() {}

    /**
     * \brief Returns new MainLoopContext
     *
     * Creates and returns a new MainLoopContext object. This context can be used to take
     * complete control over the order and time of execution of the abstract middleware
     * dispatching mechanism.
     *
     * @return A new MainLoopContext object
     */
    std::shared_ptr<MainLoopContext> getNewMainLoopContext() const;

    /**
     * \brief Create a factory for the loaded runtime.
     *
     * Create a factory for the loaded runtime
     *
     * @param In case mainloop integration shall be used, a std::shared_ptr<MainLoopContext> can be passed in.
     *        If no parameter is given, internal threading will handle sending and receiving of messages automatically.
     *
     * @return Factory object for this runtime
     */
    virtual std::shared_ptr<Factory> createFactory(std::shared_ptr<MainLoopContext> = std::shared_ptr<MainLoopContext>(NULL),
                                                   const std::string factoryName = "",
                                                   const bool nullOnInvalidName = false) = 0;

    /**
     * \brief Returns the ServicePublisher object for this runtime.
     *
     * Returns the ServicePublisher object for this runtime. Use the interface
     * provided by the ServicePublisher to publish and de-publish the services that
     * your application will provide to the outside world over the middleware
     * represented by this runtime. A ServicePublisher exists once per middleware.
     *
     * @return The ServicePublisher object for this runtime
     */
    virtual std::shared_ptr<ServicePublisher> getServicePublisher() = 0;
};


} // namespace CommonAPI

#endif // COMMONAPI_RUNTIME_H_
