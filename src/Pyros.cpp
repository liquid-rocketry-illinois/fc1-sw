#include "pyros.h"

#include <Arduino.h>
#include "RCP_Target/RCP_Target.h"

#include "Peripherals.h"

namespace Pyros {
    void setup() {
        pinMode(Peripherals::GPIO::PYRO1_ARM, OUTPUT);
        pinMode(Peripherals::GPIO::PYRO2_ARM, OUTPUT);
        pinMode(Peripherals::GPIO::PYRO1_FIRE, OUTPUT);
        pinMode(Peripherals::GPIO::PYRO2_FIRE, OUTPUT);
        pinMode(Peripherals::GPIO::PYRO1_PRESENT, INPUT);
        pinMode(Peripherals::GPIO::PYRO2_PRESENT, INPUT);
    }

    void set(Pyro pyro) {
        if(pyro == Pyro::CH1) {
            digitalWrite(Peripherals::GPIO::PYRO1_ARM, HIGH);
            digitalWrite(Peripherals::GPIO::PYRO1_FIRE, HIGH);
        }

        else if(pyro == Pyro::CH2) {
            digitalWrite(Peripherals::GPIO::PYRO2_ARM, HIGH);
            digitalWrite(Peripherals::GPIO::PYRO2_FIRE, HIGH);
        }
    }

    void unset(Pyro pyro) {
        if(pyro == Pyro::CH1) {
            digitalWrite(Peripherals::GPIO::PYRO1_ARM, LOW);
            digitalWrite(Peripherals::GPIO::PYRO1_FIRE, LOW);
        }

        else if(pyro == Pyro::CH2) {
            digitalWrite(Peripherals::GPIO::PYRO2_ARM, LOW);
            digitalWrite(Peripherals::GPIO::PYRO2_FIRE, LOW);
        }
    }
} // namespace Pyros

bool RCP::readSimpleActuator(uint8_t id) {
    return digitalRead(id == 0 ? Peripherals::GPIO::PYRO1_PRESENT : Peripherals::GPIO::PYRO2_PRESENT) == HIGH;
}

bool RCP::writeSimpleActuator(uint8_t id, RCP_SimpleActuatorState state) {
    bool finalstate = state == RCP_SIMPLE_ACTUATOR_TOGGLE ? !readSimpleActuator(id) : state == RCP_SIMPLE_ACTUATOR_ON;
    if(finalstate) Pyros::set(id == 0 ? Pyros::Pyro::CH1 : Pyros::Pyro::CH2);
    else Pyros::unset(id == 0 ? Pyros::Pyro::CH1 : Pyros::Pyro::CH2);
    return readSimpleActuator(id);
}
