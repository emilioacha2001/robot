#include "include/Motor.hpp"
#include "include/BMI160.hpp"
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>

using namespace std;
#define IMU_ADDR 0x69

int main() {
    Motor motor1(7, 35, 37, 22);
    Motor motor2(32, 38, 36, 26);
    BMI160 imu("/dev/i2c-2", IMU_ADDR);

    motor1.setSpeed(-1024);
    motor2.setSpeed(-1024);

    // while (true) {
    //     motor1.setSpeed(700);
    //     motor2.setSpeed(700);

    //     cout << "Pulses1: " << motor1.getPulseCount() << " Pulses2: " << motor2.getPulseCount() << endl;
    //     cout << "Speed1: " << motor1.getPulsesPerSecond() << " Speed2: " << motor2.getPulsesPerSecond() << endl;
    // }
    sleep(5);
    return 0;
}

// CompileOptions: -Iinclude src/Motor.cpp src/BMI160.cpp -std=c++17 -lwiringPi