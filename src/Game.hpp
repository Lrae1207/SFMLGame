#pragma once

#include "Controller.hpp" // Handler for user input
#include <vector> // Dynamic Arrays
#include <iostream> // Debugging output

/* Structs for data storage */
struct Rect {
	float top, left, bottom, right;
};

// Vector mathmatical functions
namespace vmath {
	sf::Vector2f normalizeVector(sf::Vector2f vector);
	float getMagnitude(sf::Vector2f vector);
}

/* Game Management */

/* Collision */

class Collider {
public:
	float layer = 0;
	bool isCircle = false;
	bool isOverlapped = false;
	bool isRigid = false;
	bool isVisible = true;
};

class RectCollider : public Collider {
private:
	Rect colliderShape;
public:
	RectCollider(float top, float left, float bottom, float right);
	Rect getCollider();
	void setCollider(Rect r);
};

class RadiusCollider : public Collider {
private:
	float radius;
	sf::Vector2f center;
public:
	RadiusCollider(float r, float x, float y);
	float getRadius();
	void setRadius(float r);
	sf::Vector2f getCenter();
	void setCenter(sf::Vector2f c);
};

class CollisionManager {
private:
	std::vector<Collider*> colliders;
	bool isCollision(RadiusCollider* circle, RectCollider* rect);
	bool isCollision(RadiusCollider* circle1, RadiusCollider* circle2);
	bool isCollision(RectCollider* rect1, RectCollider* rect2);
public:
	CollisionManager();
	Collider* makeCollider(float radius, sf::Vector2f center);
	Collider* makeCollider(Rect rect);
	void handleCollisions();
};

/*
	Class representing the shape to be rendered on an object
*/
enum shape_type {rectangle, circle, polygon};
class ShapeComponent {
public:
	shape_type shapeType;

	sf::Vector2f rectSize;
	sf::Vector2f positionOffset;
	sf::Vector2f origin;
	float rotationDegree;
	float circleRadius;

	sf::Color fillColor;
	sf::Color outlineColor;
	float outlineThickness;

	/* Constructor and destructor */
	ShapeComponent();
	~ShapeComponent();

	/* Public drawable shape construction functions */
	sf::CircleShape constructCircle();
	sf::RectangleShape constructRectangle();
};

/*
	No clue why I made this. I will probably use this for an image texture holder.
*/
class SpriteComponent {
public:	
	sf::Texture texture;
	// Constructors and destructors
	SpriteComponent();
};

/*
	A class containing information on positioning and scale
*/
class Transform {
public:
	// Transform private properties
	//sf::Vector2f position;
	sf::Vector2f size;
	sf::Vector2f origin;
	float rotationDegree;
	sf::Vector2f position;

public:
	// Constructors and destructors

	Transform();
	/* Sick one liners*/
	/* Get attribute functions */
	sf::Vector2f getSize();
	sf::Vector2f getPosition();
	sf::Vector2f getOrigin();
	float getRotation();

	/* Set attribute functions */
	void setSize(sf::Vector2f newSize);
	void setPosition(sf::Vector2f newPosition);
	void setOrigin(sf::Vector2f newOrigin);
	void setRotation(float newAngleDegree);

	/* Add to attribute functions */
	void addToSize(sf::Vector2f newSize);
	void addToPosition(sf::Vector2f newPosition);
	void addToOrigin(sf::Vector2f newOrigin);
	void addRotation(float newAngleDegree);
};

// Integers with the intended use as a unique identifier
using objectRef = unsigned int;
/*
	Game Object for use within the engine
*/
class GameObject {
private:
	Collider* collider;
	Transform transform;
	ShapeComponent shape;
public:
	objectRef id;

	// Constructor and destructor
	GameObject();
	GameObject(Collider *col);
	~GameObject();

	Collider* getCollider();
	ShapeComponent& getShapeComponent();
	Transform& getTransform();
};

/*
	Game Engine
*/
class Game {
private:
	// Private variables
	sf::Event event;
	sf::VideoMode videoMode;

	CollisionManager *collisionManager;

	// For keeping track of a unique id
	objectRef nextId = 1;
	
	std::vector<GameObject*> gameObjects;

	// Private functions
	void init();
public:
	sf::RenderWindow* window;

	// Constructors
	Game();
	virtual ~Game();

	// Accessor
	const bool windowActive() const;

	// Game update and render functions
	void update();
	void render();
	void drawCollider(Rect r);
	void drawCollider(float r, sf::Vector2f c);

	// Object functions
	objectRef makeObjectRef();
	GameObject *makeObject();
	GameObject *getObject(objectRef targId);

	// Controller
	Controller controller;
};
