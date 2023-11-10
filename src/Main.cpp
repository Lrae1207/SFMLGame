// if stuff doesn't work:
//https://www.youtube.com/watch?v=YfMQyOw1zik

// Internal function
#include "InternalGameFunction.hpp"


int main() {
	Game game;
	
	// Create an instance of each internal game function object 
	PlayerControl playerController(&game);
	HealthBar healthBar(&game);

	// While the game is active
	while (game.windowActive()) {
		// Update and render game
		game.update();
		game.render();

		// Call all internal game function object update functions
		playerController.update();
		healthBar.update();
	}
	return 0;
}