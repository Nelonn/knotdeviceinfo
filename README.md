# libknotdeviceinfo

Library, that fetch device info. Supports: Linux, macOS, Windows, Android

For pretty Android device model see [knotandroidinfo](https://github.com/noseam-env/flowdrop-android/tree/master/knotandroidinfo)

## Example

```c
#include <knot/deviceinfo.h>
#include <stdio.h>

int main(void) {
    KNDeviceInfo device_info;
    KNDeviceInfoFetch(&device_info);
    printf("Device Info:\n");
    if (device_info.platform) {
        printf("- Platform: %s\n", KNPlatformToString(device_info.platform));
    }
    if (device_info.uuid) {
        printf("- UUID: %s\n", device_info.uuid);
    }
    if (device_info.name) {
        printf("- Name: %s\n", device_info.name);
    }
    if (device_info.model_raw) {
        printf("- Model Raw: %s\n", device_info.model_raw);
    }
    if (device_info.model_pretty) {
        printf("- Model Pretty: %s\n", device_info.model_pretty);
    }
    if (device_info.kernel_version) {
        printf("- Kernel Version: %s\n", device_info.kernel_version);
    }
    if (device_info.kernel_build) {
        printf("- Kernel Build: %s\n", device_info.kernel_build);
    }
    if (device_info.system_name) {
        printf("- System Name: %s\n", device_info.system_name);
    }
    if (device_info.system_version) {
        printf("- System Version: %s\n", device_info.system_version);
    }
    KNDeviceInfoFree(&device_info);
}
```

## License

The source code is published under MIT License, the license is available [here](https://github.com/noseam-env/knotdeviceinfo/blob/master/LICENSE).

## Authors

- **Michael Neonov** ([email](mailto:two.nelonn@gmail.com), [github](https://github.com/Nelonn))
