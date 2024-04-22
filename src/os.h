/*
 * This file is part of knotdeviceinfo.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/knotdeviceinfo/blob/master/LEGAL
 */

#if defined(__APPLE__)
#  include <TargetConditionals.h>
#  if TARGET_OS_IPHONE
#    define OS_IOS 1
#    define OS IOS
#  elif TARGET_OS_MAC
#    define OS_MAC 1
#    define OS MAC
#  endif
#elif defined(__linux__) || defined(LINUX)
#  define OS_LINUX 1
#  define OS LINUX
#elif defined(_WIN32) || defined(_WIN64)
#  define OS_WIN 1
#  define OS WIN
#else
#  error Unsupported OS
#endif
