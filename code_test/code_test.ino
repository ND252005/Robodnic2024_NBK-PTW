#include <Motor.h>
Motor motor_2(36, 39, 18, 23);  // ID 2
Motor motor_3(33, 32, 19, 22);    // ID 3

Motor motor_1(34, 35, 13, 15);  // ID 1
Motor motor_4(26, 25, 5, 17);  // ID 4

void setup() {
  motor_1.begin(32, 3, 255, 1);
  motor_2.begin(32, 3, 255, 1);
  motor_3.begin(32, 3, 255, 1);
  motor_4.begin(32, 3, 255, 1);
}

void loop() {
  for(int i = 0; i < 256; i++){
    motor_1.setMotorSpeed(i);
    motor_2.setMotorSpeed(i);
    motor_3.setMotorSpeed(i);
    motor_4.setMotorSpeed(i);
    delay(20);
  }
  for(int i = 0; i < 256; i++){
    motor_1.setMotorSpeed(-i);
    motor_2.setMotorSpeed(-i);
    motor_3.setMotorSpeed(-i);
    motor_4.setMotorSpeed(-i);
    delay(20);
  }

}
