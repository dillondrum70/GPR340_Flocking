#include "VFormation.h"
#include "FormationManager.h"
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"

#include <algorithm>

Vector2 VFormationRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
    Vector2 FormationForce;

    //Ugly, I know, but so is half this implementation
    for (FormationManager* form : this->world->formations)
    {
        form->pattern->slowDist = slowDist;
    }
    
    //join new formation or stay in current
    if (!neighborhood.empty())
    {
        int currentID = boid->getFormationID();
        Vector2 pos = boid->getPosition();

        //check if formation ID needs to be changed
        {
            int closeForm = -1; //closest formation
            int dist = boid->getDetectionRadius(); //how close closest formation is

            //find closest formation
            for (Boid* neighbor : neighborhood)
            {
                int neighborID = neighbor->getFormationID();
                int neighborDist = Vector2::getDistance(neighbor->getPosition(), pos);

                if (neighborID >= 0 && neighborDist < dist)
                {
                    closeForm = neighborID;
                    dist = neighborDist;
                }
            }

            std::vector<FormationManager*>& formations = this->world->formations;

            if (closeForm < 0) //create new formation if no one nearby regardless of current formation the boid is in, allows for separation
            {
                CreateNewFormation(boid);
            }
            else if (closeForm != currentID) //if IDs don't match, add boid to new formation
            {
                formations[closeForm]->AddBoid(boid);
            }
            //else, closest formation is the formation the boid is currently in, do nothing
        }

        //if boid did not join new formation, move, otherwise the boid's target isn't set yet so they can't move towards slot
        if (currentID == boid->getFormationID())
        {
            Vector2 displacement = boid->target.position - pos;
            float mag = displacement.getMagnitude();

            if (mag > maxDist) //if too far away from formation, leave it
            {
                CreateNewFormation(boid);
            }
            else
            {
                FormationForce = displacement.normalized();

                //boids slow down as they get close to target, allows others to catch up
                if (mag < slowDist)
                {
                    float slowFactor = (mag / slowDist);
                    slowFactor = std::clamp(slowFactor, .5f, 1.f);
                    boid->setSpeed(boid->getMaxSpeed() * slowFactor);
                }
                else
                {
                    boid->setSpeed(boid->getMaxSpeed());
                }
            }
        }
    }

    return FormationForce;
}

void VFormationRule::CreateNewFormation(Boid* boid)
{
    std::vector<FormationManager*>& formations = this->world->formations;
    int currentID = boid->getFormationID();

    //remove from old formation if not close
    if (currentID >= 0)
    {
        formations[currentID]->RemoveBoid(boid);
    }

    int index = formations.size();
    formations.push_back(new FormationManager(this->world, new VFormation(), index));

    formations[index]->AddBoid(boid);
}

bool VFormationRule::drawImguiRuleExtra() {
    ImGui::SetCurrentContext(world->engine->imGuiContext);
    bool valusHasChanged = false;

    if (ImGui::DragInt("Slow Distance", &slowDist, 0.05f)) {
        valusHasChanged = true;
    }

    return valusHasChanged;
}




int VFormation::calculateNumberOfSlots(std::vector<SlotAssignment*> slotAssignments)
{
    int filledSlots = 0;

    for (SlotAssignment* assign : slotAssignments)
    {
        if (assign->slotNumber >= maxSlotNumber)
        {
            filledSlots = assign->slotNumber;
        }
    }

    return filledSlots + 1;
}

//drift offset when characters are in the set of slots
Static VFormation::GetDriftOffset(std::vector<SlotAssignment*> slotAssignments)
{
    Static result;
    for (int i = 0; i < slotAssignments.size(); i++)
    {
        Static location = GetSlotLocation(slotAssignments[i]->slotNumber, slotAssignments.size());
        result.position += location.position;
        result.orientation += location.orientation;
    }

    int totalAssignments = slotAssignments.size();
    result.position /= totalAssignments;
    result.orientation /= totalAssignments;

    result.position += Vector2::getVector2FromRadian(result.orientation) * slowDist;

    return result;
}

//return location of a slot index relative to center of mass
Static VFormation::GetSlotLocation(int slotNumber, int totalSlots)
{
    Static result;
    int row = NextTriangularRoot(slotNumber); //doubles as both the row and the number of slots in the row

    //uses sum of first n natural numbers equation
    //position in current row, i.e. 5 is (5 - ((3 - 1) * (3) * .5)) = 5 - (6 * .5) = 2, position 2 in row 3
    int rowPos = slotNumber - ((row - 1) * (row) * .5); 

    float middleRow = NextTriangularRoot(totalSlots - 1) / 2.;

    //center is in the middle of the formation, not the front
    //calculate y position relative to center of formation
    result.position.y = (row - middleRow) * minDistance;
    
    //calculate position in the middle of row
    float middleOfRow = (row / 2.) - .5f;

    //calculate x position relative to center of row
    result.position.x = (rowPos - middleOfRow) * minDistance;

    result.orientation = 0;

    return result;
}

//can pattern support <slotCount> slots?
bool VFormation::SupportsSlots(int slotCount)
{
    if (slotCount <= maxSlotNumber)
    {
        return true;
    }

    return false;
}

float VFormation::NextTriangularRoot(int num)
{
    return std::floor((std::sqrt((8 * num) + 1) - 1) / 2) + 1;
}