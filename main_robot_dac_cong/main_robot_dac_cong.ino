#include <Arduino.h>
#include "Relay.h"
#include <SoftwareSerial.h>
// #include "BluetoothSerial.h"
#define RXD2 14 
#define TXD2 12
#include "Motor.h"
#define hand 0
#define fire 1
#define lamp 2
// #define fire 3

// SoftwareSerial wheels(14, 12);  // RX, TX
// SoftwareSerial rotary(14, 12);
// fire config
#define fire_duration 500



Motor fire_1(36, 39, 18, 23);
Motor fire_2(5, 17, 13, 15);

// BluetoothSerial SerialBT;

Relay myRelay;

void fire_process(){
    long fire_timeout = 0;

     myRelay.set(fire, 1);
    fire_timeout = millis() + fire_duration/2;
    while(millis() < fire_timeout);

     myRelay.set(fire, 0);
    fire_timeout = millis() + fire_duration/2;
    while(millis() < fire_timeout);
}

void combo(char stt){
    if(stt == 'R' or stt == 'L' or stt == 'M'){
        fire_process();
        fire_process();
  }
}

void reset() {
  myRelay.set(hand, 0);
  myRelay.set(fire, 0);
  myRelay.set(lamp, 0);
  // myRelay.set(fire, 0);
  fire_1.setMotorSpeed(0);
  fire_2.setMotorSpeed(0);
}

void setup() {
  myRelay.init();
  Serial.begin(115200);              // Initialize serial communication at 9600 bps
  // SerialBT.begin("Dac_cong_robot");  // Set the Bluetooth device name
  delay(100);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // wheels.begin(38400);
  // rotary.begin(38400);
  // Serial.println("The device started, now you can pair it with Bluetooth!");
  reset();

  //setup dàn bắn
  fire_1.begin(32, 10, 255, 1);
  fire_2.begin(32, 10, 255, 1);
}

void loop() {
  // Kiểm tra nếu có dữ liệu từ Software Serial
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    processSerialCommand(command);
  }
  // if (SerialBT.available()) {
  //   String command = SerialBT.readStringUntil('\n');
  //   processSerialCommand(command);
  // }
  if (Serial2.available()) {
      static String string_2 = "";
      char ch = Serial2.read();
      if(ch == '\n'){
          processSerialCommand(string_2);
          string_2 = "";
      }else{
          string_2 += ch;
    }
  }
}

void processSerialCommand(String command) {
  Serial.println(command);
  command.trim();  // Remove any leading/trailing whitespace
  //xylanh nâng hạ tay
  if (command.startsWith("H")) {
    String values = command.substring(1, command.length());
    Serial.println(values);
    if (values == "U"){
      myRelay.set(hand, 1);
      myRelay.set(3, 1);
      
    }
    if (values == "D"){
      myRelay.set(hand, 0);
      myRelay.set(3, 0);
    }
  }
  //xylanh fire
  else if (command.startsWith("F")) {
    int values = command.substring(1, command.length()).toInt();
    if (values == 1){
      myRelay.set(fire, 1);
      delay(500);
      myRelay.set(fire, 0);
    } 
    else myRelay.set(fire, 0);
  }
  //xylanh thả đèn (lamp)
  else if (command.startsWith("L")) {
    int values = command.substring(1, command.length()).toInt();

    if (values == 1) myRelay.set(lamp, 1);

    else myRelay.set(lamp, 0);
  }
  else if(command.startsWith("C")){
    char stt = command.charAt(1);
    combo(stt);
  }

  else if (command.startsWith("SG")) {
    int values = command.substring(2, command.length()).toInt();
    fire_1.setMotorSpeed(-values);
    fire_2.setMotorSpeed(values);
  } else {
    Serial.println("Unknown command");
    Serial.println(command);
  }
}
