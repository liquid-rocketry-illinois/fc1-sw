#include <Arduino.h>
#include <Servo.h>

#include "RCP.h"
#include "peripherals.h"
#include "Servos.h"

namespace Servos {
    static Servo servos[NUM_SERVOS];
    static float values[NUM_SERVOS];

    void setup() {
        servos[0].attach(Peripherals::GPIO::SERVO1_CTL);
        servos[1].attach(Peripherals::GPIO::SERVO2_CTL);
        servos[2].attach(Peripherals::GPIO::SERVO3_CTL);
        servos[3].attach(Peripherals::GPIO::SERVO4_CTL);
    }

    void setPosition(uint8_t servo, float degrees) {
        if(servo >= NUM_SERVOS) return;
        servos[servo].write(static_cast<int>(degrees));
        values[servo] = degrees;
    }

    float getSetpoint(uint8_t servo) {
        if(servo >= NUM_SERVOS) return 0;
        return values[servo];
    }

    void handleRCP(uint8_t id, bool r_nw, float wval) {
        if(!r_nw) {
            setPosition(id, wval);
        }

        RCP::sendOneFloat(RCP_DEVCLASS_ANGLED_ACTUATOR, id, values + id);
    }

}