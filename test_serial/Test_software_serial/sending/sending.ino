#include <SoftwareSerial.h>
#include "BluetoothSerial.h"

String device_name = "ESP32-softwareserial-testing";

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

// Chọn các chân GPIO cho TX và RX
SoftwareSerial mySerial(14, 12);  // RX, TX

void setup() {
  // Khởi động serial monitor cho debug
  Serial.begin(115200);
  while (!Serial) {
    ; // Chờ Serial sẵn sàng
  }

  // Khởi động Software Serial
  mySerial.begin(9600);
  Serial.println("Software serial started");
  SerialBT.begin(device_name);  //Bluetooth device name
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

}

String str = "";
void loop() {
  // Kiểm tra nếu có dữ liệu từ Software Serial
  if (mySerial.available()) {
    char c = mySerial.read();
    Serial.write(c);  // In ra serial monitor
  }

  if (SerialBT.available()) {
    char c = SerialBT.read();  // Thay đổi từ mySerial.read() thành SerialBT.read()
    str += c;
    if(c == '\n'){
      mySerial.write(str.c_str());
      str = "";
    }
  }
}
  