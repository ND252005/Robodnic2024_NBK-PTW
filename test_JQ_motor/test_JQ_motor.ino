#include <Motor.h>
#include "BluetoothSerial.h"
#include <SoftwareSerial.h>

SoftwareSerial mainbot(4, 16);  // RX, TX

BluetoothSerial SerialBT;

Motor rotary_storage(36, 39, 18, 23);  // ID 2 
Motor rotary_floor(33, 32, 19, 22); // ID 3

const long loopInterval = 40; // 1000 ms / 25 Hz = 40 ms

int storage_resolution = 300;
int storage_steps = 0;
int floor_resolution = 950;

float rotary_P = 5;
float wheel_P = 0.5;
float I = 0.1;
float rotary_D = 0.15;
float wheel_D = 0.02;

int floorcode2dir[3] = {-1, 0, 1};

void rotate_storage(int direction) {    
    // long storage_taget =  direction * storage_resolution;
    long storage_taget =  direction;

    rotary_storage.goto_position(storage_taget);
    rotary_storage.goto_position(500);
}

void rotate_floor(int direction) {
  long floor_target = direction * floor_resolution;
  Serial.println(floor_target);
  rotary_floor.goto_position(floor_target);
}


void setup() {
    Serial.begin(115200);
    // SerialBT.begin("4DC_Servo"); // Set the Bluetooth device name
    // Serial.println("The device started, now you can pair it with Bluetooth!");
    mainbot.begin(38400);
    // max_i_error, skip_error, max_speed, ofset
    rotary_storage.begin(32, 3, 255, 1);
    rotary_storage.setPID(rotary_P, I, rotary_D);

    rotary_floor.begin(32, 3, 255, 1);
    rotary_floor.setPID(rotary_P, I, rotary_D);

    // digitalWrite(22, 1);

}

#define soft_start_value 50   //2s
void motor_update() {
    static long next_time = millis();
    if(millis() > next_time) {
        rotary_storage.computeAndSetMotorSpeed();
        rotary_floor.computeAndSetMotorSpeed();

        Serial.print(rotary_storage.getCounter());
        Serial.print("\t");
        Serial.println(rotary_floor.getCounter());
        next_time += loopInterval;
    }
}
String str = "";
void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        processSerialCommand(command);
    }

    // Check for serial commands
    if (SerialBT.available()) {
        String command = SerialBT.readStringUntil('\n');
        processSerialCommand(command);
    }

  if (mainbot.available()) {
    // String receivedMessage = mainbot.readStringUntil('\n'); // Đọc chuỗi từ SoftwareSerial
    // Serial.println("Received: " + receivedMessage); // In ra Serial Monitor
    String command = mainbot.readStringUntil('\n');
    processSerialCommand(command);  
  }
    motor_update();

}

void processSerialCommand(String command) {
    Serial.println(command);  
    command.trim();  // Remove any leading/trailing whitespace

    if (command.startsWith("H")) {
      String storage_sign = command.substring(1, command.length());
      Serial.println(storage_sign);
      if(storage_sign == "D" and storage_steps < 1200){
        Serial.println("tay dap xuong!");
        storage_steps += storage_resolution;
      }
      if(storage_sign == "U" and storage_steps > 0){
        Serial.println("tay dap len!");
        storage_steps -= storage_resolution;
      }
      rotate_storage(storage_steps);
      Serial.println(storage_steps);
      
      // if(storage_stt) rotary_storage.goto_position(400);
      // else rotary_storage.goto_position(0);
      // rotate_storage(storage_stt);
      // rotary_storage.goto_position(400 * 4);
      
    } else if(command.startsWith("P")) {
      int floor_stt = command.substring(1).toInt();
      Serial.println(floor_stt);
      int target = floorcode2dir[floor_stt-1];
      if(target == 0) Serial.println("0 do!");
      if(target == 1) Serial.println("45 do!");
      if(target == -1) Serial.println("-45 do!");
      if(target >-2 and target < 2){
        Serial.println(target);
        rotate_floor(target);
      }
    } else {
        Serial.println(command);
        Serial.println("Unknown command");
    }
}
