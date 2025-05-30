#include <Wire.h>

#include "Sensors.h"
#include "RCP.h"

namespace Sensors::Ambient {
    MS5611 baro(BARO_ADDR, &Wire);
    SensirionI2cSht4x sht;

    SensorStatus baroStatus = SensorStatus::RESET;
    SensorStatus shtStatus = SensorStatus::RESET;

    static float baroTare = 0;
    static float humTare = 0;
    static float tempTare;

    void setup() {
        baroStatus = baro.begin() ? SensorStatus::READY : SensorStatus::INIT_FAIL;

        if(baroStatus == SensorStatus::READY)
            RCPDebug("[Barometer] Initialization Complete");

        else
            RCPDebug("[Barometer] Initialization Failed");

        sht.begin(Wire, SHT40_I2C_ADDR_44);
        bool error = sht.softReset();
        uint32_t serial;
        error = error || sht.serialNumber(serial);
        if(error) {
            shtStatus = SensorStatus::INIT_FAIL;
            RCPDebug("[Temp/Humidity]: Initialization Failed");
        }

        else {
            shtStatus = SensorStatus::READY;
            RCPDebug("[Temp/Humidity]: Initialization Complete");
        }
    }

    void getData(AmbientData& data) {
        if(baroStatus != SensorStatus::INIT_FAIL) {
            bool ok = baro.read() == MS5611_READ_OK;

            if(ok) {
                data.pressure = (baro.getPressure() / 1000.0f) + baroTare;
                baroStatus = SensorStatus::DATA_READY;
            }

            else {
                data.pressure = 0;
                baroStatus = SensorStatus::DATA_FAIL;
            }
        }

        if(shtStatus != SensorStatus::INIT_FAIL) {
            // These temporaries are needed because of the struct packing
            float temp;
            float hum;
            bool error = sht.measureLowestPrecision(temp, hum);
            data.temperature = temp + tempTare;
            data.humidity = hum + humTare;
            shtStatus = error ? SensorStatus::DATA_FAIL : SensorStatus::DATA_READY;
        }
    }

    void tare(RCP_DeviceClass devclass, uint8_t id, uint8_t channel, float tareVal) {
        (void) id;
        (void) channel;

        switch(devclass) {
        case RCP_DEVCLASS_AM_PRESSURE:
            baroTare = tareVal;
            break;

        case RCP_DEVCLASS_RELATIVE_HYGROMETER:
            humTare = tareVal;
            break;

        case RCP_DEVCLASS_AM_TEMPERATURE:
            tempTare = tareVal;
            break;

        default:
            break;
        }
    }
}
