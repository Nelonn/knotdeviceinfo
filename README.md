# libknotdeviceinfo

Library, that fetch device info. Supports: Linux, macOS, Windows

The source code is published under GPL-3.0, the license is available [here](https://github.com/noseam-env/libknotdeviceinfo/blob/master/LICENSE).

## Example

```c++
#include <knot/deviceinfo.h>
#include <iostream>

int main() {
    KNDeviceInfo info{};
    KNDeviceInfoFetch(info);
    std::cout << info.platform << " " << info.system_version << std::endl;
}
```

## Authors

- **Michael Neonov** ([email](mailto:two.nelonn@gmail.com), [github](https://github.com/Nelonn))
