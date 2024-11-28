#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <iostream>
#include <chrono>
#include <thread>

// Function to read a 16-bit value from a specified register
int read16Bit(int file, int reg) {
    char data[2];
    if (write(file, &reg, 1) != 1) {
        std::cerr << "Failed to set register address." << std::endl;
        return -1;
    }

    if (read(file, data, 2) != 2) {
        std::cerr << "Failed to read data." << std::endl;
        return -1;
    }

    int value = (data[0] << 8) | data[1];
    if (value > 32767) value -= 65536;  // Convert to signed value
    return value;
}

// Function to initialize the MPU6050 and return the file descriptor
int initializeMPU6050(const char *filename) {
    int file = open(filename, O_RDWR);
    if (file < 0) {
        std::cerr << "Failed to open the i2c bus." << std::endl;
        return -1;
    }

    if (ioctl(file, I2C_SLAVE, 0x68) < 0) {
        std::cerr << "Failed to connect to the device." << std::endl;
        return -1;
    }

    // Wake up the MPU6050 by writing to the power management register (0x6B)
    char wakeUp = 0x6B;  // Power management register
    char wakeUpValue = 0x00;  // Wake up the device
    if (write(file, &wakeUp, 1) != 1 || write(file, &wakeUpValue, 1) != 1) {
        std::cerr << "Failed to wake up MPU6050." << std::endl;
        return -1;
    }

    return file;
}

// Function to read all 3 axes from the accelerometer
void readAccelerometerData(int file) {
    int accelX = read16Bit(file, 0x3B);  // Register 0x3B is for X axis
    int accelY = read16Bit(file, 0x3D);  // Register 0x3D is for Y axis
    int accelZ = read16Bit(file, 0x3F);  // Register 0x3F is for Z axis

    if (accelX == -1 || accelY == -1 || accelZ == -1) {
        std::cerr << "Failed to read accelerometer data." << std::endl;
        return;
    }

    // Convert to g's (assuming ±2g range)
    float accelX_in_g = (float)accelX / 16384.0;
    float accelY_in_g = (float)accelY / 16384.0;
    float accelZ_in_g = (float)accelZ / 16384.0;

    // Output the values
    std::cout << "Accel X in g's: " << accelX_in_g << ", ";
    std::cout << "Accel Y in g's: " << accelY_in_g << ", ";
    std::cout << "Accel Z in g's: " << accelZ_in_g << std::endl;
}

int main() {
    const char *filename = "/dev/i2c-2";  // Adjust to your I2C bus
    int file = initializeMPU6050(filename);
    if (file < 0) {
        return -1;
    }

    // Continuously read accelerometer data in a loop
    while (true) {
        readAccelerometerData(file);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Delay for 500ms
    }

    close(file);  // Close the I2C bus when done
    return 0;
}
