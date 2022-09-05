#include "SeparationRule.h"
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"

#include <algorithm>

Vector2 SeparationRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
    //Try to avoid boids too close
    Vector2 separatingForce = Vector2::zero();

    float desiredDistance = desiredMinimalDistance;

    // todo: implement a force that if neighbor(s) enter the radius, moves the boid away from it/them
    if (!neighborhood.empty()) {
        Vector2 position = boid->transform.position;
        int countCloseFlockmates = 0;

        //iterate over close neighbors and calculate sum of positions
        for (Boid* neighbor : neighborhood)
        {
            if (Vector2::getDistance(neighbor->getPosition(), position) < desiredMinimalDistance)
            {
                countCloseFlockmates++;
                separatingForce += position - neighbor->getPosition(); //doing start - end instead of end - start so it is already negative
            }
        }
        
        //do not divide by 0
        if (countCloseFlockmates != 0)
        {
            //average sum of the positions
            separatingForce = separatingForce / countCloseFlockmates;
        }
    }

    float xComp = separatingForce.x, yComp = separatingForce.y;
    
    if (xComp != 0)
    {
        xComp = 1 / xComp;
    }
    else
    {
        xComp = 1;
    }
    if (yComp != 0)
    {
        yComp = 1 / yComp;
    }
    else
    {
        yComp = 1;
    }
    
    separatingForce = Vector2(xComp, yComp);
    separatingForce = Vector2::normalized(separatingForce);

    return separatingForce;
}

bool SeparationRule::drawImguiRuleExtra() {
    ImGui::SetCurrentContext(world->engine->imGuiContext);
    bool valusHasChanged = false;

    if (ImGui::DragFloat("Desired Separation", &desiredMinimalDistance, 0.05f)) {
        valusHasChanged = true;
    }

    return valusHasChanged;
}
