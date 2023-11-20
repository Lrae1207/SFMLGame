#include "InternalGameFunction.hpp"
//https://stackoverflow.com/questions/54518519/c-setter-doesnt-change-variable-value

/* PlayerControl definitions */

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
}

Rocket::Rocket(Game *engine) {
	game = engine;
	playerObj = game->makeObject(static_cast<Collider*>(new RectCollider(0, 0, 100, 100)));
	playerObj->getTransform().position = sf::Vector2f(100, 100);
	playerObj->getShapeComponent().rectSize = sf::Vector2f(100, 250);
	playerObj->getShapeComponent().fillColor = C_LGRAY1;
}

void Rocket::update() {
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

PauseMenu::PauseMenu(Game* engine) {
	game = engine;
	background = game->makeObject();

	sf::Vector2f windowSize = game->getWindowSize();
	background->setActive(false);
	background->getShapeComponent().fillColor = changeAlpha(C_DGRAY1,200);

	sf::Vector2f margin;
	margin.x = 450.0f;
	margin.y = 90.0f;

	background->setLayer(0);
	background->getTransform().setPosition(sf::Vector2f(windowSize.x/2,windowSize.y/2));
	background->getShapeComponent().rectSize = sf::Vector2f(windowSize.x-margin.x,windowSize.y-margin.y);// this right here

	if (pausedFont.loadFromFile("Raleway-Regular.ttf")) {
		std::cout << "Loaded \"Raleway-Regular.ttf\"\n";
	} else {
		std::cout << "Failed load \"Raleway-Regular.ttf\"\n";
	}

	pausedText.setCharacterSize(24);
	pausedText.setString("PAUSED");
	pausedText.setFont(pausedFont);

	sf::Vector2f pos = vmath::divideVector(windowSize, 2);
	pos.x = pos.x - (pausedText.getLocalBounds().width / 2);
	pos.y = 50;
	pausedText.setPosition(pos);
}

void PauseMenu::update() {
	if (game->isPaused()) { // Show menu objects
		background->setActive(true);
		game->drawText(pausedText);
;	} else {
		background->setActive(false);
	}
}

Planet::Planet(Game* engine, float r, float m, sf::Vector2f pos) {
	game = engine;
	radius = r;
	mass = m;

	col = new RadiusCollider(r, pos.x, pos.y);
	col->isCircle = true;

	obj = game->makeObject(col);
	obj->getShapeComponent().shapeType = shape_type::circle;
	obj->getShapeComponent().circleRadius = r;

	obj->getTransform().setPosition(pos);
}

void Planet::update() {
	/* Update collider */

	col->setCenter(obj->getTransform().position);
	col->setRadius(radius);
}