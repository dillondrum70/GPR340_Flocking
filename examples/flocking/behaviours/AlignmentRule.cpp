#include "AlignmentRule.h"
#include "../gameobjects/Boid.h"

Vector2 AlignmentRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
    Vector2 averageVelocity = Vector2::zero();

    // todo: add your code here to align each boid in a neighborhood
    // hint: iterate over the neighborhood
    
    //sum the velocities to find the average
    for (Boid* neighbor : neighborhood)
    {
        averageVelocity += neighbor->getVelocity() * getBaseWeightMultiplier();
    }

    /*if (!neighborhood.empty())
    {
        averageVelocity = averageVelocity / neighborhood.size();
    }*/
    

    return Vector2::normalized(averageVelocity);
}