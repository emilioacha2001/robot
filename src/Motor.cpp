#include "../include/Motor.hpp"

Motor::Motor(int enable, int IN1, int IN2) {
    wiringPiSetupPhys();
    this->hasEncoder = false;
    Motor::setupMotor(enable, IN1, IN2);
}

Motor::Motor(int enable, int IN1, int IN2, int encoder1, int encoder2, EncoderInterruptHandler handler) {
    wiringPiSetupPhys();
    this->hasEncoder = true;
    Motor::setupMotor(enable, IN1, IN2);
    Motor::setupEncoder(encoder1, encoder2, handler);
}

Motor::~Motor() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    pwmWrite(enable, 0);

    if (hasEncoder) {
        digitalWrite(encoder1, LOW);
        digitalWrite(encoder2, LOW);
    }
}

void Motor::setSpeed(int speed) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    if (speed > 1000) {
        pwmWrite(enable, 1000);
    } else if (speed <= 0) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
    } else {
        pwmWrite(enable, speed);
    }
}

void Motor::setupMotor(int enable, int IN1, int IN2) {
    this->enable = enable;
    this->IN1 = IN1;
    this->IN2 = IN2;

    pinMode(enable,PWM_OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
}

void Motor::setupEncoder(int encoder1, int encoder2, EncoderInterruptHandler handler) {
    this->encoder1 = encoder1;
    this->encoder2 = encoder2;
    this->pulseCount = 0;

    pinMode(encoder1, INPUT);
    pinMode(encoder2, INPUT);

    pullUpDnControl(encoder1, PUD_UP);
    pullUpDnControl(encoder2, PUD_UP);

    wiringPiISR(encoder1, INT_EDGE_BOTH, handler);
}

void Motor::handleInterrupt() {
    bool state = digitalRead(encoder1);

    if (state != lastState) {
        pulseCount++;
        lastState = state;
    }
}

int Motor::getPulseCount() {
    return pulseCount;
}