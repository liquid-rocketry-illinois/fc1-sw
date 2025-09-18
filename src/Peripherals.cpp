#include "Peripherals.h"

// Actual definitions for hardware peripherals
namespace Peripherals {
    TwoWire I2C0(&sercom0, GPIO::I2C0_SDA, GPIO::I2C0_SCL);
    TwoWire I2C1(&sercom2, GPIO::I2C1_SDA, GPIO::I2C1_SCL);

    SPIClass SDSPI(SERCOM3, GPIO::SD_MISO, GPIO::SD_MOSI, GPIO::SD_SCK, DIPO::PAD_2, DOPO::MOSI_0_SCK_1, SERCOM3_GCLK_ID_CORE, [] { MCLK->APBBMASK.reg |= MCLK_APBBMASK_SERCOM3; });
    SPIClass SPI0(SERCOM5, GPIO::SPI0_MISO, GPIO::SPI0_MOSI, GPIO::SPI0_SCK, DIPO::PAD_3, DOPO::MOSI_0_SCK_1, SERCOM5_GCLK_ID_CORE, [] { MCLK->APBDMASK.reg |= MCLK_APBDMASK_SERCOM5; });

    // CANSAME5x CAN(GPIO::TX_CAN, GPIO::RX_CAN);
}

SPIClass SPI(nullptr, 0, 0, 0, DIPO::PAD_0, DOPO::MOSI_0_SCK_1, 0, [] {});