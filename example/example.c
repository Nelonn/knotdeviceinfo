#include <knot/deviceinfo.h>
#include <stdio.h>

int main(void) {
    KNDeviceInfo deviceInfo;
    KNDeviceInfoFetch(&deviceInfo);
    printf("Device Info:\n");
    if (deviceInfo.uuid) {
        printf("- UUID: %s\n", deviceInfo.uuid);
    }
    if (deviceInfo.name) {
        printf("- Name: %s\n", deviceInfo.name);
    }
    if (deviceInfo.model) {
        printf("- Model: %s\n", deviceInfo.model);
    }
    if (deviceInfo.platform) {
        printf("- Platform: %s\n", deviceInfo.platform);
    }
    if (deviceInfo.system_name) {
        printf("- System Name: %s\n", deviceInfo.system_name);
    }
    if (deviceInfo.system_version) {
        printf("- System Version: %s\n", deviceInfo.system_version);
    }
    KNDeviceInfoFree(&deviceInfo);
}
