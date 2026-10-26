#include <cstdlib>
#include <memory>
#include <random>
#include <string>

#include "InputGenerators.h"
#include "Rules.h"
#include "RunSession.h"
#include "ShopSystem.h"

using namespace std;

int main(int argc, char* argv[]) {
    random_device rd;
    unsigned int seed = (argc >= 3 && string(argv[1]) == "--seed")
                            ? static_cast<unsigned int>(strtoul(argv[2], nullptr, 10))
                            : rd();

    unique_ptr<IInputGenerator> gen = make_unique<RandomInputGenerator>(seed);
    unique_ptr<IScoringRule> scoring = make_unique<SimpleScoringRule>();
    unique_ptr<IRewardRule> reward = make_unique<BonusRewardRule>();

    RunSession session(move(gen), move(scoring), move(reward),
                       ShopSystem{});
    session.Run();
    return 0;
}
