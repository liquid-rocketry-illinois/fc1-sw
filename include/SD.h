#ifndef SD_H
#define SD_H

#include "Sensors.h"

namespace SD {
  constexpr uint8_t SPI_CS = 0;
  constexpr uint32_t SPI_MAX_CLOCK = 10000000UL;

  void init();
  void setTest(uint8_t testnum);
  void endTest();
  void writeDataCSV(const Sensors::SensorData& data);
}

#endif //SD_H
