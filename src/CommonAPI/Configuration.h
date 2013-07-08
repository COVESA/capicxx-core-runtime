/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef COMMONAPI_CONFIGURATION_H_
#define COMMONAPI_CONFIGURATION_H_


#include <unordered_map>
#include <vector>
#include <string>
#include <cstring>


namespace CommonAPI {


static const char COMMONAPI_CONFIG_SUFFIX[] = ".conf";
static const char COMMONAPI_GLOBAL_CONFIG_ROOT[] = "/etc/CommonAPI/";
static const char COMMONAPI_GLOBAL_CONFIG_FQN[] = "/etc/CommonAPI/CommonAPI.conf";

static const char COMMONAPI_STD_LIB_PATH[] = "/usr/lib:/usr/local/lib/";
static const char COMMONAPI_ENVIRONMENT_BINDING_PATH[] = "COMMONAPI_BINDING_PATH";

static const char CATEGORY_ENDING = '}';

static const char CATEGORY_IDENTIFIER_BINDING[] = "{binding:";

static const char BINDING_PARAMETER_ALIAS[] = "alias";
static const char BINDING_PARAMETER_LIBPATH[] = "libpath";
static const char BINDING_PARAMETER_GENPATH[] = "genpath";
static const char BINDING_PARAMETER_DEFAULT[] = "default";


/**
 * Represents the contents of all parsed CommonAPI Configuration files.
 *
 * For more information on how to configure CommonAPI, see attached documentation.
 */
class Configuration {
 public:
    /**
     * Returns the instance of the Configuration.
     *
     * When first calling this method, all configuration files that are found are parsed and
     * the values are stored within this class.
     *
     * @return The singleton instance of the CommonAPI Configuration.
     */
    static const Configuration& getInstance();

    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;
    Configuration(Configuration&&) = delete;
    Configuration& operator=(Configuration&&) = delete;

    /**
     * Returns the search paths on which binding specific libraries may be found.
     *
     * Default search paths are /usr/lib and /usr/local/lib, those two will always be returned.
     * If additional search paths have been configured, those will also be returned.
     *
     * @return
     */
    const std::vector<std::string>& getLibrarySearchPaths() const;

    /**
     * Returns the actual middleware identifier for the given alias.
     *
     * If no such alias has been configured, the given alias itself will be returned.
     *
     * @return The middleware identifier or the given alias itself, if no mapping to a middleware identifier was found.
     */
    const std::string& getMiddlewareNameForAlias(const std::string& alias) const;

    /**
     * Returns the specified library path for the given middleware identifier.
     *
     * If a path to a specific library has been configured for the given middleware identifier, this path will be returned.
     * If no such path has been configured, the empty string will be returned.
     *
     * @return The path to the middleware library, if any is known, the empty string "" otherwise.
     */
    const std::string& getMiddlewareLibraryPath(const std::string& middlewareIdentifier) const;

    /**
     * Returns the paths to other generic libraries configured for a specific binding.
     *
     * This function is meant to be called by middleware libraries. Will return all configured paths to
     * generic libraries. You likely wil want to use the utility functions provided in <CommonAPI/utils.h>
     * to do the loading. To arrange and time the loading is responsibility of the middleware only.
     *
     * @return A vector containing all generic libraries associated with the given middlewareIdentifier.
     */
    const std::vector<std::string>& getGenericLibraryPaths(const std::string& middlewareIdentifier) const;

    /**
     * Returns the configured default middleware identifier.
     *
     * If no default has been configured, the empty string "" will be returned.
     *
     * @return The configured default middleware identifier.
     */
    const std::string& getDefaultMiddlewareIdentifier() const;

 private:
    Configuration() = default;

    void readConfigFile(std::ifstream& addressConfigFile);
    void retrieveCommonApiConfiguration();
    void readEnvironmentVariables();
};



} // namespace CommonAPI

#endif /* COMMONAPI_CONFIGURATION_H_ */
