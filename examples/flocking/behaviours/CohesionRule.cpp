#include "CohesionRule.h"
#include "../gameobjects/Boid.h"

Vector2 CohesionRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
    Vector2 cohesionForce;

    // todo: add your code here to make a force towards the center of mass
    // hint: iterate over the neighborhood

    for (Boid* neighbor : neighborhood)
    {
        cohesionForce += neighbor->getPosition();
    }

    if (neighborhood.size() > 0)
    {
        cohesionForce = cohesionForce / neighborhood.size();
    }

    cohesionForce -= boid->getPosition();

    return cohesionForce;
}