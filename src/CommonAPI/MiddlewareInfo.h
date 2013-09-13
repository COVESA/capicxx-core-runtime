/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef MIDDLEWAREINFO_H_
#define MIDDLEWAREINFO_H_


#include <memory>
#include <cstring>
#include "types.h"


namespace CommonAPI {


class Runtime;


typedef std::shared_ptr<Runtime> (*MiddlewareRuntimeLoadFunction) ();


struct MiddlewareInfo {
    const char* middlewareName_;
    MiddlewareRuntimeLoadFunction getInstance_;
    Version version_;

    MiddlewareInfo(const char* middlewareName,
                   MiddlewareRuntimeLoadFunction middlewareRuntimeLoadFunction,
                   Version version):
        middlewareName_(middlewareName),
        getInstance_(middlewareRuntimeLoadFunction),
        version_(version) {}
};


} // namespace CommonAPI


#endif /* MIDDLEWAREINFO_H_ */
