#ifndef FORMATION_MANAGER_H
#define FORMATION_MANAGER_H

#include "FormationPattern.h"
#include "../gameobjects/Boid.h"
#include "VFormation.h"

class FormationManager {

public:

	World* world;

	FormationManager(World* worldVal) 
	{ 
		world = worldVal; 
		pattern = new VFormation();
	}; //VFormation will be default
	FormationManager(World* worldVal, FormationPattern* initialPattern) 
	{
		world = worldVal;
		pattern = initialPattern;
	};

	~FormationManager() { ClearSlots(); }

	void ClearSlots()
	{
		for (SlotAssignment* slot : slotAssignments)
		{
			delete slot;
		}

		slotAssignments.clear();

		if (pattern)
		{
			delete pattern;
			pattern = nullptr;
		}
	}

	//stores boids and slots they are assigned to
	std::vector<SlotAssignment*> slotAssignments;

	//represents drift offset for filled slots
	Static driftOffset;

	//pattern used
	FormationPattern* pattern = nullptr; //VFormation will be default

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