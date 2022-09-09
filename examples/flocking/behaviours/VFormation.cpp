#include "VFormation.h"
#include "../gameobjects/Boid.h"

Vector2 VFormationRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
    Vector2 FormationForce;

    FormationForce = boid->target.position - boid->getPosition();

    return FormationForce;
}

int VFormation::calculateNumberOfSlots(std::vector<SlotAssignment> slotAssignments)
{
    int filledSlots = 0;
    if (SlotAssignment assign : slotAssignments)
    {
        if (assign.slotNumber >= maxSlotNumber)
        {
            filledSlots = assignment.slotNumber;
        }
    }

    return filledSlots + 1;
}

//drift offset when characters are in the set of slots
Static VFormation::GetDriftOffset(std::vector<SlotAssignment> slotAssignments)
{
    Static result;
    for (SlotAssignment assign : slotAssignments)
    {
        location = GetSlotLocation(assign.slotNumber);
        result.postition += location.position;
        result.orientation += location.orientation;
    }

    int totalAssignments = slotAssignments.size();
    result.position /= totalAssignments;
    result.orientation /= totalAssignments;

    return result;
}

//return location of a slot index
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
    return std::floor((std::sqrt((8 * slotNumber) + 1) - 1) / 2) + 1;
}