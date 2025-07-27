#include <Arduino.h>

#include "Pyros.h"
#include "RCP.h"
#include "Sensors.h"
#include "Servos.h"

void setup() {
    // Comment/uncomment the two versions of setup code below to choose between pyro channels and sensors

    // Setup code for sensor usage
    Serial.begin(115200);
    while(!Serial) {
        yield();
        delay(10);
    }

    Peripherals::I2C0.begin();
    Peripherals::I2C1.begin();
    Peripherals::SPI0.begin();
    Peripherals::SDSPI.begin();

    RCP::init();
    RCP::setReady(true);
    RCPDebug("[RCP] Initialization complete!");
    Sensors::setup();
    Pyros::setup();
    Servos::setup();


    // Setup code for pyro channel testing
    // RCP::init();
    // RCP::setReady(true);
    // Pyros::setup();
    // RCP::testNum = 0;
    // RCP::testState = RCP_TEST_RUNNING;
    // Test::tests[0]->initialize();
}

void loop() {
    RCP::yield();
    RCP::runTest();
    Sensors::yield();
}

void RCP::systemReset() {
    // NVIC_SystemReset();
    RCPDebug("[RCP] Hardware resets dont work :(");
}

