#pragma once
#include "Game.hpp" // Engine
#include <limits>

/*
	This file contains definitions for the function of the game utilizing the engine. Keep each individual function inside of a class to make management and organization easier.
*/

// These classes reference eachother
class Rocket;
class Part;
class GameManager;

/* Managers */
class RocketParticleManager {
private:
	Game* game;
	
	Part* parentPart;

	float lifespan;
	float lastRelease = 0.0f;
	float particlesPerSecond = 0.0f;
	
	sf::Vector2f launchDirection = sf::Vector2f(0.0f,1.0f);
	sf::Vector2f startPosition = sf::Vector2f(0.0f,0.0f);
	
	float launchSpeed;
	float particleDrag = 0.9f;
public:
	std::vector<Particle*> particles = {};
	RocketParticleManager(Game* engine, float maxDuration, float perSecond, float speed, float drag, Part *relativeTo);
	void setStartPosition(sf::Vector2f startPos) { startPosition = startPos; }
	void update();
};

/* UI objects */

class PauseMenu {
private:
	Game* game;
	GameObject* background;

	sf::Font pausedFont;
	sf::Text pausedText;
public:
	PauseMenu(Game* engine);
	void update();
};

/* 2D game objects */

struct PartPrefab {
	float thrust;
	float weight;
	float durability;
	GameObject object;
};

class Rocket {
private:
	long long dTime = 0;
	Game* game;
	GameManager *manager;

	std::vector<Part*> parts = {};

	bool isThrust = false;

	// Physics
	sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f netForce = sf::Vector2f(0.0f, 0.0f);
	float totalThrust;
	float totalWeight;

	GameObject* parentObject; // Parent of all rocket parts
public:
	float thrustScaler = powf(10,0);

	void applyControlRotation(float force);

	Rocket(Game* engine);
	void toggleThrust() { isThrust = !isThrust; game->debugLog("rocketThrust : " + boolToString(isThrust), LOG_CYAN); }
	void update();
	Part* makePart(float thrust, float weight, GameObject* obj);
	Part* instantiatePart(PartPrefab prefab);
	void registerPart(Part* p);
};

class Part {
public:
	RocketParticleManager* particleManager = nullptr;
	float thrust = 0;
	sf::Vector2f thrustDirection;
	sf::Vector2f offset; // from parent/rocket

	float weight = 1;
	float durability = 100;

	GameObject* object = nullptr;

	Rocket* rocket = nullptr;
	Part(Rocket* r) { rocket = r; }
	Part(Rocket* r, float t) { rocket = r; thrust = t; }
};

/* Interface for controlling the rocket */
class PlayerControl {
private:
	// Engine references
	Rocket* rocket;
	Game* game;

	// Input
	bool spaceHeld = false;

	// Control response functions
	void onSpaceDown();
	void onADown();
	void onDDown();

	// Private functions
	void init();
public:
	// Constructors and Destructors
	PlayerControl(Game* engine, Rocket* r);

	void update();
};

class Planet {
private:
	GameObject* obj;
	RadiusCollider* col;

	Game* game;
	sf::Color landColor;
	sf::Color atmosphereColor;

	bool isFixed = true;

	float radius;
	float mass;
public:
	float baseAirDensity;

	Planet(Game *engine,float r, float m, sf::Vector2f pos);
	void setFixed(bool fixed) { isFixed = fixed; }

	void setLandColor(sf::Color color) { landColor = color;	obj->getShapeComponent()->fillColor = color; }
	sf::Color getLandColor() { return landColor; }
	void setAtmosphereColor(sf::Color color) { atmosphereColor = color; }
	sf::Color getAtmosphereColor() { return atmosphereColor; }
	GameObject* getObject() { return obj; }
	float getMass() { return mass; }
	float getRadius() { return radius; }

	void update();
};

class GameManager {
private:
	Game* game;
	std::vector<Planet*> planets;
public:
	GameManager(Game* engine);

	void registerPlanet(Planet* p);
	Planet* getNearestPlanet(sf::Vector2f position);
};