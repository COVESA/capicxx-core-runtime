// Copyright (C) 2013-2020 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.hpp> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_CONTAINERUTILS_HPP_
#define COMMONAPI_CONTAINERUTILS_HPP_

#include <functional>
#include <memory>

#include <CommonAPI/Export.hpp>

namespace CommonAPI {

class ClientId;

struct COMMONAPI_EXPORT SharedPointerClientIdContentHash {
    size_t operator()(const std::shared_ptr<ClientId> &_t) const;
};

struct COMMONAPI_EXPORT SharedPointerClientIdContentEqual {
    bool operator()(const std::shared_ptr<ClientId> &_lhs, const std::shared_ptr<ClientId> &_rhs) const;
};

}  // namespace CommonAPI

#endif // COMMONAPI_CONTAINERUTILS_HPP_
