/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_EVENT_H_
#define COMMONAPI_EVENT_H_

#include <functional>
#include <list>
#include <tuple>

namespace CommonAPI {

enum class SubscriptionStatus {
	RETAIN,
	CANCEL
};


template <typename... _Arguments>
class Event {
 public:
	typedef std::tuple<_Arguments...> ArgumentsTuple;
	typedef std::function<void(const _Arguments&...)> Listener;
	typedef std::function<SubscriptionStatus(const _Arguments&...)> CancellableListener;
	typedef std::list<CancellableListener> ListenersList;
	typedef typename ListenersList::iterator Subscription;

	class CancellableListenerWrapper;

	inline Subscription subscribe(Listener listener);
	Subscription subscribeCancellableListener(CancellableListener listener);
	void unsubscribe(Subscription listenerSubscription);

	virtual ~Event() { }

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
	const bool firstListenerAdded = listenersList_.empty();

	listenersList_.emplace_front(std::move(listener));
	Subscription listenerSubscription = listenersList_.begin();

	if (firstListenerAdded)
		onFirstListenerAdded(*listenerSubscription);

	onListenerAdded(*listenerSubscription);

	return listenerSubscription;
}

template <typename... _Arguments>
void Event<_Arguments...>::unsubscribe(Subscription listenerSubscription) {
	const CancellableListener cancellableListener = *listenerSubscription;

	listenersList_.erase(listenerSubscription);

	onListenerRemoved(cancellableListener);

	const bool lastListenerRemoved = listenersList_.empty();
	if (lastListenerRemoved)
		onLastListenerRemoved(cancellableListener);
}

template <typename... _Arguments>
SubscriptionStatus Event<_Arguments...>::notifyListeners(const _Arguments&... eventArguments) {
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

	return listenersList_.empty() ? SubscriptionStatus::CANCEL : SubscriptionStatus::RETAIN;
}

template <typename... _Arguments>
bool Event<_Arguments...>::hasListeners() const {
	return !listenersList_.empty();
}

} // namespace CommonAPI

#endif // COMMONAPI_EVENT_H_
