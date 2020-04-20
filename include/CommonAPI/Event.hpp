// Copyright (C) 2013-2017 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
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

#include <CommonAPI/Types.hpp>

namespace CommonAPI {

/**
 * \brief Class representing an event
 *
 * Class representing an event
 */
template<typename... Arguments_>
class Event {
public:
    typedef std::tuple<Arguments_...> ArgumentsTuple;
    typedef std::function<void(const Arguments_&...)> Listener;
    typedef uint32_t Subscription;
    typedef std::set<Subscription> SubscriptionsSet;
    typedef std::function<void(const CallStatus)> ErrorListener;
    typedef std::tuple<Listener, ErrorListener> Listeners;
    typedef std::map<Subscription, Listeners> ListenersMap;

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
    Subscription subscribe(Listener listener, ErrorListener errorListener = nullptr);

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
    void notifyListeners(const Arguments_&... _eventArguments);
    void notifySpecificListener(const Subscription _subscription, const Arguments_&... _eventArguments);
    void notifySpecificError(const Subscription _subscription, const CallStatus status);
    void notifyErrorListeners(const CallStatus status);

    virtual void onFirstListenerAdded(const Listener &_listener) {
        (void)_listener;
    }
    virtual void onListenerAdded(const Listener &_listener, const Subscription _subscription) {
        (void)_listener;
        (void)_subscription;
    }

    virtual void onListenerRemoved(const Listener &_listener, const Subscription _subscription) {
        (void)_listener;
        (void) _subscription;
    }
    virtual void onLastListenerRemoved(const Listener &_listener) {
        (void)_listener;
    }

private:
    ListenersMap subscriptions_;
    Subscription nextSubscription_;

    ListenersMap pendingSubscriptions_;
    SubscriptionsSet pendingUnsubscriptions_;

    std::recursive_mutex mutex_;
    std::mutex abi_placeholder_;
};

template<typename ... Arguments_>
typename Event<Arguments_...>::Subscription Event<Arguments_...>::subscribe(Listener listener, ErrorListener errorListener) {
    Subscription subscription;
    bool isFirstListener;
    Listeners listeners;

    {
        std::lock_guard<std::recursive_mutex> itsLock(mutex_);
        subscription = nextSubscription_++;
        isFirstListener = (0 == pendingSubscriptions_.size()) && (pendingUnsubscriptions_.size() == subscriptions_.size());
        listeners = std::make_tuple(listener, std::move(errorListener));
        pendingSubscriptions_[subscription] = std::move(listeners);
    }

    if (isFirstListener) {
        if (!pendingUnsubscriptions_.empty())
            onLastListenerRemoved(listener);
        onFirstListenerAdded(listener);
    }
    onListenerAdded(listener, subscription);

    return subscription;
}

template<typename ... Arguments_>
void Event<Arguments_...>::unsubscribe(const Subscription subscription) {
    bool isLastListener(false);
    bool hasUnsubscribed(false);
    Listener listener;

    {
        std::lock_guard<std::recursive_mutex> itsLock(mutex_);
        auto listenerIterator = subscriptions_.find(subscription);
        if (subscriptions_.end() != listenerIterator) {
            if (pendingUnsubscriptions_.end() == pendingUnsubscriptions_.find(subscription)) {
                if (0 == pendingSubscriptions_.erase(subscription)) {
                    pendingUnsubscriptions_.insert(subscription);
                    listener = std::get<0>(listenerIterator->second);
                    hasUnsubscribed = true;
                }
                isLastListener = (pendingUnsubscriptions_.size() == subscriptions_.size());
            }
        }
        else {
            listenerIterator = pendingSubscriptions_.find(subscription);
            if (pendingSubscriptions_.end() != listenerIterator) {
                listener = std::get<0>(listenerIterator->second);
                if (0 != pendingSubscriptions_.erase(subscription)) {
                    isLastListener = (pendingUnsubscriptions_.size() == subscriptions_.size());
                    hasUnsubscribed = true;
                }
            }
        }
        isLastListener = isLastListener && (0 == pendingSubscriptions_.size());
    }

    if (hasUnsubscribed) {
        onListenerRemoved(listener, subscription);
        if (isLastListener) {
            onLastListenerRemoved(listener);
        }
    }
}

template<typename ... Arguments_>
void Event<Arguments_...>::notifyListeners(const Arguments_&... eventArguments) {
    std::lock_guard<std::recursive_mutex> itsLock(mutex_);
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

    for (auto iterator = subscriptions_.begin(); iterator != subscriptions_.end(); iterator++) {
        (std::get<0>(iterator->second))(eventArguments...);
    }
}

template<typename ... Arguments_>
void Event<Arguments_...>::notifySpecificListener(const Subscription subscription, const Arguments_&... eventArguments) {
    std::lock_guard<std::recursive_mutex> itsLock(mutex_);
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

    for (auto iterator = subscriptions_.begin(); iterator != subscriptions_.end(); iterator++) {
        if (subscription == iterator->first) {
            (std::get<0>(iterator->second))(eventArguments...);
        }
    }
}

template<typename ... Arguments_>
void Event<Arguments_...>::notifySpecificError(const Subscription subscription, const CallStatus status) {
    std::lock_guard<std::recursive_mutex> itsLock(mutex_);
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

    for (auto iterator = subscriptions_.begin(); iterator != subscriptions_.end(); iterator++) {
        if (subscription == iterator->first) {
            ErrorListener listener = std::get<1>(iterator->second);
            if (listener) {
                listener(status);
            }
        }
    }

    if (status != CommonAPI::CallStatus::SUCCESS) {
        auto listenerIterator = subscriptions_.find(subscription);
        if (subscriptions_.end() != listenerIterator) {
            if (pendingUnsubscriptions_.end() == pendingUnsubscriptions_.find(subscription)) {
                if (0 == pendingSubscriptions_.erase(subscription)) {
                    pendingUnsubscriptions_.insert(subscription);
                }
            }
        }
        else {
            listenerIterator = pendingSubscriptions_.find(subscription);
            if (pendingSubscriptions_.end() != listenerIterator) {
                pendingSubscriptions_.erase(subscription);
            }
        }
    }
}

template<typename ... Arguments_>
void Event<Arguments_...>::notifyErrorListeners(const CallStatus status) {
    std::lock_guard<std::recursive_mutex> itsLock(mutex_);
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

    for (auto iterator = subscriptions_.begin(); iterator != subscriptions_.end(); iterator++) {
        ErrorListener listener = std::get<1>(iterator->second);
        if (listener) {
            listener(status);
        }
    }
}


} // namespace CommonAPI

#endif // COMMONAPI_EVENT_HPP_
