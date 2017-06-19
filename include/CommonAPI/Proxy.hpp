// Copyright (C) 2013-2017 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_PROXY_HPP_
#define COMMONAPI_PROXY_HPP_

#include <cstdint>
#include <memory>
#include <type_traits>
#include <future>

#include <CommonAPI/Address.hpp>
#include <CommonAPI/Attribute.hpp>
#include <CommonAPI/Export.hpp>
#include <CommonAPI/Version.hpp>

namespace CommonAPI {

typedef Event<AvailabilityStatus> ProxyStatusEvent;
typedef ReadonlyAttribute<Version> InterfaceVersionAttribute;

class COMMONAPI_EXPORT Proxy {
public:
    virtual ~Proxy();

    const Address &getAddress() const;

    std::future<void> getCompletionFuture();

    virtual bool isAvailable() const = 0;

    virtual bool isAvailableBlocking() const = 0;

    virtual ProxyStatusEvent& getProxyStatusEvent() = 0;

    virtual InterfaceVersionAttribute& getInterfaceVersionAttribute() = 0;

protected:
    Address address_;
    std::promise<void> completed_;
};

} // namespace CommonAPI

#endif // COMMONAPI_PROXY_HPP_
