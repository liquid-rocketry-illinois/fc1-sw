#include "RCP_Target/procedures.h"

#include "Controls.h"
#include "Peripherals.h"
#include "Pyros.h"
#include "Sensors.h"

namespace Test {
    class RocketFlyer : public Procedure {
    public:
        ~RocketFlyer() override = default;

        void initialize() override { Controls::init(); }

        void execute() override { Controls::update(Sensors::latestReadings); }
    };

    // Clang format turned off so everything can be nicely human organized
    // clang-format off
    static Tests tests = {
        new RocketFlyer(), // The main procedure which runs the controls code
        new SequentialProcedure( // Utility to set off pyros 1 minute after power on
                    new WaitProcedure(60000),
                    new OneShot([] {
                        RCP::writeSimpleActuator(0, RCP_SIMPLE_ACTUATOR_ON);
                        RCP::writeSimpleActuator(1, RCP_SIMPLE_ACTUATOR_ON);
                    }),
                    new WaitProcedure(100),
                    new OneShot([] {
                        RCP::writeSimpleActuator(0, RCP_SIMPLE_ACTUATOR_OFF);
                        RCP::writeSimpleActuator(1, RCP_SIMPLE_ACTUATOR_OFF);
                    })
                ),
        new Procedure(),
        new Procedure(),
        new Procedure(),
        new Procedure(),
        new Procedure(),
        new Procedure(),
        new Procedure(),
        new Procedure(),
        new Procedure(),
        new Procedure(),
        new Procedure(),
        new Procedure(),
        new Procedure(),
        new Procedure()
    };
    // clang-format on

    Tests& getTests() { return tests; }

} // namespace Test
