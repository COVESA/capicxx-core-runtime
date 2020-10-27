// Copyright (C) 2015-2020 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <CommonAPI/CallInfo.hpp>
#include <CommonAPI/Runtime.hpp>

namespace CommonAPI {

CallInfo::CallInfo()
        : CallInfo(DEFAULT_SEND_TIMEOUT_MS, 0) {
}

CallInfo::CallInfo(Timeout_t _timeout)
        : CallInfo(_timeout, 0) {
}

CallInfo::CallInfo(const CallInfo &_other)
        : CallInfo(_other.timeout_, _other.sender_) {
}

CallInfo::CallInfo(Timeout_t _timeout, Sender_t _sender)
        : timeout_(_timeout), sender_(_sender) {

    Timeout_t defaultCallTimeout = Runtime::get()->getDefaultCallTimeout();
    const char *env = std::getenv("COMMONAPI_OVERRIDE_GLOBAL_CALL_TIMEOUT");

    if ((_timeout == DEFAULT_SEND_TIMEOUT_MS) && defaultCallTimeout) {
        timeout_ = defaultCallTimeout;
    }
    if (env) {
        Timeout_t globalCallTimeout = std::stoi(env);
        if (0 <= globalCallTimeout || globalCallTimeout == -1) {
            timeout_ = globalCallTimeout;
        }
    }
}

} // namespace CommonAPI