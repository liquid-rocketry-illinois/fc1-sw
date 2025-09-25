/*
 * Unfortunately, the PWM code that ships with the arduino framework does not work properly. However,
 * some kind soul has written a library specifically for using PWM on the SAMD/Ex mcus, hence why
 * that is used instead of the arduino Servo library and pwm functions.
 *
 * For whatever reason, the implementor of SAMD_PWM made the SAMD_PWM objects not destructable, so
 * we have to use pointers that never get deallocated. Its not necessarily a problem for us since we
 * dont really want to deallocate them anyways, its just a strange choice of implementation.
 *
 */

#include "Servos.h"

#include <Arduino.h>
#include <SAMD_PWM.h>

#include "Peripherals.h"
#include "RCP_Target/RCP_Target.h"

namespace Servos {
    static SAMD_PWM* servos[NUM_SERVOS];
    static float values[NUM_SERVOS];

    void setup() {
        for(uint8_t i = 0; i < NUM_SERVOS; i++) {
            servos[i] = new SAMD_PWM(PINS[i], 0, 0);
        }
    }

    void setPosition(uint8_t servo, float degrees) {
        if(servo >= NUM_SERVOS) return;
        values[servo] = degrees;
        servos[servo]->setPWM(PINS[servo], PWM_FREQ, degrees);
        // TODO: figure out the exact math to turn degrees into duty cycle
    }

    float getSetpoint(uint8_t servo) {
        if(servo >= NUM_SERVOS) return 0;
        return values[servo];
    }

} // namespace Servos

float RCP::readAngledActuator(uint8_t id) {
    return Servos::values[id];
}

float RCP::angledActuatorWrite_CLBK(uint8_t id, float controlVal) {
    Servos::setPosition(id, controlVal);
    return controlVal;
}


