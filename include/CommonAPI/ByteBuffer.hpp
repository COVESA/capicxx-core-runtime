// Copyright (C) 2013-2020 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.hpp> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_BYTE_BUFFER_HPP_
#define COMMONAPI_BYTE_BUFFER_HPP_

#include <vector>
#include <cstdint>

namespace CommonAPI {

typedef std::vector<uint8_t> ByteBuffer;

} // namespace CommonAPI

#endif // COMMONAPI_BYTE_BUFFER_HPP_
