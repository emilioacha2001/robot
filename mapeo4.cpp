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

    // Mapeo 1
    int maxSpeed = 1024;
    int maxPulses1 = 0;
    int maxPulses2 = 0;
    ofstream mapeo1("graphs/mapeo1_carga.txt");
    chrono::duration<float> sampleTime(1);
    bool negativeIteration = true;

    for (long speed = 0; speed <= maxSpeed; speed++) {
        if (speed < 600) {
            if (negativeIteration) mapeo1 << -speed << ", 0, 0" << endl;
            else mapeo1 << speed << ", 0, 0" << endl;
            continue;
        };

        if (negativeIteration) {
            motor1.setSpeed(-speed);
            motor2.setSpeed(-speed);
        } else {
            motor1.setSpeed(speed);
            motor2.setSpeed(speed);

        }

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

        if (average1 >= maxPulses1) maxPulses1 = average1;
        if (average2 >= maxPulses2) maxPulses2 = average2;

        if (negativeIteration) {
            cerr << " Speed: " << -speed << " Average1: " << average1 << " Average2: " << average2 << " Samples: " << sampleCount << endl;
            mapeo1 << -speed << ", " << -average1 << ", " << -average2 << endl;
        } else {
            cerr << " Speed: " << speed << " Average1: " << average1 << " Average2: " << average2 << " Samples: " << sampleCount << endl;
            mapeo1 << speed << ", " << average1 << ", " << average2 << endl;
        }

        if (speed == maxSpeed && negativeIteration) {
            speed = 0;
            negativeIteration = false;
        }
    }
    mapeo1.close();

    vector<std::tuple<double, double, double>> data;
    std::ifstream inputFile("graphs/mapeo1_carga.txt");
    double speed, average1, average2;
    std::string line;

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string value;
        double speed, average1, average2;

        // Parse the comma-separated values
        std::getline(ss, value, ',');
        speed = std::stod(value);
        std::getline(ss, value, ',');
        average1 = std::stod(value);
        std::getline(ss, value, ',');
        average2 = std::stod(value);

        data.emplace_back(speed, average1, average2);
    }
    inputFile.close();

    std::sort(data.begin(), data.end(), [](auto &a, auto &b) {
        return std::get<0>(a) < std::get<0>(b);
    });

    ofstream mapeo1Sorted("graphs/mapeo1_carga.txt");
    for (auto &row : data) {
        mapeo1Sorted << std::get<0>(row) << ", " << std::get<1>(row) << ", " << std::get<2>(row) << endl;
    }
    mapeo1Sorted.close();

    return 0;
}

// CompileOptions: -Iinclude src/Motor.cpp -std=c++17 -lwiringPi