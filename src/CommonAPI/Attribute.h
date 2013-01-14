/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_ATTRIBUTE_H_
#define COMMONAPI_ATTRIBUTE_H_

#include "types.h"
#include "Event.h"

#include <cstdint>
#include <functional>
#include <future>
#include <memory>


namespace CommonAPI {

template <typename _ValueType>
class ReadonlyAttribute {
 public:
	typedef _ValueType ValueType;
	typedef std::function<void(const CallStatus&, _ValueType)> AttributeAsyncCallback;

	virtual ~ReadonlyAttribute() { }

	virtual CallStatus getValue(_ValueType& value) const = 0;
	virtual std::future<CallStatus> getValueAsync(AttributeAsyncCallback attributeAsyncCallback) = 0;
};

template <typename _ValueType>
class Attribute: public ReadonlyAttribute<_ValueType> {
 public:
	typedef typename ReadonlyAttribute<_ValueType>::ValueType ValueType;
	typedef typename ReadonlyAttribute<_ValueType>::AttributeAsyncCallback AttributeAsyncCallback;

	virtual ~Attribute() { }

	virtual void setValue(const _ValueType& requestValue, CallStatus& callStatus, _ValueType& responseValue) = 0;
	virtual std::future<CallStatus> setValueAsync(const _ValueType& requestValue,
												  AttributeAsyncCallback attributeAsyncCallback) = 0;
};

template <typename _AttributeBaseClass>
class _ObservableAttributeImpl: public _AttributeBaseClass {
 public:
	typedef typename _AttributeBaseClass::ValueType ValueType;
	typedef typename _AttributeBaseClass::AttributeAsyncCallback AttributeAsyncCallback;
	typedef Event<ValueType> ChangedEvent;

	virtual ~_ObservableAttributeImpl() { }

	virtual ChangedEvent& getChangedEvent() = 0;
};

template <typename _ValueType>
struct ObservableReadonlyAttribute: _ObservableAttributeImpl< ReadonlyAttribute<_ValueType> > {
};

template <typename _ValueType>
struct ObservableAttribute: _ObservableAttributeImpl< Attribute<_ValueType> > {
};

} // namespace CommonAPI

#endif // COMMONAPI_ATTRIBUTE_H_
