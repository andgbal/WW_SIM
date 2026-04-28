#pragma once
#include "../core/engine.hpp"
#include <cmath>

namespace Model {

    // Weibull Survival Function: Probability that item still works after t uses
    // R(t) = exp( - (t / η)^β )
    double weibullSurvival(double t, double beta, double eta);

    // Break probability after this use: 1 - R(t)
    double weibullBreakProbability(double t, double beta, double eta);

    // Convenience: decide if item breaks after current use
    bool willBreakOnNextUse(double currentUses, double beta, double eta);

    // Damage system
    enum class DamageType { LINE, CIRCLE, RECTANGLE };

    struct DamageRequest {
        Position origin;
        Dir direction;
        int damage;
        float range;      // Radius for circle, length for line
        float width;      // Used for rectangles or cones
        DamageType type;
    };

    struct DamageReport {
        struct Hit {
            Position pos;
            int damageDealt;
        };
        std::vector<Hit> actualHits; // Only tiles where an entity was found
    };

} // namespace Model