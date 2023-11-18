#include "InternalGameFunction.hpp"
//https://stackoverflow.com/questions/54518519/c-setter-doesnt-change-variable-value

/* PlayerControl definitions */

void PlayerControl::updatePlayer() {
	// For some reason this isn't setting the position
	playerObj->getTransform().setPosition(this->position);
	sf::Vector2f v = playerObj->getTransform().getPosition();

	/* Update Collider */
	Collider* col = playerObj->getCollider();
	if (col != nullptr) {
		Rect newCollider;

		sf::Vector2f offset = playerObj->getTransform().getPosition();
		sf::Vector2f size = playerObj->getShapeComponent().rectSize;

		newCollider.left = offset.x - size.x / 2;
		newCollider.top = offset.y - size.y / 2;
		newCollider.right = newCollider.left + size.x;
		newCollider.bottom = newCollider.top + size.y;
		static_cast<RectCollider*>(col)->setCollider(newCollider);
	}

	game->playerPosition = playerObj->getTransform().getPosition();
}

void PlayerControl::onADown() {

}

void PlayerControl::onDDown() {
	
}

void PlayerControl::onSpaceDown() {
	if (!spaceHeld) {
		rocket->toggleThrust();
	}
}

PlayerControl::PlayerControl(Game *engine, Rocket* r) {
	game = engine;
	rocket = r;
	init();
}

void PlayerControl::init() {
	inputForce = 0.5f;
	drag = 0.1;
	position = sf::Vector2f(80.0f, 200.0f);
	playerObj = game->makeObject(static_cast<Collider*>(new RectCollider(0, 0, 100, 100)));
	playerObj->getTransform().position = sf::Vector2f(100,100);
	playerObj->getShapeComponent().rectSize = sf::Vector2f(100, 250);
	playerObj->getShapeComponent().fillColor = C_LGRAY1;
}


void PlayerControl::update() {
	// Handle inputs

	if (game->controller.getKeyDown(KEYCODESPACE)) {
		onSpaceDown();
	} else {
		spaceHeld = false;
	}
	if (game->controller.getKeyDown(KEYCODEA) || game->controller.getKeyDown(KEYCODELEFTARROW)) {
		onADown();
	}
	if (game->controller.getKeyDown(KEYCODED) || game->controller.getKeyDown(KEYCODERIGHTARROW)) {
		onDDown();
	}

	// Crouching
	if (game->controller.getKeyDown(KEYCODELCNTRL) || game->controller.getKeyDown(KEYCODERCNTRL)) {
		playerObj->getTransform().setSize(sf::Vector2f(1.0f, 0.5f));
	} else {
		// Sprinting
		playerObj->getTransform().setSize(sf::Vector2f(1.0f, 1.0f));
		if (game->controller.getKeyDown(KEYCODELSHIFT) || game->controller.getKeyDown(KEYCODERSHIFT)) {
			inputForce = 1.50f;
		}
		else {
			inputForce = 0.5f;
		}
	}

	// Scale velocity down by a factor of drag
	updatePlayer();
}

Rocket::Rocket(Game *engine) {
	game = engine;
}

Menu::Menu(Game* engine) {
	game = engine;
}
