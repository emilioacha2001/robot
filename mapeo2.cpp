#include "include/Motor.hpp"
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <fstream>
#include <chrono>
#include <vector>
#include <sciplot/sciplot.hpp>

using namespace std;
using namespace sciplot;


int main() {
    Motor motor1(7, 35, 37, 22);
    Motor motor2(32, 38, 36, 26);
    motor1.setSpeed(0); motor2.setSpeed(0);
    auto currentTime = chrono::high_resolution_clock::now();

    // Mapeo 2
    int maxSpeed = 1024;
    int maxPulses1 = 653;
    int maxPulses2 = 667;
    ofstream mapeo2("graphs/mapeo2.txt");

    long double speed1 = 0;
    long double speed2 = 0;
    chrono::duration<float> duration1;
    chrono::duration<float> duration2;
    chrono::duration<float> totalDuration;
    bool reached1 = false; bool reached2 = false;

    motor1.setSpeed(maxSpeed); motor2.setSpeed(maxSpeed);
    int currentPulses1 = motor1.getPulseCount();
    int currentPulses2 = motor2.getPulseCount();
    while (!reached1 and !reached2) {
        speed1 = motor1.getPulseCount() - currentPulses1;
        speed2 = motor2.getPulseCount() - currentPulses2;

        if (speed1 >= maxPulses1) {
            reached1 = true;
            duration1 = currentTime - chrono::high_resolution_clock::now();
        }
        if (speed2 >= maxPulses2) {
            reached2 = true;
            duration2 = currentTime - chrono::high_resolution_clock::now();
        }

        totalDuration = chrono::high_resolution_clock::now() - currentTime;
        mapeo2 << totalDuration.count() << ", " << speed1 << ", " << speed2 << endl;
     }

    cout << "time1: " << duration1.count() << " time2: " << duration2.count();

    return 0;
}

// CompileOptions: -Iinclude src/Motor.cpp -std=c++17 -lwiringPi
