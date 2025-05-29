#include <Arduino.h>
#include "RCP.h"

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
}

void RCP::systemReset() {
    NVIC_SystemReset();
}

