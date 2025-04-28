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
  free(info->model_raw);
  free(info->model_pretty);
  free(info->platform);
  free(info->kernel_version);
  free(info->kernel_build);
  free(info->system_name);
  free(info->system_version);
}
