<h1 align = "center">🌟LilyGO T-A76XX🌟</h1>

# 1️⃣ Hardware

| Product(PinMap)         | SOC              | Flash | PSRAM     | Schematic                                        |
| ----------------------- | ---------------- | ----- | --------- | ------------------------------------------------ |
| [T-A7670X][1]           | ESP32-WROVER-E   | 4MB   | 8MB(QSPI) | [schematic](./schematic/T-A7670X-V1.1.pdf)       |

[1]: https://www.lilygo.cc/products/t-sim-a7670e


# Quick start ⚡

## 2️⃣ PlatformIO Quick Start (Recommended)

1. For users using the ESP32 version (non ESP32S3 version), please install the **CH9102 USB bridge** driver for the first time.
   - [Windows](https://www.wch-ic.com/downloads/CH343SER_ZIP.html)
   - [Mac OS](https://www.wch-ic.com/downloads/CH34XSER_MAC_ZIP.html)
2. Install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/)
3. Search for the `PlatformIO` plugin in the `VisualStudioCode` extension and install it.
4. After the installation is complete, you need to restart `VisualStudioCode`
5. After restarting `VisualStudioCode`, select `File` in the upper left corner of `VisualStudioCode` -> `Open Folder` -> select the `LilyGO-T-A76XX` directory
6. Wait for the installation of third-party dependent libraries to complete
7. Click on the `platformio.ini` file, and in the `platformio` column
8. Select the board name you want to use in `default_envs` and uncomment it.
9. Uncomment one of the lines `src_dir = xxxx` to make sure only one line works
10. Click the (✔) symbol in the lower left corner to compile
11. Connect the board to the computer USB-C , Micro-USB is used for module firmware upgrade
12. Click (→) to upload firmware
13. Click (plug symbol) to monitor serial output


## 3️⃣ Resources

1. SIMCOM official website document center
   - [SIMCOM official A7670X All Datasheet](https://cn.simcom.com/product/A7670X.html)
   - [SIMCOM official A7608X All Datasheet](https://cn.simcom.com/product/A7608X-H-E_SA.html)
   - [SIMCOM official SIM7672 All Datasheet](https://en.simcom.com/product/SIM7672.html)
2. A7670/A7608-Datasheet
   - [A76xx AT Command](./datasheet/A76XX/A76XX_Series_AT_Command_Manual_V1.09.pdf)
   - [A76xx MQTT(S) Application](./datasheet/A76XX/A76XX%20Series_MQTT(S)_Application%20Note_V1.02.pdf)
   - [A76xx HTTP(S) Application](./datasheet/A76XX/A76XX%20Series_HTTP(S)_Application%20Note_V1.02.pdf)
   - [A76xx GNSS Application](./datasheet/A76XX/A76XX%20Series_GNSS_Application%20Note_V1.02.pdf)
   - [A76xx FTP Application](./datasheet/A76XX/A76XX%20Series_FTP(S)_Application%20Note_V1.02.pdf)
   - [A76xx LBS Application](./datasheet/A76XX/A76XX%20Series_LBS_Application%20Note_V1.02.pdf)
   - [A76xx SSL Application](./datasheet/A76XX/A76XX%20Series_SSL_Application%20Note_V1.02.pdf)
   - [A76xx Sleep Application](./datasheet/A76XX/A76XX%20Series_Sleep%20Mode_Application%20Note_V1.02.pdf)
   - [A76xx Hardware Design manual](./datasheet/A76XX/A7670C_R2_硬件设计手册_V1.06.pdf)
   - [A76xx TCPIP Application](./datasheet/A76XX/A76XX%20Series_TCPIP_Application%20Note_V1.02.pdf)
3. SIM767XG-Datasheet
   - [SIM7672 Hardware Design manual](./datasheet/SIM7672G/SIM7672X_Series_Hardware_Design_V1.02.pdf)
   - [SIM7672 AT Command](./datasheet/SIM7672G/SIM767XX%20Series_AT_Command_Manual_V1.01.pdf)
4. Schematic
   - [T-A7670X Schematic](./schematic/T-A7670X-V1.1.pdf)
5. Dimensions
   - [T-A7670X DWG](./dimensions/T-A7670X-V1.1.dwg)
  






