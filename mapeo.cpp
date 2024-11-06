#include "include/Motor.hpp"
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>

#define MAX_MOTORS 2
Motor* motors[MAX_MOTORS] = {nullptr, nullptr};

void encoderInterrupt(int index) {
    if (motors[index] != nullptr) {
        motors[index]->handleInterrupt();
    }
}

void encoderInterrupt0() {
    encoderInterrupt(0);
}

int main() {
    Motor motor1(7, 35, 37, 22, 24 ,encoderInterrupt0);
    motors[0] = &motor1;
    Motor motor2(32, 38, 36);
    motors[1] = &motor2;

    // Mapeo 1
    int maxSpeed = 1000;

    for (int speed = 0; speed <= maxSpeed; speed++) {
        motor1.setSpeed(speed);
        motor2.setSpeed(speed);
    }

    // sleep(3);

    // Mapeo 2
    // Mapeo 3

    return 0;
}

// CompileOptions: -Iinclude src/Motor.cpp -std=c++17 -lwiringPi