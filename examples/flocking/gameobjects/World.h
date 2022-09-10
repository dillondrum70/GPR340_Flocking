#ifndef WORLD_H
#define WORLD_H

#include "../behaviours/FlockingRule.h"
#include "Boid.h"
#include "GameObject.h"

class Particle;

class FormationManager;
class VFormation;

// todo: make world a game engine scene manager
class World: public GameObject {
private:
    /// MEMBERS
    /// Configuration
    // Boids
    int nbBoids = 29;

    bool hasConstantSpeed = false;
    float desiredSpeed = 120.0f;

    bool hasMaxAcceleration = false;
    float maxAcceleration = 10.0f;

    float detectionRadius = 35;

    //Display
    bool showRadius = false;
    bool showRules = false;
    bool showAcceleration = false;

    //rules
    std::vector<std::unique_ptr<FlockingRule>> boidsRules;
    // todo: make this a vector
    std::vector<float> defaultWeights; // array of default rules weight

    /// METHODS
    void initializeRules();

    ~World() { ClearFormations(); }

    void ClearFormations()
    {
        for (FormationManager* form : formations)
        {
            delete form;
        }

        formations.clear();
    }

    //Boids

    void applyFlockingRulesToAllBoids();
    void setNumberOfBoids(int number);
    void randomizeBoidPositionAndVelocity(Boid* boid);
    void warpParticleIfOutOfBounds(Particle* particle);

    Boid* createBoid();

public:
    //cached list to manipulate objects
    std::vector<Boid*> boids;

    //formations
    std::vector<FormationManager*> formations;

    /// METHODS
    explicit World(Engine* pEngine);

    //Getters
    std::vector<Boid*>* getAllBoids();

    int getNbBoids() const;

    /// Setters

    //ImGui
    void drawGeneralUI();
    void drawRulesUI();

    //Update
    void Update(float deltaTime) override;

    void UpdateFormationIDs(); //sets formation IDs to their index in the vector

    void showConfigurationWindow(float deltaTime);
    void drawPerformanceUI(float deltaTime);

    void OnGui(ImGuiContext *context) override;
    void Start() override;
};


#endif
