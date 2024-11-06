#ifndef MOTOR_HPP
#define MOTOR_HPP
#include <wiringPi.h>

typedef void (*EncoderInterruptHandler)();

class Motor {
private:
    int enable, IN1, IN2;
    int encoder1, encoder2;
    bool hasEncoder;
    bool lastState;
    int pulseCount;
    EncoderInterruptHandler encoderHandler;

    void setupMotor(int ENA, int IN1, int IN2);
    void setupEncoder(int encoder1, int encoder2, EncoderInterruptHandler handler);
public:
    Motor(int ENA, int IN1, int IN2);
    Motor(int ENA, int IN1, int IN2, int encoder1, int encoder2, EncoderInterruptHandler handler);
    void setSpeed(int speed);
    int getPulseCount();
    void handleInterrupt(); 
    ~Motor();
};

#endif // MOTOR_HPP