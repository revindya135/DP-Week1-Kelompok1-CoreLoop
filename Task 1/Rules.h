#ifndef RULES_H
#define RULES_H

#include "Interfaces.h"

// Skor dasar sama dengan nilai input.
class SimpleScoringRule : public IScoringRule {
public:
    int ComputeBaseScore(const TurnInput& input) override {
        return input.value;
    }
};

// Reward datar: sebesar skor dasar.
class FlatRewardRule : public IRewardRule {
public:
    int ComputeReward(int baseScore, int round) override {
        (void)round;
        return baseScore;
    }
};

// Reward bonus: skor dasar ditambah dua.
class BonusRewardRule : public IRewardRule {
public:
    int ComputeReward(int baseScore, int round) override {
        (void)round;
        return baseScore + 2;
    }
};

#endif
