/*
 * This file is part of knotdeviceinfo.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/knotdeviceinfo/blob/master/LEGAL
 */

#include "target.h"

#if defined(OS_MAC)

#include <knot/deviceinfo.h>
#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>
#include <ctype.h>
#include <sys/utsname.h>
#include <sys/sysctl.h>

const char* KNDeviceInfo_getCurrentHostName();
const char* KNDeviceInfo_getModelName();

char* KNDeviceInfo_getUUID() {
    io_service_t platformExpert = IOServiceGetMatchingService(
        kIOMasterPortDefault, // kIOMainPortDefault is available from macOS 12.0, but we want more versions support
        IOServiceMatching("IOPlatformExpertDevice")
    );
    if (!platformExpert) {
        return NULL;
    }

    CFTypeRef uuidProperty = IORegistryEntryCreateCFProperty(
        platformExpert,
        CFSTR(kIOPlatformUUIDKey),
        kCFAllocatorDefault,
        0
    );
    if (!uuidProperty) {
        IOObjectRelease(platformExpert);
        return NULL;
    }

    CFStringRef uuidStringRef = (CFStringRef) uuidProperty;
    CFIndex bufferSize = CFStringGetLength(uuidStringRef) + 1;
    char* uuid = (char*) malloc(bufferSize);
    if (!uuid) {
        CFRelease(uuidProperty);
        IOObjectRelease(platformExpert);
        return NULL;
    }
    if (!CFStringGetCString(uuidStringRef, uuid, bufferSize, kCFStringEncodingUTF8)) {
        free(uuid);
        CFRelease(uuidProperty);
        IOObjectRelease(platformExpert);
        return NULL;
    }

    CFRelease(uuidProperty);
    IOObjectRelease(platformExpert);

    // UUID to lowercase
    for (unsigned char* ptr = (unsigned char*) uuid; *ptr; ptr++) {
        *ptr = tolower(*ptr);
    }

    return uuid;
}

char* KNDeviceInfo_getVersion() {
    FILE* pipe = popen("sw_vers -productVersion", "r");
    if (!pipe) {
        return NULL;
    }

    char buffer[128];
    if (!fgets(buffer, sizeof(buffer), pipe)) {
        pclose(pipe);
        return NULL;
    }
    pclose(pipe);

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0'; // Removing trailing newline
    }

    char* version = (char*) malloc((len + 1) * sizeof(char));
    if (!version) {
        return NULL;
    }

    strcpy(version, buffer);
    return version;
}

char* KNDeviceInfo_getModelIdentifier() {
  size_t len = 0;
  if (sysctlbyname("hw.model", NULL, &len, NULL, 0) != 0 || len == 0) {
    return NULL;
  }
  char* buf = malloc(len);
  if (!buf) return NULL;
  if (sysctlbyname("hw.model", buf, &len, NULL, 0) != 0) {
    free(buf);
    return NULL;
  }
  return buf;
}

void KNDeviceInfoFetch(KNDeviceInfo* info) {
    memset(info, 0, sizeof(KNDeviceInfo));

    struct utsname uts = {0};
    if (uname(&uts) == 0) {
        if (strlen(uts.release) > 0) { // 23.6.0
            info->kernel_version = strdup(uts.release);
        }
        if (strlen(uts.version) > 0) { // Darwin Kernel Version 23.6.0: Thu Sep 12 23:34:49 PDT 2024; root:xnu-10063.141.1.701.1~1/RELEASE_X86_64
            info->kernel_build = strdup(uts.version);
        }
    }

    info->platform = KN_PLATFORM_MACOS;
    info->uuid = KNDeviceInfo_getUUID();
    info->name = strdup(KNDeviceInfo_getCurrentHostName());
    info->model_raw = KNDeviceInfo_getModelIdentifier();
    info->model_pretty = strdup(KNDeviceInfo_getModelName());
    info->system_name = strdup("macOS");
    info->system_version = KNDeviceInfo_getVersion();
}

#endif // OS_MAC
