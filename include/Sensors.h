#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#include "MS5611.h"
#include "LIS2MDL.h"
#include "SensirionI2cSht4x.h"
#include "ICM42688.h"
#include "BMI088.h"

namespace Sensors {
    enum class SensorStatus {
        RESET = 0,
        READY,
        INIT_FAIL,
        DATA_READY,
        DATA_FAIL,
    };

    struct AxisData {
        float x;
        float y;
        float z;
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
        extern LIS2MDL mag;
        extern SensorStatus magStatus;

        void setup();
        void getData(AxisData& data);
    }

    namespace IMU {
        constexpr uint8_t ICM_ADDR = 0x69;
        constexpr uint8_t BMI_ACCEL_ADDR = 0x18;
        constexpr uint8_t BMI_GYRO_ADDR = 0x68;

        struct IMUData {
            AxisData accel;
            AxisData gyro;
        };

        extern ICM42688 icm;
        extern Bmi088 bmi;

        extern SensorStatus icmStatus;
        extern SensorStatus bmiStatus;

        void setup();
        void getData(IMUData& icmData, IMUData& bmiData);
    }

}

#endif //SENSORS_H
