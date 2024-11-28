#ifndef BMI160_HPP
#define BMI160_HPP
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <chrono>
#include <thread>

#define BMI160_CMD_REG 0x7E
#define BMI160_ACC_CONF 0x40
#define BMI160_ACC_RANGE 0x41
#define BMI160_GYR_CONF 0x42
#define BMI160_GYR_RANGE 0x43
#define BMI160_ACC_DATA 0x12
#define BMI160_GYR_DATA 0x0C

class BMI160 {
private:
    uint8_t address;
    int file;
    std::thread sensorThread;

    bool initializeBMI160(int file);
    bool writeRegister(uint8_t reg, uint8_t value);
    void updateSensorData();
public:
    int16_t accData[3] = {0};
    int16_t gyrData[3] = {0};
    BMI160(const char* file_name, uint8_t addr);   
    ~BMI160();
};

#endif // BMI160_HPP