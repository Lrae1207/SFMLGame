#pragma once

#ifndef INTERNALGAMEFUNCTION_HPP
#define INTERNALGAMEFUNCTION_HPP

#include "Game.hpp" // Engine
#include <limits>

/*
	This file contains definitions for the function of the game utilizing the engine. Keep each individual function inside of a class to make management and organization easier.
*/

namespace intern {
	// These classes reference eachother
	class Rocket;
	class Part;
	class GameManager;

	/* Managers */
	class RocketParticleManager {
	private:
		engine::Game* game;

		Part* parentPart;

		float lifespan;
		float lastRelease = 0.0f;
		float particlesPerSecond = 0.0f;

		sf::Vector2f launchDirection = sf::Vector2f(0.0f, 1.0f);
		sf::Vector2f startPosition = sf::Vector2f(0.0f, 0.0f);

		float launchSpeed;
		float particleDrag = 0.9f;
	public:
		std::vector<engine::Particle*> particles = {};
		RocketParticleManager(engine::Game* engine, float maxDuration, float perSecond, float speed, float drag, Part* relativeTo);
		void setStartPosition(sf::Vector2f startPos) { startPosition = startPos; }
		void update();
	};

	/* UI objects */

	class PauseMenu {
	private:
		engine::Game* game;
		engine::GameObject* background;

		sf::Font pausedFont;
		engine::Text *pausedMessage;
	public:
		PauseMenu(engine::Game* engine);
		void update();
	};

	/* 2D game objects */

	struct PartPrefab {
		float thrust;
		float weight;
		float durability;
		std::vector<sf::Vector2f> polygon;
	};

	class Rocket : public engine::GameObject {
	private:
		long long dTime = 0;
		engine::Game* game;
		GameManager* manager;

		std::vector<Part*> parts = {};

		bool isThrust = false;

		// Physics
		sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);
		sf::Vector2f netForce = sf::Vector2f(0.0f, 0.0f);
		float totalThrust;
		float totalWeight;

		engine::Line *velocityLine;
		engine::Line *gravityLine;
	public:
		float thrustScaler = powf(10, 0);

		void applyControlRotation(float force);

		engine::Game* getEngine() { return game; };
		Rocket(engine::Game* engine);
		void toggleThrust() { isThrust = !isThrust; game->debugLog("rocketThrust : " + engine::boolToString(isThrust), LOG_CYAN); }
		void update();
		Part* makePart(float thrust, float weight, engine::GameObject* obj);
		Part* instantiatePart(PartPrefab prefab);
		void registerPart(Part* p);
		engine::GameObject* getObject() { return dynamic_cast<engine::GameObject*>(this); }
	};

	class Part : public engine::GameObject {
	public:
		RocketParticleManager* particleManager = nullptr;
		float thrust = 0;
		sf::Vector2f thrustDirection;
		sf::Vector2f offset; // from parent/rocket

		float weight = 1;
		float durability = 100;

		Rocket* rocket = nullptr;
		Part(Rocket* r) : engine::GameObject(r->getEngine()) { objName = "rocketPart";  rocket = r; }
		Part(Rocket* r, float t) : engine::GameObject(r->getEngine()) { objName = "rocketPart"; rocket = r; thrust = t; }
	};

	/* Interface for controlling the rocket */
	class PlayerControl {
	private:
		// Engine references
		Rocket* rocket;
		engine::Game* game;

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
		PlayerControl(engine::Game* engine, Rocket* r);

		void update();
	};

	class Planet : public engine::GameObject {
	private:
		engine::PolygonCollider* col;

		engine::Game* game;
		sf::Color landColor;
		sf::Color atmosphereColor;

		bool isFixed = true;

		float radius;
		float mass;
	public:
		float baseAirDensity;

		Planet(engine::Game* engine, float r, float m, sf::Vector2f pos);
		void setFixed(bool fixed) { isFixed = fixed; }

		void setLandColor(sf::Color color) { landColor = color;	getShapeComponent()->fillColor = color; }
		sf::Color getLandColor() { return landColor; }
		void setAtmosphereColor(sf::Color color) { atmosphereColor = color; }
		sf::Color getAtmosphereColor() { return atmosphereColor; }
		engine::GameObject* getObject() { return dynamic_cast<engine::GameObject*>(this); }
		float getMass() { return mass; }
		float getRadius() { return radius; }

		void update();
	};

	class GameManager {
	private:
		engine::Game* game;
		std::vector<Planet*> planets;
	public:
		GameManager(engine::Game* engine);

		void registerPlanet(Planet* p);
		Planet* getNearestPlanet(sf::Vector2f position);
	};
}
#endif