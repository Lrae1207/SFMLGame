// if stuff doesn't work:
//https://www.youtube.com/watch?v=YfMQyOw1zik

// Internal function
#include "InternalGameFunction.hpp"

int main() {
	engine::Game game;

	// Create an instance of each internal game function object

	/* UI */
	intern::PauseMenu pauseMenu(&game);
	game.debugLog("Loaded GameObject(pauseMenu)", LOG_GREEN);

	/* 2D Objects */
	intern::Planet earth(&game,49700,5000,sf::Vector2f(400.0f,50000.0f));
	earth.setLandColor(sf::Color(0,100,0));
	game.debugLog("Loaded GameObject(earth)", LOG_GREEN);
	intern::GameManager manager(&game);
	manager.registerPlanet(&earth);
	game.debugLog("Loaded GameObject(manager)", LOG_GREEN);
	game.setManager(static_cast<void*>(&manager));
	intern::Rocket rocket(&game);
	game.debugLog("Loaded GameObject(rocket)", LOG_GREEN);
	intern::PlayerControl playerController(&game,&rocket);
	game.debugLog("Loaded GameObject(playerController)", LOG_GREEN);

	// Initiation code
	engine::PolygonCollider* collider = new engine::PolygonCollider();
	collider->setIsCircle(false);
	engine::GameObject *g = new engine::GameObject(static_cast<engine::Collider*>(collider));
	intern::Part* p = rocket.makePart(9,100,g);
	engine::ShapeComponent* shape = p->object->getShapeComponent();
	shape->fillColor = sf::Color(100,100,100);
	// rectsize/ need to add points
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