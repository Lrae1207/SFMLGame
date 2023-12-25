#include "InternalGameFunction.hpp"
//https://stackoverflow.com/questions/54518519/c-setter-doesnt-change-variable-value

/* PlayerControl definitions */

void PlayerControl::onADown() {
	rocket->applyControlRotation(-1.0f);
}

void PlayerControl::onDDown() {
	rocket->applyControlRotation(1.0f);
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

void Rocket::applyControlRotation(float force) {
	parentObject->getTransform()->addRotation(force);
}

/* Create the rocket */
Rocket::Rocket(Game *engine) {
	game = engine;
	manager = static_cast<GameManager*>(game->getManager());

	parentObject = game->makeObject();
	parentObject->setVisibility(false);

	game->setCamFocus(parentObject);

	Transform *t = parentObject->getTransform();
	t->setPosition(sf::Vector2f(400, 300));

	totalThrust = 0;
	totalWeight = 0;
}

/* Update the collider and position of the rocket */
void Rocket::update() {
	if (game->isPaused()) { return; }
	dTime += game->getDeltaTime();

	/* Update Colliders */
	totalThrust = 0;
	totalWeight = 0;

	sf::Vector2f newForce = sf::Vector2f(0.0f,0.0f);

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
		t->setOrigin(rocketTransform.position);
		t->setRotation(rocketTransform.getRotation());
		p->object->updateCollider();

		p->object->updateCollider();

		/* Calculate the average velocity and weight */
		totalThrust += p->thrust;
		totalWeight += p->weight;
		newForce = vmath::addVectors(newForce, p->thrustDirection);
	}

	// Rotate newForce
	newForce = vmath::rotateByDegrees(newForce, parentObject->getTransform()->getRotation());
	/*
			F * t
		v = -----
			  m
	*/
	if (isThrust) {
		totalWeight += 0.0001; // Prevents division by 0
		sf::Vector2f v = vmath::multiplyVector(newForce, thrustScaler * game->getTimescale() / totalWeight);
		velocity = vmath::addVectors(velocity,v);
	}

	// Gravity
	Planet* closestPlanet = manager->getNearestPlanet(parentObject->getTransform()->getPosition());

	sf::Vector2f rocketPos = parentObject->getTransform()->getPosition();
	sf::Vector2f planetPos = closestPlanet->getObject()->getTransform()->getPosition();
	
	sf::Vector2f gravityDirection = vmath::normalizeVector(vmath::subtractVectors(planetPos,rocketPos));
	float gravityMagnitude = phys::calculateGravityAccel(rocketPos.x, rocketPos.y,planetPos.x,planetPos.y,closestPlanet->getMass());

	game->debugLine(rocketPos,vmath::addVectors(gravityDirection,rocketPos));

	gravityDirection = vmath::multiplyVector(gravityDirection,gravityMagnitude*game->getTimescale());
	velocity = vmath::addVectors(velocity,gravityDirection);

	float distFromPlanet = phys::distance2D(planetPos.x, planetPos.y, rocketPos.x, rocketPos.y);

	float drag = pow(1 - (1 / (distFromPlanet+0.000000001)),3);
	velocity = vmath::multiplyVector(velocity,drag);

	if (distFromPlanet < closestPlanet->getRadius()) {
		game->setBackgroundBrightness(1.0f);
	} else {
		game->setBackgroundBrightness(powf(1.00001, -5*(distFromPlanet-closestPlanet->getRadius())));
	}

	parentObject->getTransform()->addToPosition(velocity);

	if (dTime > 500000000) { // 2 second(s)
		game->debugLog("rocket::transform::position : (" + std::to_string(parentObject->getTransform()->getPosition().x) + "," + std::to_string(parentObject->getTransform()->getPosition().y) + ")", LOG_YELLOW);
		game->debugLog("rocket::velocity : (" + std::to_string(velocity.x) + "," + std::to_string(velocity.y) + ")", LOG_YELLOW);
		game->debugLog("rocket::altitude: " + std::to_string(distFromPlanet-closestPlanet->getRadius()), LOG_YELLOW);
		dTime = 0;
	}
}

/* Create and register a new part with the given fields */
Part* Rocket::makePart(float thrust, float weight, GameObject* obj) {
	Part* newPart = new Part(this);
	newPart->thrust = thrust;
	newPart->weight = weight;
	newPart->object = obj;
	if (thrust != 0) {
		newPart->particleManager = new RocketParticleManager(game,1.0f,2.0f,1.0f,0.9f,newPartd);
		newPart->particleManager->setStartPosition(newPart->offset);
	}

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
	col->setPoints(1000);
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

GameManager::GameManager(Game* engine) {
	planets = {};
	game = engine;
}

void GameManager::registerPlanet(Planet* p) {
	planets.push_back(p);
}

Planet* GameManager::getNearestPlanet(sf::Vector2f position) {
	if (planets.size() == 0) {
		return nullptr;
	}

	int nearestIndex = 0;
	float lowestDistance = std::numeric_limits<float>::max();
	for (int i = 0; i < planets.size(); ++i) {
		Planet* planet = planets[i];
		float d;
		if ((d = vmath::getDistance(planet->getObject()->getTransform()->getPosition(), position) < lowestDistance)) {
			lowestDistance = d;
			nearestIndex = i;
		}
	}

	return planets[nearestIndex];
}

RocketParticleManager::RocketParticleManager(Game* engine, float maxDuration, float perSecond, float speed, float drag, Part* relativeTo) {
	lifespan = maxDuration;
	particlesPerSecond = perSecond;
	game = engine;
	launchSpeed = speed;
	particleDrag = drag;
	parentPart = relativeTo;
}

void RocketParticleManager::update() {
	for (Particle *particle : particles) {
		particle->update();
		if (particle->lifeTime > lifespan) {
			particle->deleteParticle();
		}
	}
}