#pragma once

// Precompile later
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

#include <map>


/*
	Class for controlling user inputs and facilitating interactions between user and program.
*/

class Controller {
private:
	// This is a map containing all the needed inputs and whether they were pressed between the current and last update
	std::map<sf::Keyboard::Key, bool> keyDownMap;
	// Private functions
public:
	// Constructor and destructors
	Controller();
	~Controller();

	// Key state find functions for other classes
	void handleKeyUp(const sf::Keyboard::Key key);
	void handleKeyDown(const sf::Keyboard::Key key);

	// Get key down
	bool getKeyDown(const sf::Keyboard::Key key);
	bool getKeyUp(const sf::Keyboard::Key key);

	void* mouseMoveHandler;
	// Calls an external mouse update handler function
	void handleMouseMove(const sf::Vector2i mousePos);
	// Add key hold handling
};