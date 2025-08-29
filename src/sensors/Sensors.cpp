#include "Sensors.h"

#include "RCP_Target/RCP_Target.h"
#include "SD.h"

namespace Sensors {
    SensorData latestReadings = {0};

    void setup() {
        Ambient::setup();
        Mag::setup();
        IMU::setup();
        // GNSS::setup();
    }

    void yield() {
        latestReadings.timestamp = RCP::millis();
        Ambient::getData(latestReadings.ambientData);
        Mag::getData(latestReadings.magData);
        IMU::getData(latestReadings.icmData, latestReadings.bmiData);
        // GNSS::getData(latestReadings.gnssData);

        if(RCP::getTestState() == RCP_TEST_RUNNING) {
            SD::writeDataCSV(latestReadings);
        }

        if(RCP::getDataStreaming()) {
            RCP::sendOneFloat(RCP_DEVCLASS_AM_PRESSURE, 0, latestReadings.ambientData.pressure);
            RCP::sendOneFloat(RCP_DEVCLASS_RELATIVE_HYGROMETER, 0, latestReadings.ambientData.humidity);
            RCP::sendOneFloat(RCP_DEVCLASS_AM_TEMPERATURE, 0, latestReadings.ambientData.temperature);

            RCP::sendThreeFloat(RCP_DEVCLASS_MAGNETOMETER, 0, reinterpret_cast<float*>(&latestReadings.magData));

            RCP::sendThreeFloat(RCP_DEVCLASS_ACCELEROMETER, 0, reinterpret_cast<float*>(&latestReadings.icmData.accel));
            RCP::sendThreeFloat(RCP_DEVCLASS_GYROSCOPE, 0, reinterpret_cast<float*>(&latestReadings.icmData.gyro));

            RCP::sendThreeFloat(RCP_DEVCLASS_ACCELEROMETER, 1, reinterpret_cast<float*>(&latestReadings.bmiData.accel));
            RCP::sendThreeFloat(RCP_DEVCLASS_GYROSCOPE, 1, reinterpret_cast<float*>(&latestReadings.bmiData.gyro));

            RCP::sendFourFloat(RCP_DEVCLASS_GPS, 0, reinterpret_cast<float*>(&latestReadings.gnssData));
        }
    }
} // namespace Sensors

RCP::Floats4 RCP::readSensor(RCP_DeviceClass devclass, uint8_t id) {
    using Sensors::latestReadings;
    Floats4 floats{};
    switch(devclass) {
    case RCP_DEVCLASS_AM_PRESSURE:
        floats.vals[0] = latestReadings.ambientData.pressure;
        break;

    case RCP_DEVCLASS_RELATIVE_HYGROMETER:
        floats.vals[0] = latestReadings.ambientData.humidity;
        break;

    case RCP_DEVCLASS_AM_TEMPERATURE:
        floats.vals[0] = latestReadings.ambientData.temperature;
        break;

    case RCP_DEVCLASS_MAGNETOMETER:
        memcpy(floats.vals, &latestReadings.magData, 12);
        break;

    case RCP_DEVCLASS_ACCELEROMETER:
        memcpy(floats.vals, id == 0 ? &latestReadings.icmData.accel : &latestReadings.bmiData.accel, 12);
        break;

    case RCP_DEVCLASS_GYROSCOPE:
        memcpy(floats.vals, id == 0 ? &latestReadings.icmData.gyro : &latestReadings.bmiData.gyro, 12);
        break;

    case RCP_DEVCLASS_GPS:
        memcpy(floats.vals, &latestReadings.gnssData, 16);
        break;

    default:
        break;
    }

    return floats;
}

void RCP::writeSensorTare(RCP_DeviceClass devclass, uint8_t id, uint8_t dataChannel, float tareVal) {
    using namespace Sensors;
    switch(devclass) {
    case RCP_DEVCLASS_AM_PRESSURE:
    case RCP_DEVCLASS_RELATIVE_HYGROMETER:
    case RCP_DEVCLASS_AM_TEMPERATURE:
        Ambient::tare(devclass, id, dataChannel, tareVal);
        break;

    case RCP_DEVCLASS_MAGNETOMETER:
        Mag::tare(devclass, id, dataChannel, tareVal);
        break;

    case RCP_DEVCLASS_ACCELEROMETER:
    case RCP_DEVCLASS_GYROSCOPE:
        IMU::tare(devclass, id, dataChannel, tareVal);
        break;

    case RCP_DEVCLASS_GPS:
        GNSS::tare(devclass, id, dataChannel, tareVal);
        break;

    default:
        break;
    }
}
