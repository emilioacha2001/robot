#include "../include/BMI160.hpp"

BMI160::BMI160(const char* file_name, uint8_t addr) {
    this->file = open(file_name, O_RDWR);
    this->address = addr;
    if (file < 0) {
        std::cerr << "Error abriendo " << file_name << std::endl;
        return;
    }

    if (ioctl(this->file, I2C_SLAVE, this->address) < 0) {
        std::cerr << "Error al establecer la dirección I2C." << std::endl;
        close(this->file);
        return;
    }

    if (!initializeBMI160(this->file)) {
        std::cerr << "Error al inicializar el BMI160." << std::endl;
        close(this->file);
        return;
    }

    sensorThread = std::thread(&BMI160::updateSensorData, this);
}

BMI160::~BMI160() {
    close(this->file);
    sensorThread.join();
}

bool BMI160::writeRegister(uint8_t reg, uint8_t value) {
    char config[2] = {static_cast<char>(reg), static_cast<char>(value)};
    return write(this->file, config, 2) == 2;
}

bool BMI160::initializeBMI160(int file) {
    if (!writeRegister(BMI160_CMD_REG, 0x11)) return false;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    if (!writeRegister(BMI160_ACC_CONF, 0x28)) return false;
    if (!writeRegister(BMI160_ACC_RANGE, 0x03)) return false;

    if (!writeRegister(BMI160_CMD_REG, 0x15)) return false;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    if (!writeRegister(BMI160_GYR_CONF, 0x28)) return false;
    if (!writeRegister(BMI160_GYR_RANGE, 0x00)) return false;

    return true;
}

void BMI160::updateSensorData() {  
    uint8_t accelPointer = BMI160_ACC_DATA;
    uint8_t gyroPointer = BMI160_GYR_DATA;

    char bufAcc[6] = {0};
    char bufGyro[6] = {0};

    while (true) {
        if (write(this->file, &accelPointer, 1) != 1 || read(this->file, bufAcc, 6) != 6) {
            std::cerr << "Error leyendo datos del acelerómetro." << std::endl;
            return;
        }   

        if (write(this->file, &gyroPointer, 1) != 1 || read(this->file, bufGyro, 6) != 6) {
            std::cerr << "Error leyendo datos del giroscopio." << std::endl;
            return;
        }

        for (int i = 0; i < 3; ++i) {
            this->accData[i] = static_cast<int16_t>((bufAcc[2 * i + 1] << 8) | bufAcc[2 * i]);
            this->gyrData[i] = static_cast<int16_t>((bufGyro[2 * i + 1] << 8) | bufGyro[2 * i]);
        }
        // std::cout << "Accel: " << this->accData[0] << " " << this->accData[1] << " " << this->accData[2] << std::endl;
        // std::cout << "Gyro: " << this->gyrData[0] << " " << this->gyrData[1] << " " << this->gyrData[2] << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}