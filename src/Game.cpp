#include "Game.hpp"

/* Get the timestamp in nanoseconds */
long long getTimens() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string boolToString(bool b) {
	if (b) {
		return "true";
	} else {
		return "false";
	}
}

//https://www.youtube.com/watch?v=PXnhYBG0AEA

//  Returns a normalized vector (x and y divided by magnitude)
sf::Vector2f vmath::normalizeVector(sf::Vector2f vector) {
	float magnitude = sqrtf(vector.x * vector.x + vector.y * vector.y);

	// This means vector is <0,0>
	if (magnitude == 0) {
		return vector;
	}
	vector = sf::Vector2f(vector.x / magnitude, vector.y / magnitude);
	return vector;
}

float vmath::getMagnitude(sf::Vector2f vector) {
	return sqrtf(vector.x * vector.x + vector.y * vector.y);;
}

sf::Vector2f vmath::addVectors(sf::Vector2f v1, sf::Vector2f v2) {
	return sf::Vector2f(v1.x + v2.x, v1.y + v2.y);
}

/* Returns v1 - v2 */
sf::Vector2f vmath::subtractVectors(sf::Vector2f v1, sf::Vector2f v2) {
	return sf::Vector2f(v1.x - v2.x, v1.y - v2.y);
}

sf::Vector2f vmath::multiplyVector(sf::Vector2f v1, float i) {
	return sf::Vector2f(v1.x * i, v1.y * i);
}

sf::Vector2f vmath::divideVector(sf::Vector2f v1, float i) {
	return sf::Vector2f(v1.x / i, v1.y / i);
}

sf::Vector2f vmath::utof(sf::Vector2u v1) { 
	return sf::Vector2f(v1.x, v1.y); 
};

/* Default constructor and destructor for ShapeComponent */
ShapeComponent::ShapeComponent() {
	fillColor = sf::Color(255, 0, 255, 255);
	outlineColor = sf::Color(0, 0, 0, 255);
	outlineThickness = 0.0f;
	shapeType = rectangle;
	rotationDegree = 0.0f;
	circleRadius = 1.0f;

	rectSize = sf::Vector2f(50.0f, 50.0f);
	origin = sf::Vector2f(0.0f, 0.0f);
}

ShapeComponent::~ShapeComponent() {

}

/*
	Constructs a drawable RectangleShape from the data held in this class
*/
sf::CircleShape ShapeComponent::constructCircle() {
	sf::CircleShape returnShape = sf::CircleShape(circleRadius, circleRadius/10+10);
	returnShape.setFillColor(fillColor);
	returnShape.setOrigin(sf::Vector2f(circleRadius,circleRadius));
	returnShape.setOutlineColor(outlineColor);
	returnShape.setOutlineThickness(outlineThickness);
	return returnShape;
}

/*
	Constructs a drawable RectangleShape from the data held in this class
*/
sf::RectangleShape ShapeComponent::constructRectangle() {
	sf::RectangleShape returnShape;
	returnShape.setSize(rectSize);
	returnShape.setOrigin(sf::Vector2f(rectSize.x / 2, rectSize.y / 2));
	returnShape.setFillColor(fillColor);
	returnShape.setOutlineColor(outlineColor);
	returnShape.setOutlineThickness(outlineThickness);
	return returnShape;
}

/* Default constructor for TransformComponent */
Transform::Transform() {
	size = sf::Vector2f(1.0f, 1.0f);
	position = sf::Vector2f(0.0f, 0.0f);
	origin = sf::Vector2f(0.0f, 0.0f);

	rotationDegree = 0;
}

/* Sick one liners*/
/* Get attribute functions */
sf::Vector2f Transform::getSize() { return size; };
sf::Vector2f Transform::getPosition() { return position; };
sf::Vector2f Transform::getOrigin() { return origin; };
float Transform::getRotation() { return rotationDegree; };

/* Set attribute functions */
void Transform::setSize(sf::Vector2f newSize) { size = newSize; };
void Transform::setPosition(sf::Vector2f newPosition) {
	position = sf::Vector2f(newPosition.x, newPosition.y);
};
void Transform::setOrigin(sf::Vector2f newOrigin) { origin = newOrigin; };
void Transform::setRotation(float newAngleDegree) { rotationDegree = newAngleDegree; };

/* Add to attribute functions */
void Transform::addToSize(sf::Vector2f newSize) { size = size + newSize; };
void Transform::addToPosition(sf::Vector2f newPosition) { position = position + newPosition; };
void Transform::addToOrigin(sf::Vector2f newOrigin) { origin = origin + newOrigin; };
void Transform::addRotation(float newAngleDegree) { rotationDegree += newAngleDegree; };

/* Default constructor for SpriteComponent */
SpriteComponent::SpriteComponent::SpriteComponent() {

}


/* GameObject constructor and destructor */
GameObject::GameObject() {
	id = 0;
	ShapeComponent shape;
	collider = nullptr;
}

/* GameObject constructor and destructor */
GameObject::GameObject(Collider* col) {
	id = 0;
	transform = Transform();
	shape = ShapeComponent();
	collider = col;
}


GameObject::~GameObject() {

}

/* Public class functions */

Collider* GameObject::getCollider() {
	return collider;
}

void GameObject::updateCollider() {
	if (collider->isCircle) {

	} else {
		RectCollider* c = static_cast<RectCollider*>(collider);
		Rect r;

		r.left = transform.getPosition().x - shape.rectSize.x/2;
		r.top = transform.getPosition().y - shape.rectSize.y/2;
		r.right = transform.getPosition().x + shape.rectSize.x/2;
		r.bottom = transform.getPosition().y + shape.rectSize.y/2;
		c->setCollider(r);

		collider = static_cast<Collider*>(c);
		collider->isCircle = false;
	}
}

/*
	@ return ShapeComponent
	Returns the shapeComponent
*/
ShapeComponent* GameObject::getShapeComponent() {
	return &shape;
}

/*
	@ return Transform
	Returns the Transform
*/
Transform* GameObject::getTransform() {
	return &transform;
}

/*
	@ return void
	Creates the window
*/
void Game::init() {
	startTime = getTimens();
	timeScale = 1;

	window = nullptr;
	videoMode = sf::VideoMode(800, 600);

	window = new sf::RenderWindow(videoMode, "A game", sf::Style::Titlebar | sf::Style::Close);
	window->setFramerateLimit(120);

	cameraTransform = new Transform();
	cameraTransform->setPosition(sf::Vector2f(0, 0));
	cameraTransform->setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	debugLog("Instantiated Engine(game)", LOG_GREEN);
}

/*
	Constructor for game class
	Anything that happens initially goes here
*/
Game::Game() {
	debugLog("Loaded Engine(game)", LOG_GREEN);
	init();
}

/*
	@ return objectRef(unsigned int) - id of new gameobject
	Creates a default GameObject.
*/
objectRef Game::makeObjectRef() {
	GameObject* newObject = new GameObject();
	newObject->id = nextId;
	nextId++;
	gameObjects.push_back(newObject);
	return newObject->id;
}

/*
	@ return GameObject
	Creates a default GameObject.
*/
GameObject* Game::makeObject() {
	GameObject* newObject = new GameObject();
	newObject->id = nextId;
	nextId++;
	gameObjects.push_back(newObject);
	return newObject;
}

/*
	@ return GameObject
	Creates a default GameObject.
*/
GameObject* Game::makeObject(Collider* col) {
	GameObject* newObject = new GameObject(col);
	newObject->id = nextId;
	nextId++;
	gameObjects.push_back(newObject);
	return newObject;
}

/*
	@ return GameObject - gameobject with specified objectId
	Iterates through the gameObject list to find and return the object with the given id.
	If the object isn't found, it returns an object with an id of 0. This id should be checked before use.
*/
GameObject* Game::getObject(objectRef targId) {
	for (GameObject* obj : gameObjects) {
		if (obj->id == targId) {
			return obj;
		}
	}
	GameObject* newObject = new GameObject();
	newObject->id = 0;
	return newObject;
}

void Game::registerObject(GameObject* obj) {
	obj->id = nextId++;
	gameObjects.push_back(obj);
}

void Game::removeObject(GameObject* obj) {
	for (int i = 0; i < gameObjects.size(); ++i) {
		if (gameObjects[i] == obj) {
			gameObjects.erase(gameObjects.begin() + i);
			break;
		}
	}
}


// Destructor
Game::~Game() {
	delete window;
}

/*
	Used for the external game loop to make sure the window isn't open
*/
const bool Game::windowActive() const {
	return window->isOpen();
}


// Update and render functions

/*
	Updates the positions and any needed changes since last update

	- Handles events (such as keypresses)
*/
void Game::update() {
	lastTime = currentTime;
	currentTime = getTimens();

	/* Event polling */
	while (window->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed: // When 'X' in top right is pressed
			window->close();
			break;
		case sf::Event::KeyPressed: // When a key press event is detected hand this off to the controller
			controller.handleKeyDown(event.key.code);
			break;
		case sf::Event::KeyReleased: // When a key release event is detected hand this off to the controller
			controller.handleKeyUp(event.key.code);
			break;
		case sf::Event::MouseMoved:
			controller.handleMouseMove(sf::Mouse::getPosition(*window));
			break;
		}
	}
	/* Input handling */
	if (controller.getKeyDown(KEYCODEF1) && !f1Held) {
		showColliders = !showColliders;
		f1Held = true;
		debugLog("debugMode : " + boolToString(showColliders), LOG_CYAN);
	} else if (!controller.getKeyDown(KEYCODEF1)) {
		f1Held = false;
	}

	if (controller.getKeyDown(KEYCODEESC) && !escHeld) {
		paused = !paused;
		escHeld = true;
		debugLog("paused : " + boolToString(paused), LOG_CYAN);
	} else if (!controller.getKeyDown(KEYCODEESC)) {
		escHeld = false;
	}

	/* Update camera
	if (vmath::getMagnitude(vmath::subtractVectors(playerPosition, vmath::divideVector(cameraTransform->getSize(), 2))) > 1000) {
		cameraTransform->setPosition(vmath::subtractVectors(playerPosition, vmath::divideVector(cameraTransform->getSize(), 2)));
	}
	*/
}

/*
	Renders the window in its current state

	- Clears the screen to a color
	- Renders objects
	- Writes changes to the window
*/
void Game::render() {
	// Clear the screen with this color as argument
	window->clear(sf::Color(100, 100, 100, 255));

	std::vector<sf::CircleShape> circleColliders; // Put these into one later
	std::vector<sf::RectangleShape> rectColliders;

	std::vector<sf::RectangleShape> uiObjects;

	std::vector<sf::CircleShape> circleObjects;
	std::vector<sf::RectangleShape> rectObjects;

	// Render objects by drawing directly on window
	for (GameObject* obj : gameObjects) {
		/* Draw the object */
		Transform transform = *obj->getTransform();
		ShapeComponent shapeComp = *obj->getShapeComponent();

		if (!obj->getVisibility()) { // Skip inactive objects
			continue;
		}

		if (shapeComp.shapeType == shape_type::rectangle) { // Create and set attributes of rectangle shape
			sf::RectangleShape drawShape = shapeComp.constructRectangle();
			if (obj->getLayer() == 0) {
				drawShape.move(transform.getPosition().x, transform.getPosition().y);
				drawShape.rotate(transform.rotationDegree);
				uiObjects.push_back(drawShape);
			}
			else {
				drawShape.move(transform.getPosition().x - cameraTransform->getPosition().x, transform.getPosition().y - cameraTransform->getPosition().y);
				drawShape.rotate(transform.rotationDegree);
				rectObjects.push_back(drawShape);
			}
		} else if (shapeComp.shapeType == shape_type::circle) { // Create and set attributes of circle shape
			sf::CircleShape drawShape = shapeComp.constructCircle();
			if (obj->getLayer() == 0) {
				drawShape.move(transform.getPosition().x, transform.getPosition().y);
			}
			else {
				drawShape.move(transform.getPosition().x - cameraTransform->getPosition().x, transform.getPosition().y - cameraTransform->getPosition().y);
			}
			drawShape.scale(transform.getSize());
			drawShape.rotate(transform.rotationDegree);
			circleObjects.push_back(drawShape);
		}

		/* Add the collider shapes to the buffer */
		Collider* col = obj->getCollider();
		if (showColliders && col != nullptr) {
			if (col->isCircle) { // Create and set attributes of circle collider
				RadiusCollider* collider = static_cast<RadiusCollider*>(col);
				sf::CircleShape drawShape;
				drawShape.setOrigin(sf::Vector2f(collider->getRadius(), collider->getRadius()));
				drawShape.setPosition(sf::Vector2f(collider->getCenter().x - cameraTransform->getPosition().x, collider->getCenter().y - cameraTransform->getPosition().y));
				drawShape.setRadius(collider->getRadius());
				drawShape.setFillColor(sf::Color(0, 0, 0, 0));
				drawShape.setOutlineColor(sf::Color(0, 255, 0));
				drawShape.setOutlineThickness(3.0f);
				circleColliders.push_back(drawShape);
			} else { // Create and set attributes of  rectangle collider
				RectCollider* collider = static_cast<RectCollider*>(col);
				sf::RectangleShape drawShape;
				Rect colliderShape = collider->getCollider();
				drawShape.setPosition(sf::Vector2f(colliderShape.left - cameraTransform->getPosition().x, colliderShape.top - cameraTransform->getPosition().y));
				drawShape.setSize(sf::Vector2f(colliderShape.right - colliderShape.left, colliderShape.bottom - colliderShape.top));
				drawShape.setFillColor(sf::Color(0, 0, 0, 0));
				drawShape.setOutlineColor(sf::Color(0, 255, 0));
				drawShape.setOutlineThickness(3.0f);
				rectColliders.push_back(drawShape);
			}
		}
	}

	/* Draw the objects */
	for (sf::CircleShape shape : circleObjects) { window->draw(shape); }
	for (sf::RectangleShape shape : rectObjects) { 
		window->draw(shape); }

	/* Draw the colliders */
	for (sf::CircleShape shape : circleColliders) { window->draw(shape); }
	for (sf::RectangleShape shape : rectColliders) {
		window->draw(shape); }

	/* Draw UI */
	for (sf::RectangleShape shape : uiObjects) { window->draw(shape); }

	/* Draw text */
	for (sf::Text text : textBuffer) { window->draw(text); }

	// Write changes to window
	window->display();

	textBuffer = {};
}

void Game::drawText(sf::Text text){
	textBuffer.push_back(text);
}

void Game::drawCollider(Rect r) {
	sf::RectangleShape drawShape;
	drawShape.setFillColor(sf::Color(0, 0, 0, 0));
	drawShape.setOutlineThickness(10.0f);
	drawShape.setOutlineColor(sf::Color(0, 255, 0));
	drawShape.setOrigin(sf::Vector2f(0, 0));
	drawShape.setPosition(r.left, r.top);
	drawShape.setSize(sf::Vector2f(r.right - r.left, r.bottom - r.top));
	window->draw(drawShape);
}

void Game::drawCollider(float r, sf::Vector2f c) {
	sf::CircleShape drawShape;
	drawShape.setFillColor(sf::Color(0, 0, 0, 0));
	drawShape.setOutlineThickness(10.0f);
	drawShape.setOutlineColor(sf::Color(0, 255, 0));
	drawShape.setPosition(c);
	drawShape.setRadius(r);
	window->draw(drawShape);
}

void Game::debugLog(std::string str, std::string colorStr) {
	std::cout << getElapsedTime() << "|" << colorStr << str << LOG_RESET <<  "\n";
}


RectCollider::RectCollider(float top, float left, float right, float bottom) {
	Rect col;
	col.top = top;
	col.left = left;
	col.right = right;
	col.bottom = bottom;

	colliderShape = col;
}

Rect RectCollider::getCollider() {
	return colliderShape;
}

void RectCollider::setCollider(Rect r) {
	colliderShape = r;
}

RadiusCollider::RadiusCollider(float r, float x, float y) {
	radius = r;
	center = sf::Vector2f(x, y);
}

float RadiusCollider::getRadius() {
	return radius;
}

void RadiusCollider::setRadius(float r) {
	radius = r;
}

sf::Vector2f RadiusCollider::getCenter() {
	return center;
}

void RadiusCollider::setCenter(sf::Vector2f c) {
	center = c;
}

CollisionManager::CollisionManager() {


}

Collider* CollisionManager::makeCollider(float radius, sf::Vector2f center) {
	RadiusCollider* col = new RadiusCollider(radius, center.x, center.y);
	col->setRadius(radius);
	col->setCenter(center);

	return (static_cast<Collider*>(col));
}

Collider* CollisionManager::makeCollider(Rect rect) {
	RectCollider* col = new RectCollider(rect.top, rect.left, rect.bottom, rect.right);
	col->setCollider(rect);

	return (static_cast<Collider*>(col));
}

// https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
// Returns a collision between a circle and a rectangle
bool CollisionManager::isCollision(RadiusCollider* circle, RectCollider* rect) {
	float cRadius = circle->getRadius();
	sf::Vector2f circleCenter = circle->getCenter();
	Rect rectShape = rect->getCollider();
	float rHeight = rectShape.bottom - rectShape.top;
	float rWidth = rectShape.right - rectShape.left;

	sf::Vector2f circleDistance;

	circleDistance.y = abs(circleCenter.y - (rectShape.bottom - rHeight));
	circleDistance.x = abs(circleCenter.x - (rectShape.right - rWidth));

	if (circleDistance.x > (rWidth / 2 + cRadius)) { return false; }
	if (circleDistance.y > (rHeight / 2 + cRadius)) { return false; }

	if (circleDistance.x <= (rWidth / 2)) { return true; }
	if (circleDistance.y <= (rHeight / 2)) { return true; }

	float cornerDistance_sq = powf(circleDistance.x - rWidth / 2, 2) + powf(circleDistance.y - rHeight / 2, 2);

	return (cornerDistance_sq <= (cRadius * cRadius));
}

// Returns a collision between two circles
bool CollisionManager::isCollision(RadiusCollider* circle1, RadiusCollider* circle2) {

	return false;
}

// Returns a collision between two rectangles
bool CollisionManager::isCollision(RectCollider* rect1, RectCollider* rect2) {
	return false;
}

void CollisionManager::handleCollisions() {
	// Needs to be optimized; basic O(n^2) algorithm
	for (Collider* c1 : colliders) {
		for (Collider* c2 : colliders) {
			if (c1 == c2) { continue; };
			bool collisionOccurred = false;
			if (c1->isCircle && c2->isCircle) { // Both colliders are circles
				RadiusCollider* col1 = static_cast<RadiusCollider*>(c1);
				RadiusCollider* col2 = static_cast<RadiusCollider*>(c2);
				collisionOccurred = isCollision(col1, col2);
			}
			else if (!c1->isCircle && c2->isCircle) { // Collider 2 is a circle and collider 1 isn't
				RectCollider* col1 = static_cast<RectCollider*>(c1);
				RadiusCollider* col2 = static_cast<RadiusCollider*>(c2);
				collisionOccurred = isCollision(col2, col1);
			}
			else if (c1->isCircle && !c2->isCircle) { // Collider 1 is a circle and collider 2 isn't
				RadiusCollider* col1 = static_cast<RadiusCollider*>(c1);
				RectCollider* col2 = static_cast<RectCollider*>(c2);
				collisionOccurred = isCollision(col1, col2);
			}
			else { // Both aren't circles
				RectCollider* col1 = static_cast<RectCollider*>(c1);
				RectCollider* col2 = static_cast<RectCollider*>(c2);
				collisionOccurred = isCollision(col1, col2);
			}

			c1->isOverlapped = collisionOccurred;
			c2->isOverlapped = collisionOccurred;
		}
	}
}

sf::Color changeAlpha(sf::Color color, int alpha) {
	return sf::Color(color.r,color.g,color.b,alpha);
}
