/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_UTILS_H_
#define COMMONAPI_UTILS_H_

#ifndef WIN32
#include <dirent.h>
#include <dlfcn.h>
#endif
#include <sys/stat.h>

#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>

#include <locale>
#include <functional>

#ifdef WIN32
#include <xfunctional>
#define WIN32_LEAN_AND_MEAN // this prevents windows.h from including winsock.h, which causes duplicate definitions with winsock2.h
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace CommonAPI {


#if  __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
#  define COMMONAPI_DEPRECATED __attribute__ ((__deprecated__))
#elif defined(_MSC_VER) && (_MSC_VER >= 1300)
#  define COMMONAPI_DEPRECATED __declspec(deprecated)
#else
#  define COMMONAPI_DEPRECATED
#endif


/**
 * \brief Returns the fully qualified name of the binary.
 *
 * @return The name of the currently executing binary.
 */
std::string getCurrentBinaryFileFQN();

/**
 * \brief Splits a std::string according to the given delim-char.
 *
 * The string will be splitted at each position the delim char is encountered. The delim itself
 * will be removed from the result.
 *
 * @param s: The string that is to be splitted
 * @param delim: The character that separates the resulting string tokens in the original string
 * @param elems: Reference to the vector that shall be filled with the splitted string elements.
 *
 * @return A reference to the vector you passed in (elems)
 */
std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems);

/**
 * \brief Splits a std::string according to the given delim-char.
 *
 * The string will be splitted at each position the delim char is encountered. The delim itself
 * will be removed from the result.
 *
 * @param s: The string that is to be splitted
 * @param delim: The character that separates the resulting string tokens in the original string
 *
 * @return A vector containing the splitted string elements.
 */
std::vector<std::string> split(const std::string& s, char delim);

inline bool isspace(char c) {
    return std::isspace(c, std::locale());
}

/**
 * \brief Trims whitespaces from beginning and end of a std::string.
 *
 * @param toTrim: The string that is to be trimmed.
 */
void trim(std::string& toTrim);

bool notIsdigit(char c);

/**
 * \brief Checks whether the given string contains nothing but digits.
 *
 * @param toCheck: The string that is to be checked on the presence of anything but digits.
 *
 * @return true if toCheck contains nothing but digits, false otherwise.
 */
bool containsOnlyDigits(const std::string& toCheck);

bool notIsalnum(char c);

/**
 * \brief Checks whether the given string contains nothing but alphanumeric characters.
 *
 * @param toCheck: The string that is to be checked on the presence of anything but alphanumeric characters.
 *
 * @return true if toCheck contains nothing but alphanumeric characters, false otherwise.
 */
bool containsOnlyAlphanumericCharacters(const std::string& toCheck);

/**
 * \brief Checks whether the given std::string is a valid CommonAPI domain name.
 *
 * @param domainName: The std::string that is to be checked.
 *
 * @return true if domainName is a valid CommonAPI domainName, false otherwise.
 */
bool isValidDomainName(const std::string& domainName);

/**
 * \brief Checks whether the given std::string is a valid CommonAPI service name.
 *
 * @param serviceName: The std::string that is to be checked.
 *
 * @return true if serviceName is a valid CommonAPI serviceName, false otherwise.
 */
bool isValidServiceName(const std::string& serviceName);

/**
 * \brief Checks whether the given std::string is a valid CommonAPI instance ID.
 *
 * @param instanceId: The std::string that is to be checked.
 *
 * @return true if instanceId is a valid CommonAPI instance ID, false otherwise.
 */
bool isValidInstanceId(const std::string& instanceId);

/**
 * \brief Checks whether the given std::string is a valid CommonAPI address.
 *
 * @param commonApiAddressName: The std::string that is to be checked.
 *
 * @return true if commonApiAddress is a valid CommonAPI address, false otherwise.
 */
bool isValidCommonApiAddress(const std::string& commonApiAddress);

#ifndef WIN32
/**
 * \brief Loads a specific generic library at runtime.
 *
 * The library will be loaded using dlopen(3) with the flags (RTLD_NOW | RTLD_GLOBAL), if all pre-checks are
 * successful. Pre-checks include the verification that the given parameters actually point to a library and
 * optionally if the library matches the standard name pattern for CommonAPI generic libraries. The standard
 * name pattern is "lib<wellKnownMiddlewareName>Gen-<arbitraryName>.so[.major[.minor.revision]]".
 *
 * @param wellKnownMiddlewareName: The name of the middleware that requests the loading of the library.
 * @param libraryName: The name of the library that shall be loaded.
 * @param path: The path at which the library is to be found. path + library name together make up the fully
 *              qualified name of the library.
 * @param checkStandardNamePattern: If set to true, it will be ensured the library matches the CommonAPI
 *                                  standard name pattern for generic libraries. This is meant as a safety measure
 *                                  to prevent the loading of unnecessary or the wrong libraries. Set to false if
 *                                  you are sure about what you are doing.
 * @return true if the library could be loaded successfully, false otherwise.
 *
 * @note The well known middleware name is included as a parameter because the additional libraries normally are needed
 *       only by specific middlewares. This name however will only be taken into consideration during name checking
 *       if the checkStandardNamePattern flag is set to true.
 */
bool loadGenericLibrary(const std::string& wellKnownMiddlewareName, const std::string& libraryName, const std::string& path, bool checkStandardNamePattern = true);

/**
 * \brief Loads a specific generic library at runtime.
 *
 * The library will be loaded using dlopen(3) with the flags (RTLD_NOW | RTLD_GLOBAL), if all pre-checks are
 * successful. Pre-checks include the verification that the given parameters actually point to a library and
 * optionally if the library matches the standard name pattern for CommonAPI generic libraries. The standard
 * name pattern is "lib<wellKnownMiddlewareName>Gen-<arbitraryName>.so[.major[.minor.revision]]".
 *
 * @param wellKnownMiddlewareName: The name of the middleware that requests the loading of the library.
 * @param fqnOfLibrary: The fully qualified name of the library.
 * @param checkStandardNamePattern: If set to true, it will be ensured the library matches the CommonAPI
 *                                  standard name pattern for generic libraries. This is meant as a safety measure
 *                                  to prevent the loading of unnecessary or the wrong libraries. Set to false if
 *                                  you are sure about what you are doing.
 * @return true if the library could be loaded successfully, false otherwise.
 *
 * @note The well known middleware name is included as a parameter because the additional libraries normally are needed
 *       only by specific middlewares. This name however will only be taken into consideration during name checking
 *       if the checkStandardNamePattern flag is set to true.
 */
bool loadGenericLibrary(const std::string& wellKnownMiddlewareName,
                               const std::string& fqnOfLibrary,
                               bool checkStandardNamePattern = true);

/**
 * \brief Searches the given path for additional generic CommonAPI libraries and loads them.
 *
 * All libraries for which the pre-checks are successful will be loaded using dlopen(3) with the flags
 * (RTLD_NOW | RTLD_GLOBAL). Pre-checks include the verification that the given parameters actually point
 * to a library and if the library matches the standard name pattern for CommonAPI generic libraries.
 * The standard name pattern is "lib<wellKnownMiddlewareName>Gen-<arbitraryName>.so[.major[.minor.revision]]".
 *
 * @param wellKnownMiddlewareName: The name of the middleware that requests the loading of the library. Will
 *                                 be used to perform the name check.
 * @param singleSearchPath: The directory that is to be searched for libraries.
 */
void findAndLoadGenericLibraries(const std::string& requestedMiddlewareName, const std::string& singleSearchPath);

/**
 * \brief Searches the given paths for additional generic CommonAPI libraries and loads them.
 *
 * All libraries for which the pre-checks are successful will be loaded using dlopen(3) with the flags
 * (RTLD_NOW | RTLD_GLOBAL). Pre-checks include the verification that the given parameters actually point
 * to a library and if the library matches the standard name pattern for CommonAPI generic libraries.
 * The standard name pattern is "lib<wellKnownMiddlewareName>Gen-<arbitraryName>.so[.major[.minor.revision]]".
 *
 * @param wellKnownMiddlewareName: The name of the middleware that requests the loading of the library. Will
 *                                 be used to perform the name check.
 * @param searchPaths: The directories that are to be searched for libraries.
 */
void findAndLoadGenericLibraries(const std::string& requestedMiddlewareName, const std::vector<std::string>& searchPaths);
#endif

} //namespace CommonAPI


#endif /* COMMONAPI_UTILS_H_ */
