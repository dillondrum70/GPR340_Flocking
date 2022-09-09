#ifndef FORMATION_MANAGER_H
#define FORMATION_MANAGER_H

#include "FormationPattern.h"

class FormationManager {

public:

	FormationManager() {};
	FormationManager(FormationPattern initialPattern) { pattern = initialPattern; };

	//stores boids and slots they are assigned to
	std::vector<SlotAssignment> slotAssignments;

	//represents drift offset for filled slots
	Static driftOffset;

	//pattern used
	FormationPattern& pattern;

	//update assignment of characters to slots
	void UpdateSlotAssignments();

	//add boid to slot, return false if all slots full
	bool AddBoid(Boid* boid);

	//remove boid from slot
	void RemoveBoid(Boid* boid);

	//send new target locations to boids
	void UpdateSlots();

	//the invisible "leader"
	Static GetAnchorPoint();

};

#endif