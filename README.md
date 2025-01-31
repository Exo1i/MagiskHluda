## **Florida on Boot: Seamlessly Start Florida on Boot with Enhanced Stealth**

![GitHub repo size](https://img.shields.io/github/repo-size/Exo1i/MagiskHluda)
![GitHub downloads](https://img.shields.io/github/downloads/Exo1i/magiskhluda/total)

**🌟 This Magisk module is based on [Florida](https://github.com/Ylarod/Florida), a modified version of [Frida](https://github.com/frida/frida/) designed to be more undetectable.**

**🌟 It ensures that Florida automatically starts on boot for you.**

## **New Web UI for Easier Control**  

I’ve added a **web-based user interface** that allows for easier control of the Florida server. The Web UI includes:

- A **start/stop server** button.
- Access to the **command usage documentation**.
- A display for **server status** and input for additional parameters.

This Web UI simplifies the management of the Florida server. It allows you to start/stop the server and also provide extra parameters without needing a terminal.

## **System UI Crashes Warning**

Please note that **stopping the Florida server may cause the System UI to crash**, which can result in a restart of the System UI. This issue is known, and it may cause temporary instability in the UI. Take care when stopping the server, and make sure to save any important work before doing so.

## **Meet your companion:**

- **Powerful tool for dynamic analysis, reverse engineering, and security testing.**
- Florida is a more undetectable version of Frida-server for Android, offering you the ability to interact with and explore apps while remaining unnoticed.

## **Supported Architectures:**

- arm64
- arm
- x86
- x86_64

## **Getting Started:**

1. **Download the latest release:** Head over to the releases page and download the latest zip.
2. **Install via Magisk:** Use Magisk to install the module.

## **Always Up-to-Date:**

- **Updates are checked every half a day.**

## **Troubleshooting:**

- If you run into any issues, refer to the [troubleshooting guide](https://github.com/Exo1i/MagiskHluda/blob/main/troubleshooting.md).

## **Building from Source:**

1. **Install dependencies:** Install `rapidjson` and `restclient-cpp` using [vcpkg](https://vcpkg.io/en/getting-started):
   ```bash
   ./vcpkg install rapidjson restclient-cpp
   ```

2. **Run the code:** Execute `main.cpp` in your preferred IDE (Clion or Visual Studio is recommended).

## **Credits:**

- [StongR-Frida by hzzheyang](https://github.com/hzzheyang/strongR-frida-android)
- [Florida](https://github.com/Ylarod/Florida)
- Special thanks to _magisk-frida: [https://github.com/ViRb3/magisk-frida](https://github.com/ViRb3/magisk-frida)_ for inspiration, and to the enhancement request: [https://github.com/ViRb3/magisk-frida/issues/16](https://github.com/ViRb3/magisk-frida/issues/16).

**Get started with Florida today and enjoy enhanced stealth for your dynamic analysis!**

## **Still Being Detected?**

Consider looking into [ZygiskFrida](https://github.com/lico-n/ZygiskFrida) as an alternative.

## **Learning Journey:**

This project is my first semester project. It's been an interesting learning experience, and I've gained a lot of knowledge along the way.
