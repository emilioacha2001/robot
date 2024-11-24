#ifndef MOTOR_HPP
#define MOTOR_HPP
#include <wiringPi.h>
#include <thread>
#include <chrono>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>

class Motor {
private:
    int enable, IN1, IN2;
    int encoder1, encoder2;
    bool hasEncoder;
    int lastState;
    bool pauseThread;
    std::vector<double> coefDeltaSpeed, coefSpeedDelta;
    std::thread pulseThread;

    void setupMotor(int ENA, int IN1, int IN2);
    void setupEncoder(int encoder1);
    void pulseWatcher();
public:
    int pulseCount;
    int pulsesPerSecond;
    float encoderResolution = 341.2;
    float gearRatio = 1/34;
    float diameter = 0.1;
    float wheelCircumference = diameter * M_PI;
    float travelDistance;
    Motor(int ENA, int IN1, int IN2);
    Motor(int ENA, int IN1, int IN2, int encoder1);
    Motor(int ENA, int IN1, int IN2, int encoder1, string filename);
    void setSpeed(int speed);
    int getPulseCount();
    int getPulsesPerSecond();
    float getTravelDistance();
    void resetPulseCount();
    void resetTravelDistance();
    double getPWMFromSpeed(double speed, bool isRadians = true);
    std::vector<double> coefFile(std::string filename);
    ~Motor();
};

#endif // MOTOR_HPP