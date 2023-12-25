#pragma once

#include "Physics.hpp"
#include "MACROS.hpp" // Makes key names easier
#include "Controller.hpp" // Handler for user input
#include <vector> // Dynamic Arrays
#include <iostream> // Debugging output
#include <chrono> // Timestamps
#include <cmath> // For vector math

long long getTimens();
std::string boolToString(bool b);
sf::Color changeAlpha(sf::Color color, int alpha);

/* Structs for data storage */
struct Rect {
	float top, left, bottom, right;
};

class Line {
public:
	sf::Vector2f start;
	sf::Vector2f end;
	sf::Color color;
	float thickness = 1.0f;
	float magnitude = 35.0f;
	Line(sf::Vector2f begin, sf::Vector2f stop, sf::Color color);
};

// Vector mathmatical functions
namespace vmath {
	sf::Vector2f normalizeVector(sf::Vector2f vector);
	float getMagnitude(sf::Vector2f vector);

	float getDistance(sf::Vector2f v1, sf::Vector2f v2);
	
	sf::Vector2f rotateByDegrees(sf::Vector2f vector, float degrees);

	sf::Vector2f addVectors(sf::Vector2f v1, sf::Vector2f v2);
	sf::Vector2f subtractVectors(sf::Vector2f v1, sf::Vector2f v2);
	float dotProduct(sf::Vector2f a, sf::Vector2f b);
	
	sf::Vector2f multiplyVector(sf::Vector2f v1, float i);
	sf::Vector2f divideVector(sf::Vector2f v1, float i);

	sf::Vector2f utof(sf::Vector2u v1);
	float projectVector(sf::Vector2f a, sf::Vector2f b);
}

/* Game Management */

/* Collision */

class Collider {
public:
	float layer = 0;
	bool isCircle = false;
	bool isOverlapped = false;
	bool isRigid = false;
};

class PolygonCollider : Collider {
private:
	bool isCircle;
	float rotation = 0.0f;
	std::vector<sf::Vector2f> polygon = {};
public:
	PolygonCollider();
	std::vector<sf::Vector2f> getPolygon() { return polygon; }
	void setPolygon(std::vector<sf::Vector2f> p) { polygon = p; }
	bool shapeIsCircle() { return isCircle; }
	float getRotation() { return rotation; };
	void setRotation(float r) { rotation = r; }
};

class RectCollider : public Collider {
private:
	Rect colliderShape;
	float rotation = 0.0f;
public:
	RectCollider(float top, float left, float bottom, float right);
	Rect getCollider();
	void setCollider(Rect r);
	float getRotation() { return rotation; };
	void setRotation(float r) { rotation = r; }
};

class RadiusCollider : public Collider {
private:
	float radius;
	int points = 30;
	sf::Vector2f center;
public:
	RadiusCollider(float r, float x, float y);
	float getRadius();
	void setRadius(float r);
	sf::Vector2f getCenter();
	void setCenter(sf::Vector2f c);
	int getPoints() { return points; }
	void setPoints(int p) { points = p; }
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
enum shape_type { rectangle, circle, polygon };
class ShapeComponent {
public:
	shape_type shapeType;

	sf::Vector2f rectSize;
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
	// sf::Vector2f position;
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
	int layer = 1; // Layer 0 = UI
	bool isVisible = true;
public:
	objectRef id;

	// Constructor and destructor
	GameObject();
	GameObject(Collider* col);
	~GameObject();

	int getLayer() { return layer; }
	void setLayer(int l) { layer = l; }

	bool getVisibility() { return isVisible; }
	void setVisibility(bool v) { isVisible = v; }

	Collider* getCollider();
	void updateCollider();
	ShapeComponent* getShapeComponent();
	Transform* getTransform();
};

struct Camera {
	Transform* transform;
	GameObject* focus;
};

class Particle;

/*
	Game Engine
*/
class Game {
private:
	long long currentTime;
	long long lastTime;
	long long startTime;
	float timeScale;

	// Private variables
	sf::Event event;
	sf::VideoMode videoMode;

	float backgroundBrightness = 1.0f;

	void* manager;

	// Keypress handling
	bool showColliders = false;
	bool f1Held = false;
	bool paused = false;
	bool escHeld = false;

	// Camera data
	Camera camera;

	// Collisions
	CollisionManager* collisionManager;

	// Buffers
	std::vector<sf::Text> textBuffer = {};
	std::vector<Line> lineBuffer = {};
	std::vector<Particle*> particleBuffer = {};

	// Gameobjects
	objectRef nextId = 1;
	std::vector<GameObject*> gameObjects;

	// Private functions
	void init();
public:
	sf::RenderWindow* window;

	// Constructors
	Game();
	virtual ~Game();

	// Accessors
	const bool windowActive() const;
	bool isPaused() { return paused; }
	sf::Vector2f getWindowSize() { return vmath::utof(window->getSize()); }

	// Time Accessors
	float getTimescale() { return timeScale; }
	long long getElapsedTime() { return getTimens() - startTime; }
	long long getDeltaTime() { return currentTime - lastTime; }

	// Get and set
	void* getManager() { return manager; };
	void setManager(void* m) { manager = m; }
	void setBackgroundBrightness(float brightness) { backgroundBrightness = brightness; }

	// Camera functions
	void setCamFocus(GameObject* obj) { camera.focus = obj; }

	// Game update and render functions
	void update();
	void render();
	void drawText(sf::Text text);
	void drawCollider(Rect r);
	void drawCollider(float r, sf::Vector2f c);
	void debugLog(std::string str, std::string colorStr);
	void debugLine(sf::Vector2f start, sf::Vector2f end);

	// Object functions
	objectRef makeObjectRef();
	GameObject* makeObject();
	GameObject* makeObject(Collider* col);
	GameObject* getObject(objectRef targId);
	void registerObject(GameObject *obj);
	void removeObject(GameObject *obj);
	void registerParticle(Particle *p);
	void removeParticle(Particle* p);

	// Controller
	Controller controller;
};

class Particle {
private:
	Transform* transform;
	ShapeComponent* shape;
	Game* game;
	sf::Vector2f velocity;
	float drag;
public:
	float lifeTime = 0.0f;
	int id = 0;
	Particle(Game* engine, sf::Vector2f v);

	void startRendering();
	void stopRendering();
	void deleteParticle();

	Transform* getTransform() { return transform; }
	ShapeComponent* getShape() { return shape; }

	void setDrag(float d) { drag = d; }

	void update();
};