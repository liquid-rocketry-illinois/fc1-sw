#include <SPI.h>

#define PMUXING(pin, i) (pin % 2 == 0 ? PORT_PMUX_PMUXE(i) : PORT_PMUX_PMUXO(i))

SPISettings::SPISettings() : clock(4000000), bitOrder(MSBFIRST), dataMode(SPI_MODE0) {}

SPISettings::SPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode) :
    clock(clock), bitOrder(bitOrder), dataMode(dataMode) {}

SPIClass::SPIClass(Sercom* sercom, uint8_t miso, uint8_t mosi, uint8_t sck, DIPO dipo, DOPO dopo, uint32_t GCLK_ID,
                   void (*MCLK_SETTER)()) :
    sercom(sercom), miso(miso), mosi(mosi), sck(sck), dipo(dipo), dopo(dopo), GCLK_ID(GCLK_ID),
    MCLK_SETTER(MCLK_SETTER) {}

byte SPIClass::transfer(uint8_t data) {
    sercom->SPI.DATA.reg = data;
    while(!sercom->SPI.INTFLAG.bit.TXC) {}
    uint32_t rx = sercom->SPI.DATA.reg;
    while(!sercom->SPI.INTFLAG.bit.RXC) {}

    return static_cast<byte>(rx);
}

uint16_t SPIClass::transfer16(uint16_t data) {
    uint16_t ret = 0;
    if(bitOrder == MSBFIRST) {
        ret = transfer(data >> 8) << 8;
        ret += transfer(data);
    }

    else {
        ret = transfer(data) << 8;
        ret += transfer(data >> 8);
    }

    return ret;
}

void SPIClass::transfer(void* buf, size_t count) {
    if(!buf || !count) return;

    auto* buffer = static_cast<uint8_t*>(buf);

    for(size_t i = 0; i < count; i++) {
        buffer[i] = transfer(buffer[i]);
    }
}

void SPIClass::beginTransaction(SPISettings settings) {
    bitOrder = settings.bitOrder;

    // Make sure the device is currently disabled
    sercom->SPI.CTRLA.bit.ENABLE = 0;
    while(sercom->SPI.SYNCBUSY.bit.ENABLE);

    int cpha = 0;
    int cpol = 0;

    cpha = settings.dataMode & 0x1ul;
    cpol = (settings.dataMode & 0x2ul) >> 1;

    SERCOM5->SPI.CTRLA.bit.CPHA = cpha;
    SERCOM5->SPI.CTRLA.bit.DORD = bitOrder == LSBFIRST ? 1 : 0;
    SERCOM5->SPI.CTRLA.bit.CPOL = cpol;

    // Baud is set to 23 to create 1MHz baud rate
    SERCOM5->SPI.BAUD.reg = (48000000 / (2 * settings.clock)) - 1;

    SERCOM5->SPI.CTRLA.bit.ENABLE = 1;
    while(SERCOM5->SPI.SYNCBUSY.bit.ENABLE);
}

void SPIClass::endTransaction(void) {
    sercom->SPI.CTRLA.bit.ENABLE = 0;
    while(sercom->SPI.SYNCBUSY.bit.ENABLE);
}

void SPIClass::begin() {
    // MCU docs:
    // https://onlinedocs.microchip.com/oxy/GUID-F5813793-E016-46F5-A9E2-718D8BCED496-en-US-14/GUID-B3DD274A-0286-4C14-8A91-0B9A5980582F.html
    // Enable main clock to APBD connection to SERCOM5
    MCLK_SETTER();

    // Set up GCLK 4 with source of the 48MHz DFLL
    GCLK->GENCTRL[4].bit.GENEN = 1;
    GCLK->GENCTRL[4].bit.SRC = GCLK_SOURCE_DFLL48M;
    GCLK->GENCTRL[4].bit.DIV = 0;
    GCLK->GENCTRL[4].bit.DIVSEL = 0;

    // Enable the SERCOM5 peripheral core clock channel to source from GCLK4
    GCLK->PCHCTRL[GCLK_ID].bit.GEN = GCLK_PCHCTRL_GEN_GCLK4_Val;
    while(GCLK->SYNCBUSY.bit.GENCTRL4);
    GCLK->PCHCTRL[GCLK_ID].bit.CHEN = 1;
    while(!GCLK->PCHCTRL[GCLK_ID].bit.CHEN);

    // Setup MISO

    PinDescription pmiso = g_APinDescription[miso];

    PORT->Group[pmiso.ulPort].PINCFG[pmiso.ulPin].bit.PMUXEN = 1;
    PORT->Group[pmiso.ulPort].PMUX[pmiso.ulPin >> 1].reg = PMUXING(pmiso.ulPin, 3);


    // Setup MOSI

    PinDescription pmosi = g_APinDescription[mosi];

    PORT->Group[pmosi.ulPort].PINCFG[pmosi.ulPin].bit.PMUXEN = 1;
    PORT->Group[pmosi.ulPort].PMUX[pmosi.ulPin >> 1].reg = PMUXING(pmosi.ulPin, 3);
    PORT->Group[pmosi.ulPort].DIRSET.reg = 1 << pmosi.ulPin;


    // Setup SCK

    PinDescription psck = g_APinDescription[sck];

    PORT->Group[psck.ulPort].PINCFG[psck.ulPin].bit.PMUXEN = 1;
    PORT->Group[psck.ulPort].PMUX[psck.ulPin >> 1].reg = PMUXING(pmosi.ulPin, 3);


    // Begin setting up the SEROCM
    /* Perform a software reset */
    sercom->SPI.CTRLA.bit.SWRST = 1;

    /* Wait for synchronization */
    while(sercom->SPI.SYNCBUSY.bit.SWRST || sercom->SPI.SYNCBUSY.bit.ENABLE);

    // Make sure the device is currently disabled
    sercom->SPI.CTRLA.bit.ENABLE = 0;
    while(sercom->SPI.SYNCBUSY.bit.ENABLE);

    sercom->SPI.CTRLA.bit.MODE = 3;
    sercom->SPI.CTRLA.bit.DOPO = static_cast<uint8_t>(dopo);
    sercom->SPI.CTRLA.bit.DIPO = static_cast<uint8_t>(dipo);
    sercom->SPI.CTRLB.bit.RXEN = 1;
    sercom->SPI.CTRLB.bit.CHSIZE = 0;
}

void SPIClass::end() {
    sercom->SPI.CTRLA.bit.SWRST = 1;
    while(sercom->SPI.SYNCBUSY.bit.SWRST || sercom->SPI.SYNCBUSY.bit.ENABLE) {}
}

// void SPIClass::setBitOrder(BitOrder order) {
//     bitOrder = order;
// }
//
// void SPIClass::setDataMode(uint8_t uc_mode) {
//     if((uc_mode & (0x1ul)) == 0)
//         cpha = 0;
//     else
//         cpha = 1;
//
//     if((uc_mode & (0x2ul)) == 0)
//         cpol = 0;
//     else
//         cpol = 1;
// }
//
// void SPIClass::setClockDivider(uint8_t uc_div) {
//     clock = uc_div;
// }



/*
 See the datasheet for info on these settings
    sercom->SPI.CTRLA.bit.ENABLE = 0;
while(sercom->SPI.SYNCBUSY.bit.ENABLE);
SERCOM5->SPI.CTRLA.bit.MODE = 3;
SERCOM5->SPI.CTRLA.bit.DOPO = static_cast<uint8_t>(dopo);
SERCOM5->SPI.CTRLA.bit.DIPO = static_cast<uint8_t>(dipo);
SERCOM5->SPI.CTRLA.bit.CPHA = 1;
SERCOM5->SPI.CTRLA.bit.DORD = bitOrder == LSBFIRST ? 1 : 0;
SERCOM5->SPI.CTRLA.bit.CPOL = 0;
SERCOM5->SPI.CTRLB.bit.RXEN = 1;
SERCOM5->SPI.CTRLB.bit.CHSIZE = 0;
// Baud is set to 23 to create 1MHz baud rate
SERCOM5->SPI.BAUD.reg = 23;

SERCOM5->SPI.CTRLA.bit.ENABLE = 1;
while(SERCOM5->SPI.SYNCBUSY.bit.ENABLE);
 *
 *
 */