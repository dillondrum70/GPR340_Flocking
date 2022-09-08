#ifndef FORMATION_PATTERN_H
#define FORMATION_PATTERN_H

#include <vector>
#include <memory>
#include "Vector2.h"
#include "Vector3.h"

class Boid;
struct Static;

struct SlotAssignment {
	Boid* boid;
	int slotNumber;
};

class FormationPattern {

public:

	//drift offset when characters are in the set of slots
	virtual Static GetDriftOffset(std::vector<SlotAssignment> slotAssignments) = 0;

	//return location of a slot index
	virtual Static GetSlotLocation(int slotNumber) = 0;

	//can pattern support <slotCount> slots?
	virtual bool SupportsSlots(int slotCount) = 0;
};

#endif