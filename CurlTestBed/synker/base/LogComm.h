

// The same as HotCodeLog.h

#ifndef _HOT_CODE_LOG_DEF__
#define _HOT_CODE_LOG_DEF__

#if defined(CC_TARGET_PLATFORM) && CC_TARGET_PLATFORM == 2

#include <stdlib.h>
#include <jni.h>
#include <android/log.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "platform/android/jni/JniHelper.h" 
#define  LOG_TAG    "workpool"

#	define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#else
#	define LOGW(...) printf(__VA_ARGS__)
#endif

#endif // HOT-Code-Log-header