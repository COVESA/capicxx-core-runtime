/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
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

} // namespace CommonAPI

#endif // COMMON_API_DBUS_ATTRIBUTE_EXTENSION_H_
