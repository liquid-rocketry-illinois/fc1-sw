#include "Sensors.h"
#include "RCP.h"

namespace Sensors::Mag {
    LIS2MDL mag(I2C_MODE, 0x1E);
    SensorStatus magStatus;

    void setup() {
        bool ok = mag.begin() == MAG_SUCCESS;

        if(ok) {
            mag.settings.magSampleRate = LIS2MDL_MAG_ODR_100Hz;
            mag.settings.tempCompensationEnabled = LIS2MDL_TEMP_COMPENSATION_ENABLED;
            mag.settings.selfTestEnabled = LIS2MDL_SELFTEST_ENABLED;
            if(mag.writeSettings() != MAG_SUCCESS) {
                RCPDebug("[LIS2MDL] Failed to write settings");
                magStatus = SensorStatus::INIT_FAIL;
                return;
            }

            RCPDebug("[LIS2MDL] Initialization Complete");
            magStatus = SensorStatus::READY;
        }

        else {
            RCPDebug("[LIS2MDL] Failed to initialize device");
            magStatus = SensorStatus::INIT_FAIL;
        }
    }

    void getData(AxisData& data) {
        if(magStatus == SensorStatus::INIT_FAIL) return;
        magStatus = SensorStatus::DATA_READY;
        data.x = mag.readFloatMagX();
        data.y = mag.readFloatMagY();
        data.z = mag.readFloatMagZ();
    }
}