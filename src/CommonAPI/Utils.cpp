// Copyright (C) 2014-2020 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <cctype>
#include <algorithm>
#include <sstream>
#include <functional>

#include <CommonAPI/Utils.hpp>

namespace CommonAPI {

std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems) {
    std::istringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

void trim(std::string& toTrim) {
    toTrim.erase(
        toTrim.begin(),
        std::find_if(
            toTrim.begin(),
            toTrim.end(),
            [](int c) {return !std::isspace(c);}
        )
    );

    toTrim.erase(
        std::find_if(
            toTrim.rbegin(),
            toTrim.rend(),
            [](int c) {return !std::isspace(c);}).base(),
            toTrim.end()
    );
}

}//namespace CommonAPI
