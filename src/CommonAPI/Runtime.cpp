/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <dlfcn.h>

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <stdexcept>

#include "Runtime.h"
#include "Configuration.h"
#include "utils.h"


namespace CommonAPI {


static std::unordered_map<std::string, MiddlewareRuntimeLoadFunction>* registeredRuntimeLoadFunctions_;
static bool isDynamic_ = false;

static const char COMMONAPI_LIB_PREFIX[] = "libCommonAPI-";
static const char MIDDLEWARE_INFO_SYMBOL_NAME[] = "middlewareInfo";


inline bool Runtime::tryLoadLibrary(const std::string& libraryPath,
                                    void** sharedLibraryHandle,
                                    MiddlewareInfo** foundMiddlewareInfo) {

    //In case we find an already loaded library again while looking for another one,
    //there is no need to look at it
    if (dlopen(libraryPath.c_str(), RTLD_NOLOAD)) {
        return false;
    }

    //In order to place symbols of the newly loaded library ahead of already resolved symbols, we need
    //RTLD_DEEPBIND. This is necessary for this case: A library already is linked at compile time, but while
    //trying to resolve another library dynamically we might find the very same library again.
    //dlopen() doesn't know about the compile time linked library and will close it if dlclose() ever is
    //called, thereby causing memory corruptions. Therefore, we must be able to access the middlewareInfo
    //of the newly dlopened library in order to determine whether it already has been linked.
    *sharedLibraryHandle = dlopen(libraryPath.c_str(), RTLD_LAZY | RTLD_LOCAL | RTLD_DEEPBIND);
    if (sharedLibraryHandle == NULL) {
        return false;
    }

    *foundMiddlewareInfo = static_cast<MiddlewareInfo*>(dlsym(*sharedLibraryHandle, MIDDLEWARE_INFO_SYMBOL_NAME));

    //In this context, a resolved value of NULL it is just as invalid as if dlerror() was set additionally.
    if (!*foundMiddlewareInfo) {
        dlclose(*sharedLibraryHandle);
        return false;
    }

    if (!(*foundMiddlewareInfo)->middlewareName_ || !(*foundMiddlewareInfo)->getInstance_) {
        dlclose(sharedLibraryHandle);
        return false;
    }

    return true;
}


bool Runtime::checkAndLoadLibrary(const std::string& libraryPath,
                                  const std::string& requestedBindingIdentifier,
                                  bool keepLibrary) {

    void* sharedLibraryHandle = NULL;
    MiddlewareInfo* foundMiddlewareInfo;
    if (!tryLoadLibrary(libraryPath, &sharedLibraryHandle, &foundMiddlewareInfo)) {
        return false;
    }

    if (foundMiddlewareInfo->middlewareName_ != requestedBindingIdentifier) {
        //If library was linked at compile time (and therefore an appropriate runtime loader is registered),
        //the library must not be closed!
        auto foundRegisteredRuntimeLoader = registeredRuntimeLoadFunctions_->find(foundMiddlewareInfo->middlewareName_);
        if (foundRegisteredRuntimeLoader == registeredRuntimeLoadFunctions_->end()) {
            dlclose(sharedLibraryHandle);
        }
        return false;
    }

    if (!keepLibrary) {
        dlclose(sharedLibraryHandle);
    } else {
        //Extend visibility to make symbols available to all other libraries that are loaded afterwards,
        //e.g. libraries containing generated binding specific code.
        sharedLibraryHandle = dlopen(libraryPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
        if (!sharedLibraryHandle) {
            return false;
        }
        registeredRuntimeLoadFunctions_->insert( {foundMiddlewareInfo->middlewareName_, foundMiddlewareInfo->getInstance_} );
    }

    return true;
}


bool Runtime::checkAndLoadDefaultLibrary(std::string& foundBindingIdentifier, const std::string& libraryPath) {
    void* sharedLibraryHandle = NULL;
    MiddlewareInfo* foundMiddlewareInfo;
    if (!tryLoadLibrary(libraryPath, &sharedLibraryHandle, &foundMiddlewareInfo)) {
        return false;
    }

    //Extend visibility to make symbols available to all other linked libraries,
    //e.g. libraries containing generated binding specific code
    sharedLibraryHandle = dlopen(libraryPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (!sharedLibraryHandle) {
        return false;
    }
    registeredRuntimeLoadFunctions_->insert( {foundMiddlewareInfo->middlewareName_, foundMiddlewareInfo->getInstance_} );
    foundBindingIdentifier = foundMiddlewareInfo->middlewareName_;

    return true;
}


const std::vector<std::string> Runtime::readDirectory(const std::string& path) {
    std::vector<std::string> result;
    struct stat filestat;

    DIR *directory = opendir(path.c_str());

    if (!directory) {
        return std::vector<std::string>();
    }

    struct dirent* entry;

    while ((entry = readdir(directory))) {
        const std::string fqnOfEntry = path + entry->d_name;

        if (stat(fqnOfEntry.c_str(), &filestat)) {
            continue;
        }
        if (S_ISDIR(filestat.st_mode)) {
            continue;
        }

        if (strncmp(COMMONAPI_LIB_PREFIX, entry->d_name, strlen(COMMONAPI_LIB_PREFIX)) != 0) {
            continue;
        }

        const char* fileNamePtr = entry->d_name;
        while ((fileNamePtr = strchr(fileNamePtr + 1, '.'))) {
            if (strncmp(".so", fileNamePtr, 3) == 0) {
                break;
            }
        }

        if (fileNamePtr) {
            result.push_back(fqnOfEntry);
        }
    }

    closedir (directory);

    std::sort( result.begin(), result.end() );

    return result;
}


struct LibraryVersion {
    int32_t major;
    int32_t minor;
    int32_t revision;
};

bool operator<(LibraryVersion const& lhs, LibraryVersion const& rhs) {
    if (lhs.major == rhs.major) {
        if (lhs.minor == rhs.minor) {
            return lhs.revision < rhs.revision;
        }
        return lhs.minor < rhs.minor;
    }
    return lhs.major < rhs.major;
}


std::shared_ptr<Runtime> Runtime::checkDynamicLibraries(const std::string& requestedMiddlewareName, LoadState& loadState) {
    const std::string& middlewareLibraryPath = Configuration::getInstance().getMiddlewareLibraryPath(requestedMiddlewareName);

    if (middlewareLibraryPath != "") {
        if (!checkAndLoadLibrary(middlewareLibraryPath, requestedMiddlewareName, true)) {
            //A path for requestedMiddlewareName was configured, but no corresponding library was found
            loadState = LoadState::CONFIGURATION_ERROR;
            return std::shared_ptr<Runtime>(NULL);
        } else {
            const std::string currentBinaryFQN = getCurrentBinaryFileFQN();
            const uint32_t lastPathSeparatorPosition = currentBinaryFQN.find_last_of("/\\");
            const std::string currentBinaryPath = currentBinaryFQN.substr(0, lastPathSeparatorPosition + 1);
            auto foundRuntimeLoader = registeredRuntimeLoadFunctions_->find(requestedMiddlewareName);
            if (foundRuntimeLoader != registeredRuntimeLoadFunctions_->end()) {
                return (foundRuntimeLoader->second)();
            }
            //One should not get here
            loadState = LoadState::BINDING_ERROR;
            return std::shared_ptr<Runtime>(NULL);
        }
    }

    const std::vector<std::string>& librarySearchPaths = Configuration::getInstance().getLibrarySearchPaths();

    LibraryVersion highestVersionFound = {0, 0, 0};
    std::string fqnOfHighestVersion = "";
    struct stat filestat;

    for (const std::string& singleSearchPath: librarySearchPaths) {
        std::vector<std::string> orderedLibraries = readDirectory(singleSearchPath);

        for (const std::string& fqnOfEntry : orderedLibraries) {
            std::string versionString;
            LibraryVersion currentLibraryVersion = {-1, -1, -1};

            const char* fileNamePtr = fqnOfEntry.c_str();
            while ((fileNamePtr = strchr(fileNamePtr + 1, '.'))) {
                if (strncmp(".so", fileNamePtr, 3) == 0) {
                    break;
                }
            }

            const char* positionOfFirstDot = strchr(fileNamePtr + 1, '.');
            if (positionOfFirstDot) {
                versionString = positionOfFirstDot + 1;
            }

            std::vector<std::string> versionElements = split(versionString, '.');
            if (versionElements.size() >= 1 && containsOnlyDigits(versionElements[0])) {
                currentLibraryVersion.major = strtol(versionElements[0].c_str(), NULL, 0);
            }
            if (versionElements.size() >= 3 && containsOnlyDigits(versionElements[2])) {
                currentLibraryVersion.minor = strtol(versionElements[1].c_str(), NULL, 0);
                currentLibraryVersion.revision = strtol(versionElements[2].c_str(), NULL, 0);
            }

            if (highestVersionFound < currentLibraryVersion) {
                if (!checkAndLoadLibrary(fqnOfEntry, requestedMiddlewareName, false)) {
                    continue;
                }
                highestVersionFound = currentLibraryVersion;
                fqnOfHighestVersion = fqnOfEntry;
            }
        }
    }

    if (fqnOfHighestVersion != "") {
        checkAndLoadLibrary(fqnOfHighestVersion, requestedMiddlewareName, true);

        auto foundRuntimeLoader = registeredRuntimeLoadFunctions_->find(requestedMiddlewareName);
        if (foundRuntimeLoader != registeredRuntimeLoadFunctions_->end()) {
            std::shared_ptr<Runtime> loadedRuntime = foundRuntimeLoader->second();
            if (!loadedRuntime) {
                loadState = LoadState::BINDING_ERROR;
            }
            return loadedRuntime;
        }
    }

    loadState = LoadState::NO_LIBRARY_FOUND;

    return std::shared_ptr<Runtime>();
}


std::shared_ptr<Runtime> Runtime::checkDynamicLibraries(LoadState& loadState) {
    const std::vector<std::string>& librarySearchPaths = Configuration::getInstance().getLibrarySearchPaths();

    for (const std::string& singleSearchPath : librarySearchPaths) {
        std::vector<std::string> orderedLibraries = readDirectory(singleSearchPath);

        for (const std::string& fqnOfEntry: orderedLibraries) {
            std::string foundBindingName;
            if (!checkAndLoadDefaultLibrary(foundBindingName, fqnOfEntry)) {
                continue;
            }

            auto foundRuntimeLoader = registeredRuntimeLoadFunctions_->find(foundBindingName);
            if (foundRuntimeLoader != registeredRuntimeLoadFunctions_->end()) {
                return (foundRuntimeLoader->second)();
            }
        }
    }

    loadState = LoadState::NO_LIBRARY_FOUND;

    return std::shared_ptr<Runtime>();
}


void Runtime::registerRuntimeLoader(const std::string& middlewareName, const MiddlewareRuntimeLoadFunction& middlewareRuntimeLoadFunction) {
    if (!registeredRuntimeLoadFunctions_) {
        registeredRuntimeLoadFunctions_ = new std::unordered_map<std::string, MiddlewareRuntimeLoadFunction>();
    }
    if (!isDynamic_) {
        registeredRuntimeLoadFunctions_->insert( {middlewareName, middlewareRuntimeLoadFunction});
    }
}


std::shared_ptr<Runtime> Runtime::load() {
    LoadState dummyState;
    return load(dummyState);
}


std::shared_ptr<Runtime> Runtime::load(LoadState& loadState) {
    isDynamic_ = true;
    loadState = LoadState::SUCCESS;
    if(!registeredRuntimeLoadFunctions_) {
        registeredRuntimeLoadFunctions_ = new std::unordered_map<std::string, MiddlewareRuntimeLoadFunction>();
    }

    const std::string& defaultBindingIdentifier = Configuration::getInstance().getDefaultMiddlewareIdentifier();
    if (defaultBindingIdentifier != "") {
        const auto defaultRuntimeLoader = registeredRuntimeLoadFunctions_->find(defaultBindingIdentifier);
        if (defaultRuntimeLoader != registeredRuntimeLoadFunctions_->end()) {
            return (defaultRuntimeLoader->second)();
        }
        return checkDynamicLibraries(defaultBindingIdentifier, loadState);

    } else {
        const auto defaultRuntimeLoader = registeredRuntimeLoadFunctions_->begin();
        if (defaultRuntimeLoader != registeredRuntimeLoadFunctions_->end()) {
            return (defaultRuntimeLoader->second)();
        }
        return checkDynamicLibraries(loadState);
    }
}


std::shared_ptr<Runtime> Runtime::load(const std::string& middlewareIdOrAlias) {
    LoadState dummyState;
    return load(middlewareIdOrAlias, dummyState);
}

std::shared_ptr<Runtime> Runtime::load(const std::string& middlewareIdOrAlias, LoadState& loadState) {
    isDynamic_ = true;
    loadState = LoadState::SUCCESS;
    if (!registeredRuntimeLoadFunctions_) {
        registeredRuntimeLoadFunctions_ = new std::unordered_map<std::string, MiddlewareRuntimeLoadFunction>();
    }

    const std::string middlewareName = Configuration::getInstance().getMiddlewareNameForAlias(middlewareIdOrAlias);

    auto foundRuntimeLoader = registeredRuntimeLoadFunctions_->find(middlewareName);
    if (foundRuntimeLoader != registeredRuntimeLoadFunctions_->end()) {
        return (foundRuntimeLoader->second)();
    }

    return checkDynamicLibraries(middlewareName, loadState);
}


std::shared_ptr<MainLoopContext> Runtime::getNewMainLoopContext() const {
    return std::make_shared<MainLoopContext>();
}

std::shared_ptr<Factory> Runtime::createFactory(const std::string factoryName,
                                                const bool nullOnInvalidName) {
    return createFactory(std::shared_ptr<MainLoopContext>(NULL), factoryName, nullOnInvalidName);
}

std::shared_ptr<Factory> Runtime::createFactory(std::shared_ptr<MainLoopContext> mainLoopContext,
                                                const std::string factoryName,
                                                const bool nullOnInvalidName) {
    if(mainLoopContext && !mainLoopContext->isInitialized()) {
        return std::shared_ptr<Factory>(NULL);
    }
    return doCreateFactory(mainLoopContext, factoryName, nullOnInvalidName);
}


} // namespace CommonAPI
