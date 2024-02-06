#pragma once


#ifndef GAME_HPP
#include "MACROS.hpp" // Makes key names easier
#include "Controller.hpp" // Handler for user input
#include <vector> // Dynamic Arrays
#include <iostream> // Debugging output
#include <chrono> // Timestamps
#include <cmath> // For vector math

#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include <cmath>

namespace phys {
	const double PI = 3.1415926535;
	const double RADTODEG = 180.0f / PI;
	const double DEGTORAD = PI / 180.0f;
	const double GRAVITY_CONSTANT = 6.6743f * pow(10, -11);
	const double GGRAM = pow(10, 9); // gigagram 10^9

	double distance2D(double x1, double y1, double x2, double y2);

	// Calculate object 1s speed of acceleration towards object 2
	double calculateGravityAccel(double x1, double y1, double x2, double y2, double mass2);
}
#endif

namespace engine {
	class Game;
	class GameObject;

	class EngineComponent {
	public:
		int engine_index = 0;
	};

	/* Objects that can be drawn by the engine */
	class Renderable {
	public:
		int layer = 1; // 0 is UI
		int type_id; // type of object
		/*
			id:
			0 - Line
			1 - Particle
			2 - RectCollider
			3 - RadiusCollider
			4 - PolygonCollider
			5 - ShapeComponent
			6 - Text
		*/
	};

	long long getTimens();
	std::string boolToString(bool b);
	sf::Color changeAlpha(sf::Color color, int alpha);

	/* Structs for data storage */

	/* Holds 4 values*/
	struct Rect {
		float top, left, bottom, right;
	};

	/* Holds data for a line */
	class Line : public Renderable {
	public:
		sf::Vector2f start;
		sf::Vector2f end;
		sf::Color color;
		float thickness = 1.0f;
		float magnitude = 35.0f;
		Line(sf::Vector2f begin, sf::Vector2f stop, sf::Color color);
	};

	/* Wrapper class for sf::Text */
	class Text : public Renderable {
	public:
		sf::Text drawableText;
		Text(sf::Text text) { drawableText = text; }
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

	class Collider : public Renderable, public EngineComponent {
	public:
		float layer = 0;
		bool isCircle = false;
		bool isOverlapped = false;
		bool isRigid = false;
	};

	class PolygonCollider : public Collider, public EngineComponent {
	private:
		GameObject* parentObject;
		bool isCircle;
		int circlePoints = 100;
		std::vector<sf::Vector2f> vertices = {};
	public:
		PolygonCollider();
		PolygonCollider(bool makeCircle);

		GameObject* getParent() { return parentObject; }
		void setParent(GameObject* obj) { parentObject = obj; }

		std::vector<sf::Vector2f> getPolygon() { return vertices; }
		void setPolygon(std::vector<sf::Vector2f> p) { vertices = p; }

		bool shapeIsCircle() { return isCircle; }
		void setIsCircle(bool setTo) { isCircle = setTo; }

		int getPoints() { return circlePoints; }
		void setPoints(int points) { circlePoints = points; }
	};

	class CollisionManager: public EngineComponent {
	private:
		std::vector<Collider*> colliders;
	public:
		CollisionManager();
	};

	/*
		Class representing the shape to be rendered on an object
	*/
	class ShapeComponent : public Renderable, public EngineComponent {
	public:
		GameObject *parentObject;
		std::vector<sf::Vector2f> vertices = {};
		bool isVisible = true;
		bool isCircle = false;

		sf::Vector2f origin;
		float rotationDegree;

		sf::Color fillColor;
		sf::Color outlineColor;
		float outlineThickness;

		/* Constructor and destructor */
		ShapeComponent();
		~ShapeComponent();

		/* Public drawable shape construction functions */
		sf::ConvexShape constructShape();
	};

	/*
		No clue why I made this. I will probably use this for an image texture holder.
	*/
	class SpriteComponent : public EngineComponent {
	public:
		sf::Texture texture;
		// Constructors and destructors
		SpriteComponent();
	};

	/*
		A class containing information on positioning and scale
	*/
	class Transform : public EngineComponent {
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
	class GameObject : public EngineComponent {
	private:
		Collider* collider;
		Transform transform;
		ShapeComponent shape;
		bool isVisible = true;
	public:
		objectRef id;

		// Constructor and destructor
		GameObject();
		GameObject(Collider* col);
		~GameObject();

		void destroy(Game *engine);

		bool getVisibility() { return isVisible; }
		void setVisibility(bool v) { isVisible = v; }

		Collider* getCollider();
		void updateCollider();
		ShapeComponent* getShapeComponent();
		Transform* getTransform();
	};

	struct Camera : public EngineComponent {
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
		std::vector<EngineComponent*> engineComponents = {nullptr};
		std::vector<Renderable*> renderableObjects = {};
		std::vector<Renderable*> uiRenderableObjects = {};

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

		// UI draw functions
		void *drawText(sf::Text *text);
		void *drawText(Text *text);
		void *drawLine(Line *line);

		// Screen draw functions
		void* drawShape(ShapeComponent *shape);

		// Render removal functions
		bool removeFromRender(void *removePtr);
		bool removeFromUI(void *removePtr);

		void debugLog(std::string str, std::string colorStr);
		void *debugLine(sf::Vector2f start, sf::Vector2f end);

		// Object functions
		GameObject* makeObject();
		GameObject* makeObject(Collider* col);
		GameObject* getObject(objectRef targId);
		bool deleteObject(GameObject* delObj);
		bool deleteObject(objectRef targId);

		// Controller
		Controller controller;
	};

	class Particle : Renderable {
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
}
#endif