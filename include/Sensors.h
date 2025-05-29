#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#include "MS5611.h"
#include "LIS2MDL.h"
#include "SensirionI2cSht4x.h"

namespace Sensors {
    enum class SensorStatus {
        RESET = 0,
        READY,
        INIT_FAIL,
        DATA_READY,
        DATA_FAIL,
    };

    namespace Ambient {
        constexpr uint8_t BARO_ADDR = 0x77;

        struct AmbientData {
            float pressure;
            float humidity;
            float temperature;
        };

        extern MS5611 baro;
        extern SensirionI2cSht4x sht;

        extern SensorStatus baroStatus;
        extern SensorStatus shtStatus;

        void setup();
        void getData(AmbientData& data);
    }

    namespace Mag {
        typedef float MagData[3];

        extern LIS2MDL mag;
        extern SensorStatus magStatus;

        void setup();
        void getData(MagData& data);
    }

}

#endif //SENSORS_H
