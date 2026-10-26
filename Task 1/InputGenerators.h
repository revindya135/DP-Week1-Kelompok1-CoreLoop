#ifndef INPUT_GENERATORS_H
#define INPUT_GENERATORS_H

#include <random>

#include "Interfaces.h"

using namespace std;

// Input deterministik: nilai mengikuti nomor ronde.
class FixedInputGenerator : public IInputGenerator {
public:
    TurnInput Generate(int round) override {
        return TurnInput{round};
    }
};

// Input acak 1-5 dengan seed yang dapat ditentukan agar run bisa diulang.
class RandomInputGenerator : public IInputGenerator {
public:
    explicit RandomInputGenerator(unsigned int seed) : rng_(seed), dist_(1, 5) {}

    TurnInput Generate(int round) override {
        (void)round;
        return TurnInput{dist_(rng_)};
    }

private:
    mt19937 rng_;
    uniform_int_distribution<int> dist_;
};

#endif
