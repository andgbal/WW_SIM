#include "model.hpp"
#include <cmath>
#include <vector>

namespace Model {

    // Item endurance calculation

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

    // damage system

    struct HitInfo {
        Position pos;
        int damageDealt;
    };

    DamageReport Model::processLineDamage(const DamageRequest& req, Map& map) {
        DamageReport report;
        int dx = 0, dy = 0;
        
        if (req.direction == Dir::NORTH) dy = -1;
        else if (req.direction == Dir::SOUTH) dy = 1;
        else if (req.direction == Dir::EAST)  dx = 1;
        else if (req.direction == Dir::WEST)  dx = -1;

        for (int i = 1; i <= static_cast<int>(req.range); ++i) {
            int tx = req.origin.x + (dx * i);
            int ty = req.origin.y + (dy * i);

            Tile* tile = map.getTile(tx, ty);
            if (!tile) break; // Out of bounds

            if (tile->hasZombie) {
                report.actualHits.push_back({{tx, ty}, req.damage});
                break; // Stop at first entity hit
            }
        }
        return report;
    }

    DamageReport Model::processRadialDamage(const DamageRequest& req, Map& map) {
        DamageReport report;
        int r = static_cast<int>(req.range);

        // Check a bounding box around the explosion
        for (int y = req.origin.y - r; y <= req.origin.y + r; y++) {
            for (int x = req.origin.x - r; x <= req.origin.x + r; x++) {
                
                // Circular distance check
                float dist = std::sqrt(std::pow(x - req.origin.x, 2) + std::pow(y - req.origin.y, 2));
                
                if (dist <= req.range) {
                    Tile* tile = map.getTile(x, y);
                    if (tile && tile->hasZombie) {
                        report.actualHits.push_back({{x, y}, req.damage});
                    }
                }
            }
        }
        return report;
    }

} // namespace Model