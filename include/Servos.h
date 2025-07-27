#ifndef SERVOS_H
#define SERVOS_H

#include <cstdint>

#include "Peripherals.h"

namespace Servos {
    constexpr uint8_t NUM_SERVOS = 4;
    constexpr float PWM_FREQ = 1000.0f;
    static constexpr uint8_t PINS[] = {Peripherals::GPIO::SERVO1_CTL, Peripherals::GPIO::SERVO2_CTL,
                                       Peripherals::GPIO::SERVO3_CTL, Peripherals::GPIO::SERVO4_CTL};

    void setup();
    void setPosition(uint8_t servo, float degrees);
    float getSetpoint(uint8_t servo);
    void handleRCP(uint8_t id, bool r_nw, float wval);
} // namespace Servos

#endif // SERVOS_H
