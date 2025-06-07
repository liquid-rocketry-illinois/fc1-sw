#include "peripherals.h"

namespace Peripherals {
    TwoWire I2C0(&sercom0, GPIO::I2C0_SDA, GPIO::I2C0_SCL);
    TwoWire I2C1(&sercom2, GPIO::I2C1_SDA, GPIO::I2C1_SCL);

    SPIClass SDSPI(&sercom3, GPIO::SD_MOSI, GPIO::SD_SCK, GPIO::SD_MOSI, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_2);
    SPIClass SPI0(&sercom5, GPIO::SPI0_MISO, GPIO::SPI0_SCK, GPIO::SPI0_MOSI, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_2);

    CANSAME5x CAN(GPIO::TX_CAN, GPIO::RX_CAN);
}