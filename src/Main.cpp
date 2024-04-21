// if stuff doesn't work:
//https://www.youtube.com/watch?v=YfMQyOw1zik

// Internal function
#include "InternalGameFunction.hpp"

/* I wouldn't mess with this file too much */
int main() {
	/* 2 parameters are the framerate cap and physics update cap */
	engine::Game game(60,120);

	/* Parameter is the path of a scene to load */
	//game.loadScene("../scenes/game_scene.json");

	game.start();

	// While the game is active
	while (game.windowActive()) {
		// Update and render game
		game.update();
		game.render();
	}

	return 0;
}