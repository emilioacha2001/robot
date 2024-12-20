#include "../include/Motor.hpp"

Motor::Motor(int enable, int IN1, int IN2) {
    wiringPiSetupPhys();
    this->hasEncoder = false;
    Motor::setupMotor(enable, IN1, IN2);
}

Motor::Motor(int enable, int IN1, int IN2, int encoder1) {
    wiringPiSetupPhys();
    this->hasEncoder = true;
    Motor::setupMotor(enable, IN1, IN2);
    Motor::setupEncoder(encoder1);

    pulseThread = std::thread(&Motor::pulseWatcher, this);
}

Motor::~Motor() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    pwmWrite(enable, 0);

    if (hasEncoder) {
        digitalWrite(encoder1, LOW);
        digitalWrite(encoder2, LOW);

        pauseThread = true;
        pulseThread.join();
    }
}

void Motor::setSpeed(int speed) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    if (speed > 1024) {
        speed = 1024;
    } else if (speed == 0) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
    } else if (speed < 0) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        speed = speed * -1;
    }
    
    pwmWrite(enable, speed);
}

void Motor::setupMotor(int enable, int IN1, int IN2) {
    this->enable = enable;
    this->IN1 = IN1;
    this->IN2 = IN2;

    pinMode(enable,PWM_OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
}

void Motor::setupEncoder(int encoder1) {
    this->encoder1 = encoder1;
    this->pulseCount = 0;

    pinMode(encoder1, INPUT);
}

void Motor::pulseWatcher() {
    std::chrono::duration<float> sampleTime(1);
    auto start = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    int pulses = 0;
    int lastState = digitalRead(encoder1);

    while (!pauseThread) {
        int currentState = digitalRead(encoder1);
        if (currentState != lastState && currentState == 1) {
            this->pulseCount++;
            pulses++;
        }
        lastState = currentState;
        delayMicroseconds(10);

        currentTime = std::chrono::high_resolution_clock::now();
        if (currentTime - start >= sampleTime) {
            this->pulsesPerSecond = pulses;
            pulses = 0;
            start = currentTime;
        }

        this->travelDistance = (this->pulseCount / encoderResolution) * wheelCircumference;
    }
}

vector<double> Motor::coefFile(string filename) {
    std::vector<double> coef;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: File not found" << std::endl;
        return coef;
    }

    std::string line;
    while (getline(file, line)) coef.push_back(stol(line));
    file.close();
    
    return coef;
}

double Motor::getPWMFromSpeed(double speed, bool isRadians = true) {
    if (isRadians) {
        speed = speed * this->gearRatio * this->encoderResolution / (2 * M_PI);
    }

    double pwm = this->coefDeltaSpeed[0];
    for (int i = 1; i < this->coefSpeedDelta.size(); i++) {
        pwm += this->coefDeltaSpeed[i] * pow(speed, i);
    }
    
    return pwm;
}

int Motor::getPulseCount() {
    return pulseCount;
}

int Motor::getPulsesPerSecond() {
    return pulsesPerSecond;
}

float Motor::getTravelDistance() {
    return travelDistance;
}

void Motor::resetTravelDistance() {
    this->travelDistance = 0;
}

void Motor::resetPulseCount() {
    this->pulseCount = 0;
}