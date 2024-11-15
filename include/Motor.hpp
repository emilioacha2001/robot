#ifndef MOTOR_HPP
#define MOTOR_HPP
#include <wiringPi.h>
#include <thread>
#include <chrono>
#include <iostream>

class Motor {
private:
    int enable, IN1, IN2;
    int encoder1, encoder2;
    bool hasEncoder;
    int lastState;
    int pulseCount;
    int pulsesPerSecond;
    bool pauseThread;
    std::thread pulseThread;

    void setupMotor(int ENA, int IN1, int IN2);
    void setupEncoder(int encoder1);
    void pulseWatcher();
public:
    Motor(int ENA, int IN1, int IN2);
    Motor(int ENA, int IN1, int IN2, int encoder1);
    void setSpeed(int speed);
    int getPulseCount();
    int getPulsesPerSecond();
    void resetPulseCount();
    ~Motor();
};

#endif // MOTOR_HPP