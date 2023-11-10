#pragma once
#include "Game.hpp" // Engine

/*
	This file contains definitions for the function of the game utilizing the engine. Keep each individual function inside of a class to make management and organization easier.
*/

/* UI objects */

/* Represents the health bar for the player */
class HealthBar {
private:
	// Engine references
	Game* game;

	// Obejct variables
	GameObject* bottomBar;
	ShapeComponent *bottomBarShape;
	Transform *bottomBarTransform;
	GameObject* topBar;
	ShapeComponent *topBarShape;
	Transform *topBarTransform;

	// Health bar information variables
	float baseWidth;
	float maxHealth;
	float healthPoints;

	void init();
public:
	HealthBar(Game* engine);
	
	void changeHealth(float amount);
	void changeMaxHealth(float amount);
	void update();
};

/* 2D game objects */

/* Controls the gameobject of the character */
class PlayerControl {
private:
	// Engine references
	Game *game;

	// Player-reference variables
	GameObject* playerObj;

	// Player information variables
	sf::Vector2f position;
	sf::Vector2f velocity;

	// Force exerted by input keys
	float inputForce;

	// Drag factor
	float drag;

	// Represents the velocity created by user input
	sf::Vector2f inputMovement;

	// Control response functions
	void updatePlayer();
	void onWDown();
	void onADown();
	void onSDown();
	void onDDown();

	// Private functions
	void init();
public:
	// Constructors and Destructors
	PlayerControl(Game *engine);

	void update();
};

/* Contains references to the player and its components for other classes to access in one place */
class Player {
public:
	PlayerControl *controller;
	HealthBar *healthBar;

	Player(PlayerControl *c, HealthBar *hb);

	/* Methods */
	void damagePlayer(float damage);
	void healPlayer(float health);
};

/* Inherited class of an enemy */
class Enemy {
protected:
	Game* game;
	Player* player;
};

class Zombie : Enemy {
private:
	float maxHealth;
	float healthPoints;
public:
	Zombie(Game* engine,Player* p);
};