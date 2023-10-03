// Copyright (C) 2015-2020 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef COMMONAPI_EXPORT_HPP_
#define COMMONAPI_EXPORT_HPP_

#ifdef _WIN32
    #define COMMONAPI_EXPORT __declspec(dllexport)
    #define COMMONAPI_EXPORT_CLASS_EXPLICIT

    #define COMMONAPI_METHOD_EXPORT COMMONAPI_EXPORT

    #if COMMONAPI_DLL_COMPILATION
        #define COMMONAPI_IMPORT_EXPORT __declspec(dllexport)
    #else
        #define COMMONAPI_IMPORT_EXPORT __declspec(dllimport)
    #endif
#else
    #define COMMONAPI_EXPORT __attribute__ ((visibility ("default")))
    #define COMMONAPI_EXPORT_CLASS_EXPLICIT COMMONAPI_EXPORT
    #define COMMONAPI_IMPORT_EXPORT

    #define COMMONAPI_METHOD_EXPORT

#endif

#endif // COMMONAPI_EXPORT_HPP_
