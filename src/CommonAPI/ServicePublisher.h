/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_SERVICE_PUBLISHER_H_
#define COMMONAPI_SERVICE_PUBLISHER_H_

#include <memory>
#include <string>

namespace CommonAPI {

class Factory;

/**
 * \brief Manages all services that shall be published by the application.
 *
 * Stubs for all services that shall be published will be registered here.
 * This class is defined as singleton per loaded runtime (i.e. per loaded middleware).
 */
class ServicePublisher {
 public:
    virtual ~ServicePublisher() {}

    /**
     * \brief Registers and publishes a service.
     *
     * Registers and publishes a service. Which service is to be published is defined
     * by the stub-pointer that is given as parameter. The given factory will be used
     * to construct all necessary middleware specific objects to do the publishing.
     *
     * \note Note that a call to this method will always result in a registration
     * with the middleware the given factory was instantiated for, not the middleware
     * that matches the runtime this ServicePublisher was retrieved from. Accordingly,
     * unregistering the service will have to be done by using the ServicePublisher
     * that is provided by the runtime matching the middleware that also provided
     * the given factory.
     *
     * @param serviceAddress The CommonAPI address the service shall be reachable at
     * @param stub The stub that provides an implementation for the service
     * @param factory The factory that will be used to construct all necessary middleware specific objects
     *
     * @return 'true' if the service was published successfully, 'false' if not or if another service that uses
     * the exact same address already is registered.
     */
    template<typename _Stub>
    bool registerService(std::shared_ptr<_Stub> stub,
                         const std::string& serviceAddress,
                         std::shared_ptr<Factory> factory);

    /**
     * \brief Registers and publishes a service.
     *
     * Registers and publishes a service. Which service is to be published is defined
     * by the stub-pointer that is given as parameter. The given factory will be used
     * to construct all necessary middleware specific objects to do the publishing.
     *
     * \note Note that a call to this method will always result in a registration
     * with the middleware the given factory was instantiated for, not the middleware
     * that matches the runtime this ServicePublisher was retrieved from. Accordingly,
     * unregistering the service will have to be done by using the ServicePublisher
     * that is provided by the runtime matching the middleware that also provided
     * the given factory.
     *
     * @param participantId The CommonAPI participant ID the service shall be identified with
     * @param serviceName The CommonAPI service name the service shall provide
     * @param domain The CommonAPI domain the service shall be reachable at
     * @param stub The stub that provides an implementation for the service
     * @param factory The factory that will be used to construct all necessary middleware specific objects
     *
     * @return 'true' if the service was published successfully, 'false' if not or if another service that uses
     * the exact same address already is registered.
     */
    template<typename _Stub>
    bool registerService(std::shared_ptr<_Stub> stub,
                         const std::string& participantId,
                         const std::string& serviceName,
                         const std::string& domain,
                         std::shared_ptr<Factory> factory);

    /**
     * \brief Unregisters and depublishes the service that was published for the given address.
     *
     * Unregisters and depublishes the service that was published for the given CommonAPI address.
     *
     * @param The CommonAPI address the service was registered for
     *
     * @return 'true' if there was a service for the given address and depublishing
     * was successful, 'false' otherwise
     */
    virtual bool unregisterService(const std::string& serviceAddress) = 0;

    /**
     * \brief Unregisters and depublishes the service that was published for the given address.
     *
     * Unregisters and depublishes the service that was published for the given CommonAPI address.
     *
     * @param The CommonAPI participant ID the service was identified with
     * @param The CommonAPI service name the service provided
     * @param The CommonAPI domain the service was registered for
     *
     * @return 'true' if there was a service for the given address and depublishing
     * was successful, 'false' otherwise
     */
    bool unregisterService(const std::string& participantId,
                           const std::string& serviceName,
                           const std::string& domain) {
        std::string serviceAddress(participantId + ":" + serviceName + ":" + domain);
        return unregisterService(serviceAddress);
    }
};

} // namespace CommonAPI

#include "ServicePublisher.hpp"

#endif /* COMMONAPI_SERVICE_PUBLISHER_H_ */
