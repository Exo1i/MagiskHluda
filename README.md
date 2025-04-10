# **Florida on Boot: Seamlessly Start a More Undetectable Frida-server on Boot**

![GitHub repo size](https://img.shields.io/github/repo-size/Exo1i/MagiskHluda)
![GitHub downloads](https://img.shields.io/github/downloads/Exo1i/magiskhluda/total)

## **Overview**

This Magisk module is based on [Florida](https://github.com/Ylarod/Florida), a modified version of [Frida](https://github.com/frida/frida/) designed to be more undetectable. It ensures that Florida automatically starts on boot, providing a seamless experience for security testing and reverse engineering.

## **New Web UI for Easier Control**

A **web-based user interface** has been added, making it easier to manage the Florida server:

- **Start/Stop Server** button.
- Access to **command usage documentation**.
- Display for **server status**.
- Input for **custom parameters**.

This Web UI simplifies the management of the Florida server, allowing you to interact with it without needing a terminal.

## **System UI Crashes Warning**

Stopping the Florida server **may cause the System UI to crash**. This is a known issue and may cause temporary instability. Be cautious and save any important work before stopping the server.

## **Features & Benefits**

- **Powerful tool** for dynamic analysis, reverse engineering, and security testing.
- **Enhanced stealth**, making Florida a more undetectable version of Frida-server for Android.

## **Supported Architectures**

- arm64
- arm
- x86
- x86_64

## **Getting Started**

1. **Download the latest release:** Visit the [Releases](https://github.com/Exo1i/MagiskHluda/releases) page and download the latest ZIP file that corresponds to your device's architecture or just use the universal package.
2. **Install via Magisk/KSU/KSUN/APatch:** Use any root solution to install the module.

## **Automatic Updates**

- **Updates are checked every 12 hours**, ensuring you always have the latest version.

## **Troubleshooting**

If you encounter any issues, refer to the [Troubleshooting Guide](https://github.com/Exo1i/MagiskHluda/blob/main/troubleshooting.md).

## **Building from Source**

1. **Install dependencies:** Install `rapidjson` and `restclient-cpp` using [vcpkg](https://vcpkg.io/en/getting-started):
   ```bash
   ./vcpkg install rapidjson restclient-cpp
   ```
2. **Run the code:** Execute `main.cpp` in your preferred IDE (CLion or Visual Studio is recommended).

## **Credits**

Special thanks to:
- [StrongR-Frida by hzzheyang](https://github.com/hzzheyang/strongR-frida-android)
- [Florida](https://github.com/Ylarod/Florida)
- [magisk-frida by ViRb3](https://github.com/ViRb3/magisk-frida), particularly the enhancement request: [Issue #16](https://github.com/ViRb3/magisk-frida/issues/16)

## **Change Frida Port - Manually**

If you want to modify default port (27042) you can unzip the release an edit ./module_template/customize.sh.
    ```bash
    if ! test -f "$MODPATH/module.cfg"; then
    {
        echo "port=13337"
    }
    ```

After change you can connect to frida using
    ```
    adb forward tcp:13337 tcp:13337
    frida.exe -H 127.0.0.1:13337 -f com.app
    objection -N -h 192.168.0.x -p 13337 -g "App Name" explore
    ```

## **Still Being Detected?**

If Florida is still being detected, consider using [ZygiskFrida](https://github.com/lico-n/ZygiskFrida) as an alternative.

## **Learning Journey**

This project is my **first semester project**, and it has been an invaluable learning experience. I have gained significant knowledge and skills along the way.

