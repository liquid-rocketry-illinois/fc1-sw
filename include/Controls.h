#ifndef CONTROLS_H
#define CONTROLS_H

#include "Sensors.h"

namespace Controls {
  void init();
  void update(const Sensors::SensorData& data);
}

#endif //CONTROLS_H
