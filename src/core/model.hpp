#pragma once
#include <cmath>

namespace Model {

    // Weibull Survival Function: Probability that item still works after t uses
    // R(t) = exp( - (t / η)^β )
    double weibullSurvival(double t, double beta, double eta);

    // Break probability after this use: 1 - R(t)
    double weibullBreakProbability(double t, double beta, double eta);

    // Convenience: decide if item breaks after current use
    bool willBreakOnNextUse(double currentUses, double beta, double eta);

} // namespace Model