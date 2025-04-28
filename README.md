# libknotdeviceinfo

Library, that fetch device info. Supports: Linux, macOS, Windows

For Android see [knotandroidinfo](https://github.com/noseam-env/flowdrop-android/tree/master/knotandroidinfo)

## Example

```c
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
    if (deviceInfo.kernel_version) {
        printf("- Kernel Version: %s\n", deviceInfo.kernel_version);
    }
    if (deviceInfo.kernel_build) {
        printf("- Kernel Build: %s\n", deviceInfo.kernel_build);
    }
    if (deviceInfo.system_name) {
        printf("- System Name: %s\n", deviceInfo.system_name);
    }
    if (deviceInfo.system_version) {
        printf("- System Version: %s\n", deviceInfo.system_version);
    }
    KNDeviceInfoFree(&deviceInfo);
}
```

## License

The source code is published under MIT License, the license is available [here](https://github.com/noseam-env/knotdeviceinfo/blob/master/LICENSE).

## Authors

- **Michael Neonov** ([email](mailto:two.nelonn@gmail.com), [github](https://github.com/Nelonn))
