/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_STUB_H_
#define COMMONAPI_STUB_H_

#include <memory>
#include <string>
#include <type_traits>

namespace CommonAPI {

class StubAdapter {
 public:
    virtual ~StubAdapter() { }

    virtual const std::string getAddress() const = 0;
    virtual const std::string& getDomain() const = 0;
    virtual const std::string& getServiceId() const = 0;
    virtual const std::string& getInstanceId() const = 0;
};

struct StubBase {
	virtual ~StubBase() {}
};

template <typename _StubAdapter, typename _StubRemoteEventHandler>
class Stub : public StubBase {
	static_assert(std::is_base_of<StubAdapter, _StubAdapter>::value, "Invalid StubAdapter Class!");
 public:
	typedef _StubAdapter StubAdapterType;
	typedef _StubRemoteEventHandler RemoteEventHandlerType;

	virtual ~Stub() { }

	virtual _StubRemoteEventHandler* initStubAdapter(const std::shared_ptr<_StubAdapter>& stubAdapter) = 0;
};

} // namespace CommonAPI

#endif // COMMONAPI_STUB_H_
