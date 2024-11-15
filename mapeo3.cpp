#include "include/Motor.hpp"
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <fstream>
#include <chrono>
#include <vector>
#include <tuple>
#include <algorithm>
#include <sstream>
#include <sciplot/sciplot.hpp>

using namespace std;
using namespace sciplot;

int main() {
    Motor motor1(7, 35, 37, 22);
    Motor motor2(32, 38, 36, 26);
    ofstream file("graphs/mapeo3.txt");

    while (motor1.getTravelDistance() < 1) {
        motor1.setSpeed(1024);
        motor2.setSpeed(1024);

        cout << "Motor 1: " << motor1.travelDistance << "m " << motor1.pulseCount << endl;
        file << motor1.travelDistance << ", " << motor1.pulseCount << endl;
    }

    file.close();

    return 0;
}

// CompileOptions: -Iinclude src/Motor.cpp -std=c++17 -lwiringPi