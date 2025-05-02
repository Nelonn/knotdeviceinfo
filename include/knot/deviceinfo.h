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

// All fields are nullable
typedef struct KNDeviceInfo {
    char* uuid; // 036880f7-e66c-4d39-8294-a348a2034cf9
    char* name; // The device name specified by the user
    char* model_raw; // MacPro7,1
    char* model_pretty; // Mac Pro
    char* platform; // Linux, macOS, Windows
    char* kernel_version; // Available only on Linux and macOS (uname -r)
    char* kernel_build; // Available only on Linuxx and macOS (uname -v)
    char* system_name; // Debian GNU/Linux, macOS, Windows
    char* system_version; // 14.7.1 (on macOS), 10 (on Windows)
} KNDeviceInfo;

void KNDeviceInfoFetch(KNDeviceInfo* dest);

void KNDeviceInfoFree(KNDeviceInfo*);

#ifdef __cplusplus
}
#endif

#endif //KNOTDEVICEINFO_H
