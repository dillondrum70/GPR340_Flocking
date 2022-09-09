#include "FormationManager.h"


#include "imgui.h"
#include "../utils/ImGuiExtra.h"
#include "Polygon.h"
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"

void FormationManager::UpdateSlotAssignments()
{
	for (int i = 0; i < slotAssignments.size(); i++)
	{
		slotAssignments[i].slotNumber = i;
	}

	driftOffset = pattern.GetDriftOffset(slotAssignments);
}

//add boid to slot, return false if all slots full
bool FormationManager::AddBoid(Boid* boid)
{
	int occupiedSlots = slotAssignments.size();

	if (pattern.SupportsSlots(occupiedSlots + 1))
	{
		SlotAssignment newAssignment = SlotAssignment();
		newAssignment.boid = boid;
		slotAssignments.push_back(newAssignment);
		UpdateSlotAssignments();
		return true;
	}

	return false;
}

//remove boid from slot
void FormationManager::RemoveBoid(Boid* boid)
{
	for (int i = 0; i < slotAssignments.size(); i++)
	{
		if (slotAssignments[i].boid == boid)
		{
			slotAssignments.erase(slotAssignments.begin() + i);
			break;
		}
	}
}

//send new target locations to boids
void FormationManager::UpdateSlots()
{
	Static anchor = GetAnchorPoint();

	int slotCount = slotAssignemnts.size();
	
	for (int i = 0; i < slotCount; i++)
	{
		int slotNumber = slotAssignments[i].slotNumber;
		Static slot = pattern.GetSlotLocation(slotNumber, slotCount);

		Static location;
		float xOffset = (slot.position.x * std::cos(anchor.orientation)) - (slot.position.y * std::sin(anchor.orientation));
		float yOffset = (slot.position.x * std::sin(anchor.orientation)) + (slot.position.y * std::cos(anchor.orientation));
		location.position = anchor.position + Vector2(xOffset, yOffset);
		location.orientation = anchor.orientation + slot.orientation;

		location.position -= driftOffset.position;
		location.orientation -= driftOffset.orientation;

		slotAssignments[i].boid->target = location;

	}
}

//the first position in formation
Static FormationManager::GetAnchorPoint()
{
	Static result;
	int count = 0;

	for (SlotAssignment a : slotAssignments)
	{
		count++;
		//sum positions
		result.position += a.boid->getPosition();

		//sum the atan of the velocity to get the sum of orientations
		Vector2 dir = a.boid->getVelocity();
		result.orientation += std::atan2(dir.x, dir.y);
	}

	result.position /= count; //average the positions to get the center like with cohesion

	result.orientation /= count; //average the orientations

	return 
}
