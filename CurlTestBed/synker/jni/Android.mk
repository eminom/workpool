LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libworkpool

# LOCAL_MODULE_FILENAME := libworkpool-static

LOCAL_SRC_FILES := \
../fsm/DownloadWork.cc \
../fsm/HotTaskIn.cc \
../fsm/MapInfo.cc \
../fsm/PhaseMan.cc \
../fsm/TaskMan.cc \
../fsm/xxhashwrapper.cc \
../pathhelper/PathHelper_Android.cc \
../task/BinaryFileTask.cc \
../task/BinaryStreamHashPostTask.cc \
../task/BinaryStreamPostTask.cc \
../task/BinaryStreamTask.cc \
../task/PlainTextPostTask.cc \
../task/PlainTextTask.cc \
../task/SimpleTask.cc \
../thread/AsyncNotifier.cc \
../thread/ThreadPool.cc \
../thread/Workload.cc \
../thread/WorkloadWrapper.cc \
../deps/sha1/sha1.cpp \
../deps/cJSON/cJSON.c \
../deps/xxhash/xxhash.c

# LOCAL_STATIC_LIBRARIES += libxx

##################
# It seems that all curl in cocos2d-x(under android branch) is declared as linking again dynamically.

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/.. \
$(LOCAL_PATH)/../../../../../../cocos2d-x/external/curl/include/android \
$(LOCAL_PATH)/../../../../../../cocos2d-x/cocos

include $(BUILD_STATIC_LIBRARY)