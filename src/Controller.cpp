#include "Controller.hpp"

// Constructor and destructor
Controller::Controller() {
	// Initialize response functions here
}

Controller::~Controller() {
}

/* Calls the response for the given keyup event and updates the state of the key */
void Controller::handleKeyUp(const sf::Keyboard::Key key) {
	// If the key hasn't been added yet
	if (keyDownMap.find(key) == keyDownMap.end()) {
		keyDownMap.insert({ key,false });
	}
	else {
		keyDownMap[key] = false;
	}
}

/* Calls the response for the given keydown event and updates the state of the key */
void Controller::handleKeyDown(const sf::Keyboard::Key key) {
	// If the key hasn't been added yet
	if (keyDownMap.find(key) == keyDownMap.end()) {
		keyDownMap.insert({ key,true });
	}
	else {
		keyDownMap[key] = true;
	}
}

bool Controller::getKeyDown(const sf::Keyboard::Key key) {
	if (keyDownMap.find(key) == keyDownMap.end()) {
		return false;
	}
	return keyDownMap[key];
}

bool Controller::getKeyUp(const sf::Keyboard::Key key) {
	if (keyDownMap.find(key) == keyDownMap.end()) {
		return true;
	}
	return !keyDownMap[key];
}


/* Calls the response for a mouse update event */
void Controller::handleMouseMove(const sf::Vector2i mouseState) {
	if (mouseMoveHandler != nullptr) {
		void (*f)(sf::Vector2i) = static_cast<void(*)(sf::Vector2i)>(mouseMoveHandler);
		(*f)(mouseState);
	}
}