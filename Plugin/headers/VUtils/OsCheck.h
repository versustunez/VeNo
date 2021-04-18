#pragma once

#if defined (_WIN32) || defined (_WIN64)
    #define isWin 1
#elif defined(__FreeBSD__) || (__OpenBSD__)
    #define isBSD 1
#elif defined(LINUX) || defined(__linux__)
    #define isLinux 1
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        #define isIOS 1
    #else
        #define isMAC 1
    #endif
#else
    #error "Unknown platform!"
#endif