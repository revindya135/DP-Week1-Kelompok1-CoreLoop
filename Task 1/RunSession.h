#ifndef RUN_SESSION_H
#define RUN_SESSION_H

#include <iostream>
#include <memory>
#include <utility>

#include "Interfaces.h"
#include "ShopSystem.h"

using namespace std;

// INVARIANT: pemilik urutan fase dan satu-satunya tempat money berubah.
class RunSession {
public:
    RunSession(unique_ptr<IInputGenerator> gen,
               unique_ptr<IScoringRule> scoring,
               unique_ptr<IRewardRule> reward,
               ShopSystem shop)
        : gen_(move(gen)),
          scoring_(move(scoring)),
          reward_(move(reward)),
          shop_(move(shop)) {}

    void Run() {
        cout << "=== RUN START ===\n";
        while (round <= TOTAL_ROUNDS) {
            cout << "Round " << round << "\n";
            PlayRound();
        }
        cout << "=== RUN END ===\n";
        cout << "Final money: " << money << "\n";
    }

private:
    void PlayRound() {
        GenerateInputPhase();
        ComputeScorePhase();
        ComputeRewardPhase();
        UpdateMoneyPhase();
        ShopPhase();
        AdvanceRoundPhase();
    }

    void GenerateInputPhase() {
        input_ = gen_->Generate(round);
        cout << "[PLAY] input generated: " << input_.value << "\n";
    }

    void ComputeScorePhase() {
        baseScore_ = scoring_->ComputeBaseScore(input_);
        cout << "[SCORE] base score: " << baseScore_ << "\n";
    }

    void ComputeRewardPhase() {
        gain_ = reward_->ComputeReward(baseScore_, round);
        cout << "[REWARD] gain: " << gain_ << "\n";
    }

    void UpdateMoneyPhase() {
        money += gain_;
        cout << "[MONEY] money: " << money << "\n";
    }

    void ShopPhase() {
        shop_.ShowOffer(round);
    }

    void AdvanceRoundPhase() {
        ++round;
    }

    unique_ptr<IInputGenerator> gen_;
    unique_ptr<IScoringRule> scoring_;
    unique_ptr<IRewardRule> reward_;
    ShopSystem shop_;

    static constexpr int TOTAL_ROUNDS = 3;
    int money = 0;
    int round = 1;
    TurnInput input_{0};
    int baseScore_ = 0;
    int gain_ = 0;
};

#endif
