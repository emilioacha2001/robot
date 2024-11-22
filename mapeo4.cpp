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
    motor1.setSpeed(0); motor2.setSpeed(0);
    auto start = chrono::high_resolution_clock::now();
    auto currentTime = chrono::high_resolution_clock::now();
    chrono::duration<float> sampleTime(2);
    ofstream mapeo("graphs/mapeo3_simple.txt");

    // Mapeo 1
    int speeds[9] = {620, 670, 720, 770, 820, 870, 920, 970, 1024};

    for (int i = 0; i < 9; i++) {
        motor1.setSpeed(speeds[i]);
        motor2.setSpeed(speeds[i]);

        currentTime = chrono::high_resolution_clock::now();
        chrono::duration<float> duration = chrono::high_resolution_clock::now() - currentTime;
        int currentPulses1 = motor1.getPulseCount();
        int currentPulses2 = motor2.getPulseCount();
        long double samplePulses1 = 0;
        long double samplePulses2 = 0;
        int sampleCount = 0;
        while (duration <= sampleTime) {
            samplePulses1 += motor1.getPulseCount() - currentPulses1;
            samplePulses2 += motor2.getPulseCount() - currentPulses2;

            duration = chrono::high_resolution_clock::now() - currentTime;
            sampleCount++;
        }

        long double average1 = samplePulses1 / sampleCount;
        long double average2 = samplePulses2 / sampleCount;

        cerr << "Speed: " << speeds[i] << " Average1: " << average1/2 << " Average2: " << average2/2 << " Time: " << (currentTime - start).count() << endl;
        mapeo << speeds[i] << ", " << average1/2 << ", " << average2/2 << ", " << (currentTime - start).count() << endl;

        motor1.setSpeed(0);
        motor2.setSpeed(0);
        usleep(10000000/2);
    }

    motor1.setSpeed(0);
    motor2.setSpeed(0);

    return 0;
}

// CompileOptions: -Iinclude src/Motor.cpp -std=c++17 -lwiringPi