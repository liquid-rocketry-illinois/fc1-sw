#include "Sensors.h"

#include "RCP.h"
#include "SD.h"

namespace Sensors {
    SensorData latestReadings = {0};

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
