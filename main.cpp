#include "include/Motor.hpp"
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>

int main() {
    Motor motor1(7, 35, 37);
    Motor motor2(32, 38, 36);

    motor1.setSpeed(-1024);
    motor2.setSpeed(-1024);

    sleep(3);
    return 0;
}

// CompileOptions: -Iinclude src/Motor.cpp -std=c++17 -lwiringPi