// Copyright (C) 2014-2020 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.hpp> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_UTILS_HPP_
#define COMMONAPI_UTILS_HPP_

#include <string>
#include <vector>

#include <CommonAPI/Export.hpp>

namespace CommonAPI {

std::vector<std::string> COMMONAPI_EXPORT split(const std::string& s, char delim);
void COMMONAPI_EXPORT trim(std::string &_s);

} //namespace CommonAPI

#endif // COMMONAPI_UTILS_HPP_
