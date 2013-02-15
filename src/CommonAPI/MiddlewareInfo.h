/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef MIDDLEWAREINFO_H_
#define MIDDLEWAREINFO_H_


#include <memory>
#include <cstring>


namespace CommonAPI {


class Runtime;


inline int FNV1aHash(const char* s) {
    const int FNV_offset_basis = 2166136261u;
    const int FNV_prime = 16777619;

    int hashValue = FNV_offset_basis;
    for (unsigned int i = 0; i < strlen(s); i++) {
        hashValue = (hashValue ^ s[i]) * FNV_prime;
    }
    return hashValue;
}


typedef std::shared_ptr<Runtime> (*MiddlewareRuntimeLoadFunction) ();


struct MiddlewareInfo {
    const char* middlewareName_;
    const int middlewareId_;
    MiddlewareRuntimeLoadFunction getInstance_;

    MiddlewareInfo(const char* middlewareName, MiddlewareRuntimeLoadFunction middlewareRuntimeLoadFunction):
        middlewareName_(middlewareName),
        middlewareId_(FNV1aHash(middlewareName)),
        getInstance_(middlewareRuntimeLoadFunction) {
}
};


} // namespace CommonAPI


#endif /* MIDDLEWAREINFO_H_ */
