#include "Util.h"

#ifndef MOTOR
#define MOTOR

class Motor
{
    private:
        int pin1;
        int pin2;
        int speedPin = -1;

    public:
        void setup(int, int);
        void setup(int, int, int);
        void moveForaward();
        void moveBackward();
        void stop();
        void setSpeed(int speed);
};

class MotorController
{
    private:
        int nominal_speed = 255;
    public:
        Motor right_motor;
        Motor left_motor;
        void setNominalSpeed(int);
        void moveForward();
        void turnRight();
        void turnSlightlyRight();
        void turnLeft();
        void turnSlightlyLeft();
        void stop();
        void regulate(int);
};

void Motor::setup(int pin1, int pin2)
{
    this -> pin1 = pin1;
    this -> pin2 = pin2;

    pmo(pin1);
    pmo(pin2);
}

void Motor::setup(int pin1, int pin2, int speedPin)
{
    setup(pin1, pin2);
    this -> speedPin = speedPin;
    pmo(speedPin);
}

void Motor::moveForaward()
{
    dwh(pin1);
    dwl(pin2);
}

void Motor::moveBackward()
{
    dwl(pin1);
    dwh(pin2);
}

void Motor::stop()
{
    dwl(pin1);
    dwl(pin2);
}

void Motor::setSpeed(int speed)
{
    if (speed >= 0)
    {  
        moveForaward();
        if (speedPin != -1) aw(speedPin, speed < 255 ? speed : 255);
    }
    else 
    {
        moveBackward();
        if (speedPin != -1) aw(speedPin, -speed < 255 ? -speed : 255);
    }
}

void MotorController::setNominalSpeed(int nominal_speed)
{
    this -> nominal_speed = nominal_speed;
}

void MotorController::moveForward()
{
    right_motor.setSpeed(nominal_speed);
    left_motor.setSpeed(nominal_speed);
}

void MotorController::turnRight()
{
    right_motor.setSpeed(-nominal_speed);
    left_motor.setSpeed(nominal_speed);
}

void MotorController::turnSlightlyRight()
{
    right_motor.stop();
    left_motor.setSpeed(nominal_speed);
}

void MotorController::turnLeft()
{
    right_motor.setSpeed(nominal_speed);
    left_motor.setSpeed(-nominal_speed);
}

void MotorController::turnSlightlyLeft()
{
    right_motor.setSpeed(nominal_speed);
    left_motor.stop();
}

void MotorController::regulate(int pid)
{
    right_motor.setSpeed(nominal_speed - pid);
    left_motor.setSpeed(nominal_speed + pid);
}

#endif
