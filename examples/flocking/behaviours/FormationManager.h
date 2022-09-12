#ifndef FORMATION_MANAGER_H
#define FORMATION_MANAGER_H

#include "FormationPattern.h"
#include "../gameobjects/Boid.h"
#include "VFormation.h"

class FormationManager {

public:

	World* world;

	int id = -1;

	FormationManager(World* worldVal, int idVal) 
	{ 
		world = worldVal; 
		pattern = new VFormation();
		id = idVal;
	}; //VFormation will be default
	FormationManager(World* worldVal, FormationPattern* initialPattern, int idVal) 
	{
		world = worldVal;
		pattern = initialPattern;
		id = idVal;
	};

	~FormationManager() { ClearSlots(); }

	void ClearSlots()
	{
		for (SlotAssignment* slot : slotAssignments)
		{
			if (slot)
			{
				slot->boid->setFormationID(-1); //clear formation info for boid
				delete slot;
			}
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

	int GetID() { return id; }

	//update assignment of characters to slots
	void UpdateSlotAssignments();

	//update formation IDs stored by boids when formation ID changes
	void UpdateIDs();

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