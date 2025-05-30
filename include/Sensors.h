#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#include "BMI088.h"
#include "ICM42688.h"
#include "LIS2MDL.h"
#include "MS5611.h"
#include "SensirionI2cSht4x.h"
#include "SparkFun_u-blox_GNSS_v3.h"
#include "RCP.h"

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
    struct AxisData {
        float x;
        float y;
        float z;
    };

    static_assert(sizeof(AxisData) == 12);

    namespace Ambient {
        constexpr uint8_t BARO_ADDR = 0x77;

        struct AmbientData {
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
        void tare(RCP_DeviceClass devclass, uint8_t id, uint8_t channel, float tareVal);
    }

    namespace Mag {
        extern LIS2MDL mag;
        extern SensorStatus magStatus;

        void setup();
        void getData(AxisData& data);
        void tare(RCP_DeviceClass devclass, uint8_t id, uint8_t channel, float tareVal);
    }

    namespace IMU {
        constexpr uint8_t ICM_ADDR = 0x69;
        constexpr uint8_t BMI_ACCEL_ADDR = 0x18;
        constexpr uint8_t BMI_GYRO_ADDR = 0x68;

        struct IMUData {
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
        void tare(RCP_DeviceClass devclass, uint8_t id, uint8_t channel, float tareVal);
    }

    namespace GNSS {
        constexpr uint8_t GNSS_ADDR = 0x42;

        struct GNSSData {
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
        void tare(RCP_DeviceClass devclass, uint8_t id, uint8_t channel, float tareVal);
    }

    struct SensorData {
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
    void handleRCPSensorRead(RCP_DeviceClass devclass, uint8_t id, uint8_t tareChannel, float tareVal);
}

#endif //SENSORS_H
