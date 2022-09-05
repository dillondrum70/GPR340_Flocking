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
                Vector2 displacement = position - neighbor->getPosition();

                //invert magnitude of displacement so closer boids have a stronger pull than further ones
                float mag = displacement.getMagnitude();
                if (mag != 0)
                {
                    //rescale components to new magnitude length, inverse of current magnitude
                    displacement *= (1 / mag) / mag;
                }

                separatingForce += displacement;//doing start - end instead of end - start so it is already negative
            }
        }
        
        //do not divide by 0
        if (countCloseFlockmates != 0)
        {
            //average sum of the positions
            separatingForce = separatingForce / countCloseFlockmates;
        }
    }

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
