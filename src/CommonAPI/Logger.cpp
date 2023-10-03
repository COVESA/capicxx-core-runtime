// Copyright (C) 2015-2020 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <CommonAPI/Runtime.hpp>
#include <CommonAPI/Logger.hpp>

#ifdef USE_DLT
#ifndef ANDROID
#include <dlt/dlt.h>
#endif
#endif

#ifdef ANDROID
#include <utils/Log.h>

#ifdef ALOGE
#undef ALOGE
#endif

#define ALOGE(LOG_TAG, ...) ((void)ALOG(LOG_ERROR, LOG_TAG, __VA_ARGS__))
#ifndef LOGE
#define LOGE ALOGE
#endif

#ifdef ALOGW
#undef ALOGW
#endif

#define ALOGW(LOG_TAG, ...) ((void)ALOG(LOG_WARN, LOG_TAG, __VA_ARGS__))
#ifndef LOGE
#define LOGW ALOGW
#endif

#ifdef ALOGI
#undef ALOGI
#endif

#define ALOGI(LOG_TAG, ...) ((void)ALOG(LOG_INFO, LOG_TAG, __VA_ARGS__))
#ifndef LOGE
#define LOGI ALOGI
#endif

#ifdef ALOGD
#undef ALOGD
#endif

#define ALOGD(LOG_TAG, ...) ((void)ALOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#ifndef LOGE
#define LOGD ALOGD
#endif

#ifdef ALOGV
#undef ALOGV
#endif

#define ALOGV(LOG_TAG, ...) ((void)ALOG(LOG_VERBOSE, LOG_TAG, __VA_ARGS__))
#ifndef LOGE
#define LOGV ALOGV
#endif

#endif

#include <cstdint>
#include <fstream>
#include <memory>
#include <mutex>
#include <iostream>

namespace CommonAPI {

class Logger::LoggerImpl {
public:
    LoggerImpl() :
            maximumLogLevel_(Logger::Level::CAPI_LOG_INFO),
            useConsole_(true),
            useDlt_(false)
#ifdef USE_DLT
#ifndef ANDROID
            , ownAppID_(false)
#endif
#endif
    {
    }

    void init(bool _useConsole, const std::string &_fileName, bool _useDlt,
               const std::string& _level) {
        useConsole_ = _useConsole;
        maximumLogLevel_ = stringAsLevel(_level);
        useDlt_ = _useDlt;

        if (!_fileName.empty()) {
            file_ = std::make_shared<std::ofstream>();
            if (file_) {
                file_->open(_fileName.c_str(),
                        std::ofstream::out | std::ofstream::app);
            }
        }
#ifdef USE_DLT
#ifndef ANDROID
        if (useDlt_) {
            std::string app = Runtime::getProperty("LogApplication");
            if (!app.empty()) {
                ownAppID_ = true;
                DLT_REGISTER_APP(app.c_str(), "CommonAPI Application");
            }

            std::string context = Runtime::getProperty("LogContext");
            if (context == "")
                context = "CAPI";
            DLT_REGISTER_CONTEXT(dlt_, context.c_str(), "CAPI");
        }
#endif
#endif
    }

    ~LoggerImpl() {
#ifdef USE_DLT
#ifndef ANDROID
        if (useDlt_) {
            DLT_UNREGISTER_CONTEXT(dlt_);
            if (ownAppID_) {
                DLT_UNREGISTER_APP()
                ;
            }
        }
#endif
#endif
    }

    bool isLogged(Logger::Level _level) {
        return (_level <= maximumLogLevel_) ? true : false;
    }

    void doLog(Logger::Level _level, const std::string &_message) {
        if (useConsole_) {
            std::lock_guard<std::mutex> itsLock(mutex_);

#ifndef ANDROID
            std::cerr << "[CAPI][" << levelAsString(_level) << "] " << _message
                    << std::endl;
#else
           std::string app = Runtime::getProperty("LogApplication");

            switch (_level) {
                case Logger::Level::CAPI_LOG_FATAL:
                    ALOGE(app.c_str(), "CAPI: %s ", _message.c_str());
                    break;
                case Logger::Level::CAPI_LOG_ERROR:
                    ALOGE(app.c_str(), "CAPI: %s ", _message.c_str());
                    break;
                case Logger::Level::CAPI_LOG_WARNING:
                    ALOGW(app.c_str(), "CAPI: %s ", _message.c_str());
                    break;
                case Logger::Level::CAPI_LOG_INFO:
                    ALOGI(app.c_str(), "CAPI: %s ", _message.c_str());
                    break;
                case Logger::Level::CAPI_LOG_DEBUG:
                    ALOGD(app.c_str(), "CAPI: %s ", _message.c_str());
                    break;
                case Logger::Level::CAPI_LOG_VERBOSE:
                    ALOGV(app.c_str(), "CAPI: %s ", _message.c_str());
                    break;
                default:
                    ALOGI(app.c_str(), "CAPI: %s ", _message.c_str());
            };
#endif // !ANDROID
        }
        if (file_ && file_->is_open()) {
            std::lock_guard<std::mutex> itsLock(mutex_);
            (*(file_.get())) << "[CAPI][" << levelAsString(_level) << "] "
                    << _message << std::endl;
        }

#ifdef USE_DLT
#ifndef ANDROID
        if (useDlt_) {
            DLT_LOG_STRING(dlt_, levelAsDlt(_level), _message.c_str());
        }
#endif
#endif

    }

    static Logger::Level stringAsLevel(const std::string &_level) {
        Logger::Level level(Logger::Level::CAPI_LOG_INFO);
        if (_level == "none") {
            level = Logger::Level::CAPI_LOG_NONE;
        } else if (_level == "fatal") {
            level = Logger::Level::CAPI_LOG_FATAL;
        } else if (_level == "error") {
            level = Logger::Level::CAPI_LOG_ERROR;
        } else if (_level == "warning") {
            level = Logger::Level::CAPI_LOG_WARNING;
        } else if (_level == "info") {
            level = Logger::Level::CAPI_LOG_INFO;
        } else if (_level == "debug") {
            level = Logger::Level::CAPI_LOG_DEBUG;
        } else if (_level == "verbose") {
            level = Logger::Level::CAPI_LOG_VERBOSE;
        }
        return level;
    }

private:
    static std::string levelAsString(Logger::Level _level) {
        switch (_level) {
            case Logger::Level::CAPI_LOG_NONE:
                return "NONE";
            case Logger::Level::CAPI_LOG_FATAL:
                return "FATAL";
            case Logger::Level::CAPI_LOG_ERROR:
                return "ERROR";
            case Logger::Level::CAPI_LOG_WARNING:
                return "WARNING";
            case Logger::Level::CAPI_LOG_INFO:
                return "INFO";
            case Logger::Level::CAPI_LOG_DEBUG:
                return "DEBUG";
            case Logger::Level::CAPI_LOG_VERBOSE:
                return "VERBOSE";
            default:
                return "";
        }
    }

#ifdef USE_DLT
#ifndef ANDROID
    static DltLogLevelType levelAsDlt(Logger::Level _level) {
        switch (_level) {
            case Logger::Level::CAPI_LOG_NONE:
                return DLT_LOG_OFF;
            case Logger::Level::CAPI_LOG_FATAL:
                return DLT_LOG_FATAL;
            case Logger::Level::CAPI_LOG_ERROR:
                return DLT_LOG_ERROR;
            case Logger::Level::CAPI_LOG_WARNING:
                return DLT_LOG_WARN;
            case Logger::Level::CAPI_LOG_INFO:
                return DLT_LOG_INFO;
            case Logger::Level::CAPI_LOG_DEBUG:
                return DLT_LOG_DEBUG;
            case Logger::Level::CAPI_LOG_VERBOSE:
                return DLT_LOG_VERBOSE;
            default:
                return DLT_LOG_DEFAULT;
        }
    }
#endif
#endif

private:
    std::mutex mutex_;

    Logger::Level maximumLogLevel_;

    bool useConsole_;

    std::shared_ptr<std::ofstream> file_;

    bool useDlt_;
#ifdef USE_DLT
#ifndef ANDROID
    DLT_DECLARE_CONTEXT(dlt_)
    bool ownAppID_;
#endif
#endif
};

Logger::Logger() = default;
Logger::~Logger() = default;

std::shared_ptr<Logger::LoggerImpl> Logger::getLoggerImpl()
{
    static std::shared_ptr<Logger::LoggerImpl> loggerImpl = std::make_shared<Logger::LoggerImpl>();
    return loggerImpl;
}

void Logger::init(bool _useConsole, const std::string &_fileName, bool _useDlt,
                  const std::string& _level) {
    getLoggerImpl()->init(_useConsole, _fileName, _useDlt, _level);
}

bool Logger::isLogged(Level _level) {
    return getLoggerImpl()->isLogged(_level);
}

void Logger::doLog(Level _level, const std::string& _message) {
    getLoggerImpl()->doLog(_level, _message);
}

} //namespace CommonAPI
