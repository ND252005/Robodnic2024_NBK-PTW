#include <Motor.h>
// #include "BluetoothSerial.h"
#include <SoftwareSerial.h>

#define RXD2 16
#define TXD2 4

int goc_duoi = 200;
int goc_tren = -1200;
// BluetoothSerial SerialBT;
const long loopInterval = 40; // 1000 ms / 25 Hz = 40 ms


Motor motor_left_backup(36, 39, 18, 23);  // ID 2 
Motor motor_right_backup(33, 32, 19, 22); // ID 3

Motor motor_left(34, 35, 13, 15);   // ID 1
Motor motor_right(26, 25, 5, 17);   // ID 4

void move(int code, int speed) {
    Serial.println(code);
    Serial.println(speed);

    if(code == 0){
      motor_left.setMotorSpeed(0);
      motor_left_backup.setMotorSpeed(0);
      motor_right.setMotorSpeed(0);
      motor_right_backup.setMotorSpeed(0);
    } else if(code == 1){
      motor_left.setMotorSpeed(speed);
      motor_left_backup.setMotorSpeed(speed);
      motor_right.setMotorSpeed(speed);
      motor_right_backup.setMotorSpeed(speed);
    } else if(code == 2){
      motor_left.setMotorSpeed(-speed);
      motor_left_backup.setMotorSpeed(-speed);
      motor_right.setMotorSpeed(-speed);
      motor_right_backup.setMotorSpeed(-speed);
    } else if(code == 3){
      motor_left.setMotorSpeed(-speed/2);
      motor_left_backup.setMotorSpeed(-speed/2);
      motor_right.setMotorSpeed(speed/2);
      motor_right_backup.setMotorSpeed(speed/2);
    } else if(code == 4){
      motor_left.setMotorSpeed(speed/2);
      motor_left_backup.setMotorSpeed(speed/2);
      motor_right.setMotorSpeed(-speed/2);
      motor_right_backup.setMotorSpeed(-speed/2);
    } else if(code == 5){
      motor_left.setMotorSpeed(0);
      motor_left_backup.setMotorSpeed(0);
      motor_right.setMotorSpeed(speed);
      motor_right_backup.setMotorSpeed(speed);
    } else if(code == 6){
      motor_left.setMotorSpeed(speed);
      motor_left_backup.setMotorSpeed(speed);
      motor_right.setMotorSpeed(0);
      motor_right_backup.setMotorSpeed(0);
    } else if(code == 7){
      motor_left.setMotorSpeed(0);
      motor_left_backup.setMotorSpeed(0);
      motor_right.setMotorSpeed(-speed);
      motor_right_backup.setMotorSpeed(-speed);

    } else if(code == 8){
      motor_left.setMotorSpeed(-speed);
      motor_left_backup.setMotorSpeed(-speed);
      motor_right.setMotorSpeed(0);
      motor_right_backup.setMotorSpeed(0);
    }

    // Serial.println(target);
    // Serial.println(distance_speed);
    // Serial.println();
}


float wheel_P = 0.5;
float I = 0.1;
// float rotary_D = 0.15;   //140RPM
float wheel_D = 0.02;


void setup() {
    Serial.begin(115200);
    // SerialBT.begin("4DC_Servo_TEST"); // Set the Bluetooth device name
    // Serial.println("The device started, now you can pair it with Bluetooth!");
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

    motor_left.begin(32, 3, 255, 1);
    // motor_left.setPID(wheel_P, I, wheel_D);
    motor_left_backup.begin(32, 3, 255, 1);
    // motor_left.setPID(wheel_P, I, wheel_D);

    motor_right.begin(32, 3, 255, 1);
    // motor_right.setPID(wheel_P, I, wheel_D);
    motor_right_backup.begin(32, 3, 255, 1);
    // motor_right_backup.setPID(wheel_P, I, wheel_D);
}
#define soft_start_value  50
void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        processSerialCommand(command);
    }
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
    // Check for serial commands
    // if (SerialBT.available()) {
    //     String command = SerialBT.readStringUntil('\n');
    //     processSerialCommand(command);
    // }
}

static int wheels_speed = 0;

void processSerialCommand(String command) {
    Serial.println(command);
    command.trim();  // Remove any leading/trailing whitespace

    if(command.startsWith("M")){
      int direction = command.substring(1).toInt();
      move(direction, wheels_speed);
    }
    if(command.startsWith("SS")){
      wheels_speed = command.substring(2, command.length()).toInt();
    }

}
