#include "Sensors.h"
#include "RCP.h"

namespace Sensors::GNSS {
    SFE_UBLOX_GNSS gnss;
    SensorStatus gnssStatus = SensorStatus::RESET;

    void setup() {
        gnssStatus = SensorStatus::INIT_FAIL;

        if(!gnss.begin(Wire)) {
            RCPDebug("[GPS] Failed to initialize");
            return;
        }

        bool error = gnss.setI2CAddress(GNSS_ADDR);
        error = error || gnss.setI2COutput(COM_TYPE_UBX);
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
        data.lat = static_cast<float>(gnss.getLatitude()) / 10000000.0;
        data.lon = static_cast<float>(gnss.getLongitude()) / 10000000.0;
        data.alt = static_cast<float>(gnss.getAltitude()) / 1000.0;
        data.gs = static_cast<float>(gnss.getGroundSpeed()) / 1000.0;
    }
}