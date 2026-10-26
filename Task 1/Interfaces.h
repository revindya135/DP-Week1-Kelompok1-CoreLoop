#ifndef INTERFACES_H
#define INTERFACES_H

#include "Types.h"

// Kontrak pembangkit input per ronde.
class IInputGenerator {
public:
    virtual ~IInputGenerator() = default;
    virtual TurnInput Generate(int round) = 0;
};

// Kontrak aturan skor dasar.
class IScoringRule {
public:
    virtual ~IScoringRule() = default;
    virtual int ComputeBaseScore(const TurnInput& input) = 0;
};

// Kontrak aturan reward; parameter round wajib agar reward bisa bergantung ronde.
class IRewardRule {
public:
    virtual ~IRewardRule() = default;
    virtual int ComputeReward(int baseScore, int round) = 0;
};

#endif
