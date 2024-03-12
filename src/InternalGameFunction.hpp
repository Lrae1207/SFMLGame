#pragma once

#ifndef INTERNALGAMEFUNCTION_HPP
#define INTERNALGAMEFUNCTION_HPP

#include "Game.hpp" // Engine
#include <limits>

/*
	This file contains definitions for the function of the game utilizing the engine. Keep each individual function inside of a class to make management and organization easier.
*/

namespace intern {
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
}
#endif