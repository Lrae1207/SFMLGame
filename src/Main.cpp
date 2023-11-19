// if stuff doesn't work:
//https://www.youtube.com/watch?v=YfMQyOw1zik

// Internal function
#include "InternalGameFunction.hpp"


int main() {
	Game game;

	// Create an instance of each internal game function object

	/* UI */
	PauseMenu pauseMenu(&game);

	/* 2D Objects */
	Planet earth(&game,9700,1000,sf::Vector2f(400.0f,10000.0f));

	earth.setLandColor(sf::Color(0,100,0));

	Planet moon(&game, 10, 1000, sf::Vector2f(400.0f, 400.0f));

	Rocket rocket(&game);
	PlayerControl playerController(&game,&rocket);
	// While the game is active
	while (game.windowActive()) {
		// Update and render game
		game.update();
		game.render();

		// Call all internal game function object update functions
		playerController.update();
		earth.update();
		moon.update();
		pauseMenu.update();
	}

	return 0;
}