// Copyright (C) 2015-2020 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.hpp> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_CALLINFO_HPP_
#define COMMONAPI_CALLINFO_HPP_

#include <CommonAPI/Config.hpp>
#include <CommonAPI/Types.hpp>
#include <string>

namespace CommonAPI {

struct COMMONAPI_EXPORT CallInfo {
    CallInfo();
    CallInfo(Timeout_t _timeout);
    CallInfo(const CallInfo &_other);
    CallInfo(Timeout_t _timeout, Sender_t _sender);

    Timeout_t timeout_;
    Sender_t sender_;
};

} // namespace CommonAPI

#endif // COMMONAPI_CALLINFO_HPP_
