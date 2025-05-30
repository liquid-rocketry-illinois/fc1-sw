#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#include "MS5611.h"
#include "LIS2MDL.h"
#include "SensirionI2cSht4x.h"
#include "ICM42688.h"
#include "BMI088.h"
#include "SparkFun_u-blox_GNSS_v3.h"

namespace Sensors {
    enum class SensorStatus {
        RESET = 0,
        READY,
        INIT_FAIL,
        DATA_READY,
        DATA_FAIL,
    };

    // This "packed" attribute ensures all members are back to back in memory, so when they are serialized
    // we dont have to worry about alignment or padding. The static asserts make sure the packed size of
    // the structs is correct
    __attribute__ ((packed)) struct AxisData {
        float x;
        float y;
        float z;
    };

    static_assert(sizeof(AxisData) == 12);

    namespace Ambient {
        constexpr uint8_t BARO_ADDR = 0x77;

        __attribute__ ((packed)) struct AmbientData {
            float pressure;
            float humidity;
            float temperature;
        };

        static_assert(sizeof(AmbientData) == 12);

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

        __attribute__ ((packed)) struct IMUData {
            AxisData accel;
            AxisData gyro;
        };

        static_assert(sizeof(IMUData) == 24);

        extern ICM42688 icm;
        extern Bmi088 bmi;

        extern SensorStatus icmStatus;
        extern SensorStatus bmiStatus;

        void setup();
        void getData(IMUData& icmData, IMUData& bmiData);
    }

    namespace GNSS {
        constexpr uint8_t GNSS_ADDR = 0x42;

        __attribute__ ((packed)) struct GNSSData {
            float lat;
            float lon;
            float alt;
            float gs;
        };

        static_assert(sizeof(GNSSData) == 16);

        extern SFE_UBLOX_GNSS gnss;
        extern SensorStatus gnssStatus;

        void setup();
        void getData(GNSSData& data);
    }

    __attribute__ ((packed)) struct SensorData {
        uint32_t timestamp;
        Ambient::AmbientData ambientData;
        AxisData magData;
        IMU::IMUData icmData;
        IMU::IMUData bmiData;
        GNSS::GNSSData gnssData;
    };

    static_assert(sizeof(SensorData) == 92);

    extern SensorData latestReadings;

    void yield();
}

#endif //SENSORS_H
