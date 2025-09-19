/*
 * This file is part of knotdeviceinfo.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/knotdeviceinfo/blob/master/LEGAL
 */

#include <knot/deviceinfo.h>

#include <stdlib.h>

const char* KNPlatformToString(KNPlatform platform) {
    static const char* platform_strings[] = {
        "Unknown",
        "Embedded",
        "Linux",
        "Windows",
        "macOS",
        "iOS",
        "Android",
        "FreeBSD",
        "OpenBSD",
        "NetBSD",
        "DragonFlyBSD",
        "Solaris",
        "AIX",
        "HP-UX",
        "z/OS",
        "Haiku",
        "Fuchsia",
        "QNX",
        "VxWorks",
        "RTEMS",
        "ChromeOS",
        "Tizen",
        "webOS",
        "ROS",
        "VMware ESXi",
        "Citrix XenServer",
    };
    if (platform < KN_PLATFORM_COUNT) {
        return platform_strings[platform];
    }
    return "Unknown";
}

void KNDeviceInfoFree(KNDeviceInfo* info) {
  free(info->uuid);
  free(info->name);
  free(info->model_raw);
  free(info->model_pretty);
  free(info->kernel_version);
  free(info->kernel_build);
  free(info->system_name);
  free(info->system_version);
}
