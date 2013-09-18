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
#include <unordered_map>
#include <string>
#include <cassert>
#include <cstring>
#include <mutex>


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
    enum class LoadState {
        SUCCESS,
        NO_LIBRARY_FOUND,
        CONFIGURATION_ERROR,
        BINDING_ERROR
    };

    virtual ~Runtime() {}

    /**
     * \brief Loads the default runtime.
     *
     * Loads the runtime for the default middleware binding. This can be
     *   * One of the middleware bindings that were linked at compile time
     *   * The first middleware binding that is encountered when resolving bindings at runtime
     *   * The middleware binding that was configured as default in the corresponding configuration
     *     file (throws an error if no such binding exists)
     *
     * @return The runtime object for the default binding, or null if any error occurred
     */
    static std::shared_ptr<Runtime> load();

    /**
     * \brief Loads the default runtime and notifies the caller of any errors.
     *
     * Loads the runtime for the default middleware binding. This can be
     *   * One of the middleware bindings that were linked at compile time
     *   * The first middleware binding that is encountered when resolving bindings at runtime
     *   * The middleware binding that was configured as default in the corresponding configuration
     *     file (throws an error if no such binding exists)
     *
     * @param loadState: An enumeration that will be set appropriately after loading has finished or
     *                   aborted. May be used for debugging purposes.
     *
     * @return The runtime object for the default binding, or null if any error occurred. In the latter
     *         case, loadState will be set to an appropriate value.
     */
    static std::shared_ptr<Runtime> load(LoadState& loadState);

    /**
     * \brief Loads specified runtime.
     *
     * Loads the runtime for the specified middleware binding. The given well known name can be either
     * the well known name defined by a binding, or a configured alias for a binding.
     *
     * @param middlewareIdOrAlias A well known name or an alias for a binding
     *
     * @return The runtime object for specified binding, or null if any error occurred.
     */
    static std::shared_ptr<Runtime> load(const std::string& middlewareIdOrAlias);

    /**
     * \brief Loads specified runtime.
     *
     * Loads the runtime for the specified middleware binding. The given well known name can be either
     * the well known name defined by a binding, or a configured alias for a binding.
     *
     * @param middlewareIdOrAlias A well known name or an alias for a binding.
     * @param loadState: An enumeration that will be set appropriately after loading has finished or
     *                   aborted. May be used for debugging purposes.
     *
     * @return The runtime object for specified binding, or null if any error occurred. In the latter
     *         case, loadState will be set to an appropriate value.
     */
    static std::shared_ptr<Runtime> load(const std::string& middlewareIdOrAlias, LoadState& loadState);

    /**
     * \brief Called by bindings to register their runtime loaders. Do not call from applications.
     *
     * Called by bindings to register their runtime loaders. Do not call from applications.
     */
    static void registerRuntimeLoader(const std::string& middlewareName, const MiddlewareRuntimeLoadFunction& middlewareRuntimeLoadFunction);

    /**
     * \brief Returns new MainLoopContext.
     *
     * Creates and returns a new MainLoopContext object. This context can be used to take
     * complete control over the order and time of execution of the abstract middleware
     * dispatching mechanism. Make sure to register all callback functions before subsequently
     * handing it to createFactory(), as during creation of the factory object the callbacks may
     * already be called.
     *
     * @return A new MainLoopContext object
     */
    std::shared_ptr<MainLoopContext> getNewMainLoopContext() const;

    /**
     * \brief Create a factory for the loaded runtime.
     *
     * Create a factory for the loaded runtime
     *
     * @param mainLoopContext: In case mainloop integration shall be used, a std::shared_ptr<MainLoopContext> can be passed in.
     *        If no parameter is given, internal threading will handle sending and receiving of messages automatically.
     *        If the mainloop context is not initialized, no factory will be returned. See documentation of
     *        MainLoopContext::isInitialized().
     *
     * @param factoryName: If additional configuration parameters for the specific middleware factory shall be provided,
     *        the appropriate set of parameters may be identified by this name. See accompanying documentation for
     *        usage of configuration files.
     *
     * @param nullOnInvalidName: If a factoryName is provided, this parameter determines whether the standard configuration
     *        for factories shall be used if the specific parameter set cannot be found, or if instead no factory
     *        shall be returned in this case.
     *
     * @return Factory object for this runtime
     */
    std::shared_ptr<Factory> createFactory(std::shared_ptr<MainLoopContext> mainLoopContext = std::shared_ptr<MainLoopContext>(NULL),
                                           const std::string factoryName = "",
                                           const bool nullOnInvalidName = false);

    /**
     * \brief Create a factory for the loaded runtime.
     *
     * Create a factory for the loaded runtime
     *
     * @param factoryName: If additional configuration parameters for the specific middleware factory shall be provided,
     *        the appropriate set of parameters may be identified by this name. See accompanying documentation for
     *        usage of configuration files.
     *
     * @param nullOnInvalidName: If a factoryName is provided, this parameter determines whether the standard configuration
     *        for factories shall be used if the specific parameter set cannot be found, or if instead no factory
     *        shall be returned in this case.
     *
     * @return Factory object for this runtime
     */
    std::shared_ptr<Factory> createFactory(const std::string factoryNamey,
                                           const bool nullOnInvalidName = false);

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

 protected:
    virtual std::shared_ptr<Factory> doCreateFactory(std::shared_ptr<MainLoopContext> mainLoopContext,
                                                     const std::string& factoryName,
                                                     const bool nullOnInvalidName = false) = 0;

 private:
    static const std::vector<std::string> readDirectory(const std::string& path);

    static std::shared_ptr<Runtime> checkDynamicLibraries(LoadState& loadState);
    static std::shared_ptr<Runtime> checkDynamicLibraries(const std::string& middlewareName, LoadState& loadState);

    static bool tryLoadLibrary(const std::string& libraryPath, void** sharedLibraryHandle, MiddlewareInfo** foundMiddlewareInfo);
    static bool checkAndLoadLibrary(const std::string& libraryPath, const std::string& requestedMiddlewareName, bool keepLibrary);
    static bool checkAndLoadDefaultLibrary(std::string& foundBindingName, const std::string& libraryPath);

    static void closeHandle(void* libraryHandle);
};


} // namespace CommonAPI

#endif // COMMONAPI_RUNTIME_H_
