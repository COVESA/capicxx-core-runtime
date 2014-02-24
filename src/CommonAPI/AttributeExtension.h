/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMON_API_DBUS_ATTRIBUTE_EXTENSION_H_
#define COMMON_API_DBUS_ATTRIBUTE_EXTENSION_H_

#include "types.h"
#include "Event.h"

#include <cstdint>
#include <functional>
#include <memory>


namespace CommonAPI {

template<typename _AttributeType>
class AttributeExtension {
 public:
    inline _AttributeType& getBaseAttribute() {
        return baseAttribute_;
    }

 protected:
    AttributeExtension() = delete;
    AttributeExtension(_AttributeType& baseAttribute): baseAttribute_(baseAttribute) {
    }

    _AttributeType& baseAttribute_;
};

#ifdef WIN32
template<typename _AttributeType>
class WINDummyAttributeExtension : public CommonAPI::AttributeExtension<_AttributeType> {
    typedef AttributeExtension<_AttributeType> __baseClass_t;
    WINDummyAttribute dummyAttribute;
public:
    WINDummyAttributeExtension() {};
    WINDummyAttributeExtension(Proxy& proxy) :
    AttributeExtension<_AttributeType>(dummyAttribute) {}

    ~WINDummyAttributeExtension() {}
};
#endif

} // namespace CommonAPI

#endif // COMMON_API_DBUS_ATTRIBUTE_EXTENSION_H_
