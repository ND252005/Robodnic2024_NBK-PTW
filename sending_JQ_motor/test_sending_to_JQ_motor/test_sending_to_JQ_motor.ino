#include <SoftwareSerial.h>
#include "BluetoothSerial.h"

String device_name = "Robot_phao_kich";

// Check if Bluetooth is available
// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
// #endif

// // Check Serial Port Profile
// #if !defined(CONFIG_BT_SPP_ENABLED)
// #error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
// #endif

BluetoothSerial SerialBT;

// Chọn các chân GPIO cho TX và RX
SoftwareSerial DC_servo(14, 12);  // RX, TX

void setup() {
  // Khởi động serial monitor cho debug
  Serial.begin(115200);
  while (!Serial) {
    ; // Chờ Serial sẵn sàng
  }

  // Khởi động Software Serial
  DC_servo.begin(9600);
  Serial.println("Software serial started");
  SerialBT.begin(device_name);  //Bluetooth device name
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

}

void loop() {
  // Kiểm tra nếu có dữ liệu từ Software Serial
  if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        processSerialCommand(command);
  }
  if (SerialBT.available()) {
        String command = SerialBT.readStringUntil('\n');
        processSerialCommand(command);  
  }
}

void processSerialCommand(String command) {
    Serial.println(command);
    command.trim();  // Remove any leading/trailing whitespace
    if(command.startsWith("K")){
      String str = command.substring(1, command.length());
      DC_servo.println(str); // Gửi chuỗi qua SoftwareSerial
      Serial.println("Sent: " + str); // In ra Serial Monitor để debug
    } else {
        Serial.println("Unknown command");
        Serial.println(command);
    }
}



