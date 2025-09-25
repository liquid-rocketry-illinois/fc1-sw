#include "pyros.h"

#include <Arduino.h>
#include "RCP_Target/RCP_Target.h"

#include "Peripherals.h"

namespace Pyros {
    static RCP_SimpleActuatorState pyros[2];

    void setup() {
        pyros[0] = RCP_SIMPLE_ACTUATOR_OFF;
        pyros[1] = RCP_SIMPLE_ACTUATOR_OFF;
        pinMode(Peripherals::GPIO::PYRO1_ARM, OUTPUT);
        pinMode(Peripherals::GPIO::PYRO2_ARM, OUTPUT);
        pinMode(Peripherals::GPIO::PYRO1_FIRE, OUTPUT);
        pinMode(Peripherals::GPIO::PYRO2_FIRE, OUTPUT);
        pinMode(Peripherals::GPIO::PYRO1_PRESENT, INPUT);
        pinMode(Peripherals::GPIO::PYRO2_PRESENT, INPUT);
    }

    void set(Pyro pyro) {
        if(pyro == Pyro::CH1) {
            pyros[0] = RCP_SIMPLE_ACTUATOR_ON;
            digitalWrite(Peripherals::GPIO::PYRO1_ARM, HIGH);
            digitalWrite(Peripherals::GPIO::PYRO1_FIRE, HIGH);
        }

        else if(pyro == Pyro::CH2) {
            pyros[1] = RCP_SIMPLE_ACTUATOR_ON;
            digitalWrite(Peripherals::GPIO::PYRO2_ARM, HIGH);
            digitalWrite(Peripherals::GPIO::PYRO2_FIRE, HIGH);
        }
    }

    void unset(Pyro pyro) {
        if(pyro == Pyro::CH1) {
            pyros[0] = RCP_SIMPLE_ACTUATOR_OFF;
            digitalWrite(Peripherals::GPIO::PYRO1_ARM, LOW);
            digitalWrite(Peripherals::GPIO::PYRO1_FIRE, LOW);
        }

        else if(pyro == Pyro::CH2) {
            pyros[1] = RCP_SIMPLE_ACTUATOR_OFF;
            digitalWrite(Peripherals::GPIO::PYRO2_ARM, LOW);
            digitalWrite(Peripherals::GPIO::PYRO2_FIRE, LOW);
        }
    }
} // namespace Pyros

RCP_SimpleActuatorState RCP::readSimpleActuator(uint8_t id) { return Pyros::pyros[id]; }

RCP_SimpleActuatorState RCP::simpleActuatorWrite_CLBK(uint8_t id, RCP_SimpleActuatorState state) {
    RCP_SimpleActuatorState finalstate;

    if(state == RCP_SIMPLE_ACTUATOR_TOGGLE)
        finalstate =
            readSimpleActuator(id) == RCP_SIMPLE_ACTUATOR_ON ? RCP_SIMPLE_ACTUATOR_OFF : RCP_SIMPLE_ACTUATOR_ON;

    else finalstate = state;

    if(finalstate == RCP_SIMPLE_ACTUATOR_ON) Pyros::set(id == 0 ? Pyros::Pyro::CH1 : Pyros::Pyro::CH2);
    else Pyros::unset(id == 0 ? Pyros::Pyro::CH1 : Pyros::Pyro::CH2);
    return readSimpleActuator(id);
}
