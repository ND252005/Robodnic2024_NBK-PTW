#include <SoftwareSerial.h>

// Chọn các chân GPIO cho TX và RX
SoftwareSerial mySerial(4, 16);  // RX, TX

void setup() {
  // Khởi động serial monitor cho debug
  Serial.begin(115200);
  while (!Serial) {
    ; // Chờ Serial sẵn sàng
  }

  // Khởi động Software Serial
  mySerial.begin(115200);
  Serial.println("Software serial started");
}
String str = "";
void loop() {
  // Kiểm tra nếu có dữ liệu từ Software Serial
  if (mySerial.available()) {
    char c = mySerial.read();
    str += c;
    if(c == '\n'){
      Serial.println(str);
      str = "";
    }
      // In ra serial monitor
  }

  // Kiểm tra nếu có dữ liệu từ Serial Monitor
  if (Serial.available()) {
    char c = Serial.read();
    mySerial.write(c);  // Gửi dữ liệu đến Software Serial
  }
}
