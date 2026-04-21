#include <BleCombo.h>
#include <nvs_flash.h>

// ====== Config ======
#define DEVICE_NAME    "C3-HID-v1"
#define DEVICE_MFG     "Sachintha"
#define BATTERY_LEVEL  100

const bool ERASE_BONDS_ON_BOOT = true;

// ====== HID instances (renamed to avoid clash with library globals) ======
BleComboKeyboard bleKB(DEVICE_NAME, DEVICE_MFG, BATTERY_LEVEL);
BleComboMouse    bleMouse(&bleKB);

// ====== Globals ======
unsigned long lastAction = 0;
const unsigned long ACTION_INTERVAL_MS = 5000;

void wipeBleBonds() {
  Serial.println("[NVS] Erasing flash to clear old BLE bonds...");
  nvs_flash_erase();
  nvs_flash_init();
  Serial.println("[NVS] Done.");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=== ESP32-C3 BLE HID Combo ===");

  if (ERASE_BONDS_ON_BOOT) {
    wipeBleBonds();
  }

  bleKB.begin();
  bleMouse.begin();

  Serial.printf("Advertising as: %s\n", DEVICE_NAME);
  Serial.println("On your Mac: System Settings -> Bluetooth -> connect to this device.");
  Serial.println("Waiting for BLE host to connect...");
}

void sendDemoSequence() {
  Serial.println("[HID] Sending demo sequence");

  bleKB.print("Hello from ESP32-C3 ");
  delay(300);

  bleKB.write(KEY_RETURN);
  delay(300);

  bleKB.press(KEY_LEFT_GUI);
  bleKB.press(' ');
  delay(100);
  bleKB.releaseAll();
  delay(500);

  bleKB.write(KEY_ESC);
  delay(300);

  bleMouse.move(50, 0);   delay(150);
  bleMouse.move(0, 50);   delay(150);
  bleMouse.move(-50, 0);  delay(150);
  bleMouse.move(0, -50);  delay(150);

  bleMouse.click(MOUSE_LEFT);
  delay(200);

  bleMouse.move(0, 0, -3);
  delay(300);
  bleMouse.move(0, 0, 3);

  Serial.println("[HID] Sequence done.");
}

void loop() {
  static bool wasConnected = false;
  bool connected = bleKB.isConnected();

  if (connected && !wasConnected) {
    Serial.println("[BLE] Host connected.");
    delay(2000);
  } else if (!connected && wasConnected) {
    Serial.println("[BLE] Host disconnected.");
  }
  wasConnected = connected;

  if (connected && (millis() - lastAction >= ACTION_INTERVAL_MS)) {
    sendDemoSequence();
    lastAction = millis();
  }
Hello from ESP32-CHello from ESP32-C

Hello from ESP32-Hello from ESP32-


  delay(50);
}