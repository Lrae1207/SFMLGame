#include "Controller.hpp"

// Constructor and destructor
Controller::Controller() {
	// Initialize response functions here
}

Controller::~Controller() {
}

/* Sets the response for the given keydown event to the given function pointer */
void Controller::setKeyDownEvent(const sf::Keyboard::Key key, void *eventHandler) {
	keyDownEvents.insert({ key,eventHandler });
}

/* Sets the response for the given keyup event to the given function pointer */
void Controller::setKeyUpEvent(const sf::Keyboard::Key key, void *eventHandler) {
	keyUpEvents.insert({key,eventHandler});
	// non functional
}

/* Calls the response for the given keyup event and updates the state of the key */
void Controller::handleKeyUp(const sf::Keyboard::Key key) {
	if (keyUpEvents.find(key) != keyUpEvents.end()) {
		void (*f)() = static_cast<void(*)()>(keyUpEvents[key]);
		(*f)();
	}
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
	if (keyDownEvents.find(key) != keyDownEvents.end()) {
		void (*f)() = static_cast<void(*)()>(keyDownEvents[key]);
		(*f)();
	}
	// If the key hasn't been added yet
	if (keyDownMap.find(key) == keyDownMap.end()) {
		keyDownMap.insert({key,true});
	} else {
		keyDownMap[key] = true;
	}
}

bool Controller::getKeyDown(const sf::Keyboard::Key key) {
	if (keyDownMap.find(key) == keyDownMap.end()) {
		return false;
	}
	return keyDownMap[key];
}


/* Calls the response for a mouse update event */
void Controller::handleMouseMove(const sf::Vector2i mouseState) {
	if (mouseMoveHandler != nullptr) {
		void (*f)(sf::Vector2i) = static_cast<void(*)(sf::Vector2i)>(mouseMoveHandler);
		(*f)(mouseState);
	}
}