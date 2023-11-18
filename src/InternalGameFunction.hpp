#pragma once
#include "Game.hpp" // Engine

/*
	This file contains definitions for the function of the game utilizing the engine. Keep each individual function inside of a class to make management and organization easier.
*/

/* UI objects */

class Menu {
private:
	Game* game;
	GameObject background;
public:
	Menu(Game* engine);
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
public:
	Rocket(Game* engine);
	void toggleThrust() { isThrust = !isThrust; }
};

/* Controls the gameobject of the character */
class PlayerControl {
private:
	// Engine references
	Rocket* rocket;
	Game* game;

	// Player-reference variables
	GameObject* playerObj;

	// Player information variables
	sf::Vector2f position;
	sf::Vector2f velocity;

	// Input
	bool spaceHeld = false;

	// Force exerted by input keys
	float inputForce;

	// Drag factor
	float drag;

	// Represents the velocity created by user input
	sf::Vector2f inputMovement;

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
	Game* game;

	bool isFixed = true;
	sf::Vector2f position;
public:
	Planet(Game *engine);
};