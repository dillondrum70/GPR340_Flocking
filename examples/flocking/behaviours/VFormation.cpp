#include "VFormation.h"
#include "FormationManager.h"
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"

Vector2 VFormationRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
    Vector2 FormationForce;

    FormationForce = boid->target.position - boid->getPosition();
    
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

            if (closeForm < 0) //create new formation
            {
                std::shared_ptr<FormationManager> manager = std::make_shared<FormationManager>(this->world, std::make_shared<VFormation>());
                this->world->formations.push_back(std::move(manager));
                boid->setFormationID(this->world->formations.size());
            }
            else if(closeForm != currentID) //if IDs don't match, add boid to new formation
            {
                this->world->formations[closeForm]->AddBoid(boid);
            }
            //else, closest formation is the formation the boid is currently in, do nothing
        }

        //if boid did not join new formation, move, otherwise the boid's target isn't set yet so they can't move towards slot
        if (currentID == boid->getFormationID())
        {
            FormationForce = boid->target.position - pos;
        }
    }

    return FormationForce;
}

int VFormation::calculateNumberOfSlots(std::vector<SlotAssignment> slotAssignments)
{
    int filledSlots = 0;

    for (SlotAssignment assign : slotAssignments)
    {
        if (assign.slotNumber >= maxSlotNumber)
        {
            filledSlots = assign.slotNumber;
        }
    }

    return filledSlots + 1;
}

//drift offset when characters are in the set of slots
Static VFormation::GetDriftOffset(std::vector<std::shared_ptr<SlotAssignment>> slotAssignments)
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

    return result;
}

//return location of a slot index relative to center of mass
Static VFormation::GetSlotLocation(int slotNumber, int totalSlots)
{
    Static result;
    int row = NextTriangularRoot(slotNumber); //doubles as both the row and the number of slots in the row
    int totalRows = NextTriangularRoot(totalSlots);

    //uses sum of first n natural numbers equation
    //position in current row, i.e. 5 is (5 - ((3 - 1) * (3) * .5)) = 5 - (6 * .5) = 2, position 2 in row 3
    int rowPos = slotNumber - ((row - 1) * (row) * .5); 

    int middleRow = std::ceil(totalRows / 2);
    //center is in the middle of the formation, not the front
    //calculate y position relative to center of formation
    if (row > totalRows / 2)
    {
        result.position.y = -1 * (row - middleRow) * minDistance; //negative relative to the center
    }
    else
    {
        result.position.y = (middleRow - row) * minDistance; //positive relative to the center of the formation
    }
    
    //calculate x position relative to center of formation
    int middleOfRow = std::ceil(rowPos);
    if (rowPos < middleOfRow)
    {
        result.position.x = -1 * (middleOfRow - rowPos) * minDistance;
    }
    else
    {
        result.position.x = (rowPos - middleOfRow) * minDistance;
    }

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