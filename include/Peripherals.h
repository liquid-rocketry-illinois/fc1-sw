#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <SPI.h>
#include <Wire.h>
#include <CANSAME5x.h>

namespace Peripherals {
  namespace GPIO {
    constexpr uint8_t PYRO2_PRESENT = 1;
    constexpr uint8_t PYRO2_FIRE = 2;
    constexpr uint8_t PYRO2_ARM = 3;
    constexpr uint8_t PYRO1_PRESENT = 4;
    constexpr uint8_t TCAN_STB = 5;

    constexpr uint8_t I2C0_SDA = 6;
    constexpr uint8_t I2C0_SCL = 7;

    constexpr uint8_t GPIO40_ADC0 = 8;
    constexpr uint8_t BUZZER_CTL = 9;

    constexpr uint8_t I2C1_SDA = 10;
    constexpr uint8_t I2C1_SCL = 11;

    constexpr uint8_t SD_SCK = 12;
    constexpr uint8_t SD_MOSI = 13;
    constexpr uint8_t SD_MISO = 14;

    constexpr uint8_t SD_CS = 15;

    constexpr uint8_t TX_CAN = 16;
    constexpr uint8_t RX_CAN = 17;

    constexpr uint8_t CS_ICM = 20;

    constexpr uint8_t SPI0_MISO = 21;
    constexpr uint8_t SPI0_MOSI = 22;
    constexpr uint8_t SPI0_SCK = 23;

    constexpr uint8_t PYRO1_FIRE = 24;
    constexpr uint8_t PYRO1_ARM = 25;
    constexpr uint8_t USR_BTN = 26;
    constexpr uint8_t SERVO4_CTL = 27;
    constexpr uint8_t SERVO3_CTL = 28;
    constexpr uint8_t SERVO2_CTL = 29;
    constexpr uint8_t SERVO1_CTL = 30;
    constexpr uint8_t USR_LED = 31;
    constexpr uint8_t CARD_DET = 32;
    constexpr uint8_t CS_RADIO = 33;
  }

  extern TwoWire I2C0;
  extern TwoWire I2C1;

  extern SPIClass SDSPI;
  extern SPIClass SPI0;

  extern CANSAME5x CAN;
}

#endif //PERIPHERALS_H
