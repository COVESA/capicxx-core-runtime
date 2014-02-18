/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include <fstream>

#include "Configuration.h"
#include "utils.h"


namespace CommonAPI {

std::unordered_map<std::string, std::string> knownMiddlewareAliases_;
std::unordered_map<std::string, std::string> knownMiddlewarePaths_;
std::unordered_map<std::string, std::vector<std::string> > knownGeneratedPaths_;
std::vector<std::string> librarySearchPaths_;
std::string defaultBindingIdentifier_ = "";


const Configuration& Configuration::getInstance() {
    static Configuration* instance = NULL;
    if (!instance) {
        instance = new Configuration();
        instance->retrieveCommonApiConfiguration();
    }
    return *instance;
}

const std::vector<std::string>& Configuration::getLibrarySearchPaths() const {
    return librarySearchPaths_;
}

const std::string& Configuration::getMiddlewareNameForAlias(const std::string& alias) const {
    auto foundMiddlewareName = knownMiddlewareAliases_.find(alias);
    if (foundMiddlewareName != knownMiddlewareAliases_.end()) {
        return foundMiddlewareName->second;
    } else {
        return alias;
    }
}

const std::string& Configuration::getMiddlewareLibraryPath(const std::string& middlewareIdentifier) const {
    auto foundMiddlewarePath = knownMiddlewarePaths_.find(middlewareIdentifier);
    if (foundMiddlewarePath == knownMiddlewarePaths_.end()) {
        static const std::string emptyReturn = "";
        return emptyReturn;
    }
    return foundMiddlewarePath->second;
}

const std::vector<std::string>& Configuration::getGenericLibraryPaths(const std::string& middlewareIdentifier) const {
    const auto& generatedPathsForMiddleware = knownGeneratedPaths_.find(middlewareIdentifier);
    if (generatedPathsForMiddleware != knownGeneratedPaths_.end()) {
        return generatedPathsForMiddleware->second;
    }
    static const std::vector<std::string> emptyReturn;
    return emptyReturn;
}

const std::string& Configuration::getDefaultMiddlewareIdentifier() const {
    return defaultBindingIdentifier_;
}

void Configuration::readConfigFile(std::ifstream& addressConfigFile) {
    std::string currentlyParsedBindingIdentifier = "";

    std::string readLine;

    while (addressConfigFile.good()) {
        getline(addressConfigFile, readLine);
        const size_t readLineLength = readLine.length();

        if (strncmp(readLine.c_str(), CATEGORY_IDENTIFIER_BINDING, strlen(CATEGORY_IDENTIFIER_BINDING)) == 0
                        && readLine[readLineLength - 1] == CATEGORY_ENDING) {

            std::string newBindingIdentifier = readLine.substr(
                            strlen(CATEGORY_IDENTIFIER_BINDING),
                            readLineLength - (strlen(CATEGORY_IDENTIFIER_BINDING) + 1));

            trim(newBindingIdentifier);
            if (containsOnlyAlphanumericCharacters(newBindingIdentifier)) {
                currentlyParsedBindingIdentifier = newBindingIdentifier;
            }

        } else if (currentlyParsedBindingIdentifier != "") {
            std::vector<std::string> parameterElements = split(readLine, '=');
            if (parameterElements.size() == 2) {

                if (parameterElements.at(0) == BINDING_PARAMETER_ALIAS) {
                    std::vector<std::string> aliases = split(parameterElements.at(1), ':');
                    for (const std::string& singleAlias: aliases) {
                        knownMiddlewareAliases_.insert( {singleAlias, currentlyParsedBindingIdentifier});
                    }

                } else if (parameterElements.at(0) == BINDING_PARAMETER_LIBPATH) {
                    knownMiddlewarePaths_.insert( {currentlyParsedBindingIdentifier, parameterElements.at(1)});

                } else if (parameterElements.at(0) == BINDING_PARAMETER_GENPATH) {
                    std::vector<std::string> paths = split(parameterElements.at(1), ':');
                    auto alreadyKnownPaths = knownGeneratedPaths_.find(currentlyParsedBindingIdentifier);

                    if (alreadyKnownPaths == knownGeneratedPaths_.end()) {
                        const std::vector<std::string> pathSet(paths.begin(), paths.end());
                        knownGeneratedPaths_.insert( {currentlyParsedBindingIdentifier, std::move(pathSet)} );
                    } else {
                        alreadyKnownPaths->second.insert(alreadyKnownPaths->second.end(), paths.begin(), paths.end());
                    }
                }

            } else if (parameterElements.size() == 1) {
                if (parameterElements.at(0) == BINDING_PARAMETER_DEFAULT && defaultBindingIdentifier_ == "") {
                    defaultBindingIdentifier_ = currentlyParsedBindingIdentifier;
                }
            }
        }
    }
}


void Configuration::readEnvironmentVariables() {
    librarySearchPaths_ = split(COMMONAPI_STD_LIB_PATH, ':');

    bool errorOccured = false;
#ifdef WIN32
    char* environmentBindingPath;
    size_t len;
    errno_t err = _dupenv_s(&environmentBindingPath, &len, COMMONAPI_ENVIRONMENT_BINDING_PATH);
    if (err != 0 || environmentBindingPath == NULL) {
        errorOccured = true;
    }
#else
    const char* environmentBindingPath = getenv(COMMONAPI_ENVIRONMENT_BINDING_PATH);
    if (!environmentBindingPath) {
        errorOccured = true;
    }
#endif

    if (!errorOccured) {
        std::vector<std::string> environmentPaths = split(environmentBindingPath, ':');
        librarySearchPaths_.insert(librarySearchPaths_.begin(), environmentPaths.begin(), environmentPaths.end());
    }
}


void Configuration::retrieveCommonApiConfiguration() {
    readEnvironmentVariables();

    std::string fqnOfConfigFile = getCurrentBinaryFileFQN();
    fqnOfConfigFile += COMMONAPI_CONFIG_SUFFIX;

    std::ifstream commonapiConfigFile;
    commonapiConfigFile.open(fqnOfConfigFile.c_str());

    if (commonapiConfigFile.is_open()) {
        readConfigFile(commonapiConfigFile);
        commonapiConfigFile.close();
    }

    commonapiConfigFile.clear();
    std::vector<std::string> splittedConfigFQN = split(fqnOfConfigFile, '/');
    std::string globalConfigFQN = COMMONAPI_GLOBAL_CONFIG_ROOT + splittedConfigFQN.at(splittedConfigFQN.size() - 1);
    commonapiConfigFile.open(globalConfigFQN);
    if (commonapiConfigFile.is_open()) {
        readConfigFile(commonapiConfigFile);
        commonapiConfigFile.close();
    }
    commonapiConfigFile.clear();

    commonapiConfigFile.open(COMMONAPI_GLOBAL_CONFIG_FQN);
    if (commonapiConfigFile.is_open()) {
        readConfigFile(commonapiConfigFile);
        commonapiConfigFile.close();
    }
}


} // namespace CommonAPI
