#include <Arduino.h>

#include "Pyros.h"
#include "RCP_Target/RCP_Target.h"
#include "Sensors.h"
#include "Servos.h"

void setup() {
    // Comment/uncomment the two versions of setup code below to choose between pyro channels and sensors

    // Setup code for sensor usage

    // Serial.begin(115200);
    // while(!Serial) {
    //     yield();
    //     delay(10);
    // }
    //
    // Serial.write(4);
    // Serial.write(4);
    // Serial.write(4);
    // Serial.write(4);
    // Serial.write(4);
    // uint8_t val = 5;
    // RCP::write(&val, 1);
    //
    // Peripherals::I2C0.begin();
    // Peripherals::I2C1.begin();
    // Peripherals::SPI0.begin();
    // Peripherals::SDSPI.begin();
    //
    // RCP::init();
    // RCP::setReady(true);
    // RCPDebug("[RCP] Initialization complete!");
    // RCP::RCPWriteSerialString("[RCP] Using ");
    // RCP::RCPWriteSerialString(RCPT_VERSION);
    // RCP::RCPWriteSerialString("\n");
    // Sensors::setup();
    // Pyros::setup();
    // Servos::setup();

    // -----------------------------------------

    // Setup code for pyro channel testing

    RCP::init();
    RCP::setReady(true);
    Pyros::setup();
    RCP::startProcedure(1);
}

// The loop function just updates all the various components. The core logic is handled by RCP
void loop() {
    RCP::yield();
    RCP::runTest();
    // Sensors::yield();
}

void RCP::write(const void* data, uint8_t length) {
    Serial.write(static_cast<const uint8_t*>(data), length);
}

uint8_t RCP::readAvail() {
    return Serial.available();
}

uint8_t RCP::read() {
    return Serial.read();
}

uint32_t RCP::systime() {
    return ::millis();
}

// Have not figured out proper system resets. For some reason the ARM CMSIS NVIC_SystemReset
// function doesnt work, but its supposed to.
void RCP::systemReset() {
    // NVIC_SystemReset();
    RCPDebug("[RCP] Hardware resets dont work :(");
    while(true) {}
}

