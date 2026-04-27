#include "model.hpp"
#include <math.h>

namespace Model {

    double weibullSurvival(double t, double beta, double eta) {
        if (t <= 0.0) return 1.0;
        if (eta <= 0.0) return 0.0;  // invalid
        return std::exp( -std::pow(t / eta, beta) );
    }

    double weibullBreakProbability(double t, double beta, double eta) {
        double survival = weibullSurvival(t, beta, eta);
        return 1.0 - survival;
    }

    bool willBreakOnNextUse(double currentUses, double beta, double eta) {
        double breakProb = weibullBreakProbability(currentUses, beta, eta);
        double roll = static_cast<double>(std::rand()) / RAND_MAX;
        return roll < breakProb;
    }

} // namespace Model