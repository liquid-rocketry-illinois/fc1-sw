#ifndef SERVOS_H
#define SERVOS_H

#include <Servo.h>

namespace Servos {
  extern Servo S1;
  extern Servo S2;
  extern Servo S3;
  extern Servo S4;

  void setup();
  void setPosition(Servo& servo, float degrees);
}

#endif // SERVOS_H
