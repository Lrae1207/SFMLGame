#include "InternalGameFunction.hpp"
//https://stackoverflow.com/questions/54518519/c-setter-doesnt-change-variable-value

/* PlayerControl definitions */

void intern::PlayerControl::onADown() {
	rocket->applyControlRotation(-1.0f);
}

void intern::PlayerControl::onDDown() {
	rocket->applyControlRotation(1.0f);
}

void intern::PlayerControl::onSpaceDown() {
	if (!spaceHeld) {
		rocket->toggleThrust();
	}
	spaceHeld = true;
}

intern::PlayerControl::PlayerControl(engine::Game *engine, Rocket* r) {
	game = engine;
	rocket = r;
	init();
}

void intern::PlayerControl::init() {
	
}

/* Respond to user inputs */
void intern::PlayerControl::update() {
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

void intern::Rocket::applyControlRotation(float force) {
	getTransform()->addRotation(force);
}

/* Create the rocket */
intern::Rocket::Rocket(engine::Game* engine) : engine::GameObject({},engine) {
	objName = "rocket";
	setVisibility(false);

	game = engine;
	manager = static_cast<GameManager*>(game->getManager());

	setVisibility(false);

	game->setCamFocus(dynamic_cast<GameObject*>(this));

	getTransform()->setPosition(sf::Vector2f(400, 300));

	totalThrust = 0;
	totalWeight = 0;

	sf::Vector2f center = game->getWindowSize() / 2.0f;

	gravityLine = new engine::Line();
	velocityLine = new engine::Line();

	gravityLine->start = center;
	gravityLine->thickness = 2.0f;
	gravityLine->color = C_RED;

	velocityLine->start = center;
	velocityLine->thickness = 2.0f;
	velocityLine->color = C_GREEN;

	game->drawLine(velocityLine,true);
	game->drawLine(gravityLine,true);

	game->registerObject(getObject(), false);
}

/* Update the collider and position of the rocket */
void intern::Rocket::update() {
	if (game->isPaused()) { return; }
	dTime += game->getDeltaTime();

	/* Update Colliders */
	totalThrust = 0;
	totalWeight = 0;

	sf::Vector2f newForce = sf::Vector2f(0.0f,0.0f);

	for (Part* p : parts) {
		GameObject* obj = dynamic_cast<GameObject*>(p);
		if (p->durability <= 0) { // Object is destroyed
			game->deleteObject(obj);
			obj->destroy();
			continue;
		}

		engine::Transform rocketTransform = *getTransform();

		/* Update the objects */
		engine::Transform *t = obj->getTransform();
		engine::ShapeComponent* s = obj->getShapeComponent();
		t->setPosition(engine::vmath::addVectors(rocketTransform.getPosition(),p->offset));
		t->setOrigin(rocketTransform.position);
		t->setRotation(rocketTransform.getRotation());
		obj->updateCollider();

		obj->updateCollider();

		/* Calculate the average velocity and weight */
		totalThrust += p->thrust;
		totalWeight += p->weight;
		newForce = engine::vmath::addVectors(newForce, p->thrustDirection);
	}

	// Rotate newForce
	newForce = engine::vmath::rotateByDegrees(newForce, getTransform()->getRotation());

	/*
			F * t
		v = -----
			  m
	*/

	if (isThrust) {
		totalWeight += 0.0001; // Prevents division by 0
		sf::Vector2f v = engine::vmath::multiplyVector(newForce, thrustScaler * game->getTimescale() / totalWeight);
		velocity = engine::vmath::addVectors(velocity,v);
	}

	// Gravity
	Planet* closestPlanet = manager->getNearestPlanet(getTransform()->getPosition());
	if (closestPlanet == nullptr) {
		return;
	}

	sf::Vector2f rocketPos = getTransform()->getPosition();
	sf::Vector2f planetPos = closestPlanet->getObject()->getTransform()->getPosition();
	float distFromPlanet = phys::distance2D(planetPos.x, planetPos.y, rocketPos.x, rocketPos.y);
	float altitude = distFromPlanet - closestPlanet->getRadius();

	sf::Vector2f gravityDirection = engine::vmath::normalizeVector(engine::vmath::subtractVectors(planetPos, rocketPos));
	float gravityMagnitude = phys::calculateGravityAccel(rocketPos.x, rocketPos.y, planetPos.x, planetPos.y, closestPlanet->getMass());

	if (altitude > 0) {
		velocity = engine::vmath::addVectors(velocity, engine::vmath::multiplyVector(gravityDirection, gravityMagnitude * game->getTimescale()));
	}

	float drag = pow(1 - (1 / (distFromPlanet+0.000000001)),3);
	velocity = engine::vmath::multiplyVector(velocity,drag);

	if (distFromPlanet < closestPlanet->getRadius()) {
		game->setBackgroundBrightness(1.0f);
	} else {
		game->setBackgroundBrightness(powf(1.00001, -5*(distFromPlanet-closestPlanet->getRadius())));
	}

	getTransform()->addToPosition(velocity);

	// Debug logging
	if (dTime > 500000000) { // 2 second(s)
		game->debugLog("rocket::transform::position : (" + std::to_string(getTransform()->getPosition().x) + "," + std::to_string(getTransform()->getPosition().y) + ")", LOG_YELLOW);
		game->debugLog("rocket::velocity : (" + std::to_string(velocity.x) + "," + std::to_string(velocity.y) + ")", LOG_YELLOW);
		game->debugLog("rocket::altitude: " + std::to_string(altitude), LOG_YELLOW);
		dTime = 0;
	}

	// Info lines
	velocityLine->end = velocityLine->start + engine::vmath::normalizeVector(velocity);
	gravityLine->end = gravityLine->start + gravityDirection;
}

/* Create and register a new part with the given fields */
intern::Part* intern::Rocket::makePart(float thrust, float weight, engine::GameObject * obj) {
	Part* newPart = new Part(this);
	newPart->thrust = thrust;
	newPart->weight = weight;
	engine::Rect shape;
	shape.top = -50;
	shape.bottom = 50;
	shape.left = -25;
	shape.right = 25;
	newPart->objName = "rocketPart";

	std::vector<sf::Vector2f> newPolygon = engine::rectToPolygon(shape);
	GameObject* partObj = dynamic_cast<GameObject*>(newPart);
	partObj->getCollider()->setPolygon(newPolygon);
	partObj->getShapeComponent()->vertices = newPolygon;
	game->registerObject(partObj, false);

	if (thrust != 0) {
		newPart->particleManager = new RocketParticleManager(game,1.0f,2.0f,1.0f,0.9f,newPart);
		newPart->particleManager->setStartPosition(newPart->offset);
	}

	parts.push_back(newPart);

	return newPart;
}

/* Create and register a new part using a prefab */
intern::Part* intern::Rocket::instantiatePart(PartPrefab prefab) {
	Part *newPart = new Part(this);
	newPart->thrust = prefab.thrust;
	GameObject* obj = dynamic_cast<GameObject*>(newPart);
	game->registerObject(obj, false);
	newPart->objName = "rocketPart";
	newPart->durability = prefab.durability;
	newPart->weight = prefab.weight;

	return newPart;
}

/* Register an already made part */
void intern::Rocket::registerPart(Part* p) {
	p->rocket = this;
	parts.push_back(p);
}

/* Create the pause menu and its objects */
intern::PauseMenu::PauseMenu(engine::Game* engine) {

	game = engine;
	engine::Rect backgroundShape;

	sf::Vector2f windowSize = game->getWindowSize();
	sf::Vector2f margin;
	margin.x = 300.0f;
	margin.y = 200.0f;

	backgroundShape.top = margin.y;
	backgroundShape.bottom = windowSize.y - margin.y;
	backgroundShape.left = margin.x;
	backgroundShape.right = windowSize.x - margin.x;
	background = game->makeObject(1,engine::rectToPolygon(backgroundShape),true);
	background->objName = "pauseBackground";

	background->setVisibility(false);
	background->getShapeComponent()->fillColor = engine::changeAlpha(C_DGRAY1,200);

	background->getTransform()->setPosition(sf::Vector2f(0,0));
	background->removeCollider();
	game->registerObject(background,true);

	if (pausedFont.loadFromFile("Raleway-Regular.ttf")) {
		game->debugLog("Loaded \"Raleway-Regular.ttf\"\n", LOG_GREEN);
	} else {
		game->debugLog("Failed load \"Raleway-Regular.ttf\"\n", LOG_RED);
	}

	sf::Text temp;
	temp.setFont(pausedFont);
	temp.setCharacterSize(24);
	temp.setString("Paused");

	sf::FloatRect textRect = temp.getLocalBounds();
	sf::Vector2f localOffset = sf::Vector2f(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	pausedMessage = new engine::Text(sf::Color(255, 0, 0), sf::Vector2f(windowSize.x / 2, margin.y + 15), localOffset, "Paused", 24, &pausedFont);

	pausedMessage->layer = 0;
	pausedMessage->cullThis = true;
	game->drawText(pausedMessage, true);
}

/* Show if the game is paused; Hide otherwise */
void intern::PauseMenu::update() {
 	if (game->isPaused()) { // Show menu objects
		background->getShapeComponent()->isVisible = (true);
		pausedMessage->cullThis = false;
	} else {
		background->getShapeComponent()->isVisible = (false);
		pausedMessage->cullThis = true;
	}
}

/* Create a planet with the given fields */
intern::Planet::Planet(engine::Game* engine, float r, float m, sf::Vector2f pos) : engine::GameObject({sf::Vector2f(0,r)}, engine) {
	game = engine;
	radius = r;
	mass = m;

	makeCircle(true);

	col = getCollider();
	getShapeComponent()->sides = 1000;

	getTransform()->setPosition(pos);

	col->setParent(getObject());
	game->registerObject(getObject(), false);
	objName = "planet";
}

/* Update the planet's collider and other fields */
void intern::Planet::update() {
	if (game->isPaused()) { return; }
}

intern::GameManager::GameManager(engine::Game* engine) {
	planets = {};
	game = engine;
}

void intern::GameManager::registerPlanet(Planet* p) {
	planets.push_back(p);
}

intern::Planet* intern::GameManager::getNearestPlanet(sf::Vector2f position) {
	if (planets.size() == 0) {
		return nullptr;
	}

	int nearestIndex = 0;
	float lowestDistance = std::numeric_limits<float>::max();
	for (int i = 0; i < planets.size(); ++i) {
		Planet* planet = planets[i];
		float d;
		if ((d = engine::vmath::getDistance(planet->getObject()->getTransform()->getPosition(), position) < lowestDistance)) {
			lowestDistance = d;
			nearestIndex = i;
		}
	}

	return planets[nearestIndex];
}

intern::RocketParticleManager::RocketParticleManager(engine::Game* engine, float maxDuration, float perSecond, float speed, float drag, Part* relativeTo) {
	lifespan = maxDuration;
	particlesPerSecond = perSecond;
	game = engine;
	launchSpeed = speed;
	particleDrag = drag;
	parentPart = relativeTo;
}

void intern::RocketParticleManager::update() {
	for (engine::Particle *particle : particles) {
		particle->update();
		if (particle->lifeTime > lifespan) {
			particle->deleteParticle();
		}
	}
}