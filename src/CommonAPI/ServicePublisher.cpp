/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "ServicePublisher.h"
#include "Stub.h"
#include "Factory.h"

namespace CommonAPI {

bool ServicePublisher::registerService(const std::shared_ptr<StubBase>& stubBase,
                                       const char* interfaceId,
                                       const std::string& participantId,
                                       const std::string& serviceName,
                                       const std::string& domain,
                                       const std::shared_ptr<Factory>& factory) {
    return factory->registerAdapter(stubBase, interfaceId, participantId, serviceName, domain);
}

} // namespace CommonAPI
