#include <knot/deviceinfo.h>

#include <sys/system_properties.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void KNDeviceInfo_sysPropRead(char** into, const char* prop_name) {
  const prop_info* pi = __system_property_find(prop_name);
  if (pi != 0) {
    *into = malloc(PROP_VALUE_MAX);
    __system_property_read(pi, 0, *into);
  }
}

void KNDeviceInfoFetch(KNDeviceInfo* info) {
  memset(info, 0, sizeof(KNDeviceInfo));

  KNDeviceInfo_sysPropRead(&info->name, "net.bt.name"); // Requires bluetooth permissions

  KNDeviceInfo_sysPropRead(&info->model_raw, "ro.product.model");
  //KNDeviceInfo_sysPropRead(&info->model_pretty, "ro.product.name"); // Not always pretty at least on Samsung

  info->platform = strdup("Android");

  struct utsname uts = {0};
  if (uname(&uts) == 0) {
    if (strlen(uts.release) > 0) { // 6.14.2-arch1-1
      info->kernel_version = strdup(uts.release);
    }
    if (strlen(uts.version) > 0) { // #1 SMP PREEMPT_DYNAMIC Thu, 10 Apr 2025 18:43:59 +0000
      info->kernel_build = strdup(uts.version);
    }
  }

  info->system_name = strdup("Android");
  KNDeviceInfo_sysPropRead(&info->system_version, "ro.build.version.release");
}
