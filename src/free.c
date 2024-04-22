/*
 * This file is part of knotdeviceinfo.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/knotdeviceinfo/blob/master/LEGAL
 */

#include <knot/deviceinfo.h>

#include <stdlib.h>

void KNDeviceInfoFree(KNDeviceInfo* info) {
  free(info->uuid);
  free(info->name);
  free(info->model);
  free(info->platform);
  free(info->system_version);
}
