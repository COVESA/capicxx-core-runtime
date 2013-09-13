/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ContainerUtils.h"
#include "types.h"

namespace CommonAPI {

size_t SharedPointerClientIdContentHash::operator()(const std::shared_ptr<ClientId>& t) const {
    if (t) {
        return t->hashCode();
    } else {
        return NULL;
    }
}

bool SharedPointerClientIdContentEqual::operator()(const std::shared_ptr<ClientId>& a, const std::shared_ptr<ClientId>& b) const {
    if (a && b) {
        return *a==*b;
    } else {
        return false;
    }
}

}  // namespace std
