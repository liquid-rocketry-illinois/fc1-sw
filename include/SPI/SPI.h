#ifndef SAME51_SPI_H
#define SAME51_SPI_H

#include <cstdint>

#include <same51/include/same51.h>
#include <same51/include/component/sercom.h>

#include "Arduino.h"
#include "wiring_constants.h"

#define MAX_SPI 24000000
#define SPI_CLOCK_DIV16 (MAX_SPI * 2 / 1000000)

#define SPI_MODE0 0x02
#define SPI_MODE1 0x00
#define SPI_MODE2 0x03
#define SPI_MODE3 0x01

enum class DIPO : uint8_t { PAD_0 = 0x00, PAD_1 = 0x01, PAD_2 = 0x02, PAD_3 = 0x03 };

enum class DOPO : uint8_t { MOSI_0_SCK_1 = 0x00, MOSI_3_SCK_1 = 0x02 };

class SPISettings {
public:
    SPISettings();
    SPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode);

    uint32_t clock;
    BitOrder bitOrder;
    uint8_t dataMode;
};

class SPIClass {
public:
    SPIClass(Sercom* sercom, uint8_t miso, uint8_t mosi, uint8_t sck, DIPO dipo, DOPO dopo, uint32_t GCLK_ID, void (*MCLK_SETTER)());

    byte transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);
    void transfer(void* buf, size_t count);
    // void transfer(const void* txbuf, void* rxbuf, size_t count, bool block = true);
    // void waitForTransfer(void);
    // bool isBusy(void);

    // Transaction Functions
    // void usingInterrupt(int interruptNumber);
    // void notUsingInterrupt(int interruptNumber);
    void beginTransaction(SPISettings settings);
    void endTransaction(void);

    // SPI Configuration methods
    // void attachInterrupt();
    // void detachInterrupt();

    void begin();
    void end();

    // void setBitOrder(BitOrder order);
    // void setDataMode(uint8_t uc_mode);
    // void setClockDivider(uint8_t uc_div);

private:
    Sercom* const sercom;
    const uint8_t miso;
    const uint8_t mosi;
    const uint8_t sck;
    const DIPO dipo;
    const DOPO dopo;
    const uint32_t GCLK_ID;
    void (* const MCLK_SETTER)();

    BitOrder bitOrder;
};

extern SPIClass SPI;

#endif // SAME51_SPI_H
