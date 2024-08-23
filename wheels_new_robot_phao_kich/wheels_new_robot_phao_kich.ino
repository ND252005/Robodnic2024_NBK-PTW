#include <Motor.h>
// #include "BluetoothSerial.h"
// #include <SoftwareSerial.h>

#define RXD2 16
#define TXD2 4

#define storage_resolution 633
#define steps 32 
#define up_storage 0
#define down_storage -1400 
#define mid_storage -1200
#define left_floor -633
#define right_floor 633
#define mid_floor 0
#define combo_fire_resolution 100

// Wheel config
#define speed_ratio 0.9

#define rotate_duration 500
#define min_floor 32  //2.25 degree
#define min_hand 50  
// BluetoothSerial SerialBT;
const long loopInterval = 40; // 1000 ms / 25 Hz = 40 ms
int storage_steps = 0;
int hand_abs = 0;
int floor_abs = 0;

Motor rotary_storage(36, 39, 18, 23);  // ID 2 
Motor rotary_floor(33, 32, 19, 22); // ID 3

Motor motor_left(34, 35, 13, 15);   // ID 1
Motor motor_right(26, 25, 5, 17);   // ID 4

// void rotate_floor(int direction){
//   // storage_steps += direction*storage_resolution;
//   rotary_floor.goto_position(direction*storage_resolution);
// }
void combo(char stt){
    if(stt == 'R'){
        //quay phải + bắn
        int current = rotary_floor.setpoint;
        rotary_floor.goto_position(current += min_floor);
        long t_out = millis() + 1000;
        while(millis() < t_out){
            motor_update();
            if(rotary_left.is_stop && rotary_right.is_stop)
                break;
        }

        //quay phải + bắn
        rotary_floor.goto_position(current += min_floor);


    }
    if(stt == 'L'){
         //quay phải + bắn
        int current = rotary_floor.setpoint;
        rotary_floor.goto_position(current -= min_floor);
        long t_out = millis() + 1000;
        while(millis() < t_out){
            motor_update();
            if(rotary_left.is_stop && rotary_right.is_stop)
                break;
        }

        //quay phải + bắn
        rotary_floor.goto_position(current -= min_floor);


    }
    //combo start 
    if(stt == 'S'){
        move(1, 255);
        rotary_storage.goto_position(hand_abs + mid_storage);
    }

    //combo lay bong
    if(stt == 'T'){
        move(1, 80);
        rotary_storage.goto_position(hand_abs + down_storage);
    }
}
void move(int code, int speed) {
    Serial.println(code);
    Serial.println(speed);

    if(code == 0){
      motor_left.setMotorSpeed(0);
      motor_right.setMotorSpeed(0);
    } else if(code == 1){
      motor_left.setMotorSpeed(speed);
      motor_right.setMotorSpeed(speed);
    } else if(code == 2){
      motor_left.setMotorSpeed(-speed);
      motor_right.setMotorSpeed(-speed);
    } else if(code == 3){
      motor_left.setMotorSpeed(-speed/2);
      motor_right.setMotorSpeed(speed/2);
    } else if(code == 4){
      motor_left.setMotorSpeed(speed/2);
      motor_right.setMotorSpeed(-speed/2);
    } else if(code == 5){
      motor_left.setMotorSpeed(0);
      motor_right.setMotorSpeed(speed);
    } else if(code == 6){
      motor_left.setMotorSpeed(speed);
      motor_right.setMotorSpeed(0);
    } else if(code == 7){
      motor_left.setMotorSpeed(-speed);
      motor_right.setMotorSpeed(0);
    } else if(code == 8){
      motor_left.setMotorSpeed(0);
      motor_right.setMotorSpeed(-speed);
    }

    // Serial.println(target);
    // Serial.println(distance_speed);
    // Serial.println();
}
void reset(){
  rotary_storage.goto_position(0);
  rotary_floor.goto_position(0);
}

void tuning_hand(char value){
    int sign = 0;
    if(value == '+')    sign = 1;
    if(value == '-')    sign = -1;
    hand_abs += sign * min_hand;
    rotary_storage.goto_position(
        rotary_storage.getCounter() + sign * min_hand
    );
}

void tuning_floor(char value){
    int sign = 0;
    if(value == '+')    sign = 1;
    if(value == '-')    sign = -1;
    hand_abs += sign * min_hand;
    rotary_floor.goto_position(
        rotary_floor.getCounter() + sign * min_floor
    );
}
void turning_wheels(char dir){
    if(dir == 'L'){
        motor_left.setMotorSpeed(motor_left.get_speed() * deviation);
    } 
    if(dir == 'R'){
        motor_right.setMotorSpeed(motor_right.get_speed() * deviation);
    }
}

float wheel_P = 5;
float I = 0.1;
// float rotary_D = 0.15;   //140RPM
float wheel_D = 0.02;


void setup() {
    Serial.begin(115200);
    // SerialBT.begin("4DC_Servo_TEST"); // Set the Bluetooth device name
    // Serial.println("The device started, now you can pair it with Bluetooth!");
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

    motor_left.begin(32, 3, 100, 1);
    // motor_left.setPID(wheel_P, I, wheel_D);

    motor_right.begin(32, 3, 100, 1);
    // motor_right.setPID(wheel_P, I, wheel_D);

    rotary_storage.begin(32, 10, 255, 1);
    rotary_storage.setPID(wheel_P, I, wheel_D);

    rotary_floor.begin(32, 3, 255, 1);
    rotary_floor.setPID(wheel_P, I, wheel_D);
}
#define soft_start_value  50
void motor_update() {
    static long next_time = millis();
    if(millis() > next_time) {
        // motor_left.computeAndSetMotorSpeed();
        // motor_right.computeAndSetMotorSpeed();
        rotary_storage.computeAndSetMotorSpeed();
        rotary_floor.computeAndSetMotorSpeed();

        // Print debug information
        if(false){
            // Serial.print(motor_left.getCounter());
            // Serial.print("/");
            // Serial.print(motor_left.setpoint);
            // Serial.print("\t");
        //     Serial.print(motor_right.getCounter());
        //     Serial.print("/");
        //     Serial.print(motor_right.setpoint);
        //     Serial.print("\t\t");
            Serial.print(rotary_storage.getCounter());
            Serial.print("/");
            Serial.print(rotary_storage.setpoint);
            Serial.print("\t");
            Serial.print(rotary_floor.getCounter());
            Serial.print("/");
            Serial.println(rotary_floor.setpoint);
        }
        next_time += loopInterval;
    }
}
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
    motor_update();
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
    if(command.startsWith("H")){
        char storage_sign = command.charAt(1);
        if(storage_sign == 'D'){
            rotary_storage.goto_position(hand_abs + down_storage);
        }else if( storage_sign == 'U'){
            rotary_storage.goto_position(hand_abs + up_storage);
        }else if(storage_sign == 'M'){
            rotary_storage.goto_position(hand_abs + mid_storage);
        }else if(storage_sign == '+' or storage_sign == '-'){
            tuning_hand(storage_sign);
        }
    }

    if(command.startsWith("P")){
        char floor_sign = command.charAt(1);
        if(floor_sign == 'L'){
            rotary_floor.goto_position(floor_abs + left_floor);
        }else if( floor_sign == 'R'){
            rotary_floor.goto_position(floor_abs + right_floor);
        }else if(floor_sign == 'M'){
            rotary_floor.goto_position(floor_abs + mid_floor);
        }else if(floor_sign == '+' or floor_sign == '-'){
            tuning_floor(floor_sign);
        }
    }
    if(command.startsWith("C")){
      char stt = command.charAt(1);
      combo(stt);
    }
    // if(command.startsWith("K")){
    //   int values = command.substring(1, command.length()).toInt();
    //   rotary_storage.goto_position(values);
    // }


}
