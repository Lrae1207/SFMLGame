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
	spaceHeld = true;
}

PlayerControl::PlayerControl(Game *engine, Rocket* r) {
	game = engine;
	rocket = r;
	init();
}

void PlayerControl::init() {
	
}

/* Respond to user inputs */
void PlayerControl::update() {
	if (game->isPaused()) { return; }
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

/* Create the rocket */
Rocket::Rocket(Game *engine) {
	game = engine;
	parentObject = game->makeObject();
	parentObject->setVisibility(false);

	Transform *t = parentObject->getTransform();
	t->setPosition(sf::Vector2f(400, 300));
	
	totalThrust = 0;
	totalWeight = 0;
}

/* Update the collider and position of the rocket */
void Rocket::update() {
	if (game->isPaused()) { return; }
	dTime += game->getDeltaTime();
	if (dTime > 10000000000) { // 1 second(s)
		game->debugLog("rocket::transform::position : (" + std::to_string(parentObject->getTransform()->getPosition().x) + "," + std::to_string(parentObject->getTransform()->getPosition().y) + ")", LOG_YELLOW);
		dTime = 0;
	}
	/* Update Colliders */
	totalThrust = 0;
	totalWeight = 0;

	for (Part* p : parts) {
		if (p->durability <= 0) { // Object is destroyed
			game->removeObject(p->object);
			delete p;
			continue;
		}

		Transform rocketTransform = *parentObject->getTransform();

		/* Update the objects */
		Transform *t = p->object->getTransform();
		ShapeComponent* s = p->object->getShapeComponent();
		t->setPosition(vmath::addVectors(rocketTransform.getPosition(),p->offset));

		p->object->updateCollider();

		/* Calculate the average velocity and weight */
		totalThrust += p->thrust;
		totalWeight += p->weight;
		netForce = vmath::addVectors(netForce, p->thrustDirection);
	}

	/*
			F * t
		v = -----
			  m
	*/
	if (isThrust) {
		totalWeight += 0.0001;
		sf::Vector2f v = vmath::multiplyVector(netForce, game->getTimescale() / totalWeight);
		parentObject->getTransform()->addToPosition(v);
		int i = 0;
	}

	// Gravity
	phys::calculateGravityAccel(parentObject->getTransform()->getPosition().x, parentObject->getTransform()->getPosition().y,400,10000,100);
}

/* Create and register a new part with the given fields */
Part* Rocket::makePart(float thrust, float weight, GameObject* obj) {
	Part* newPart = new Part(this);
	newPart->thrust = thrust;
	newPart->weight = weight;
	newPart->object = obj;
	parts.push_back(newPart);

	game->registerObject(newPart->object);

	return newPart;
}

/* Create and register a new part using a prefab */
Part* Rocket::instantiatePart(PartPrefab prefab) {
	Part *newPart = new Part(this);
	newPart->thrust = prefab.thrust;
	newPart->object = &prefab.object;
	newPart->durability = prefab.durability;
	newPart->weight = prefab.weight;

	game->registerObject(newPart->object);

	return newPart;
}

/* Register an already made part */
void Rocket::registerPart(Part* p) {
	p->rocket = this;
	parts.push_back(p);
}

/* Create the pause menu and its objects */
PauseMenu::PauseMenu(Game* engine) {
	game = engine;
	background = game->makeObject();

	sf::Vector2f windowSize = game->getWindowSize();
	background->setVisibility(false);
	background->getShapeComponent()->fillColor = changeAlpha(C_DGRAY1,200);

	sf::Vector2f margin;
	margin.x = 450.0f;
	margin.y = 90.0f;

	background->setLayer(0);
	background->getTransform()->setPosition(sf::Vector2f(windowSize.x/2,windowSize.y/2));
	background->getShapeComponent()->rectSize = sf::Vector2f(windowSize.x-margin.x,windowSize.y-margin.y);// this right here

	if (pausedFont.loadFromFile("Raleway-Regular.ttf")) {
		game->debugLog("Loaded \"Raleway-Regular.ttf\"\n", LOG_GREEN);
	} else {
		game->debugLog("Failed load \"Raleway-Regular.ttf\"\n", LOG_RED);
	}

	pausedText.setCharacterSize(24);
	pausedText.setString("PAUSED");
	pausedText.setFont(pausedFont);

	sf::Vector2f pos = vmath::divideVector(windowSize, 2);
	pos.x = pos.x - (pausedText.getLocalBounds().width / 2);
	pos.y = 50;
	pausedText.setPosition(pos);
}

/* Show if the game is paused; Hide otherwise */
void PauseMenu::update() {
	if (game->isPaused()) { // Show menu objects
		background->setVisibility(true);
		game->drawText(pausedText);
;	} else {
		background->setVisibility(false);
	}
}

/* Create a planet with the given fields */
Planet::Planet(Game* engine, float r, float m, sf::Vector2f pos) {
	game = engine;
	radius = r;
	mass = m;

	col = new RadiusCollider(r, pos.x, pos.y);
	col->isCircle = true;

	obj = game->makeObject(col);
	obj->getShapeComponent()->shapeType = shape_type::circle;
	obj->getShapeComponent()->circleRadius = r;

	obj->getTransform()->setPosition(pos);
}

/* Update the planet's collider and other fields */
void Planet::update() {
	if (game->isPaused()) { return; }
	/* Update collider */
	col->setCenter(obj->getTransform()->position);
	col->setRadius(radius);
}