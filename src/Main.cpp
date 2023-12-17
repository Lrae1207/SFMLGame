// if stuff doesn't work:
//https://www.youtube.com/watch?v=YfMQyOw1zik

// Internal function
#include "InternalGameFunction.hpp"

int main() {
	Game game;

	// Create an instance of each internal game function object

	/* UI */
	PauseMenu pauseMenu(&game);
	game.debugLog("Loaded GameObject(pauseMenu)", LOG_GREEN);

	/* 2D Objects */
	Planet earth(&game,99700,100,sf::Vector2f(400.0f,100000.0f));
	earth.setLandColor(sf::Color(0,100,0));
	game.debugLog("Loaded GameObject(earth)", LOG_GREEN);
	GameManager manager(&game);
	manager.registerPlanet(&earth);
	game.debugLog("Loaded GameObject(manager)", LOG_GREEN);
	game.setManager(static_cast<void*>(&manager));
	Rocket rocket(&game);
	game.debugLog("Loaded GameObject(rocket)", LOG_GREEN);
	PlayerControl playerController(&game,&rocket);
	game.debugLog("Loaded GameObject(playerController)", LOG_GREEN);

	// Initiation code
	RectCollider* collider = new RectCollider(0,0,100,100);
	collider->isCircle = false;
	GameObject *g = new GameObject(static_cast<Collider*>(collider));
	Part* p = rocket.makePart(1,1000,g);
	ShapeComponent* shape = p->object->getShapeComponent();
	shape->fillColor = sf::Color(100,100,100);
	shape->rectSize = sf::Vector2f(50,100);
	p->thrustDirection = sf::Vector2f(0,-1);
	
	// While the game is active
	while (game.windowActive()) {
		// Update and render game
		game.update();
		game.render();

		// Call all internal game function object update functions
		playerController.update();
		rocket.update();
		earth.update();
		pauseMenu.update();
	}

	return 0;
}