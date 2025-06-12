#ifndef SERVOS_H
#define SERVOS_H

namespace Servos {
  constexpr int NUM_SERVOS = 4;
  void setup();
  void setPosition(uint8_t servo, float degrees);
  float getSetpoint(uint8_t servo);
  void handleRCP(uint8_t id, bool r_nw, float wval);
}

#endif // SERVOS_H
