# ESP32-C3 BLE HID

Wireless Bluetooth Low Energy keyboard + mouse firmware for the ESP32-C3 Super Mini.
Pairs with macOS, Windows, Linux, Android, and iOS as a standard BLE HID peripheral.

## Why BLE and not USB?

The ESP32-C3's native USB peripheral is USB Serial/JTAG only — it cannot enumerate as a
USB HID device. For wired HID on this chip, you need an external CH9329 UART-to-HID
bridge. This project takes the wireless route instead using the C3's built-in BLE radio.

## Hardware

- ESP32-C3 Super Mini (any variant)
- USB-C cable for flashing and power
- Optional: LiPo battery for standalone operation

## Software

- Arduino IDE 2.x or PlatformIO
- ESP32 Arduino core v2.0.5+
- [ESP32-BLE-Combo](https://github.com/blackketter/ESP32-BLE-Combo) library

## Setup

1. Install the ESP32 board package in Arduino IDE
2. Install ESP32-BLE-Combo manually (ZIP from GitHub)
3. Remove any `ESP32 BLE Keyboard` or `ESP32 BLE Mouse` libraries — they conflict
4. Open `ESP32_C3_Keyboard_mouse.ino`
5. Select board: **ESP32C3 Dev Module**
6. Set **Partition Scheme: Huge APP (3MB No OTA/1MB SPIFFS)**
7. Enable **USB CDC On Boot**
8. Flash

## Pairing (macOS)

1. Open System Settings → Bluetooth
2. Find `C3-HID-v1` in the device list
3. Click Connect
4. If pairing fails with `GATT_INSUF_AUTHENTICATION`:
   - Forget the device on Mac
   - Run `sudo pkill bluetoothd` in Terminal
   - Set `ERASE_BONDS_ON_BOOT = true` in the sketch
   - Re-flash and try again

## Usage

After pairing, the device sends a demo HID sequence every 5 seconds:

- Types "Hello from ESP32-C3"
- Presses Enter
- Opens and closes Spotlight (Cmd+Space, Esc)
- Moves the mouse in a square
- Left clicks
- Scrolls up and down

Modify `sendDemoSequence()` to trigger your own actions.

## Configuration

Edit these at the top of the sketch:

| Constant              | Purpose                                                                      |
| --------------------- | ---------------------------------------------------------------------------- |
| `DEVICE_NAME`         | Name shown during pairing                                                    |
| `DEVICE_MFG`          | Manufacturer string                                                          |
| `BATTERY_LEVEL`       | Reported battery % (0–100)                                                   |
| `ERASE_BONDS_ON_BOOT` | Wipe stored bonds on every boot (set `false` after first successful pairing) |
| `ACTION_INTERVAL_MS`  | Delay between demo sequences                                                 |

## Troubleshooting

**Multiple definition of `Keyboard` / `Mouse`**
The library defines globals with those names. This sketch uses `bleKB` and `bleMouse` instead.

**Sketch too big**
Change partition scheme to `Huge APP (3MB No OTA/1MB SPIFFS)`.

**Mac won't pair**
Change `DEVICE_NAME` to a new string, forget old device on Mac, set
`ERASE_BONDS_ON_BOOT = true`, re-flash.

**Bluedroid errors on macOS**
Known Bluedroid stack issue. Consider switching to a NimBLE-based HID fork.

## Roadmap

- [ ] Physical button to trigger actions
- [ ] Integration with 433 MHz RF sniffer (garage remote → HID event)
- [ ] Battery level reporting from ADC
- [ ] Deep sleep between events
- [ ] Web config portal for custom action mapping

## License

MIT

## Credits

- [T-vK](https://github.com/T-vK/ESP32-BLE-Keyboard) — original BLE Keyboard library
- [Blackketter](https://github.com/blackketter/ESP32-BLE-Combo) — combo fork adding mouse support
