#include <Wire.h>

#include "Sensors.h"
#include "RCP.h"

namespace Sensors::Baro {
    MS5611 baro(BARO_ADDR, &Wire);
    SensorStatus baroStatus = SensorStatus::RESET;

    void setup() {
        baroStatus = baro.begin() ? SensorStatus::READY : SensorStatus::INIT_FAIL;

        if(baroStatus == SensorStatus::READY)
            RCPDebug("[Barometer] Initialization Complete");

        else
            RCPDebug("[Barometer] Initialization Failed");
    }

    void getData(BaroData& data) {
        bool ok = baro.read() == MS5611_READ_OK;

        if(ok) {
            data = baro.getPressure() / 1000.0f;
            baroStatus = SensorStatus::DATA_READY;
        }

        else {
            data = 0;
            baroStatus = SensorStatus::DATA_FAIL;
        }
    }
}
