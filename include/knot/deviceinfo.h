/*
 * This file is part of knotdeviceinfo.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/knotdeviceinfo/blob/master/LEGAL
 */

#ifndef KNOTDEVICEINFO_H
#define KNOTDEVICEINFO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    KN_PLATFORM_UNKNOWN = 0,
    KN_PLATFORM_EMBEDDED,
    KN_PLATFORM_LINUX,
    KN_PLATFORM_WINDOWS,
    KN_PLATFORM_MACOS,
    KN_PLATFORM_IOS,
    KN_PLATFORM_ANDROID,
    KN_PLATFORM_FREEBSD,
    KN_PLATFORM_OPENBSD,
    KN_PLATFORM_NETBSD,
    KN_PLATFORM_DRAGONFLYBSD,
    KN_PLATFORM_SOLARIS,
    KN_PLATFORM_AIX,
    KN_PLATFORM_HPUX,
    KN_PLATFORM_ZOS,
    KN_PLATFORM_HAIKU,
    KN_PLATFORM_FUCHSIA,
    KN_PLATFORM_QNX,
    KN_PLATFORM_VXWORKS,
    KN_PLATFORM_RTEMS,
    KN_PLATFORM_CHROMEOS,
    KN_PLATFORM_TIZEN,
    KN_PLATFORM_WEBOS,
    KN_PLATFORM_ROS,
    KN_PLATFORM_VMWARE_ESXI,
    KN_PLATFORM_CITRIX_XENSERVER,
    KN_PLATFORM_COUNT
} KNPlatform;

// All fields are nullable
typedef struct KNDeviceInfo {
    KNPlatform platform;
    char* uuid; // 036880f7-e66c-4d39-8294-a348a2034cf9
    char* name; // The device name specified by the user
    char* model_raw; // MacPro7,1
    char* model_pretty; // Mac Pro
    char* kernel_version; // Available only on Linux and macOS (uname -r)
    char* kernel_build; // Available only on Linux and macOS (uname -v)
    char* system_name; // Debian GNU/Linux, macOS, Windows
    char* system_version; // 14.7.1 (on macOS), 10 (on Windows)
} KNDeviceInfo;

const char* KNPlatformToString(KNPlatform platform);

void KNDeviceInfoFetch(KNDeviceInfo* dest);

void KNDeviceInfoFree(KNDeviceInfo*);

#ifdef __cplusplus
}
#endif

#endif //KNOTDEVICEINFO_H
