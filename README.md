# PSP Bluetooth *Firmware*

This is the firmware for a ESP32 based module to act as the bridge between the PSP and Bluetooth Controllers.

It is a PlatformIO project and powered by the great [BluePad32 Library](https://bluepad32.readthedocs.io/en/latest) and is based on the [template](https://github.com/ricardoquesada/esp-idf-arduino-bluepad32-template.git) project.

## The PSP Bluetooth Project

This is a part of the PSP Bluetooth project, to find information on the other components that make up this project please look here: https://github.com/psp-bluetooth

## Acknowledgements

The work ive done is just a thin layer ontop of work many other people have done, Iv'e also had lots of help from others.

* TokyoDrift - Originally created work that uses the SIO port to allow external controllers back around 2010. Since uploaded to [Github](https://github.com/unraze/PSXControllerToPSP)
* OPDitto project - A project that uses the SIO port to allow for a real second analog stick. Its SIO implementation has been extremly helpful to me. Repo [Here](https://github.com/Operation-DITTO)
* X41 - Shared an initial working SIO hello world app back when i could barely compile PSP code. This was invaluable in helping me get the hardware working right at the start of the project.
* PSP Homebrew Discord - Full if incredibly smart and helpful people too many to list individually. Without their help I'd never have managed to complete any of this.

## How does it work?
This works by exposing a series of commands which will perform actions such as disconnecting controllers or returning the state of a specific controller.

`Serial2` is used for the communication with the PSP.

| RX | TX |
| -- | -- |
| GPIO-18 | GPIO-16 |

## Commands

The protocol works by the PSP issueing a command byte to the ESP32. The ESP decides what to do based on this command byte. The PSP may also send extra request data such as RGB values or a controller index etc.

Every command responds first with a status byte then a fixed number of response bytes depending on the specific command.

Below lists all the commands, any Request data they expect from the PSP and their repsonses.

**Note:** If an error status byte (such as `[0xA]` or `[0xB]`) is returned, **no further data will follow** that byte.

| Command Byte | Description | Request Bytes | Response Bytes |
|--------------|-------------|--------------|---------------|
| `0x0C` | Get Firmware Version<br>Returns a semver string for the firmware version. | _None_ | `[0x17, <len>, <version string bytes>]`<br>Example: `[0x17, 5, '1','.','2','.','3']`<br>**If error:** `[0xA]` (no further data) |
| `0x02` | Ping Command<br>Detect if the ESP32 is connected. | _None_ | `[0x10]` |
| `0x03` | Request Controller Data<br>Get state of a controller at a given index.<br>Analog stick values: 0-255. | `[ControllerIndexByte]` | `[0xA]` if request data not received (**no further data**)<br>`[0xB]` if controller not connected (**no further data**)<br>`[0xC, <Analog RX>, <Analog RY>, <Analog LX>, <Analog LY>, <Dpad>, <Buttons High>, <Buttons Low>, <Misc Buttons High>, <Misc Buttons Low>]` if successful. For values see [Bit Masks](#bit-masks) |
| `0x04` | Enable New Connections<br>Allow pairing for new controllers. | _None_ | `[0x11]` |
| `0x05` | Disable New Connections<br>Prevent pairing of new controllers. | _None_ | `[0x1]` |
| `0x06` | Disconnect Controller<br>Disconnect and turn off specified controller. | `[ControllerIndexByte]` | `[0xA]` if request data not received (**no further data**)<br>`[0xB]` if controller not connected (**no further data**)<br>`[0x13]` if successful |
| `0x07` | Set Controller LED Colour<br>Set LED color and brightness for supported controllers.<br>Color values: 0-255. | `[ControllerIndexByte, RedValueByte, GreenValueByte, BlueValueByte]` | `[0xA]` if request data not received (**no further data**)<br>`[0xB]` if controller not connected (**no further data**)<br>`[0xD]` if successful |
| `0x08` | Set Controller Vibration<br>Set vibration magnitude and duration.<br>Delay/duration in ms, magnitude: 0-255. | `[ControllerIndexByte, Delay MS Low, Delay MS High, Duration MS Low, Duration MS High, WeakMagnitude, StrongMagnitude]` | `[0xA]` if request data not received (**no further data**)<br>`[0xB]` if controller not connected (**no further data**)<br>`[0xE]` if successful |
| `0x09` | Get Controller Information<br>Get type and battery level of controller.<br>Battery: 0 (unknown) to 255 (full). | `[ControllerIndexByte]` | `[0xA]` if request data not received (**no further data**)<br>`[0xB]` if controller not connected (**no further data**)<br>`[0xF, <Controller Type>, <Battery Level>]` if successful. For values see [Bit Masks](#bit-masks) |
| `0x0A` | Forget Bluetooth Keys<br>Unpair all existing controllers. | _None_ | `[0x14]` |
| `0x0B` | Set Player LED<br>Set player indicator LEDs. | `[ControllerIndexByte, PlayerNumberByte]` | `[0xA]` if request data not received (**no further data**)<br>`[0xB]` if controller not connected (**no further data**)<br>`[0x15]` if successful. For values see [Bit Masks](#bit-masks) |

**Error Status Bytes:**  
- `[0xA]` — Request data not received (no further data returned)  
- `[0xB]` — Controller not connected (no further data returned)
- `[0x17]` - Command was not found (no further data returned)

---

## Bit Masks
The state of indivudual buttons is done by using specific bits in the response bytes.

The following mask values can be used to get the different controller button states.

| Mask Value | Button |
| -- | -- |
| -- | **DPAD Masks** |
| 0x1 | Dpad up |
| 0x2 | Dpad down |
| 0x4 | Dpad Left |
| 0x5 | dpad Right |
| -- | **Button Masks** |
| 0x1 | Cross Button |
| 0x2 | Circle Button |
| 0x4 | Square Button |
| 0x8 | Triangle Button |
| 0x10 | L1 Button |
| 0x40 | L2 Button |
| 0x100 | L3 Button |
| 0x20 | R1 Button |
| 0x80 | R2 Button |
| 0x200 | R3 Button |
| -- | **Misc Button Masks** |
| 0x1 | System Misc Button |
| 0x2 | Capture Misc Button |
| 0x4 | Start Misc Button |
| 0x8 | Select Misc Button |
| -- | **Player LED Masks** |
| 0x1 | Player One LED |
| 0x2 | Player Two LED |
| 0x4 | Player Three LED |
| 0x8 | Player Four LED |

## Controller Types

## How to compile it

Clone the project. 

`git clone --recursive git@github.com:ste2425/PSP-Bluetooth-Firmware.git`

I use PlatformIO to build and upload.

### Using PlatformIO

1. Using Visal Studio Code install the PlatformIO extension.
2. Open the extension in the left hand menu.
2. Click on "Pick a folder", and select the recently cloned project

Let PlatformIO download the ESP-IDF toolchain and its dependencies.
This can take some time.

*Note: You may have to remove previously installed PlatformIO packages. Run `rm -rf ~/.platformio`
and reinstall PlatformIO.*

Once all installed:

1. Click on one of the pre-created boards, like `esp32dev`. Or edit `platformio.ini` file, and add your own.
2. Click on *build*

*Note*. Clicking upload should first do a build if you want to build and upload.

