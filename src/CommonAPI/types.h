/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_TYPES_H_
#define COMMONAPI_TYPES_H_

#include <cstdint>
#include <functional>
#include <unordered_set>
#include <memory>
#include <tuple>
#include "ContainerUtils.h"
#include "Event.h"

#if  __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
#  define COMMONAPI_DEPRECATED __attribute__ ((__deprecated__))
#elif defined(_MSC_VER) && (_MSC_VER >= 1300)
#  define COMMONAPI_DEPRECATED __declspec(deprecated)
#else
#  define COMMONAPI_DEPRECATED
#endif


namespace CommonAPI {

enum class AvailabilityStatus {
    UNKNOWN,
    AVAILABLE,
    NOT_AVAILABLE
};

enum class CallStatus {
    SUCCESS,
    OUT_OF_MEMORY,
    NOT_AVAILABLE,
    CONNECTION_FAILED,
    REMOTE_ERROR
};


struct Version {
    Version() = default;

    Version(const uint32_t& majorValue, const uint32_t& minorValue):
        Major(majorValue),
        Minor(minorValue) {}

    uint32_t Major;
    uint32_t Minor;
};

/**
 * \brief Identifies a client sending a call to a stub.
 *
 * The ClientId is used to identify the caller within a stub.
 * The ClientId is supposed to be added by the middleware and can be compared using the == operator.
 */
class ClientId {
public:
    virtual ~ClientId() { }
    virtual bool operator==(ClientId& clientIdToCompare) = 0;
    virtual std::size_t hashCode() = 0;
};

template <typename ... Args>
struct SelectiveBroadcastFunctorHelper {
    typedef std::function<SubscriptionStatus(Args...)> SelectiveBroadcastFunctor;
};


typedef std::unordered_set<std::shared_ptr<CommonAPI::ClientId>, SharedPointerClientIdContentHash, SharedPointerClientIdContentEqual> ClientIdList;
template <typename ... Args>
struct SelectiveBroadcastSubscriptionResult {
    typedef std::tuple<bool, typename CommonAPI::Event<Args...>::Subscription> SubscriptionResult;

};

template<typename _EnumType>
class EnumHasher {
public:
    inline size_t operator()(const _EnumType& testEnum) const {
        return static_cast<int32_t>(testEnum);
    }

};

} // namespace CommonAPI

#endif // COMMONAPI_TYPES_H_
