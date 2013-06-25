/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "Runtime.h"

#include <iostream>


namespace CommonAPI {


std::unordered_map<std::string, MiddlewareRuntimeLoadFunction>* registeredRuntimeLoadFunctions_;


void Runtime::registerRuntimeLoader(std::string middlewareName, MiddlewareRuntimeLoadFunction middlewareRuntimeLoadFunction) {
    if(!registeredRuntimeLoadFunctions_) {
        registeredRuntimeLoadFunctions_ = new std::unordered_map<std::string, MiddlewareRuntimeLoadFunction>();
    }
    registeredRuntimeLoadFunctions_->insert({middlewareName, middlewareRuntimeLoadFunction});
}


std::shared_ptr<Runtime> Runtime::load() {
    if(!registeredRuntimeLoadFunctions_) {
        registeredRuntimeLoadFunctions_ = new std::unordered_map<std::string, MiddlewareRuntimeLoadFunction> {};
    }

    auto begin = registeredRuntimeLoadFunctions_->begin();

    if (begin != registeredRuntimeLoadFunctions_->end()) {
        return (begin->second)();
    }

    return std::shared_ptr<Runtime>(NULL);
}


std::shared_ptr<Runtime> Runtime::load(const std::string& middlewareName) {
    if(!registeredRuntimeLoadFunctions_) {
        registeredRuntimeLoadFunctions_ = new std::unordered_map<std::string, MiddlewareRuntimeLoadFunction> {};
    }

    for (auto it = registeredRuntimeLoadFunctions_->begin(); it != registeredRuntimeLoadFunctions_->end(); ++it) {
        if(it->first == middlewareName) {
            return (it->second)();
        }
    }

    return std::shared_ptr<Runtime>(NULL);
}


std::shared_ptr<MainLoopContext> Runtime::getNewMainLoopContext() const {
    return std::make_shared<MainLoopContext>();
}

std::shared_ptr<Factory> Runtime::createFactory(const std::string factoryName,
                                                const bool nullOnInvalidName) {
    return createFactory(std::shared_ptr<MainLoopContext>(NULL), factoryName, nullOnInvalidName);
}

std::shared_ptr<Factory> Runtime::createFactory(std::shared_ptr<MainLoopContext> mainLoopContext,
                                                const std::string factoryName,
                                                const bool nullOnInvalidName) {
    if(mainLoopContext && !mainLoopContext->isInitialized()) {
        return std::shared_ptr<Factory>(NULL);
    }
    return doCreateFactory(mainLoopContext, factoryName, nullOnInvalidName);
}


} // namespace CommonAPI
