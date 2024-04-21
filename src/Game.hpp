#pragma once


#ifndef GAME_HPP

//https://www.youtube.com/watch?v=3elKoMMuJBA

#include "filesystem.hpp"
#include "MACROS.hpp" // Makes key names easier
#include "Controller.hpp" // Handler for user input
#include <string>
#include <vector> // Dynamic Arrays
#include <iostream> // Debugging output
#include <chrono> // Timestamps
#include <cmath> // For vector math
#include <algorithm> // For sort

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

template <typename T>
class sptr { // Smart Pointer
private:
	T* pointer; // pointer to person class
public:
	sptr() {
		pointer = nullptr;
	}

	sptr(T* guardThis) {
		pointer = guardThis;
	}

	~sptr() {
		delete pointer;
	}

	T& operator* () {
		return *pointer;
	}

	T* operator-> () {
		return pointer;
	}
};

namespace engine {
	class Game;
	class GameObject;
	class Transform;
	class Texture;

	class GameManager {
	private:
		Game* game;
	public:
		GameManager() { game = nullptr; }
		GameManager(Game* engine) {game = engine;}
		void start();
		void update();
		void physicsUpdate();
	};

	// Aliases
	using floatPolygon = std::vector<sf::Vector2f>;

	class EngineComponent {
	public:
		int engine_index = 0;
	};

	enum renderable_id {none,particle,line,circle_collider,poly_collider,shape,text,texture};

	/* Objects that can be drawn by the engine */
	class Renderable {
	public:
		int layer = 1; // 0 is UI
		int type_id = 0; // type of object
		bool cullThis = false;
		/*
			id:
			1 - Particle
			2 - Line
			3 - RadiusCollider
			4 - PolygonCollider
			5 - ShapeComponent
			6 - Text
			7 - Sprite/texture
		*/

		bool operator < (const Renderable& other) const {
			return (layer < other.layer);
		}

		bool operator > (const Renderable& other) const {
			return (layer > other.layer);
		}
	};

	long long getTimens();
	std::string boolToString(bool b);
	sf::Color changeAlpha(sf::Color color, int alpha);

	/* Structs for data storage */

	/* Holds 4 values*/
	struct Rect {
		float top, left, bottom, right;
	};

	floatPolygon rectToPolygon(Rect r);

	/* Holds data for a line */
	class Line : public Renderable {
	public:
		sf::Vector2f start;
		sf::Vector2f end;
		sf::Color color;
		float thickness = 1.0f;
		float magnitude = 35.0f;
		Line();
		Line(sf::Vector2f begin, sf::Vector2f stop, sf::Color color);
	};

	/* Class for sf::Text construction */
	class Text : public Renderable {
	public:
		sf::Vector2f position = sf::Vector2f(0.0f,0.0f);
		sf::Vector2f origin = sf::Vector2f(0.0f, 0.0f);
		sf::Color color = sf::Color(255,255,255);
		std::string textString= "";
		int charSize = 24;
		sf::Font *font = nullptr;
		Text(sf::Color col, sf::Vector2f pos, sf::Vector2f org, std::string text, int size, sf::Font* fontptr) { textString = text; position = pos; origin = org; type_id = 6; layer = 0; color = col; charSize = size; font = fontptr; }
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
		float distanceAlongProjection(sf::Vector2f a, sf::Vector2f b);

		sf::Vector2f multiplyVector(sf::Vector2f v1, float i);
		sf::Vector2f divideVector(sf::Vector2f v1, float i);

		sf::Vector2f utof(sf::Vector2u v1);
	}

	/* Game Management */

	/* Collision */

	class Collider : public Renderable, public EngineComponent {
	public:
		int layer = 0;
		bool isCircle = false;
		bool isOverlapped = false;
		bool isRigid = false;
	};

	class PolygonCollider : public Collider, public EngineComponent {
	private:
		GameObject* parentObject;
		bool isCircle;
		int circlePoints = 100;
		floatPolygon vertices = {};
	public:
		PolygonCollider(floatPolygon polygon, GameObject* obj);
		PolygonCollider(Rect r, GameObject *obj);
		PolygonCollider(float radius, GameObject* obj);

		GameObject* getParent() { return parentObject; }
		void setParent(GameObject* obj) { parentObject = obj; }

		floatPolygon getPolygon() { return vertices; }
		void setPolygon(floatPolygon p) { vertices = p; }

		bool shapeIsCircle() { return isCircle; }
		void setIsCircle(bool setTo) { isCircle = setTo; }

		int getPoints() { return circlePoints; }
		void setPoints(int points) { circlePoints = points; }

		// top, bottom are lowest and highest y values respectively
		// left, right are lowest and highest x values respectively
		Rect extrusion;
		void updateExtrusion();

		void* onCollision;
	};

	// Namespace for collision management;
	// Seperate namespace bc I expect a lot of stuff to go here
	namespace collisions {
		// Functions
		bool overlap1D(float a1, float a2, float b1, float b2);
		
		// Classes
		struct CollisionPair {
			PolygonCollider* c1 = nullptr;
			PolygonCollider* c2 = nullptr;
			float overlap = 0.0f;
		};

		class ColliderBound {
		public:
			float coord;
			PolygonCollider* col;
			ColliderBound(PolygonCollider* collider, float value);

			bool operator < (const ColliderBound& other) const {
				return (coord < other.coord);
			}
		};
		
		// Algorithms
		std::vector<CollisionPair*> broadSortAndSweep(std::vector<PolygonCollider*> colliders);
		std::vector<CollisionPair*> narrowSAT(std::vector<CollisionPair*> pairs);

		class CollisionManager : public EngineComponent {
		private:
			std::vector<CollisionPair*> computedCollisions = {};
		public:
			CollisionManager();
			std::vector<CollisionPair*> calculateCollisionPairs(std::vector<PolygonCollider*> colliders);
			void handleCollisions(std::vector<PolygonCollider*> colliders);
		};
	}

	/*
		Class representing the shape to be rendered on an object
	*/
	class ShapeComponent : public Renderable, public EngineComponent {
	public:
		GameObject *parentObject;
		floatPolygon vertices = {}; // y of index 1 represents radius of circle
		bool isVisible = true;
		bool isCircle = false;
		int sides = 3;

		sf::Vector2f origin;
		float rotationDegree;

		sf::Color fillColor;
		sf::Color outlineColor;
		float outlineThickness;

		Texture* texture;

		/* Constructor and destructor */
		ShapeComponent(floatPolygon polygon, GameObject *obj);
		ShapeComponent(float radius, GameObject* obj);
		~ShapeComponent();

		/* Public drawable shape construction functions */
		sf::ConvexShape constructShape();
	};

	/*
		No clue why I made this. I will probably use this for an image texture holder.
	*/
	class Texture : public Renderable, public EngineComponent {
	private:
		Transform* transform;
		std::string texturePath;
	public:
		// Constructors and destructors
		Texture(Transform* trans) { type_id = 7; transform = trans; }
		Texture(Transform* trans, std::string path) { type_id = 7; texturePath = path; transform = trans; };

		/* Get functions */
		std::string getTexturePath() { return texturePath; };
		Transform* getTransform() { return transform; }

		/* Set functions */
		void setTexturePath(std::string path) { texturePath = path; }
		void setTransform(Transform* trans) { transform = trans; }
	};

	struct Force {
		sf::Vector2f vector; /* position displacement on an object of mass 1 */
	};

	/*
		A class containing information on positioning and scale
	*/
	class Transform : public EngineComponent {
	private:
		/* Physics properties */
		float mass;
		float inverseMass;
	public:
		bool isPhysical = true;

		// Properties
		GameObject *parentObject;
		sf::Vector2f size;
		sf::Vector2f origin;
		float rotationDegree;
		sf::Vector2f position;

		// Physics propertiesfloatPolygon
		sf::Vector2f velocity;
		PolygonCollider* col;
		std::vector<Force> actingForces;

		// Constructors and destructors
		Transform(GameObject *obj);

		/* Mass */
		void calcInverseMass() { inverseMass = 1.0f / mass; }

		/* Sick one liners*/
		/* Get attribute functions */
		float getMass() { return mass; }
		float getInverseMass() { return inverseMass; }
		sf::Vector2f getSize();
		sf::Vector2f getPosition();
		sf::Vector2f getOrigin();
		float getRotation();
		GameObject* getParent() { return parentObject; }

		/* Set attribute functions */
		void setMass(float m) { mass = m; calcInverseMass(); }
		void setSize(sf::Vector2f newSize);
		void setPosition(sf::Vector2f newPosition);
		void setOrigin(sf::Vector2f newOrigin);
		void setRotation(float newAngleDegree);
		void setParent(GameObject* obj) { parentObject = obj; }

		/* Add to attribute functions */
		void addToSize(sf::Vector2f newSize);
		void addToPosition(sf::Vector2f newPosition);
		void addToOrigin(sf::Vector2f newOrigin);
		void addRotation(float newAngleDegree);
	};

	/*
		Game Object for use within the engine
	*/
	class GameObject : public EngineComponent {
	private:
		PolygonCollider* collider;
		Transform *transform;
		ShapeComponent *shape;
		bool isVisible = true;
		Game* engine;
	public:
		GameObject* parent = nullptr;
		std::string objName = "default";
		unsigned int id;

		// Constructor and destructor
		GameObject(Game* game);
		GameObject(PolygonCollider* col, Game* game);
		GameObject(floatPolygon polygon, Game* game);
		GameObject(floatPolygon polygon, PolygonCollider* col, Game* game);
		GameObject(float radius, float colliderRadius, Game* game);
		~GameObject();
		void destroy();

		/* Get functions */
		bool getVisibility() { return isVisible; }
		PolygonCollider* getCollider();
		ShapeComponent* getShapeComponent();
		Transform* getTransform();

		/* Set functions */
		void setVisibility(bool v) { isVisible = v; }
		void setCollider(PolygonCollider* p) { if (collider != nullptr) { delete collider; } collider = p; }
		void makeCircle(bool isCircle);

		/* Misc functions */
		void updateCollider();
		void removeCollider();
		void removeShapeComponent();
	};

	struct Camera : public EngineComponent {
		Transform* transform;
		GameObject* focus;
	};

	class Particle;

	/*
		Game Engine - contains all the functions and data for a functioning engine
	*/
	class Game {
	private:
		/* Clocking and timing */
		long long tick = 0;
		long long currentTime;
		long long lastTime;
		long long startTime;
		float timeScale;
		
		float maxFPS;
		float invFPS;

		float physUPS; /* Updates per second */
		float invPhysUPS;

		sf::Event event;
		sf::VideoMode videoMode;

		float backgroundBrightness = 1.0f;

		// Keypress handling
		bool showColliders = false;
		bool f1Held = false;
		bool paused = false;
		bool escHeld = false;

		// Camera data
		Camera camera;

		// Collisions
		collisions::CollisionManager* collisionManager = nullptr;

		// Buffers
		std::vector<EngineComponent*> engineComponents = {nullptr};
		std::vector<PolygonCollider*> colliders = {};
		std::vector<Renderable*> renderableObjects = {};
		std::vector<Renderable*> uiRenderableObjects = {};

		// Gameobjects
		unsigned int nextId = 1;
		std::vector<GameObject*> gameObjects;

		// Private functions
		void init(float frameCap);
	public:
		sf::RenderWindow* window;
		GameManager manager;

		// Constructors
		Game(float fps, float ups);
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
		void setBackgroundBrightness(float brightness) { backgroundBrightness = brightness; }

		// Camera functions
		void setCamFocus(GameObject* obj) { camera.focus = obj; }

		// Game update and render functions
		void update();
		void updateObjects(float deltaTime);
		void stopScene();
		void resetScene();
		void render();
		void start();

		void dynamicDeleteRenderable(Renderable* r);

		// Sorting algorithms
		std::vector<Renderable*> sortRenderables(std::vector<Renderable*> renders);

		// UI draw functions
		void *drawText(Text *text, bool isUI);
		void *drawLine(Line *line, bool isUI);

		// Screen draw functions
		void* drawShape(ShapeComponent *shape, bool isUI);
		void* drawCollider(PolygonCollider* col, bool registerThisCollider);
		void registerCollider(PolygonCollider* col);
		bool removeCollider(PolygonCollider* col);
		void* drawTexture(Texture* texture, bool isUI);
		bool removeTexture(Texture* texture);

		// Render removal functions
		bool removeFromRender(void *removePtr);
		bool removeFromUI(void *removePtr);

		void debugLog(std::string str, std::string colorStr);
		void *debugLine(sf::Vector2f start, sf::Vector2f end);

		// Object functions
		GameObject* makeObject(int layer, floatPolygon polygon, bool toUI);
		GameObject* makeObject(int layer, floatPolygon polygon, PolygonCollider* col, bool toUI);
		GameObject* makeObject(int layer, float radius, bool toUI);
		GameObject* makeObject();
		GameObject* registerObject(GameObject* obj, bool toUI);
		GameObject* getObject(unsigned int targId);
		bool deleteObject(GameObject* delObj);
		bool deleteObject(unsigned int targId);
		std::string generateUniqueObjectName(std::string name);

		// Filesystem functions
		bool loadScene(std::string path);
		GameObject* loadObject(std::string path);
		GameObject* loadObject(std::string path, GameObject* parent);
		sf::Font* loadFont(std::string path);
		sf::Texture* loadTexture(std::string path);

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