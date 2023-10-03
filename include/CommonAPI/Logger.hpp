// Copyright (C) 2015-2020 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.hpp> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_LOGGER_HPP_
#define COMMONAPI_LOGGER_HPP_

#include <memory>
#include <sstream>
#include <cstdint>

#include <CommonAPI/Export.hpp>

#define COMMONAPI_FATAL     CommonAPI::Logger::fatal
#define COMMONAPI_ERROR     CommonAPI::Logger::error
#define COMMONAPI_WARNING   CommonAPI::Logger::warning
#define COMMONAPI_INFO      CommonAPI::Logger::info
#define COMMONAPI_DEBUG     CommonAPI::Logger::debug
#define COMMONAPI_VERBOSE   CommonAPI::Logger::verbose

namespace CommonAPI {


class COMMONAPI_EXPORT_CLASS_EXPLICIT Logger {
public:
    enum class Level : std::uint8_t COMMONAPI_METHOD_EXPORT {
        CAPI_LOG_NONE = 0,
        CAPI_LOG_FATAL = 1,
        CAPI_LOG_ERROR = 2,
        CAPI_LOG_WARNING = 3,
        CAPI_LOG_INFO = 4,
        CAPI_LOG_DEBUG = 5,
        CAPI_LOG_VERBOSE = 6
    };
    Logger();
    ~Logger();

    template<typename... LogEntries_>
    COMMONAPI_METHOD_EXPORT static void fatal(LogEntries_&&... _entries) {
        log(Logger::Level::CAPI_LOG_FATAL, std::forward<LogEntries_>(_entries)...);
    }

    template<typename... LogEntries_>
    COMMONAPI_METHOD_EXPORT static void error(LogEntries_&&... _entries) {
        log(Logger::Level::CAPI_LOG_ERROR, std::forward<LogEntries_>(_entries)...);
    }

    template<typename... LogEntries_>
    COMMONAPI_METHOD_EXPORT static void warning(LogEntries_&&... _entries) {
        log(Logger::Level::CAPI_LOG_WARNING, std::forward<LogEntries_>(_entries)...);
    }

    template<typename... LogEntries_>
    COMMONAPI_METHOD_EXPORT static void info(LogEntries_&&... _entries) {
        log(Logger::Level::CAPI_LOG_INFO, std::forward<LogEntries_>(_entries)...);
    }

    template<typename... LogEntries_>
    COMMONAPI_METHOD_EXPORT static void debug(LogEntries_&&... _entries) {
        log(Logger::Level::CAPI_LOG_DEBUG, std::forward<LogEntries_>(_entries)...);
    }

    template<typename... LogEntries_>
    COMMONAPI_METHOD_EXPORT static void verbose(LogEntries_&&... _entries) {
        log(Logger::Level::CAPI_LOG_VERBOSE, std::forward<LogEntries_>(_entries)...);
    }

    template<typename... LogEntries_>
    COMMONAPI_METHOD_EXPORT static void log(Logger::Level _level, LogEntries_&&... _entries) {
        if (isLogged(_level)) {
            std::stringstream buffer;
            logIntern(buffer, std::forward<LogEntries_>(_entries)...);
            Logger::doLog(_level, buffer.str());
        }
    }

    COMMONAPI_METHOD_EXPORT static void init(bool _useConsole, const std::string &_fileName,
                     bool _useDlt, const std::string& _level);

private:
    class LoggerImpl;
    COMMONAPI_METHOD_EXPORT static std::shared_ptr<LoggerImpl> getLoggerImpl();

    COMMONAPI_METHOD_EXPORT static bool isLogged(Level _level);
    COMMONAPI_METHOD_EXPORT static void doLog(Level _level, const std::string& _message);

    COMMONAPI_METHOD_EXPORT static void logIntern(std::stringstream &_buffer) {
        (void)_buffer;
    }

    template<typename LogEntry_, typename... MoreLogEntries_>
    COMMONAPI_METHOD_EXPORT static void logIntern(std::stringstream &_buffer, LogEntry_&& _entry,
                           MoreLogEntries_&& ... _moreEntries) {
        _buffer << _entry;
        logIntern(_buffer, std::forward<MoreLogEntries_>(_moreEntries)...);
    }
};

} // namespace CommonAPI

#endif // COMMONAPI_LOGGER_HPP_
