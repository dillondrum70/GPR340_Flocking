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

void FormationManager::UpdateIDs()
{
	for (SlotAssignment* slot : slotAssignments)
	{
		if (slot && slot->boid)
		{
			slot->boid->setFormationID(id);
		}
	}
}

//add boid to slot, return false if all slots full
bool FormationManager::AddBoid(Boid* boid)
{
	int occupiedSlots = slotAssignments.size();

	if (boid && pattern->SupportsSlots(occupiedSlots + 1))
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
				boid->setFormationID(-1);
				delete slotAssignments[i];
				slotAssignments.erase(slotAssignments.begin() + i);
			}
		}
		else //if boid is garbage, erase it anyways
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

		//rotate and the slot position about the anchor point
		Static location;
		float xOffset = (slot.position.x * std::cos(anchor.orientation)) - (slot.position.y * std::sin(anchor.orientation));
		float yOffset = (slot.position.x * std::sin(anchor.orientation)) + (slot.position.y * std::cos(anchor.orientation));
		location.position = anchor.position + Vector2(xOffset, yOffset);

		//float xOffset = ((slot.position.x - anchor.position.x) * std::cos(anchor.orientation)) - ((anchor.position.y - slot.position.y) * std::sin(anchor.orientation)) + anchor.position.x;
		//float yOffset = anchor.position.y - ((slot.position.x - anchor.position.x) * std::sin(anchor.orientation)) + ((anchor.position.y - slot.position.y) * std::cos(anchor.orientation));
		//location.position = anchor.position; //if anchor position is applied in offset calculations

		//location.position = anchor.position + slot.position; //unrotated
		

		location.orientation = anchor.orientation + slot.orientation;  //for vformation, probably wont do anything.  slot should return 0 for orientation

		location.position -= driftOffset.position;
		location.orientation -= driftOffset.orientation;

		//set boid target, boid will move towards that target
		slotAssignments[i]->boid->target = location;

		//this is the only way to make the rule display accurate.  Otherwise, the anchor point is different by the time rules are drawn
		if (world->getShowRules())
		{
			Polygon::DrawLine(world->engine->window->sdlRenderer, location.position, location.position, Vector3::Yellow());
		}
	}

	//this is the only way to make the rule display accurate.  Otherwise, the anchor point is different by the time rules are drawn
	if (world->getShowRules())
	{
		Polygon::DrawLine(world->engine->window->sdlRenderer, anchor.position, anchor.position + (Vector2::getVector2FromRadian(anchor.orientation - (3.14 / 2.f)) * 20), Vector3::Green());
	}
}

//the first position in formation
Static FormationManager::GetAnchorPoint()
{
	Static result;
	int count = 0;
	Vector2 avgVelocity;

	for (count = 0; count < slotAssignments.size(); count++)
	{
		if (slotAssignments[count])
		{
			if (slotAssignments[count]->boid)
			{
				//sum positions
				result.position += slotAssignments[count]->boid->getPosition();

				//sum the atan of the velocity to get the sum of orientations
				avgVelocity += slotAssignments[count]->boid->getVelocity();
				
			}
		}
	}

	if (count != 0)
	{
		result.position /= count; //average the positions to get the center like with cohesion

		avgVelocity /= count; //average the orientations
	}

	result.orientation += avgVelocity.getAngleRadian();

	return result;
}
