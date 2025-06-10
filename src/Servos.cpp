#include <Arduino.h>

#include "peripherals.h"
#include "Servos.h"

namespace Servos {
    Servo S1;
    Servo S2;
    Servo S3;
    Servo S4;

    void setup() {
        S1.attach(Peripherals::GPIO::SERVO1_CTL);
        S2.attach(Peripherals::GPIO::SERVO2_CTL);
        S3.attach(Peripherals::GPIO::SERVO3_CTL);
        S4.attach(Peripherals::GPIO::SERVO4_CTL);
    }

    void setPosition(Servo& servo, int degrees) {
        servo.write(degrees);
    }
}