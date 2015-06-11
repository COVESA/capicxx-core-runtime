// Copyright (C) 2014-2015 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_STUB_HPP_
#define COMMONAPI_STUB_HPP_

#include <memory>
#include <string>
#include <type_traits>

#include <CommonAPI/Address.hpp>
#include <CommonAPI/Types.hpp>

namespace CommonAPI {

class StubAdapter {
public:
    virtual ~StubAdapter() {}
    inline const Address &getAddress() const { return address_; }

protected:
    Address address_;
};

class StubBase {
public:
    virtual ~StubBase() {}
};

template<typename _StubAdapter, typename _StubRemoteEventHandler>
class Stub: public virtual StubBase {
    static_assert(std::is_base_of<StubAdapter, _StubAdapter>::value, "Invalid StubAdapter Class!");
public:
    typedef _StubAdapter StubAdapterType;
    typedef _StubRemoteEventHandler RemoteEventHandlerType;

    virtual ~Stub() {}

    virtual _StubRemoteEventHandler* initStubAdapter(const std::shared_ptr<_StubAdapter> &_stubAdapter) = 0;

    inline const std::shared_ptr<_StubAdapter> getStubAdapter() const { return stubAdapter_; }

protected:
    std::shared_ptr<_StubAdapter> stubAdapter_;
};

enum SelectiveBroadcastSubscriptionEvent {
    SUBSCRIBED,
    UNSUBSCRIBED
};

} // namespace CommonAPI

#endif // COMMONAPI_STUB_HPP_
