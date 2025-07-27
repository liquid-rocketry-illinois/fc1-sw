#include "Servos.h"

#include <Arduino.h>
#include <SAMD_PWM.h>

#include "Peripherals.h"
#include "RCP.h"

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

    void handleRCP(uint8_t id, bool r_nw, float wval) {
        if(!r_nw) {
            setPosition(id, wval);
        }

        RCP::sendOneFloat(RCP_DEVCLASS_ANGLED_ACTUATOR, id, values + id);
    }

} // namespace Servos
