/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_SELECTIVE_EVENT_H_
#define COMMONAPI_SELECTIVE_EVENT_H_

#include "Event.h"

namespace CommonAPI {

template<typename ... _Arguments>
class SelectiveEvent: public Event<_Arguments...> {
public:
    typedef typename Event<_Arguments...>::Listener Listener;
    typedef typename Event<_Arguments...>::Subscription Subscription;

    virtual ~SelectiveEvent() {
    }

    Subscription subscribe(Listener listener) {
        // just delegate
        bool success;
        return subscribe(listener, success);
    }

    /**
     * \brief Subscribe a listener to this event and be notified of success
     *
     * Subscribe a listener to this event and be notified of success via the passed reference.
     * ATTENTION: You should not build new proxies or register services in callbacks
     * from events. This can cause a deadlock or assert. Instead, you should set a
     * trigger for your application to do this on the next iteration of your event loop
     * if needed. The preferred solution is to build all proxies you need at the
     * beginning and react to events appropriatly for each.
     *
     * @param listener A listener to be added
     * @param success Indicates whether subscription was accepted
     * @return A token identifying this subscription
     */
    virtual Subscription subscribe(Listener listener, bool& success) = 0;
};

} // namespace CommonAPI

#endif
