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
#include <memory>
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
    typedef std::map<Subscription, std::shared_ptr<Listeners>> ListenersMap;

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

    std::mutex subscriptionMutex_;
};

template<typename ... Arguments_>
typename Event<Arguments_...>::Subscription Event<Arguments_...>::subscribe(Listener listener, ErrorListener errorListener) {
    std::shared_ptr<Listeners> listeners = std::make_shared<Listeners>(std::move(listener), std::move(errorListener));
    Subscription subscription;
    bool isFirstListener;
    {
        std::lock_guard<std::mutex> guard(subscriptionMutex_);
        isFirstListener = subscriptions_.empty();
        subscriptions_[nextSubscription_] = listeners;
        subscription = nextSubscription_++;
    }

    if (isFirstListener)
        onFirstListenerAdded(std::get<0>(*listeners));
    onListenerAdded(std::get<0>(*listeners), subscription);

    return subscription;
}

template<typename ... Arguments_>
void Event<Arguments_...>::unsubscribe(const Subscription subscription) {
    bool isLastListener = false;
    bool hasUnsubscribed = false;
    std::shared_ptr<Listeners> listeners;
    {
        std::lock_guard<std::mutex> guard(subscriptionMutex_);
        auto listenerIterator = subscriptions_.find(subscription);
        if (subscriptions_.end() != listenerIterator) {
            listeners = listenerIterator->second;
            subscriptions_.erase(listenerIterator);
            hasUnsubscribed = true;
            isLastListener = subscriptions_.empty();
        }
    }

    if (hasUnsubscribed) {
        onListenerRemoved(std::get<0>(*listeners), subscription);
        if (isLastListener) {
            onLastListenerRemoved(std::get<0>(*listeners));
        }
    }
}

template<typename ... Arguments_>
void Event<Arguments_...>::notifyListeners(const Arguments_&... eventArguments) {
    std::vector<std::shared_ptr<Listeners>> listeners;
    {
        std::lock_guard<std::mutex> guard(subscriptionMutex_);
        listeners.reserve(subscriptions_.size());
        for (const auto& element : subscriptions_) {
            listeners.emplace_back(element.second);
        }
    }

    for (const auto& element : listeners) {
        (std::get<0>(*element))(eventArguments...);
    }
}

template<typename ... Arguments_>
void Event<Arguments_...>::notifySpecificListener(const Subscription subscription, const Arguments_&... eventArguments) {
    std::shared_ptr<Listeners> listeners;
    {
        std::lock_guard<std::mutex> guard(subscriptionMutex_);
        auto iterator = subscriptions_.find(subscription);
        if (iterator != subscriptions_.end()) {
            listeners = iterator->second;
        }
    }

    if (listeners) {
        (std::get<0>(*listeners))(eventArguments...);
    }
}

template<typename ... Arguments_>
void Event<Arguments_...>::notifySpecificError(const Subscription subscription, const CallStatus status) {
    std::shared_ptr<Listeners> listeners;
    {
        std::lock_guard<std::mutex> guard(subscriptionMutex_);
        auto iterator = subscriptions_.find(subscription);
        if (iterator != subscriptions_.end()) {
            listeners = iterator->second;
        }
    }

    if (listeners) {
        (std::get<1>(*listeners))(status);
    }

    if (status != CommonAPI::CallStatus::SUCCESS) {
        unsubscribe(subscription);
    }
}

} // namespace CommonAPI

#endif // COMMONAPI_EVENT_HPP_
