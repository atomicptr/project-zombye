#ifndef __OS_H__
#define __OS_H__

#ifdef _WIN32
    #define OS_WINDOWS
    #define OS_NAME "Windows"

    #ifdef _WIN64
        #define OS_WINDOWS_64
    #else
        #define OS_WINDOWS_32
    #endif
#elif __APPLE__
    #include "TargetConditionals.h"

    #if TARGET_IPHONE_SIMULATOR
        #define OS_IOS_SIMULATOR
        #define OS_NAME "iOS Simulator"
    #elif TARGET_OS_IPHONE
        #define OS_IOS
        #define OS_NAME "iOS"
    #else
        #define OS_MAC_OS_X
        #define OS_NAME "Mac OS X"
    #endif
#elif __ANDROID__
    #define OS_ANDROID
    #define OS_NAME "Android"
#elif __linux
    #define OS_LINUX
    #define OS_NAME "Linux"
#elif __unix
    #define OS_UNIX
    #define OS_NAME "Unix"
#else
    #define OS_UNKNOWN
    #define OS_NAME "Unknown"
#endif

#endif