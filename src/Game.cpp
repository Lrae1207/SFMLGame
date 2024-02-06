#include "Game.hpp"

double phys::distance2D(double x1, double y1, double x2, double y2) {
	float dx = x2 - x1;
	float dy = y2 - y1;
	float d = sqrtf(dx * dx + dy * dy);
	return d;
}
double phys::calculateGravityAccel(double x1, double y1, double x2, double y2, double mass2) {
	return GGRAM * GRAVITY_CONSTANT * mass2 / distance2D(x1, y1, x2, y2);
}

/* Get the timestamp in nanoseconds */
long long engine::getTimens() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string engine::boolToString(bool b) {
	if (b) {
		return "true";
	} else {
		return "false";
	}
}

//  Returns a normalized vector (x and y divided by magnitude)
sf::Vector2f engine::vmath::normalizeVector(sf::Vector2f vector) {
	float magnitude = sqrtf(vector.x * vector.x + vector.y * vector.y);

	// This means vector is <0,0>
	if (magnitude == 0) {
		return vector;
	}
	vector = sf::Vector2f(vector.x / magnitude, vector.y / magnitude);
	return vector;
}

float engine::vmath::getMagnitude(sf::Vector2f vector) {
	return sqrtf(vector.x * vector.x + vector.y * vector.y);;
}

float engine::vmath::dotProduct(sf::Vector2f a, sf::Vector2f b) {
	return a.x * b.x + a.y * b.y;
}

// Return a's distance along b of a projected on b
float engine::vmath::projectVector(sf::Vector2f a, sf::Vector2f b) {
	return dotProduct(a,normalizeVector(b));
}

float engine::vmath::getDistance(sf::Vector2f v1, sf::Vector2f v2) {
	return getMagnitude(subtractVectors(v1, v2));
}

sf::Vector2f engine::vmath::addVectors(sf::Vector2f v1, sf::Vector2f v2) {
	return sf::Vector2f(v1.x + v2.x, v1.y + v2.y);
}

sf::Vector2f engine::vmath::rotateByDegrees(sf::Vector2f vector, float degrees) {
	sf::Vector2f returnVector;
	degrees *= phys::DEGTORAD;
	returnVector.x = vector.x * cosf(degrees) - vector.y * sinf(degrees);
	returnVector.y = vector.x * sinf(degrees) + vector.y * cosf(degrees);
	return returnVector;
}

/* Returns v1 - v2 */
sf::Vector2f engine::vmath::subtractVectors(sf::Vector2f v1, sf::Vector2f v2) {
	return sf::Vector2f(v1.x - v2.x, v1.y - v2.y);
}

sf::Vector2f engine::vmath::multiplyVector(sf::Vector2f v1, float i) {
	return sf::Vector2f(v1.x * i, v1.y * i);
}

sf::Vector2f engine::vmath::divideVector(sf::Vector2f v1, float i) {
	return sf::Vector2f(v1.x / i, v1.y / i);
}

sf::Vector2f engine::vmath::utof(sf::Vector2u v1) {
	return sf::Vector2f(v1.x, v1.y); 
};

/* Default constructor and destructor for ShapeComponent */
engine::ShapeComponent::ShapeComponent() {
	type_id = 5;

	fillColor = sf::Color(255, 0, 255, 255);
	outlineColor = sf::Color(0, 0, 0, 255);
	outlineThickness = 0.0f;
	rotationDegree = 0.0f;

	origin = sf::Vector2f(0.0f, 0.0f);
}

engine::ShapeComponent::~ShapeComponent() {

}

sf::ConvexShape engine::ShapeComponent::constructShape() {
	sf::ConvexShape polygon;

	if (isCircle) {
		// frick circles
		// all my homies use polygons
	} else {
		polygon.setPointCount(vertices.size());
		for (int i = 0; i < vertices.size(); ++i) {
			polygon.setPoint(i,vertices[i]);
		}
	}

	/* Set color and orientational values */
	polygon.setOrigin(origin);
	polygon.setRotation(rotationDegree);
	polygon.setFillColor(fillColor);
	polygon.setOutlineColor(outlineColor);
	polygon.setOutlineThickness(outlineThickness);

	return polygon;
}

/* Default constructor for TransformComponent */
engine::Transform::Transform() {
	size = sf::Vector2f(1.0f, 1.0f);
	position = sf::Vector2f(0.0f, 0.0f);
	origin = sf::Vector2f(0.0f, 0.0f);

	rotationDegree = 0;
}

/* Sick one liners*/
/* Get attribute functions */
sf::Vector2f engine::Transform::getSize() { return size; };
sf::Vector2f engine::Transform::getPosition() { return position; };
sf::Vector2f engine::Transform::getOrigin() { return origin; };
float engine::Transform::getRotation() { return rotationDegree; };

/* Set attribute functions */
void engine::Transform::setSize(sf::Vector2f newSize) { size = newSize; };
void engine::Transform::setPosition(sf::Vector2f newPosition) {
	position = sf::Vector2f(newPosition.x, newPosition.y);
};
void engine::Transform::setOrigin(sf::Vector2f newOrigin) { origin = newOrigin; };
void engine::Transform::setRotation(float newAngleDegree) { rotationDegree = newAngleDegree; };

/* Add to attribute functions */
void engine::Transform::addToSize(sf::Vector2f newSize) { size = size + newSize; };
void engine::Transform::addToPosition(sf::Vector2f newPosition) { position = position + newPosition; };
void engine::Transform::addToOrigin(sf::Vector2f newOrigin) { origin = origin + newOrigin; };
void engine::Transform::addRotation(float newAngleDegree) { rotationDegree += newAngleDegree; };

/* Default constructor for SpriteComponent */
engine::SpriteComponent::SpriteComponent::SpriteComponent() {

}


/* GameObject constructor and destructor */
engine::GameObject::GameObject() {
	id = 0;
	ShapeComponent shape;
	collider = nullptr;
}

/* GameObject constructor and destructor */
engine::GameObject::GameObject(Collider* col) {
	id = 0;
	transform = Transform();
	shape = ShapeComponent();
	collider = col;
}


engine::GameObject::~GameObject() {

}

void engine::GameObject::destroy(Game *engine) {
	engine->deleteObject(this);
	delete this;
}

/* Public class functions */

engine::Collider* engine::GameObject::getCollider() {
	return collider;
}

void engine::GameObject::updateCollider() {

}

/*
	return ShapeComponent
	Returns the shapeComponent
*/
engine::ShapeComponent* engine::GameObject::getShapeComponent() {
	return &shape;
}

/*
	return Transform
	Returns the Transform
*/
engine::Transform* engine::GameObject::getTransform() {
	return &transform;
}

/*
	return void
	Creates the window
*/
void engine::Game::init() {
	startTime = getTimens();
	timeScale = 1;

	window = nullptr;
	videoMode = sf::VideoMode(800, 600);

	window = new sf::RenderWindow(videoMode, "A game", sf::Style::Titlebar | sf::Style::Close);
	window->setFramerateLimit(120);

	camera.transform = new Transform();
	camera.transform->setPosition(sf::Vector2f(0, 0));
	camera.transform->setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	debugLog("Instantiated Engine(game)", LOG_GREEN);
}

/*
	Constructor for game class
	Anything that happens initially goes here
*/
engine::Game::Game() {
	debugLog("Loaded Engine(game)", LOG_GREEN);
	init();
}

engine::GameObject* engine::Game::makeObject() {
	GameObject* newObject = new GameObject();
	newObject->id = nextId;
	nextId++;
	gameObjects.push_back(newObject);
	drawShape(newObject->getShapeComponent());
	return newObject;
}

engine::GameObject* engine::Game::makeObject(Collider* col) {
	GameObject* newObject = new GameObject(col);
	newObject->id = nextId;
	nextId++;
	gameObjects.push_back(newObject);
	drawShape(newObject->getShapeComponent());
	return newObject;
}

engine::GameObject* engine::Game::getObject(objectRef targId) {
	for (GameObject* obj : gameObjects) {
		if (obj->id == targId) {
			return obj;
		}
	}
	nullptr;
}

bool engine::Game::deleteObject(GameObject* delObj) {
	int i = 0;
	for (GameObject* obj : gameObjects) {
		if (obj == obj) {
			gameObjects.erase(gameObjects.begin() + i);
			return true;
		}
		i++;
	}
	return false;
}

bool engine::Game::deleteObject(objectRef targId) {
	return deleteObject(getObject(targId));
}

// Destructor
engine::Game::~Game() {
	delete window;
}

/*
	Used for the external game loop to make sure the window isn't open
*/
const bool engine::Game::windowActive() const {
	return window->isOpen();
}


// Update and render functions

/*
	Updates the positions and any needed changes since last update

	- Handles events (such as keypresses)
*/
void engine::Game::update() {
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

	// Update camera
	sf::Vector2f offset = vmath::divideVector(getWindowSize(),2);
	camera.transform->setPosition(vmath::subtractVectors(camera.focus->getTransform()->position,offset));
}

/*
	Renders the window in its current state

	- Clears the screen to a color
	- Renders objects
	- Writes changes to the window
*/
void engine::Game::render() {
	// Clear the screen with this color as argument
	window->clear(sf::Color(100*backgroundBrightness, 150*backgroundBrightness, 255*backgroundBrightness, 255));

	Transform* cameraTransform = camera.transform; // Transform of the camera

	std::vector<sf::ConvexShape> drawBuffer = {};

	// Sort first by layer
	for (Renderable* renderObj : renderableObjects) { // Convert each renderable object to a convex polygon that sfml can draw or directly draw it
		switch (renderObj->type_id) { // Change type_id to its respective type
		case 0: {
			Line* line = reinterpret_cast<Line*>(renderObj); // Convert to Line*

			// Start, end is essentially a rect
			sf::Vector2f start = line->start;
			sf::Vector2f end = line->end;
			start.x -= line->thickness;
			start.y -= line->thickness;
			end.x += line->thickness;
			end.y += line->thickness;

			// Convert the line to a rectangle for drawing
			sf::Vector2f lineDir = sf::Vector2f(vmath::getDistance(start, end), line->magnitude);
			sf::RectangleShape drawLine(lineDir);

			drawLine.setFillColor(line->color); // Color

			// Make position relative to the camera
			drawLine.setPosition(vmath::subtractVectors(line->start, cameraTransform->getPosition()));
			lineDir = vmath::subtractVectors(line->end, line->start);

			// Rotate to proper position
			drawLine.rotate(phys::RADTODEG * atan2f(lineDir.y, lineDir.x) - 90.0f);

			window->draw(drawLine); // Draw
			break;
		}
		case 1:
			break;
		case 5: {
			ShapeComponent shapeComp = *reinterpret_cast<ShapeComponent*>(renderObj);

			if (!shapeComp.isVisible) { // Skip invisible objects
				continue;
			}

			GameObject* obj = shapeComp.parentObject;
			Transform transform = *obj->getTransform();

			sf::ConvexShape polygon = shapeComp.constructShape();
			polygon.move(transform.getPosition());
			polygon.rotate(transform.getRotation());
			polygon.scale(transform.getSize());

			window->draw(polygon);

			break;
		}
		case 6: {
			Text* text = reinterpret_cast<Text*>(renderObj); // Convert to Text*
			window->draw(text->drawableText); // Draw
			break;
		}
		default:
			continue;
		}
	}

	for (Renderable* renderObj : uiRenderableObjects) { // Draw UI on top
		switch (renderObj->type_id) { // Change type_id to its respective type
		case 0: {
			Line* line = reinterpret_cast<Line*>(renderObj); // Convert to Line*

			// Start, end is essentially a rect
			sf::Vector2f start = line->start;
			sf::Vector2f end = line->end;
			start.x -= line->thickness;
			start.y -= line->thickness;
			end.x += line->thickness;
			end.y += line->thickness;

			// Convert the line to a rectangle for drawing
			sf::Vector2f lineDir = sf::Vector2f(vmath::getDistance(start, end), line->magnitude);
			sf::RectangleShape drawLine(lineDir);

			drawLine.setFillColor(line->color); // Color

			// Make position relative to the camera
			drawLine.setPosition(vmath::subtractVectors(line->start, cameraTransform->getPosition()));
			lineDir = vmath::subtractVectors(line->end, line->start);

			// Rotate to proper position
			drawLine.rotate(phys::RADTODEG * atan2f(lineDir.y, lineDir.x) - 90.0f);

			window->draw(drawLine); // Draw
			break;
		}
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4: {
			if (!showColliders) {
				continue;
			}

			PolygonCollider* col = reinterpret_cast<PolygonCollider*>(renderObj);
			std::vector<sf::Vector2f> polygon = col->getPolygon();

			sf::ConvexShape shape;
			shape.setPointCount(polygon.size());
			
			for (int i = 0; i < polygon.size(); ++i) {
				shape.setPoint(i, polygon[i]);
			}

			Transform transform = *col->getParent()->getTransform();
			shape.move(transform.getPosition());
			shape.rotate(transform.getRotation());
			shape.scale(transform.getSize());

			window->draw(shape);

			break;
		}
		case 5: {
			ShapeComponent shapeComp = *reinterpret_cast<ShapeComponent*>(renderObj);

			if (!shapeComp.isVisible) { // Skip invisible objects
				continue;
			}

			GameObject* obj = shapeComp.parentObject;
			Transform transform = *obj->getTransform();

			sf::ConvexShape polygon = shapeComp.constructShape();
			polygon.move(transform.getPosition());
			polygon.rotate(transform.getRotation());
			polygon.scale(transform.getSize());

			window->draw(polygon);
			break;
		}
		case 6: {
			Text* text = reinterpret_cast<Text*>(renderObj); // Convert to Text*
			window->draw(text->drawableText); // Draw
			break;
		}
		default:
			continue;
		}
	}

	// Write changes to window
	window->display();
}

void *engine::Game::drawText(sf::Text *text) {
	Renderable* r = dynamic_cast<Renderable*>(text);
	uiRenderableObjects.push_back(r);
	return static_cast<void*>(r);
}

void *engine::Game::drawText(Text* text) {
	Renderable* r = dynamic_cast<Renderable*>(&text->drawableText);
	uiRenderableObjects.push_back(r);
	return static_cast<void*>(r);
}

void* engine::Game::drawLine(Line* line) {
	Renderable* r = dynamic_cast<Renderable*>(line);
	uiRenderableObjects.push_back(r);
	return static_cast<void*>(r);
}

void* engine::Game::drawShape(ShapeComponent *shape) {
	Renderable* r = dynamic_cast<Renderable*>(shape);
	renderableObjects.push_back(r);
	return static_cast<void*>(r);
}

bool engine::Game::removeFromRender(void* removePtr) {
	int i = 0;
	for (Renderable* r : renderableObjects) {
		if (r == removePtr) {
			renderableObjects.erase(renderableObjects.begin() + i);
			return true;
		}
		i++;
	}
	return false;
}

bool engine::Game::removeFromUI(void* removePtr) {
	int i = 0;
	for (Renderable *r : uiRenderableObjects) {
		if (r == removePtr) {
			uiRenderableObjects.erase(uiRenderableObjects.begin() + i);
			return true;
		}
		i++;
	}
	return false;
}

void engine::Game::debugLog(std::string str, std::string colorStr) {
	std::cout << getElapsedTime() << "|" << colorStr << str << LOG_RESET <<  "\n";
}

void* engine::Game::debugLine(sf::Vector2f start, sf::Vector2f end) {
	Line line = Line(start, end, sf::Color::Green);
	Renderable* r = dynamic_cast<Renderable*>(&line);
	uiRenderableObjects.push_back(dynamic_cast<Renderable*>(&line));
	return static_cast<void*>(r);
}

engine::PolygonCollider::PolygonCollider() {
	type_id = 4;
	isCircle = false;
}

engine::PolygonCollider::PolygonCollider(bool makeCircle) {
	type_id = 4;
	isCircle = makeCircle;
}

engine::CollisionManager::CollisionManager() {

}

sf::Color engine::changeAlpha(sf::Color color, int alpha) {
	return sf::Color(color.r,color.g,color.b,alpha);
}

engine::Line::Line(sf::Vector2f begin, sf::Vector2f stop, sf::Color color) {
	type_id = 0;
	start = begin;
	end = stop;
	this->color = color;
}

engine::Particle::Particle(Game* engine, sf::Vector2f v) {
	type_id = 1;
	transform = new Transform();
	shape = new ShapeComponent();
	game = engine;
	velocity = v;
}

void engine::Particle::update() {
	lifeTime += game->getDeltaTime();

	velocity = vmath::multiplyVector(velocity, drag);
	transform->addToPosition(velocity);
}

void engine::Particle::deleteParticle() {
	delete this;
}