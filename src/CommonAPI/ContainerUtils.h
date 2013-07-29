/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef FUNCTIONALHASH_H_
#define FUNCTIONALHASH_H_

#include <functional>
#include <memory>

namespace CommonAPI {
class ClientId;

struct SharedPointerClientIdContentHash : public std::unary_function<std::shared_ptr<ClientId>, size_t> {
    size_t operator()(const std::shared_ptr<ClientId>& t) const;
};

struct SharedPointerClientIdContentEqual : public std::binary_function<std::shared_ptr<ClientId>, std::shared_ptr<ClientId>, bool> {
    bool operator()(const std::shared_ptr<ClientId>& a, const std::shared_ptr<ClientId>& b) const;
};


}  // namespace std


#endif /* FUNCTIONALHASH_H_ */
