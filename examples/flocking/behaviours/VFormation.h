#ifndef VFORMATION_RULE
#define VFORMATION_RULE

#include "FlockingRule.h"
#include "FormationPattern.h"

/* Steer toward average heading of local flockmates */
class VFormationRule : public FlockingRule {
public:
    //int targetDist = 5; //try to be within 5 pixels of target
    int slowDist = 10; //slow down within 10 pixels of target

    explicit VFormationRule(World* pWorld, float weight = 1., bool isEnabled = true) : FlockingRule(pWorld, Vector3::Yellow(), weight, isEnabled) {}

    std::unique_ptr<FlockingRule> clone() override {
        return std::make_unique<VFormationRule>(*this);
    }

    const char* getRuleName() override {
        return "V-Formation Rule";
    }

    const char* getRuleExplanation() override {
        return "Align boids to an arrowhead formation";
    }

    virtual float getBaseWeightMultiplier() override {
        return 1.;
    }

    Vector2 computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) override;
};

class VFormation : public FormationPattern
{
    int maxSlotNumber = 28;

    int calculateNumberOfSlots(std::vector<SlotAssignment*> slotAssignments);

    //drift offset when characters are in the set of slots
    Static GetDriftOffset(std::vector<SlotAssignment*> slotAssignments) override;

    //return location of a slot index
    Static GetSlotLocation(int slotNumber, int totalSlots) override;

    //can pattern support <slotCount> slots?
    bool SupportsSlots(int slotCount) override;

    float NextTriangularRoot(int num);
};

#endif
