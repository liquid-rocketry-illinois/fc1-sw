#include "SD.h"

#include <Arduino.h>

#include "SdFat.h"
#include "RCP.h"

namespace SD {
    static SdExFat sd;
    static ExFile root;
    static ExFile log;
    static bool initSuccess = false;

    void init() {
        if(!sd.begin(SdSpiConfig(SPI_CS, DEDICATED_SPI, SPI_MAX_CLOCK, &SPI))) {
            RCPDebug("[SD] Failed to initialize SD card");
            return;
        }

        if(!root.open(&sd, "/", O_RDONLY)) {
            RCPDebug("[SD] Failed to open root directory");
            return;
        }

        initSuccess = true;
        RCPDebug("[SD] Initialization Complete");
    }

    void setTest(uint8_t testnum) {
        if(!initSuccess) return;
        if(log.isOpen()) {
            RCPDebug("[SD] Log file already open! Closing previous log");
            endTest();
        }

        String path("/");
        path.concat(testnum);
        path.concat("/");

        ExFile dir;

        if(!dir.mkdir(&root, path.c_str()) || !dir.open(&root, path.c_str(), O_RDONLY)) {
            RCPDebug("[SD] Failed to make directory for test, disabling");
            dir.close();
            root.close();
            initSuccess = false;
            return;
        }

        int filecount = 0;
        while(log.openNext(&dir, O_RDONLY)) {
            filecount++;
            log.close();
        }

        path = String(filecount);
        path.concat(".log");

        if(!log.open(&dir, path.c_str(), O_WRITE | O_CREAT | O_EXCL)) {
            RCPDebug("[SD] Failed to open log file, disabling");
            initSuccess = false;
            log.close();
            dir.close();
            root.close();
            return;
        }

        log.write("timestamp,am_pressure,relhumidity,am_temp,magx,magy,magz,accel1x,accel1y,accel1z,gyro1x,gyro1y,"
                  "gyro1z,accel2x,accel2y,accel2z,gyro2x,gyro2y,gyro2z,lat,lon,alt,gs\n");

        if(!log.sync()) {
            RCPDebug("[SD] Failed to write to log, disabling");
            log.close();
            dir.close();
            root.close();
            initSuccess = false;
            return;
        }

        RCPDebug("[SD] Successfully opened log file");
    }

    void endTest() {
        if(!initSuccess || !log.isOpen()) return;
        log.close();
        root.close();
    }

    void writeDataCSV(const Sensors::SensorData& data) {
        if(!initSuccess || !log.isOpen()) return;

        String csv(data.timestamp);

        const float* ptr = &data.ambientData.pressure;
        for(int i = 0; i < 22; i++) {
            csv.concat(",");
            csv.concat(*ptr);
            ptr++;
        }

        csv.concat("\n");
        if(log.write(csv.c_str()) == static_cast<size_t>(-1) || !log.sync()) {
            RCPDebug("[SD] Failed to write live data to log, disabling");
            log.close();
            root.close();
            initSuccess = false;
        }
    }
}