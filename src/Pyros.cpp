#include "pyros.h"

#include <Arduino.h>

#include "peripherals.h"

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

    bool get(Pyro pyro) {
        return digitalRead(pyro == Pyro::CH1 ?
            Peripherals::GPIO::PYRO1_PRESENT :
            Peripherals::GPIO::PYRO2_PRESENT) == HIGH;
    }
}