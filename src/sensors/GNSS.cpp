#include "Peripherals.h"
#include "Sensors.h"
#include "RCP_Target/RCP_Target.h"

namespace Sensors::GNSS {
    SFE_UBLOX_GNSS gnss;
    SensorStatus gnssStatus = SensorStatus::RESET;

    static float gnssTares[4] = {0};

    void setup() {
        gnssStatus = SensorStatus::INIT_FAIL;
        if(!gnss.begin(Peripherals::I2C1, GNSS_ADDR)) {
            RCPDebug("[GPS] Failed to initialize");
            return;
        }

        bool error = gnss.setI2COutput(COM_TYPE_UBX);
        error = error || gnss.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT);
        error = error || gnss.setNavigationFrequency(2);

        if(error) {
            RCPDebug("[GPS] Failed to write settings");
            return;
        }

        gnssStatus = SensorStatus::READY;
        RCPDebug("[GPS] Initialization Complete");
    }

    void getData(GNSSData& data) {
        data.lat = (static_cast<float>(gnss.getLatitude()) / 10000000.0f) + gnssTares[0];
        data.lon = (static_cast<float>(gnss.getLongitude()) / 10000000.0f) + gnssTares[1];
        data.alt = (static_cast<float>(gnss.getAltitude()) / 1000.0f) + gnssTares[2];
        data.gs = (static_cast<float>(gnss.getGroundSpeed()) / 1000.0f) + gnssTares[3];
    }

    void tare(RCP_DeviceClass devclass, uint8_t id, uint8_t channel, float tareVal) {
        (void) id;

        if(devclass != RCP_DEVCLASS_GPS) return;
        gnssTares[channel] += tareVal;
    }
}