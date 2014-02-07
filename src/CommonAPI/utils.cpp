/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "utils.h"

namespace CommonAPI {
std::string getCurrentBinaryFileFQN() {
    #ifdef WIN32
        TCHAR result[MAX_PATH];
        std::basic_string<TCHAR> resultString(result, GetModuleFileName(NULL, result, MAX_PATH));
        return std::string(resultString.begin(), resultString.end());
    #else
        char fqnOfBinary[FILENAME_MAX];
        char pathToProcessImage[FILENAME_MAX];

        sprintf(pathToProcessImage, "/proc/%d/exe", getpid());
        const ssize_t lengthOfFqn = readlink(pathToProcessImage, fqnOfBinary, sizeof(fqnOfBinary) - 1);

        if (lengthOfFqn != -1) {
            fqnOfBinary[lengthOfFqn] = '\0';
            return std::string(std::move(fqnOfBinary));
        }
        else {
            return std::string("");
        }
    #endif
    }

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
                    std::find_if(toTrim.begin(),
                                 toTrim.end(),
                                 std::not1(std::ptr_fun(isspace)))
                                                    );
    toTrim.erase(
                    std::find_if(toTrim.rbegin(),
                                 toTrim.rend(),
                                 std::not1(std::ptr_fun(isspace))).base(),
                                 toTrim.end()
                    );
}

bool notIsdigit(char c) {
    return !std::isdigit(c, std::locale());
}

bool containsOnlyDigits(const std::string& toCheck) {
    auto firstNonDigitIt = std::find_if(
                    toCheck.begin(),
                    toCheck.end(),
                    notIsdigit);

    return firstNonDigitIt == toCheck.end();
}

bool notIsalnum(char c) {
    return !std::isalnum(c, std::locale());
}

bool containsOnlyAlphanumericCharacters(const std::string& toCheck) {
    auto firstNonAlphanumericCharacterIt = std::find_if(
                    toCheck.begin(),
                    toCheck.end(),
                    notIsalnum);

    return firstNonAlphanumericCharacterIt == toCheck.end();
}

bool isValidDomainName(const std::string& domainName) {
    return containsOnlyAlphanumericCharacters(domainName);
}

bool isValidServiceName(const std::string& serviceName) {
    bool isValid = serviceName[0] != '.' && serviceName[serviceName.size() - 1] != '.';

    if (isValid) {
        std::vector<std::string> splittedServiceName = split(serviceName, '.');

        for (auto serviceNameElementIt = splittedServiceName.begin();
                        serviceNameElementIt != splittedServiceName.end() && isValid;
                        ++serviceNameElementIt) {
            isValid &= containsOnlyAlphanumericCharacters(*serviceNameElementIt);
        }
    }

    return isValid;
}

bool isValidInstanceId(const std::string& instanceId) {
    //Validation rules for ServiceName and InstanceID are equivalent
    return isValidServiceName(instanceId);
}

bool isValidCommonApiAddress(const std::string& commonApiAddress) {
    std::vector<std::string> splittedAddress = split(commonApiAddress, ':');
    if (splittedAddress.size() != 3) {
        return false;
    }
    return isValidDomainName(splittedAddress[0]) && isValidServiceName(splittedAddress[1]) && isValidInstanceId(splittedAddress[2]);
}

#ifndef WIN32
bool loadGenericLibrary(const std::string& wellKnownMiddlewareName, const std::string& libraryName, const std::string& path, bool checkStandardNamePattern) {
    std::string fqnOfLibrary = path + libraryName;
    struct stat filestat;
    if (stat(fqnOfLibrary.c_str(), &filestat)) {
        return false;
    }
    if (S_ISDIR(filestat.st_mode)) {
        return false;
    }

    if (checkStandardNamePattern) {
        const std::string generatedLibPrefix = "lib" + wellKnownMiddlewareName + "Gen-";
        if (strncmp(generatedLibPrefix.c_str(), libraryName.c_str(), generatedLibPrefix.length()) != 0) {
            return false;
        }

        const char* fileNamePtr = libraryName.c_str();
        while ((fileNamePtr = strchr(fileNamePtr + 1, '.'))) {
            if (strncmp(".so\0", fileNamePtr, 4) == 0 || strncmp(".so.", fileNamePtr, 4) == 0) {
                break;
            }
        }

        if (!fileNamePtr) {
            return false;
        }
    }

    dlopen(fqnOfLibrary.c_str(), RTLD_NOW | RTLD_GLOBAL);
    return true;
}

bool loadGenericLibrary(const std::string& wellKnownMiddlewareName,
                               const std::string& fqnOfLibrary,
                               bool checkStandardNamePattern) {
    uint32_t position = fqnOfLibrary.find_last_of("/\\");
    std::string path = fqnOfLibrary.substr(0, position + 1);
    std::string file = fqnOfLibrary.substr(position + 1);
    return loadGenericLibrary(wellKnownMiddlewareName, file, path, checkStandardNamePattern);
}

void findAndLoadGenericLibraries(const std::string& requestedMiddlewareName, const std::string& singleSearchPath) {
    DIR *directory = opendir(singleSearchPath.c_str());

    if (directory != NULL) {
        struct dirent* entry;

        while ((entry = readdir(directory))) {
            loadGenericLibrary(requestedMiddlewareName, entry->d_name, singleSearchPath, true);
        }
    }
}

void findAndLoadGenericLibraries(const std::string& requestedMiddlewareName, const std::vector<std::string>& searchPaths) {
    for (const std::string& singleSearchPath : searchPaths) {
        findAndLoadGenericLibraries(requestedMiddlewareName, singleSearchPath.c_str());
    }
}
#endif
}//namespace CommonAPI
