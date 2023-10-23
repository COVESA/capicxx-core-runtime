# Cannot convert to Android.bp as resource copying has not
# yet implemented for soong as of 12/16/2016

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libcommonapi_dlt
LOCAL_MODULE_TAGS := optional
LOCAL_CLANG := true
LOCAL_PROPRIETARY_MODULE := true
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include \

LOCAL_SRC_FILES += \
    src/CommonAPI/Address.cpp \
    src/CommonAPI/ContainerUtils.cpp \
    src/CommonAPI/IniFileReader.cpp \
    src/CommonAPI/Logger.cpp \
    src/CommonAPI/LoggerImpl.cpp \
    src/CommonAPI/MainLoopContext.cpp \
    src/CommonAPI/Proxy.cpp \
    src/CommonAPI/ProxyManager.cpp \
    src/CommonAPI/Runtime.cpp \
    src/CommonAPI/Utils.cpp \

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include

LOCAL_SHARED_LIBRARIES := \
	libboost_log \
	libboost_system \
	libboost_thread \

LOCAL_CFLAGS :=  \
    -frtti -fexceptions \
    -Wno-ignored-attributes \
    -D_CRT_SECURE_NO_WARNINGS \
    -DCOMMONAPI_INTERNAL_COMPILATION \
    -DCOMMONAPI_LOGLEVEL=COMMONAPI_LOGLEVEL_VERBOSE \

include $(BUILD_SHARED_LIBRARY)



