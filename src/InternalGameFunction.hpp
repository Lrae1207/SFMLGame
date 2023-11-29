#pragma once
#include "Game.hpp" // Engine
#include "Physics.hpp"

/*
	This file contains definitions for the function of the game utilizing the engine. Keep each individual function inside of a class to make management and organization easier.
*/

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

// These two classes reference eachother
class Rocket;
class Part;

class Rocket {
private:
	long long dTime = 0;
	Game* game;

	std::vector<Part*> parts = {};

	bool isThrust = false;

	// Physics
	sf::Vector2f netForce = sf::Vector2f(0.0f, 0.0f);
	float totalThrust;
	float totalWeight;

	GameObject* parentObject;

public:
	Rocket(Game* engine);
	void toggleThrust() { isThrust = !isThrust; game->debugLog("rocketThrust : " + boolToString(isThrust), LOG_CYAN); }
	void update();
	Part* makePart(float thrust, float weight, GameObject* obj);
	Part* instantiatePart(PartPrefab prefab);
	void registerPart(Part* p);
};

class Part {
public:
	float thrust;
	sf::Vector2f thrustDirection;
	sf::Vector2f offset; // from parent/rocket

	float weight;
	float durability = 100;

	GameObject* object;

	Rocket* rocket;
	Part(Rocket* r) { rocket = r; }
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
	Planet(Game *engine,float r, float m, sf::Vector2f pos);
	void setFixed(bool fixed) { isFixed = fixed; }

	void setLandColor(sf::Color color) { landColor = color;	obj->getShapeComponent()->fillColor = color; }
	sf::Color getLandColor() { return landColor; }

	void setAtmosphereColor(sf::Color color) { atmosphereColor = color; }
	sf::Color getAtmosphereColor() { return atmosphereColor; }

	void update();
};