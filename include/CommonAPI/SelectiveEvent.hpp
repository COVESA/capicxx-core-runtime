// Copyright (C) 2013-2015 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_SELECTIVEEVENT_HPP_
#define COMMONAPI_SELECTIVEEVENT_HPP_

#include <CommonAPI/Event.hpp>

namespace CommonAPI {

template<typename ... _Arguments>
class SelectiveEvent: public Event<_Arguments...> {
public:
    typedef typename Event<_Arguments...>::Listener Listener;
    typedef typename Event<_Arguments...>::Subscription Subscription;

    virtual ~SelectiveEvent() {}
};

} // namespace CommonAPI

#endif // COMMONAPI_SELECTIVEEVENT_HPP_
