/*
 * This file is part of libknotdeviceinfo.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/libknotdeviceinfo/blob/master/LEGAL
 */

#ifndef KNOTDEVICEINFO_H
#define KNOTDEVICEINFO_H

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

#endif //KNOTDEVICEINFO_H
