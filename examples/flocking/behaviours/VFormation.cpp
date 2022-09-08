#include "VFormation.h"
#include "../gameobjects/Boid.h"

Vector2 VFormationRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
    Vector2 FormationForce;

    FormationForce = boid->target.position - boid->getPosition();

    return FormationForce;
}

//drift offset when characters are in the set of slots
Static VFormation::GetDriftOffset(std::vector<SlotAssignment> slotAssignments);

//return location of a slot index
Static VFormation::GetSlotLocation(int slotNumber);

//can pattern support <slotCount> slots?
bool VFormation::SupportsSlots(int slotCount);