#include "include/Motor.hpp"
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <fstream>
#include <chrono>

using namespace std;

int main() {
    Motor motor1(7, 35, 37, 22);
    Motor motor2(32, 38, 36, 26);
    auto start = chrono::high_resolution_clock::now();
    auto current_time = chrono::high_resolution_clock::now();

    // Mapeo 1
    int maxSpeed = 1000;
    ofstream mapeo1("graphs/mapeo1.txt");

    int pulsesPerCycle1 = 0;
    int pulsesPerCycle2 = 0;
    int previousPulses1 = 0;
    int previousPulses2 = 0;
    int sumPulses1 = 0;
    int sumPulses2 = 0;
    float averagePulses1 = 0;
    float averagePulses2 = 0;

    for (int speed = 0; speed <= maxSpeed; speed++) {
        pulsesPerCycle1 = motor1.getPulseCount() - previousPulses1;
        pulsesPerCycle2 = motor2.getPulseCount() - previousPulses2;
        sumPulses1 += pulsesPerCycle1;
        sumPulses2 += pulsesPerCycle2;
        motor1.setSpeed(speed);
        motor2.setSpeed(speed);
        previousPulses1 = motor1.getPulseCount();
        previousPulses2 = motor2.getPulseCount();
        averagePulses1 = sumPulses1 / speed;
        averagePulses2 = sumPulses2 / speed;
        current_time = chrono::high_resolution_clock::now();
        chrono::duration<float> elapsed_seconds = current_time - start;
        cout << "Speed: " << speed << " Pulses 1: " << pulsesPerCycle1 << " Pulses 2: " << pulsesPerCycle2 << endl;
        mapeo1 << elapsed_seconds.count() << ", " << averagePulses1 << ", " << averagePulses2 << endl;
        usleep(100000);
    }

    mapeo1.close();

    // Mapeo 2
    // Mapeo 3

    return 0;
}

// CompileOptions: -Iinclude src/Motor.cpp -std=c++17 -lwiringPi