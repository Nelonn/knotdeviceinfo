/*
 * This file is part of libknotdeviceinfo.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/libknotdeviceinfo/blob/master/LEGAL
 */

#ifndef KNOT_DEVICEINFO_H
#define KNOT_DEVICEINFO_H

#include <optional>
#include <string>

struct KNDeviceInfo {
    std::optional<std::string> uuid;
    std::optional<std::string> name;
    std::optional<std::string> model;
    std::optional<std::string> platform;
    std::optional<std::string> system_version;
};

void KNDeviceInfoFetch(KNDeviceInfo &info);

#endif //KNOT_DEVICEINFO_H
