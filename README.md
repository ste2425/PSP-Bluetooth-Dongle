# PSP Bluetooth *Dongle*

This is the firmware for a ESP32 based dongle to act as the bridge between the PSP and Bluetooth Controllers.

It is a PlatformIO project and powered by the great [BluePad32 Library](https://bluepad32.readthedocs.io/en/latest) and is based on the [template](https://github.com/ricardoquesada/esp-idf-arduino-bluepad32-template.git) project.

## Acknowledgements

The work ive done is just a thin layer ontop of work many other people have done, with lots of help from others.

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

---

### [0x02] Ping Command
A simple ping command to allow the PSP to detect if the ESP32 is connected.

**Request bytes** : None.

**Response Bytes** : [0x10]

---

### [0x03] Request Controller Data Command
Used to request the state of a controller at a given index.

**Request bytes**: [ControllerIndexByte]

**Response Bytes**

If Request data not recieved: [0xA]

If controller not connected at given index: [0xB]

If controller index found the following bytes are returned.

**Analog stick NOTE** All analog stick values follow the same range of `0 - 255`. `0` being the sticks left/bottom most position and `255` its right/top most position.

| index | purpose |
| -- | -- |
| 0 | Fixed status byte of `0xC` |
| 1 | Right analog stick X axis.  |
| 2 | Right analog stick Y axis. |
| 3 | Left analog stick X axis. |
| 4 | Right analog stick Y axis. |
| 5 | Dpad data. For values see [Bit Masks](#bit-masks) |
| 6 | Controller Buttons. *High Byte*. |
| 7 | Controller Buttons. *Low Byte*. For values see [Bit Masks](#bit-masks) |
| 8 | Controller Misc Buttons. *High Byte*. |
| 9 | Controller Misc Buttons. *Low Byte*. For values see [Bit Masks](#bit-masks) |

---

### [0x04] Enable New Connections Command

Enable pairing for new controllers. If enabled the dongle will autmatically connect to any controllers in pairing mode. Upto `four` controllers can be connected at once.

**Request bytes** : None.

**Response Bytes** : [0x11]

---

### [0x05] Disable New Connections Command

Prevents pairing of new controllers. Exiting paired controllers will still connect.

**Request bytes** : None.

**Response Bytes** : [0x1]

---

### [0x06] Disconnect Controller Command

Will cause the specified controller to disconnect and turn off.

**Request bytes**: [ControllerIndexByte]

**Response Bytes**

If Request data not recieved: [0xA]

If controller not connected at given index: [0xB]

If controller index found and command executed: [0x13]

---

### [0x07] Set Controller LED Colour

Some controllers such as the DS4 and the DuaSense have a built LED. The colour and brightness of this can be set.

**Note** colour values are in the range of `0 - 255`. With `255` the brightest and `0` off for that colour.

**Request bytes**: [
    ControllerIndexByte,
    RedValueByte,
    GreenValueByte,
    BlueValueByte
]

**Response Bytes**

If Request data not recieved: [0xA]

If controller not connected at given index: [0xB]

If controller index found and command executed: [0xD]

---

### [0x8] Set Controller Vibration

If the controller support vibration it can be set. If it does not this command will be ignored.

Some controllers, such as the DualSense, and a strong and weak motor. The magnitude of these can be controlled invidually.

If the controller, such as the Switch Pro, does not have this feature the strong motor value will be used.

There is also a duration value. This is usefull should the controller loose a packet of data, the vibration will not remain on forever.

**Request bytes**: 

| index | byte value |
| -- | -- |
| 0 | Controller index |
| 1 | delay in MS |
| 2 | delay in MS |
| 3 | duration in MS |
| 4 | duration in MS |
| 5 | weak magnitude |
| 6 | string magnitude |

**Response Bytes**

If Request data not recieved: [0xA]

If controller not connected at given index: [0xB]

If controller index found and command executed: [0xE]

---

### [0x9] Get Controller Information Command

This will return information on the type of controller connected and its battery level.

**Request Bytes** [ControllerIndexByte]

**Response Bytes**

If Request data not recieved: [0xA]

If controller not connected at given index: [0xB]

If controller index found and command executed: 

**Note** For battery level the range is `0 - 255`. `0` means an unknown battery state, `1` is empty and `255` is full.

| byte index | value |
| -- | -- |
| 0 | Fixed satus byte `0xF` |
| 1 | Controler Type byte. See [Controller Types](#controller-types) |
| 2 | Battery level Byte |

---

### [0xA] Forget Bluetooth Keys Command

This will instruct the dongle to remove all exist bluetooth keys for connected controllers. This will essentially unpair all existing controllers.

**Request Bytes** N/A

**Response Bytes** [0x14]

---

### [0xB] Set Player LED Command

Most controllers have an indicator for which player the controller is for, usually one to four LED's. These can be set.

**Request Bytes** [ControllerIndexByte, playerNumberByte - See [Bit Masks](#bit-masks)]

**Response Bytes**

If Request data not recieved: [0xA]

If controller not connected at given index: [0xB]

If controller index found and command executed: [0x15]

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