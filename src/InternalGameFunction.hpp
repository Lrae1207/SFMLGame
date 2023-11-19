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

class Piece {
private:
public:
};

class Rocket {
private:
	Game* game;

	std::vector<Piece*> pieces = {};

	bool isThrust = true;

	// Physics
	sf::Vector2f velocity = {0.0f,0.0f};
	float totalThrust;
	float totalWeight;
public:
	Rocket(Game* engine);
	void toggleThrust() { isThrust = !isThrust; }
};

/* Interface for controlling the rocket */
class PlayerControl {
private:
	// Engine references
	Rocket* rocket;
	Game* game;

	// Player-reference variables
	GameObject* playerObj;

	// Input
	bool spaceHeld = false;

	// Control response functions
	void updatePlayer();
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

	void setLandColor(sf::Color color) { landColor = color;	obj->getShapeComponent().fillColor = color; }
	sf::Color getLandColor() { return landColor; }

	void setAtmosphereColor(sf::Color color) { atmosphereColor = color; }
	sf::Color getAtmosphereColor() { return atmosphereColor; }

	void update();
};