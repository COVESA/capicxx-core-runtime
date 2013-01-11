/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_RUNTIME_H_
#define COMMONAPI_RUNTIME_H_


#include "MiddlewareInfo.h"
#include "Factory.h"

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


class Runtime {
 public:
    static std::shared_ptr<Runtime> load();
    static std::shared_ptr<Runtime> load(const std::string& middlewareId);
    static void registerRuntimeLoader(std::string middlewareName, MiddlewareRuntimeLoadFunction middlewareRuntimeLoadFunction);

    virtual ~Runtime() {}
    virtual std::shared_ptr<Factory> createFactory() = 0;
};


} // namespace CommonAPI

#endif // COMMONAPI_RUNTIME_H_
