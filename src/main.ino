#include <Arduino.h>
#include <RF24.h>
#include "Config.h"
#include "Motor.h"
#include <Servo.h>

struct transferred_command_datatype { bool forward_movement; bool right_movement; bool backward_movement; bool left_movement; bool treasure_position_up; };
RF24 radio(RF_RADIO_CE_PIN, RF_RADIO_CSN_PIN); // CE, CSN
Motor driver_motor;
Servo treasure_servo;
Servo direction_servo;

#ifdef TRANSMITTER
void setup()
{
    Serial.begin(9600);
    pmipup(JOYSTIC_UP_PIN); pmipup(JOYSTIC_RIGHT_PIN); pmipup(JOYSTIC_DOWN_PIN); pmipup(JOYSTIC_LEFT_PIN); pmipup(JOYSTIC_START_PIN);
    radio.begin(); radio.openWritingPipe(ADDRESS); radio.setPALevel(RF24_PA_MIN); radio.stopListening();
}

void loop()
{
    struct transferred_command_datatype data = { !dr(JOYSTIC_UP_PIN), !dr(JOYSTIC_RIGHT_PIN), !dr(JOYSTIC_DOWN_PIN), !dr(JOYSTIC_LEFT_PIN), !dr(JOYSTIC_START_PIN) };
    Serial.println((data.forward_movement << 4) + (data.right_movement << 3) + (data.backward_movement << 2) + (data.left_movement << 1) + data.treasure_position_up, BIN);
    radio.write(&data, sizeof(data));
    delay(RF_TRANSFER_DELAY);
}
#endif

#ifdef RECEIVER
void setup()
{
    Serial.begin(9600);
    driver_motor.setup(DRIVER_MOTOR_PIN1, DRIVER_MOTOR_PIN2);
    treasure_servo.attach(TREASURE_SERVO_PIN);
    direction_servo.attach(DIRECTION_SERVO_PIN);
    radio.begin(); radio.openReadingPipe(0, ADDRESS); radio.setPALevel(RF24_PA_MIN); radio.startListening();
}

void loop()
{
    if (!radio.available()) return;
    
    struct transferred_command_datatype data;
    radio.read(&data, sizeof(data));
    Serial.println((data.forward_movement << 4) + (data.right_movement << 3) + (data.backward_movement << 2) + (data.left_movement << 1) + data.treasure_position_up, BIN);
    
    if (data.backward_movement) driver_motor.moveBackward();
    if (data.forward_movement) driver_motor.moveForaward();
    
    direction_servo.write(DIRECTION_SERVO_MIDDLE_POSITION);
    if (data.left_movement) direction_servo.write(DIRECTION_SERVO_LEFT_POSITION);
    if (data.right_movement) direction_servo.write(DIRECTION_SERVO_RIGHT_POSITION);
    
    treasure_servo.write(data.treasure_position_up ? TREASURE_SERVO_OPEN_POSITION : TREASURE_SERVO_CLOSED_POSITION);
    
    delay(RF_RECIVER_DELAY);
    driver_motor.stop();
    direction_servo.write(DIRECTION_SERVO_MIDDLE_POSITION);
}
#endif