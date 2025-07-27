#ifndef PYROS_H
#define PYROS_H

namespace Pyros {
    enum class Pyro {
        CH1,
        CH2,
    };

    void setup();

    void set(Pyro pyro);
    void unset(Pyro pyro);
    bool get(Pyro pyro);
}

#endif // PYROS_H
