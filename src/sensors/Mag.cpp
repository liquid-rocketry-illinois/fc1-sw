#include "Peripherals.h"
#include "Sensors.h"
#include "RCP_Target/RCP_Target.h"

namespace Sensors::Mag {
    LIS2MDL mag(I2C_MODE, 0x1E, &Peripherals::I2C0);
    SensorStatus magStatus;
    static float tares[3] = {0};

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
        data.x = mag.readFloatMagX() + tares[0];
        data.y = mag.readFloatMagY() + tares[1];
        data.z = mag.readFloatMagZ() + tares[2];
    }

    void tare(RCP_DeviceClass devclass, uint8_t id, uint8_t channel, float tareVal) {
        if(devclass != RCP_DEVCLASS_MAGNETOMETER) return;
        (void) id;
        tares[channel] += tareVal;
    }
}