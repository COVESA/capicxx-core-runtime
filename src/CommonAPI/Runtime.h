/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_RUNTIME_H_
#define COMMONAPI_RUNTIME_H_


#include "MiddlewareInfo.h"
#include "Factory.h"
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

/**
 * \brief Represents the CommonAPI runtime bindings available.
 *
 * Represents the CommonAPI runtime bindings available.
 */
class Runtime {
 public:

    /**
     * \brief Loads the default runtime
     *
     * Loads the default runtime. This is the only one available, or the default as defined in configuration
     *
     * @return The runtime object for this binding
     */
    static std::shared_ptr<Runtime> load();
    /**
     * \brief Loads specified runtime
     *
     * Loads specified runtime. This is specified by either the well known name defined by the binding, or configured
     *
     * @return The runtime object for specified binding
     */
    static std::shared_ptr<Runtime> load(const std::string& middlewareId);
    /**
     * \brief Called by bindings to register their methods. Do not call from applications.
     *
     * Called by bindings to register their methods. Do not call from applications.
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
     * \brief Create a factory for the loaded runtime
     *
     * Create a factory for the loaded rluntime
     *
     * @param In case mainloop integration shall be used, a std::shared_ptr<MainLoopContext> can be passed in.
     *        If no parameter is given, internal threading will handle sending and receiving of messages automatically.
     *
     * @return Factory object for the loaded runtime
     */
    virtual std::shared_ptr<Factory> createFactory(std::shared_ptr<MainLoopContext> = std::shared_ptr<MainLoopContext>(NULL)) = 0;
};


} // namespace CommonAPI

#endif // COMMONAPI_RUNTIME_H_
