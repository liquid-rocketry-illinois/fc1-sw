#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#include "MS5611.h"
#include "LIS2MDL.h"

namespace Sensors {
    enum class SensorStatus {
        RESET = 0,
        READY,
        INIT_FAIL,
        DATA_READY,
        DATA_FAIL,
    };

    namespace Baro {
        constexpr uint8_t BARO_ADDR = 0x77;

        typedef float BaroData;

        extern MS5611 baro;
        extern SensorStatus baroStatus;

        void setup();
        void getData(BaroData& data);
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
