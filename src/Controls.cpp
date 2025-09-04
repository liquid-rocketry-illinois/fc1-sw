#include "Controls.h"

#include "RCP_Target/RCP_Target.h"

#include "Sensors.h"

// Hiko this is your file. You should not change anything outside of this file

namespace Controls {
    void init() {

    }

    void update(const Sensors::SensorData& data) {
        // Example of how to write to servo 0, 50 degrees:
        // RCP::writeAngledActuator(0, 50.0f);
    }
}