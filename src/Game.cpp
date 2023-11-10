#include "Game.hpp"

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

/* Default constructor and destructor for ShapeComponent */
ShapeComponent::ShapeComponent() {
	fillColor = sf::Color(255,0,255,255);
	outlineColor = sf::Color(0, 0, 0, 255);
	outlineThickness = 0.0f;
	shapeType = rectangle;
	rotationDegree = 0.0f;
	circleRadius = 1.0f;

	rectSize = sf::Vector2f(50.0f, 50.0f);
	positionOffset = sf::Vector2f(0.0f, 00.0f);
	origin = sf::Vector2f(25.0f, 25.0f);
}

ShapeComponent::~ShapeComponent() {

}

/*
	Constructs a drawable RectangleShape from the data held in this class
*/
sf::CircleShape ShapeComponent::constructCircle() {
	sf::CircleShape returnShape = sf::CircleShape(circleRadius);
	returnShape.setPosition(positionOffset);
	returnShape.setOrigin(origin);
	returnShape.setFillColor(fillColor);
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
	returnShape.setPosition(positionOffset);
	returnShape.setOrigin(origin);
	returnShape.setRotation(rotationDegree);
	returnShape.setFillColor(fillColor);
	returnShape.setOutlineColor(outlineColor);
	returnShape.setOutlineThickness(outlineThickness);
	return returnShape;
}

/* Default constructor for TransformComponent */
Transform::Transform() {
	size = sf::Vector2f(1.0f,1.0f);
	position = sf::Vector2f(500.0f, 250.0f);
	origin = sf::Vector2f(25.0f, 25.0f);

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
GameObject::GameObject(Collider *col) {
	id = 0;
	ShapeComponent shape;
	collider = col;
}


GameObject::~GameObject() {

}

/* Public class functions */

Collider* GameObject::getCollider() {
	return collider;
}

/*
	@ return ShapeComponent
	Returns the shapeComponent 
*/
ShapeComponent &GameObject::getShapeComponent() {
	return shape;
}

/*
	@ return Transform
	Returns the Transform 
*/
Transform& GameObject::getTransform() {
	return transform;
}

/*
	@ return void
	Creates the window
*/
void Game::init() {
	window = nullptr;
	videoMode = sf::VideoMode(800, 600);

	window = new sf::RenderWindow(videoMode, "A game", sf::Style::Titlebar | sf::Style::Close);
	window->setFramerateLimit(120);
}

/*
	Constructor for game class
	Anything that happens initially goes here
*/
Game::Game() {
	init();
}

/*
	@ return objectRef(unsigned int) - id of new gameobject
	Creates a default GameObject.
*/
objectRef Game::makeObjectRef() {
	GameObject *newObject = new GameObject();
	newObject->id = nextId;
	nextId++;
	gameObjects.push_back(newObject);
	return newObject->id;
}

/*
	@ return GameObject
	Creates a default GameObject.
*/
GameObject *Game::makeObject() {
	GameObject* newObject = new GameObject();
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
GameObject *Game::getObject(objectRef targId) {
	for (GameObject *obj : gameObjects) {
		if (obj->id == targId) {
			return obj;
		}
	}
	GameObject* newObject = new GameObject();
	newObject->id = 0;
	return newObject;
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
}

/*
	Renders the window in its current state

	- Clears the screen to a color
	- Renders objects
	- Writes changes to the window
*/
void Game::render() {
	// Clear the screen with this color as argument
	window->clear(sf::Color(100,100,100,255));

	sf::RenderTexture rTarget;
	// Render objects by drawing directly on window
	for (GameObject *obj : gameObjects) {
		/* Draw the object */
		Transform transform = obj->getTransform();
		ShapeComponent shapeComp = obj->getShapeComponent();
		if (shapeComp.shapeType == shape_type::rectangle) {
			sf::RectangleShape drawShape = shapeComp.constructRectangle();
			drawShape.move(transform.getPosition());
			drawShape.scale(transform.getSize());
			drawShape.rotate(transform.rotationDegree);
			window->draw(drawShape);
		} else if (shapeComp.shapeType == shape_type::circle) {
			sf::CircleShape drawShape = shapeComp.constructCircle();
			drawShape.move(transform.getPosition());
			drawShape.scale(transform.getSize());
			drawShape.rotate(transform.rotationDegree);
			window->draw(drawShape);
		}
		
		/* Draw the object's collider */
		Collider* col = obj->getCollider();
		if (col != nullptr) {
			if (col->isCircle && col->isVisible) {
				RadiusCollider* collider = static_cast<RadiusCollider*>(col);
				sf::CircleShape drawShape;
				drawShape.setFillColor(sf::Color(0, 0, 0, 0));
				drawShape.setOutlineColor(sf::Color(0, 255, 0));
				drawShape.setOutlineThickness(3.0f);
				window->draw(drawShape);
			}
			else if (col->isVisible) {
				RectCollider* collider = static_cast<RectCollider*>(col);
				sf::RectangleShape drawShape;
				Rect colliderShape = collider->getCollider();
				drawShape.setPosition(sf::Vector2f(colliderShape.left, colliderShape.top));
				drawShape.setSize(sf::Vector2f(colliderShape.right - colliderShape.left, colliderShape.bottom - colliderShape.top));
				drawShape.setFillColor(sf::Color(0, 0, 0, 0));
				drawShape.setOutlineColor(sf::Color(0, 255, 0));
				drawShape.setOutlineThickness(3.0f);
				window->draw(drawShape);
			}
		}
	}

	// Write changes to window
	window->display();
}

void Game::drawCollider(Rect r) {
	sf::RectangleShape drawShape;
	drawShape.setFillColor(sf::Color(0, 0, 0, 0));
	drawShape.setOutlineThickness(10.0f);
	drawShape.setOutlineColor(sf::Color(0, 255, 0));
	drawShape.setOrigin(sf::Vector2f(0, 0));
	drawShape.setPosition(r.left,r.top);
	drawShape.setSize(sf::Vector2f(r.right - r.left, r.bottom - r.top));
	window->draw(drawShape);
}

void Game::drawCollider(float r, sf::Vector2f c) {
	sf::CircleShape drawShape;
	drawShape.setFillColor(sf::Color(0,0,0,0));
	drawShape.setOutlineThickness(10.0f);
	drawShape.setOutlineColor(sf::Color(0,255,0));
	drawShape.setPosition(c);
	drawShape.setRadius(r);
	window->draw(drawShape);
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

//https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
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
			if (c1 == c2) { break; };
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