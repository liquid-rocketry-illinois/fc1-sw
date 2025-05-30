#include <Arduino.h>

#include "RCP.h"
#include "Sensors.h"

void setup() {
    Serial.begin(115200);
    while(!Serial) {
        yield();
        delay(10);
    }

    RCP::init();
    RCP::setReady(true);
    RCPDebug("[RCP] Initialization complete!");
}

void loop() {
    RCP::yield();
    RCP::runTest();
    Sensors::yield();
}

void RCP::systemReset() {
    NVIC_SystemReset();
}

