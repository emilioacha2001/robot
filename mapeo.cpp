#include "include/Motor.hpp"
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>

using namespace std;

int main() {
    Motor motor1(7, 35, 37, 22);
    Motor motor2(32, 38, 36, 26);

    // Mapeo 1
    int maxSpeed = 1000;

    for (int speed = 0; speed <= maxSpeed; speed++) {
        cout << "Pulses 1: " << motor1.getPulseCount() << " Pulses 2: " << motor2.getPulseCount() << endl;
        motor1.setSpeed(speed);
        motor2.setSpeed(speed);
    }

    // sleep(3);

    // Mapeo 2
    // Mapeo 3

    return 0;
}

// CompileOptions: -Iinclude src/Motor.cpp -std=c++17 -lwiringPi