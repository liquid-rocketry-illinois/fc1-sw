#include "Sensors.h"

#include "RCP.h"
#include "SD.h"

namespace Sensors {
    SensorData latestReadings = {0};

    void setup() {
        Ambient::setup();
        Mag::setup();
        IMU::setup();
        GNSS::setup();
    }

    void yield() {
        latestReadings.timestamp = millis() - RCP::timeOffset;
        Ambient::getData(latestReadings.ambientData);
        Mag::getData(latestReadings.magData);
        IMU::getData(latestReadings.icmData, latestReadings.bmiData);
        GNSS::getData(latestReadings.gnssData);

        if(RCP::testState == RCP_TEST_RUNNING) {
            SD::writeDataCSV(latestReadings);
        }

        if(RCP::dataStreaming) {
            RCP::sendOneFloat(RCP_DEVCLASS_AM_PRESSURE, 0, &latestReadings.ambientData.pressure);
            RCP::sendOneFloat(RCP_DEVCLASS_RELATIVE_HYGROMETER, 0, &latestReadings.ambientData.humidity);
            RCP::sendOneFloat(RCP_DEVCLASS_AM_TEMPERATURE, 0, &latestReadings.ambientData.temperature);

            RCP::sendThreeFloat(RCP_DEVCLASS_MAGNETOMETER, 0, reinterpret_cast<float*>(&latestReadings.magData));

            RCP::sendThreeFloat(RCP_DEVCLASS_ACCELEROMETER, 0, reinterpret_cast<float*>(&latestReadings.icmData.accel));
            RCP::sendThreeFloat(RCP_DEVCLASS_GYROSCOPE, 0, reinterpret_cast<float*>(&latestReadings.icmData.gyro));

            RCP::sendThreeFloat(RCP_DEVCLASS_ACCELEROMETER, 1, reinterpret_cast<float*>(&latestReadings.bmiData.accel));
            RCP::sendThreeFloat(RCP_DEVCLASS_GYROSCOPE, 1, reinterpret_cast<float*>(&latestReadings.bmiData.gyro));

            RCP::sendFourFloat(RCP_DEVCLASS_GPS, 0, reinterpret_cast<float*>(&latestReadings.gnssData));
        }
    }

    static void handleTare(RCP_DeviceClass devclass, uint8_t id, uint8_t tareChannel, float tareVal) {
        void (*tare)(RCP_DeviceClass, uint8_t, uint8_t, float) = nullptr;

        switch(devclass) {
        case RCP_DEVCLASS_AM_PRESSURE:
        case RCP_DEVCLASS_RELATIVE_HYGROMETER:
        case RCP_DEVCLASS_AM_TEMPERATURE:
            tare = Ambient::tare;
            break;

        case RCP_DEVCLASS_MAGNETOMETER:
            tare = Mag::tare;
            break;

        case RCP_DEVCLASS_ACCELEROMETER:
        case RCP_DEVCLASS_GYROSCOPE:
            tare = IMU::tare;
            break;

        case RCP_DEVCLASS_GPS:
            tare = GNSS::tare;
            break;

        default:
            return;
        }

        tare(devclass, id, tareChannel, tareVal);
    }

    static void handleRead(RCP_DeviceClass devclass, uint8_t id) {
        void (*writer)(const RCP_DeviceClass, const uint8_t, const float*) = nullptr;
        const float* data = nullptr;

        switch(devclass) {
        case RCP_DEVCLASS_AM_PRESSURE:
            writer = RCP::sendOneFloat;
            data = &latestReadings.ambientData.pressure;
            break;

        case RCP_DEVCLASS_RELATIVE_HYGROMETER:
            writer = RCP::sendOneFloat;
            data = &latestReadings.ambientData.humidity;
            break;

        case RCP_DEVCLASS_AM_TEMPERATURE:
            writer = RCP::sendOneFloat;
            data = &latestReadings.ambientData.temperature;
            break;

        case RCP_DEVCLASS_MAGNETOMETER:
            writer = RCP::sendThreeFloat;
            data = reinterpret_cast<const float*>(&latestReadings.magData);
            break;

        case RCP_DEVCLASS_ACCELEROMETER:
            writer = RCP::sendThreeFloat;
            data =
                reinterpret_cast<const float*>(id == 0 ? &latestReadings.icmData.accel : &latestReadings.bmiData.accel);
            break;

        case RCP_DEVCLASS_GYROSCOPE:
            writer = RCP::sendThreeFloat;
            data =
                reinterpret_cast<const float*>(id == 0 ? &latestReadings.icmData.gyro : &latestReadings.bmiData.gyro);
            break;

        case RCP_DEVCLASS_GPS:
            writer = RCP::sendFourFloat;
            data = reinterpret_cast<const float*>(&latestReadings.gnssData);
            break;

        default:
            return;
        }

        writer(devclass, id, data);
    }

    void handleRCPSensorRead(RCP_DeviceClass devclass, uint8_t id, uint8_t tareChannel, float tareVal) {
        if(tareChannel == 255) handleRead(devclass, id);
        else handleTare(devclass, id, tareChannel, tareVal);
    }

} // namespace Sensors
