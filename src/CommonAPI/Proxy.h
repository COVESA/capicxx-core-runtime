/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_PROXY_H_
#define COMMONAPI_PROXY_H_

#include "Attribute.h"

#include <cstdint>
#include <memory>
#include <type_traits>

namespace CommonAPI {

typedef Event<AvailabilityStatus> ProxyStatusEvent;
typedef ReadonlyAttribute<Version> InterfaceVersionAttribute;


class Proxy {
 public:
	virtual ~Proxy() { }

 	// The addressing scheme has the following format: "domain:service:instance"
 	virtual std::string getAddress() const = 0;

 	// i.e. "local"
 	virtual const std::string& getDomain() const = 0;

 	// i.e. "com.bmw.infotainment"
 	virtual const std::string& getServiceId() const = 0;

 	// i.e. "com.bmw.infotainment.low"
 	virtual const std::string& getInstanceId() const = 0;

	virtual bool isAvailable() const = 0;

	virtual ProxyStatusEvent& getProxyStatusEvent() = 0;

	virtual InterfaceVersionAttribute& getInterfaceVersionAttribute() = 0;
};

} // namespace CommonAPI

#endif // COMMONAPI_PROXY_H_
