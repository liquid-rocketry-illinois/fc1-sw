#include "Sensors.h"
#include "RCP.h"

namespace Sensors::IMU {
    ICM42688 icm(Wire, ICM_ADDR);
    Bmi088 bmi(Wire, BMI_ACCEL_ADDR, BMI_GYRO_ADDR);

    SensorStatus icmStatus = SensorStatus::RESET;
    SensorStatus bmiStatus = SensorStatus::RESET;

    inline float toDeg(float rads) {
        return static_cast<float>(rads * 180.0f / M_PI);
    }

    void setupICM() {
        icmStatus = SensorStatus::INIT_FAIL;
        bool error = icm.begin();

        if(error) {
            RCPDebug("[ICM] Failed to initialize");
            return;
        }

        error = icm.setGyroFS(ICM42688::dps2000);
        error = error || icm.setGyroODR(ICM42688::odr32k);
        if(error) {
            RCPDebug("[ICM-Gyro] Failed to configure settings");
            return;
        }

        error = icm.setAccelFS(ICM42688::gpm16);
        error = error || icm.setAccelODR(ICM42688::odr32k);
        if(error) {
            RCPDebug("[ICM-Accel] Failed to configure settings");
            return;
        }

        RCPDebug("[ICM] Initialization Complete");
        icmStatus = SensorStatus::READY;
    }

    void setupBMI() {
        bmiStatus = SensorStatus::INIT_FAIL;
        bool error = bmi.begin();

        if(error) {
            RCPDebug("[BMI] Failed to initialize");
            return;
        }

        error = bmi.setRange(Bmi088::ACCEL_RANGE_24G, Bmi088::GYRO_RANGE_2000DPS);
        error = error || bmi.setOdr(Bmi088::ODR_2000HZ);

        if(error) {
            RCPDebug("[BMI] Failed to configure settings");
            return;
        }

        RCPDebug("[BMI] Initialization Complete");
        bmiStatus = SensorStatus::READY;
    }

    void setup() {
        setupICM();
        setupBMI();
    }

    void getData(IMUData& icmData, IMUData& bmiData) {
        if(icmStatus != SensorStatus::INIT_FAIL) {
            bool error = icm.getAGT();

            if(error) icmStatus = SensorStatus::DATA_FAIL;
            else {
                icmStatus = SensorStatus::DATA_READY;
                icmData.accel.x = icm.accX();
                icmData.accel.y = icm.accY();
                icmData.accel.z = icm.accZ();
                icmData.gyro.x = icm.gyrX();
                icmData.gyro.y = icm.gyrY();
                icmData.gyro.z = icm.gyrZ();
            }
        }

        if(bmiStatus != SensorStatus::INIT_FAIL) {
            bmiStatus = SensorStatus::DATA_READY;
            bmi.readSensor();
            bmiData.accel.x = bmi.getAccelX_mss();
            bmiData.accel.y = bmi.getAccelX_mss();
            bmiData.accel.z = bmi.getAccelX_mss();
            bmiData.gyro.x = toDeg(bmi.getGyroX_rads());
            bmiData.gyro.y = toDeg(bmi.getGyroX_rads());
            bmiData.gyro.z = toDeg(bmi.getGyroX_rads());
        }
    }
}