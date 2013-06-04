/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef COMMONAPI_SERVICE_PUBLISHER_IMPL_H_
#define COMMONAPI_SERVICE_PUBLISHER_IMPL_H_

#include "Factory.h"

namespace CommonAPI {

template<typename _Stub>
bool ServicePublisher::registerService(std::shared_ptr<_Stub> stub,
                     const std::string& participantId,
                     const std::string& serviceName,
                     const std::string& domain,
                     std::shared_ptr<Factory> factory) {

    std::shared_ptr<StubBase> stubBase = std::dynamic_pointer_cast<StubBase>(stub);
    return factory->registerAdapter(stubBase, _Stub::StubAdapterType::getInterfaceId(), participantId, serviceName, domain);
}

template<typename _Stub>
bool ServicePublisher::registerService(std::shared_ptr<_Stub> stub,
                     const std::string& serviceAddress,
                     std::shared_ptr<Factory> factory) {
    std::string domain;
    std::string serviceName;
    std::string participantId;
    if(!factory->splitValidAddress(serviceAddress, domain, serviceName, participantId)) {
        return false;
    }

    return registerService<_Stub>(stub, participantId, serviceName, domain, factory);
}

} // namespace CommonAPI


#endif /* COMMONAPI_SERVICE_PUBLISHER_IMPL_H_ */
