/*
 * This file is part of knotdeviceinfo.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/knotdeviceinfo/blob/master/LEGAL
 */

#include "os.h"

#if defined(OS_MAC)

#include <knot/deviceinfo.h>
#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>
#include <ctype.h>

char* KNDeviceInfo_getCurrentHostName();
char* KNDeviceInfo_getDeviceModel();

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
        return NULL; // Memory allocation failed
    }

    strcpy(version, buffer);
    return version;
}

void KNDeviceInfoFetch(KNDeviceInfo* info) {
    info->uuid = KNDeviceInfo_getUUID();
    info->name = KNDeviceInfo_getCurrentHostName();
    info->model = KNDeviceInfo_getDeviceModel();
    info->platform = "macOS";
    info->system_version = KNDeviceInfo_getVersion();
}

#endif // OS_MAC
