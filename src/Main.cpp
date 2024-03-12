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

	// While the game is active
	while (game.windowActive()) {
		// Update and render game
		game.update();
		game.render();
	}

	return 0;
}