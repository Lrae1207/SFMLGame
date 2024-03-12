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

float engine::vmath::distanceAlongProjection(sf::Vector2f a, sf::Vector2f b){ 
	return dotProduct(a, normalizeVector(b)); 
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
engine::ShapeComponent::ShapeComponent(std::vector<sf::Vector2f> polygon, GameObject *obj) {
	type_id = 5;
	parentObject = obj;

	fillColor = sf::Color(255, 0, 255, 255);
	outlineColor = sf::Color(0, 0, 0, 255);
	outlineThickness = 0.0f;
	rotationDegree = 0.0f;

	vertices = polygon;

	origin = sf::Vector2f(0.0f, 0.0f);
}

engine::ShapeComponent::~ShapeComponent() {

}

sf::ConvexShape engine::ShapeComponent::constructShape() {
	sf::ConvexShape polygon;

	if (!isCircle) {
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
engine::Transform::Transform(GameObject* obj) {
	size = sf::Vector2f(1.0f, 1.0f);
	position = sf::Vector2f(0.0f, 0.0f);
	origin = sf::Vector2f(0.0f, 0.0f);
	parentObject = obj;
	rotationDegree = 0;
	if (col != nullptr) {
		col = obj->getCollider();
	}
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
engine::GameObject::GameObject(Game* game) {
	engine = game;
	id = 0;
	shape = new ShapeComponent({ sf::Vector2f(0,0),sf::Vector2f(100,0),sf::Vector2f(100,100) },this);
	transform = new Transform(this);
	collider = new PolygonCollider({ sf::Vector2f(0,0),sf::Vector2f(100,0),sf::Vector2f(100,100) },this);
}

/* GameObject constructor and destructor */
engine::GameObject::GameObject(std::vector<sf::Vector2f> polygon, Game* game) {
	engine = game;
	id = 0;
	shape = new ShapeComponent(polygon, this);
	transform = new Transform(this);
	collider = new PolygonCollider(polygon, this);
	if (polygon.size() == 1) {
		shape->isCircle = true;
		collider->setIsCircle(true);
	}
}

/* GameObject constructor and destructor */
engine::GameObject::GameObject(PolygonCollider* col, Game* game) {
	engine = game;
	id = 0;
	transform = new Transform(this);
	shape = new ShapeComponent({ sf::Vector2f(0,0),sf::Vector2f(100,0),sf::Vector2f(100,100) }, this);
	collider = col;
}

/* GameObject constructor and destructor */
engine::GameObject::GameObject(std::vector<sf::Vector2f> polygon, PolygonCollider* col, Game* game) {
	engine = game;
	id = 0;
	transform = new Transform(this);
	shape = new ShapeComponent(polygon, this);
	collider = col;
}


engine::GameObject::~GameObject() {
	delete collider;
	delete shape;
	delete transform;
}

void engine::GameObject::destroy() {
	engine->deleteObject(this);
	delete this;
}

void engine::GameObject::makeCircle(bool isCircle) {
	shape->isCircle = isCircle;
	collider->setIsCircle(isCircle);
}

/* Public class functions */

engine::PolygonCollider* engine::GameObject::getCollider() {
	return collider;
}

void engine::GameObject::updateCollider() {

}

void engine::GameObject::removeCollider() {
	engine->removeFromRender(collider);
	delete collider;
	collider = nullptr;
}

/*
	return ShapeComponent
	Returns the shapeComponent
*/
engine::ShapeComponent* engine::GameObject::getShapeComponent() {
	return shape;
}

void engine::GameObject::removeShapeComponent() {
	engine->removeFromRender(shape);
	engine->removeFromUI(shape);
	delete shape;
	shape = nullptr;
}

/*
	return Transform
	Returns the Transform
*/
engine::Transform* engine::GameObject::getTransform() {
	return transform;
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

	camera.transform = new Transform(nullptr); // this goes crazy
	camera.transform->setPosition(sf::Vector2f(0, 0));
	camera.transform->setSize(sf::Vector2f(window->getSize().x, window->getSize().y));

	collisionManager = new collisions::CollisionManager();
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

engine::GameObject* engine::Game::makeObject(int layer, std::vector<sf::Vector2f> polygon, bool toUI) {
	GameObject* newObject = new GameObject(polygon,this);
	newObject->id = nextId;
	nextId++;
	gameObjects.push_back(newObject);
	drawShape(newObject->getShapeComponent(),toUI);
	newObject->getShapeComponent()->layer = layer;
	drawCollider(newObject->getCollider(), true);
	newObject->getCollider()->layer = layer;
	return newObject;
}

engine::GameObject* engine::Game::makeObject(int layer, float radius, bool toUI) {
	GameObject* newObject = new GameObject({sf::Vector2f(0,radius)}, this);
	newObject->id = nextId;
	nextId++;
	gameObjects.push_back(newObject);
	drawShape(newObject->getShapeComponent(), toUI);
	newObject->getShapeComponent()->layer = layer;
	drawCollider(newObject->getCollider(), true);
	newObject->getCollider()->setIsCircle(true);
	newObject->getShapeComponent()->isCircle = true;
	newObject->getCollider()->layer = layer;
	return newObject;
}

engine::GameObject* engine::Game::makeObject(int layer, std::vector<sf::Vector2f> polygon, PolygonCollider* col, bool toUI) {
	GameObject* newObject = new GameObject(polygon,col,this);
	newObject->id = nextId;
	nextId++;
	gameObjects.push_back(newObject);
	drawShape(newObject->getShapeComponent(),toUI);
	newObject->getShapeComponent()->layer = layer;
	drawCollider(newObject->getCollider(), true);
	newObject->getCollider()->layer = layer;
	return newObject;
}

engine::GameObject* engine::Game::registerObject(GameObject *obj, bool toUI) {
	obj->id = nextId;
	nextId++;
	gameObjects.push_back(obj);
	drawShape(obj->getShapeComponent(), toUI);
	drawCollider(obj->getCollider(),true);
	return obj;
}

engine::GameObject* engine::Game::getObject(unsigned int targId) {
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

bool engine::Game::deleteObject(unsigned int targId) {
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
	sf::Vector2f offset = getWindowSize()/2.0f;
	if (camera.focus != nullptr) {
		camera.transform->setPosition(vmath::subtractVectors(camera.focus->getTransform()->position, offset));
	}


	/* Collision handling */
	//collisionManager->handleCollisions(colliders);

	if (!paused) {
		updateObjects();
		tick++;
	}
}

/*
	Update each individual object
*/
void engine::Game::updateObjects() {
	for (GameObject* obj : gameObjects) {

	}
}

void engine::Game::resetScene() {
	delete manager;
	delete collisionManager;
	for (EngineComponent* e : engineComponents) { delete e; }
	for (PolygonCollider* c : colliders) { delete c; }
	for (Renderable* r : renderableObjects) { dynamicDeleteRenderable(r); }
	for (Renderable* r : uiRenderableObjects) { dynamicDeleteRenderable(r); }
	for (GameObject* obj : gameObjects) { obj->destroy(); delete obj; }

	delete window;
	init();
}

void engine::Game::dynamicDeleteRenderable(Renderable* r) {
	switch (r->type_id) {
		case 1:
			delete (reinterpret_cast<Particle*>(r));
			break;
		case 2:
			delete (reinterpret_cast<Line*>(r));
			break;
		case 3:
			break;
		case 4:
			delete (reinterpret_cast<PolygonCollider*>(r));
			break;
		case 5:
			delete (reinterpret_cast<ShapeComponent*>(r));
			break;
		case 6:
			delete (reinterpret_cast<Text*>(r));
			break;
	}
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
	sortRenderables(renderableObjects);
	int index = 0;
	for (Renderable* renderObj : renderableObjects) { // Convert each renderable object to a convex polygon that sfml can draw or directly draw it
		if (renderObj == nullptr) {
			renderableObjects.erase(renderableObjects.begin() + index);
			continue;
		}
		switch (renderObj->type_id) { // Change type_id to its respective type
		case 1:
			break;
		case 2: {
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
		case 5: {
			ShapeComponent shapeComp = *reinterpret_cast<ShapeComponent*>(renderObj);

			if (!shapeComp.isVisible) { // Skip invisible objects
				continue;
			}

			GameObject* obj = shapeComp.parentObject;
			Transform transform = *obj->getTransform();

			if (shapeComp.isCircle) {
				sf::CircleShape circle;
				circle.setPointCount(shapeComp.sides);
				circle.setRadius(shapeComp.vertices[0].y);

				circle.setOrigin(sf::Vector2f(shapeComp.vertices[0].y, shapeComp.vertices[0].y));
				circle.move(transform.getPosition());
				circle.rotate(transform.getRotation());
				circle.scale(transform.getSize());

				circle.setFillColor(shapeComp.fillColor);

				circle.move(vmath::multiplyVector(camera.transform->getPosition(), -1));

				window->draw(circle);
			} else {
				sf::ConvexShape polygon = shapeComp.constructShape();
				polygon.move(transform.getPosition());
				polygon.rotate(transform.getRotation());
				polygon.scale(transform.getSize());

				polygon.move(vmath::multiplyVector(camera.transform->getPosition(), -1));

				window->draw(polygon);
			}

			break;
		}
		case 6: {
			Text* text = reinterpret_cast<Text*>(renderObj); // Convert to Text*
			if (text->cullThis) {
				continue;
			}
			sf::Text drawText;
			
			drawText.setCharacterSize(text->charSize);
			drawText.setString(text->textString);
			drawText.setFont(*text->font);
			drawText.setFillColor(text->color);
			drawText.move(text->position);
			drawText.setOrigin(text->origin);

			window->draw(drawText); // Draw
			break;
		}
		default:
			continue;
		}
		index++;
	}

	sortRenderables(uiRenderableObjects);
	index = 0;
	for (Renderable* renderObj : uiRenderableObjects) { // Draw UI on top
		if (renderObj == nullptr) {
			uiRenderableObjects.erase(uiRenderableObjects.begin() + index);
			index--;
			continue;
		}
		switch (renderObj->type_id) { // Change type_id to its respective type
		case 1:
			break;
		case 2: {
			Line* line = reinterpret_cast<Line*>(renderObj); // Convert to Line*

			// Start, end is essentially a rect
			sf::Vector2f start = line->start;
			sf::Vector2f end = line->end;

			// Convert the line to a rectangle for drawing
			sf::Vector2f lineDir = sf::Vector2f(line->magnitude, line->thickness);
			sf::RectangleShape drawLine(lineDir);

			drawLine.setOrigin(0.0f,line->thickness * 0.5f);
			drawLine.setPosition(line->start);

			drawLine.setFillColor(line->color); // Color

			lineDir = vmath::subtractVectors(line->end, line->start);

			// Rotate to proper position
			drawLine.rotate(phys::RADTODEG * atan2f(lineDir.y, lineDir.x));

			window->draw(drawLine); // Draw
			break;
		}
		case 3:
			break;
		case 4: {
			if (!showColliders) {
				continue;
			}

			PolygonCollider* col = reinterpret_cast<PolygonCollider*>(renderObj);
			std::vector<sf::Vector2f> polygon = col->getPolygon();
			Transform transform = *col->getParent()->getTransform();

			if (col->shapeIsCircle()) {
				sf::CircleShape circle;
				circle.setPointCount(col->getPoints());
				circle.setRadius(col->getPolygon()[0].y);

				circle.setOrigin(sf::Vector2f(col->getPolygon()[0].y, col->getPolygon()[0].y));
				circle.move(transform.getPosition());
				circle.rotate(transform.getRotation());
				circle.scale(transform.getSize());

				circle.setFillColor(sf::Color(0, 0, 0, 0));
				circle.setOutlineColor(sf::Color(0, 255, 0));
				circle.setOutlineThickness(5);

				circle.move(vmath::multiplyVector(camera.transform->getPosition(), -1));

				window->draw(circle);
			} else {
				sf::ConvexShape shape;
				shape.setPointCount(polygon.size());

				for (int i = 0; i < polygon.size(); ++i) {
					shape.setPoint(i, polygon[i]);
				}

				shape.move(transform.getPosition());
				shape.rotate(transform.getRotation());
				shape.scale(transform.getSize());

				shape.setFillColor(sf::Color(0, 0, 0, 0));
				shape.setOutlineColor(sf::Color(0, 255, 0));
				shape.setOutlineThickness(5);

				shape.move(vmath::multiplyVector(camera.transform->getPosition(), -1));

				window->draw(shape);
			}

			break;
		}
		case 5: {
			ShapeComponent shapeComp = *reinterpret_cast<ShapeComponent*>(renderObj);

			if (!shapeComp.isVisible) { // Skip invisible objects
				continue;
			}

			GameObject* obj = shapeComp.parentObject;
			Transform transform = *obj->getTransform();

			if (shapeComp.isCircle) {
				sf::CircleShape circle;
				circle.setPointCount(shapeComp.sides);
				circle.setRadius(shapeComp.vertices[0].y);
				
				circle.setOrigin(sf::Vector2f(shapeComp.vertices[0].y, shapeComp.vertices[0].y));
				circle.move(transform.getPosition());
				circle.rotate(transform.getRotation());
				circle.scale(transform.getSize());

				window->draw(circle);
			} else {
				sf::ConvexShape polygon = shapeComp.constructShape();
				polygon.move(transform.getPosition());
				polygon.rotate(transform.getRotation());
				polygon.scale(transform.getSize());

				window->draw(polygon);
			}
			break;
		}
		case 6: {
			Text* text = reinterpret_cast<Text*>(renderObj); // Convert to Text*
			if (text->cullThis) {
				continue;
			}
			sf::Text drawText;
			drawText.setCharacterSize(text->charSize);
			drawText.setString(text->textString);
			drawText.setFont(*text->font);
			drawText.setFillColor(text->color);
			drawText.move(text->position);
			drawText.setOrigin(text->origin);

			window->draw(drawText); // Draw
			break;
		}
		default:
			continue;
		}
		index++;
	}

	// Write changes to window
	window->display();
}

std::vector<engine::Renderable*> engine::Game::sortRenderables(std::vector<Renderable*> renders) {
	std::sort(renders.begin(), renders.end(), std::greater<Renderable*>());
	return renders;
}

void *engine::Game::drawText(Text* text, bool isUI) {
	if (text == nullptr) {
		return nullptr;
	}
	Renderable* r = dynamic_cast<Renderable*>(text);
	(isUI) ? uiRenderableObjects.push_back(r) : renderableObjects.push_back(r);
	return static_cast<void*>(r);
}

void* engine::Game::drawLine(Line* line, bool isUI) {
	if (line == nullptr) {
		return nullptr;
	}
	Renderable* r = dynamic_cast<Renderable*>(line);
	(isUI) ? uiRenderableObjects.push_back(r) : renderableObjects.push_back(r);
	return static_cast<void*>(r);
}

void* engine::Game::drawShape(ShapeComponent *shape, bool isUI) {
	if (shape == nullptr) {
		return nullptr;
	}
	Renderable* r = dynamic_cast<Renderable*>(shape);
	(isUI) ? uiRenderableObjects.push_back(r) : renderableObjects.push_back(r);
	return static_cast<void*>(r);
}


void* engine::Game::drawCollider(PolygonCollider* col, bool registerThisCollider) {
	if (col == nullptr) {
		return nullptr;
	}
	if (registerThisCollider) {
		registerCollider(col);
	}
	Renderable* r = dynamic_cast<Renderable*>(col);
	uiRenderableObjects.push_back(r);
	return static_cast<void*>(r);
}

void engine::Game::registerCollider(PolygonCollider* col) {
	colliders.push_back(col);
}

bool engine::Game::removeCollider(PolygonCollider* col) {
	int i = 0;
	for (PolygonCollider* c : colliders) {
		if (c == col) {
			renderableObjects.erase(renderableObjects.begin() + i);
			return true;
		}
		i++;
	}
	return false;
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
	std::cout << "tick:" << tick << "|" << colorStr << str << LOG_RESET << "\n";
}

void* engine::Game::debugLine(sf::Vector2f start, sf::Vector2f end) {
	Line line = Line(start, end, sf::Color::Green);
	Renderable* r = dynamic_cast<Renderable*>(&line);
	uiRenderableObjects.push_back(dynamic_cast<Renderable*>(&line));
	return static_cast<void*>(r);
}

bool engine::Game::loadScene(std::string path) {
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(path, pt);
	std::cout << pt.get<std::string>("Section1.Value1") << std::endl;
	std::cout << pt.get<std::string>("Section1.Value2") << std::endl;
}

bool engine::Game::loadObject(std::string path) {
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(path, pt);
	std::cout << pt.get<std::string>("Section1.Value1") << std::endl;
	std::cout << pt.get<std::string>("Section1.Value2") << std::endl;
}

engine::PolygonCollider::PolygonCollider(std::vector<sf::Vector2f> polygon, GameObject* obj) {
	vertices = polygon;
	type_id = 4;
	isCircle = false;
	parentObject = obj;
}

engine::PolygonCollider::PolygonCollider(Rect r, GameObject* obj) {
	type_id = 4;
	isCircle = false;
	vertices.push_back(sf::Vector2f(r.left,r.top));
	vertices.push_back(sf::Vector2f(r.right, r.top));
	vertices.push_back(sf::Vector2f(r.right, r.bottom));
	vertices.push_back(sf::Vector2f(r.left,r.bottom));
	parentObject = obj;
}

engine::PolygonCollider::PolygonCollider(bool makeCircle, float radius, GameObject* obj) {
	type_id = 4;
	isCircle = makeCircle;
	parentObject = obj;
	vertices.push_back(sf::Vector2f(0,radius));
}

void engine::PolygonCollider::updateExtrusion() {
	Rect newExtrusion;
	newExtrusion.top = 0;
	newExtrusion.bottom = 0;
	newExtrusion.left = 0;
	newExtrusion.right = 0;

	if (vertices.size() == 0) {
		return;
	}

	for (sf::Vector2f v : vertices) {
		if (v.x < newExtrusion.left) {
			newExtrusion.left = v.x;
		}
		if (v.x > newExtrusion.right) {
			newExtrusion.right = v.x;
		}
		if (v.x < newExtrusion.top) {
			newExtrusion.top = v.y;
		}
		if (v.x > newExtrusion.bottom) {
			newExtrusion.bottom = v.y;
		}
	}

	extrusion = newExtrusion;
}

// Assume that a2 >= a1 and b2 >= b1
bool engine::collisions::overlap1D(float a1, float a2, float b1, float b2) {
	return std::min(a2, b2) - std::max(a1, b1) == 0;
}

engine::collisions::ColliderBound::ColliderBound(PolygonCollider *collider, float value) {
	coord = value;
	col = collider;
}

/* Collisions */

std::vector<engine::collisions::CollisionPair*> engine::collisions::broadSortAndSweep(std::vector<PolygonCollider*> colliders) {
	std::vector<CollisionPair*> pairs = {};
	
	std::vector<ColliderBound> xorderedBounds = {};
	std::vector<ColliderBound> yorderedBounds = {};

	/* Generate lists of bounds along x and y axis */
	for (PolygonCollider* c : colliders) {
		c->updateExtrusion();
		xorderedBounds.push_back(ColliderBound(c, c->extrusion.right));
		xorderedBounds.push_back(ColliderBound(c, c->extrusion.left));
		yorderedBounds.push_back(ColliderBound(c, c->extrusion.top));
		yorderedBounds.push_back(ColliderBound(c, c->extrusion.bottom));
	}

	std::sort(xorderedBounds.begin(), xorderedBounds.end());
	std::sort(yorderedBounds.begin(), yorderedBounds.end());
	std::vector<ColliderBound> unclosedBounds = {};

	// X coordinate
	for (ColliderBound c : xorderedBounds) {
		int i = 0;
		while (i < unclosedBounds.size() && c.col != unclosedBounds[i].col) {
			i++;
		}

		if (i == unclosedBounds.size()) { // Pair not found
			for (ColliderBound bound : xorderedBounds) {
				CollisionPair newPair;
				newPair.c1 = c.col;
				newPair.c2 = unclosedBounds[i].col;
				pairs.push_back(&newPair);
			}
			unclosedBounds.push_back(c);
		} else { // Pair found
			unclosedBounds.erase(unclosedBounds.begin() + i);
		}
	}
	
	unclosedBounds = {};

	// Y coordinate
	for (ColliderBound c : yorderedBounds) {
		int i = 0;
		while (i < unclosedBounds.size() && c.col != unclosedBounds[i].col) {
			i++;
		}

		if (i == unclosedBounds.size()) { // Pair not found
			for (ColliderBound bound : yorderedBounds) {
				CollisionPair newPair;
				newPair.c1 = c.col;
				newPair.c2 = unclosedBounds[i].col;
				pairs.push_back(&newPair);
			}
			unclosedBounds.push_back(c);
		} else { // Pair found
			unclosedBounds.erase(unclosedBounds.begin() + i);
		}
	}

	return pairs;
}

std::vector<engine::collisions::CollisionPair*> engine::collisions::narrowSAT(std::vector<CollisionPair*> colliders) {
	// work here
}

engine::collisions::CollisionManager::CollisionManager() {

}

std::vector<engine::collisions::CollisionPair*> engine::collisions::CollisionManager::calculateCollisionPairs(std::vector<PolygonCollider*> colliders) {
	return (computedCollisions = narrowSAT(broadSortAndSweep(colliders)));
}

void engine::collisions::CollisionManager::handleCollisions(std::vector<PolygonCollider*> colliders) {
	calculateCollisionPairs(colliders);
	
	std::vector<PolygonCollider*> uniqueColliders = {};
	for (int i = 0; i < computedCollisions.size(); ++i) { // Fill uniqueColliders with colliders that aren't duplicates
		PolygonCollider* c1 = computedCollisions[i]->c1;
		PolygonCollider* c2 = computedCollisions[i]->c2;

		int uniqueIndex = 0;
		while (uniqueIndex < uniqueColliders.size() && uniqueColliders[uniqueIndex] != c1) {
			++uniqueIndex;
		}
		if (uniqueIndex == uniqueColliders.size()) { // Item not found in the list
			uniqueColliders.push_back(c1);
		}

		uniqueIndex = 0;
		while (uniqueIndex < uniqueColliders.size() && uniqueColliders[uniqueIndex] != c2) {
			++uniqueIndex;
		}
		if (uniqueIndex == uniqueColliders.size()) { // Item not found in the list
			uniqueColliders.push_back(c2);
		}
	}
	
	for (PolygonCollider* pc : uniqueColliders) {
		if (pc->onCollision != nullptr) {
			((void(*)())pc->onCollision)(); // Call the oncollision function of each collider if it exists
		}
	}
}


sf::Color engine::changeAlpha(sf::Color color, int alpha) {
	return sf::Color(color.r,color.g,color.b,alpha);
}

std::vector<sf::Vector2f> engine::rectToPolygon(Rect r) {
	return { sf::Vector2f(r.left,r.top), sf::Vector2f(r.right,r.top), sf::Vector2f(r.right,r.bottom), sf::Vector2f(r.left,r.bottom) };
}
engine::Line::Line() {
	type_id = 2;
	start = sf::Vector2f(0.0f, 0.0f);
	end = sf::Vector2f(0.0f, 0.0f);
	color = sf::Color(0, 255, 0);
}

engine::Line::Line(sf::Vector2f begin, sf::Vector2f stop, sf::Color color) {
	type_id = 2;
	start = begin;
	end = stop;
	this->color = color;
}

engine::Particle::Particle(Game* engine, sf::Vector2f v) {
	type_id = 1;
	//transform = new Transform();
	//shape = new ShapeComponent();
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