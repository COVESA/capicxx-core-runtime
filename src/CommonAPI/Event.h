/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_EVENT_H_
#define COMMONAPI_EVENT_H_

#include <functional>
#include <list>
#include <tuple>
#include <mutex>

namespace CommonAPI {

enum class SubscriptionStatus {
	RETAIN,
	CANCEL
};

/**
 * \brief Class representing an event
 *
 * Class representing an event
 */
template <typename... _Arguments>
class Event {
 public:
	typedef std::tuple<_Arguments...> ArgumentsTuple;
	typedef std::function<void(const _Arguments&...)> Listener;
	typedef std::function<SubscriptionStatus(const _Arguments&...)> CancellableListener;
	typedef std::list<CancellableListener> ListenersList;
	typedef typename ListenersList::iterator Subscription;

	class CancellableListenerWrapper;

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
	 * @return A token identifying this subscription
	 */
	inline Subscription subscribe(Listener listener);

	/**
     * \brief Subscribe a cancellable listener to this event
     *
     * Subscribe a cancellable listener to this event
     * ATTENTION: You should not build new proxies or register services in callbacks
     * from events. This can cause a deadlock or assert. Instead, you should set a
     * trigger for your application to do this on the next iteration of your event loop
     * if needed. The preferred solution is to build all proxies you need at the
     * beginning and react to events appropriatly for each.
     *
     * @param listener A cancellable listener to be added
     * @return A token identifying this subscription
     */
	Subscription subscribeCancellableListener(CancellableListener listener);

	/**
     * \brief Remove a listener from this event
     *
     * Remove a listener from this event
     * Note: Do not call this inside a listener notification callback it will deadlock! Use cancellable listeners instead.
     *
     * @param listenerSubscription A listener token to be removed
     */
	void unsubscribe(Subscription listenerSubscription);

	virtual ~Event() {}

 protected:
	// Returns false if all subscriptions were cancelled
	// Does not send *Removed events!
	SubscriptionStatus notifyListeners(const _Arguments&... eventArguments);

	// Events sent during subscribe()
	virtual void onFirstListenerAdded(const CancellableListener& listener) { }
	virtual void onListenerAdded(const CancellableListener& listener) { }

	// Events sent during unsubscribe()
	virtual void onListenerRemoved(const CancellableListener& listener) { }
	virtual void onLastListenerRemoved(const CancellableListener& listener) { }

	inline bool hasListeners() const;

 private:
	ListenersList listenersList_;
	std::mutex listenerListMutex_;
};

template <typename... _Arguments>
class Event<_Arguments...>::CancellableListenerWrapper {
 public:
	CancellableListenerWrapper(Listener&& listener): listener_(std::move(listener)) {
	}

	SubscriptionStatus operator()(const _Arguments&... eventArguments) {
		listener_(eventArguments...);
		return SubscriptionStatus::RETAIN;
	}

 private:
	Listener listener_;
};

template <typename... _Arguments>
typename Event<_Arguments...>::Subscription Event<_Arguments...>::subscribe(Listener listener) {
	return subscribeCancellableListener(CancellableListenerWrapper(std::move(listener)));
}

template <typename... _Arguments>
typename Event<_Arguments...>::Subscription Event<_Arguments...>::subscribeCancellableListener(CancellableListener listener) {
    listenerListMutex_.lock();
    const bool firstListenerAdded = listenersList_.empty();

	listenersList_.emplace_front(std::move(listener));
	Subscription listenerSubscription = listenersList_.begin();
	listenerListMutex_.unlock();

	if (firstListenerAdded) {
		onFirstListenerAdded(*listenerSubscription);
	}

	onListenerAdded(*listenerSubscription);

	return listenerSubscription;
}

template <typename... _Arguments>
void Event<_Arguments...>::unsubscribe(Subscription listenerSubscription) {
	const CancellableListener cancellableListener = *listenerSubscription;

	listenerListMutex_.lock();
	listenersList_.erase(listenerSubscription);
	const bool lastListenerRemoved = listenersList_.empty();
	listenerListMutex_.unlock();

	onListenerRemoved(cancellableListener);

	if (lastListenerRemoved) {
		onLastListenerRemoved(cancellableListener);
	}
}

template <typename... _Arguments>
SubscriptionStatus Event<_Arguments...>::notifyListeners(const _Arguments&... eventArguments) {
	listenerListMutex_.lock();
	for (auto iterator = listenersList_.begin(); iterator != listenersList_.end(); ) {
		const CancellableListener& cancellableListener = *iterator;
		const SubscriptionStatus listenerSubscriptionStatus = cancellableListener(eventArguments...);

		if (listenerSubscriptionStatus == SubscriptionStatus::CANCEL) {
			auto listenerIterator = iterator;
			iterator++;
			listenersList_.erase(listenerIterator);
		} else
			iterator++;
	}

	const bool lEmpty = listenersList_.empty();

	listenerListMutex_.unlock();

	return lEmpty ? SubscriptionStatus::CANCEL : SubscriptionStatus::RETAIN;
}

template <typename... _Arguments>
bool Event<_Arguments...>::hasListeners() const {
	return !listenersList_.empty();
}

} // namespace CommonAPI

#endif // COMMONAPI_EVENT_H_
