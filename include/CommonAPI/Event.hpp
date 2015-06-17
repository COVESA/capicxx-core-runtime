// Copyright (C) 2013-2015 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_EVENT_HPP_
#define COMMONAPI_EVENT_HPP_

#include <functional>
#include <mutex>
#include <map>
#include <set>
#include <tuple>

namespace CommonAPI {

/**
 * \brief Class representing an event
 *
 * Class representing an event
 */
template<typename... _Arguments>
class Event {
public:
    typedef std::tuple<_Arguments...> ArgumentsTuple;
    typedef std::function<void(const _Arguments&...)> Listener;
    typedef uint32_t Subscription;
    typedef std::set<Subscription> SubscriptionsSet;
    typedef std::map<Subscription, Listener> ListenersMap;

    /**
     * \brief Constructor
     */
    Event() : nextSubscription_(0) {};

    /**
     * \brief Subscribe a listener to this event
     *
     * Subscribe a listener to this event.
     * ATTENTION: You should not build new proxies or register services in callbacks
     * from events. This can cause a deadlock or assert. Instead, you should set a
     * trigger for your application to do this on the next iteration of your event loop
     * if needed. The preferred solution is to build all proxies you need at the
     * beginning and react to events appropriatly for each.
     *
     * @param listener A listener to be added
     * @return key of the new subscription
     */
    Subscription subscribe(Listener listener);

    /**
     * \brief Remove a listener from this event
     *
     * Remove a listener from this event
     * Note: Do not call this inside a listener notification callback it will deadlock! Use cancellable listeners instead.
     *
     * @param subscription A listener token to be removed
     */
    void unsubscribe(Subscription subscription);

    virtual ~Event() {}

protected:
    void notifyListeners(const _Arguments&... eventArguments);

    virtual void onFirstListenerAdded(const Listener& listener) {}
    virtual void onListenerAdded(const Listener& listener) {}

    virtual void onListenerRemoved(const Listener& listener) {}
    virtual void onLastListenerRemoved(const Listener& listener) {}

//private:
    ListenersMap subscriptions_;
    Subscription nextSubscription_;

    ListenersMap pendingSubscriptions_;
    SubscriptionsSet pendingUnsubscriptions_;

    std::mutex notificationMutex_;
    std::mutex subscriptionMutex_;
};

template<typename ... _Arguments>
typename Event<_Arguments...>::Subscription Event<_Arguments...>::subscribe(Listener listener) {
	Subscription subscription;
	bool isFirstListener;

	subscriptionMutex_.lock();
	subscription = nextSubscription_++;
	// TODO?: check for key/subscription overrun
	listener = pendingSubscriptions_[subscription] = std::move(listener);
	isFirstListener = (0 == subscriptions_.size());
	subscriptionMutex_.unlock();

	if (isFirstListener)
		onFirstListenerAdded(listener);
	onListenerAdded(listener);

	return subscription;
}

template<typename ... _Arguments>
void Event<_Arguments...>::unsubscribe(Subscription subscription) {
	bool isLastListener(false);
	bool hasUnsubscribed(false);

	subscriptionMutex_.lock();
	auto listener = subscriptions_.find(subscription);
	if (subscriptions_.end() != listener
			&& pendingUnsubscriptions_.end() == pendingUnsubscriptions_.find(subscription)) {
		pendingUnsubscriptions_.insert(subscription);
		isLastListener = (1 == subscriptions_.size());
		hasUnsubscribed = true;
	}
	else {
		listener = pendingSubscriptions_.find(subscription);
		if (pendingSubscriptions_.end() != listener) {
			pendingSubscriptions_.erase(subscription);
			isLastListener = (0 == subscriptions_.size());
			hasUnsubscribed = true;
		}
	}
	subscriptionMutex_.unlock();

	if (hasUnsubscribed) {
		onListenerRemoved(listener->second);
		if (isLastListener) {
			onLastListenerRemoved(listener->second);
		}
	}
}

template<typename ... _Arguments>
void Event<_Arguments...>::notifyListeners(const _Arguments&... eventArguments) {
	subscriptionMutex_.lock();
	notificationMutex_.lock();
	for (auto iterator = pendingUnsubscriptions_.begin();
		 iterator != pendingUnsubscriptions_.end();
		 iterator++) {
		subscriptions_.erase(*iterator);
	}
	pendingUnsubscriptions_.clear();

	for (auto iterator = pendingSubscriptions_.begin();
		 iterator != pendingSubscriptions_.end();
		 iterator++) {
		subscriptions_.insert(*iterator);
	}
	pendingSubscriptions_.clear();

	subscriptionMutex_.unlock();
	for (auto iterator = subscriptions_.begin(); iterator != subscriptions_.end(); iterator++) {
        iterator->second(eventArguments...);
    }

    notificationMutex_.unlock();
}

} // namespace CommonAPI

#endif // COMMONAPI_EVENT_HPP_
