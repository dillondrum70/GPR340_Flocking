#include "CohesionRule.h"
#include "../gameobjects/Boid.h"

Vector2 CohesionRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
    Vector2 cohesionForce;

    // todo: add your code here to make a force towards the center of mass
    // hint: iterate over the neighborhood

    if (!neighborhood.empty())
    {
        //sum positions
        for (Boid* neighbor : neighborhood)
        {
            cohesionForce += neighbor->getPosition();
        }

        //average the sum
        cohesionForce = cohesionForce / neighborhood.size();

        //subtract current postion to find vector pointing towards the center
        cohesionForce -= boid->getPosition();

        cohesionForce = Vector2::normalized(cohesionForce);
    }

    return cohesionForce;
}