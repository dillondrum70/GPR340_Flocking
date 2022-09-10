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
		slotAssignments[i]->slotNumber = i;
	}
	
	if (!slotAssignments.empty())
	{
		driftOffset = pattern->GetDriftOffset(slotAssignments);
	}
}

//add boid to slot, return false if all slots full
bool FormationManager::AddBoid(Boid* boid)
{
	int occupiedSlots = slotAssignments.size();

	if (pattern->SupportsSlots(occupiedSlots + 1))
	{
		//remove boid from current formation
		int currentForm = boid->getFormationID();
		if (currentForm >= 0)
		{
			this->world->formations[currentForm]->RemoveBoid(boid);
		}


		boid->setFormationID(id);

		SlotAssignment* newAssignment = new SlotAssignment();
		newAssignment->boid = boid;
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
		if (slotAssignments[i]->boid)
		{
			if (boid && slotAssignments[i]->boid == boid)
			{
				slotAssignments.erase(slotAssignments.begin() + i);
				break;
			}
		}
		else
		{
			slotAssignments.erase(slotAssignments.begin() + i);
		}
		
	}

	UpdateSlotAssignments();
}

//send new target locations to boids
void FormationManager::UpdateSlots()
{
	Static anchor = GetAnchorPoint();

	int slotCount = slotAssignments.size();
	
	for (int i = 0; i < slotCount; i++)
	{
		int slotNumber = slotAssignments[i]->slotNumber;
		Static slot = pattern->GetSlotLocation(slotNumber, slotCount);

		Static location;
		float xOffset = (slot.position.x * std::cos(anchor.orientation)) - (slot.position.y * std::sin(anchor.orientation));
		float yOffset = (slot.position.x * std::sin(anchor.orientation)) + (slot.position.y * std::cos(anchor.orientation));
		location.position = anchor.position + Vector2(xOffset, yOffset);
		//location.position = anchor.position + slot.position;

		location.orientation = anchor.orientation + slot.orientation;

		location.position -= driftOffset.position;
		location.orientation -= driftOffset.orientation;

		slotAssignments[i]->boid->target = location;

	}
}

//the first position in formation
Static FormationManager::GetAnchorPoint()
{
	Static result;
	int count = 0;

	for (count = 0; count < slotAssignments.size(); count++)
	{
		//sum positions
		result.position += slotAssignments[count]->boid->getPosition();

		//sum the atan of the velocity to get the sum of orientations
		Vector2 dir = slotAssignments[count]->boid->transform.rotation;
		result.orientation += dir.getAngleRadian();
	}

	if (count != 0)
	{
		result.position /= count; //average the positions to get the center like with cohesion

		result.orientation /= count; //average the orientations
	}

	return result;
}
