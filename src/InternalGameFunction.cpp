#include "InternalGameFunction.hpp"
//https://stackoverflow.com/questions/54518519/c-setter-doesnt-change-variable-value

/* HealthBar definitions*/

void HealthBar::init() {
	maxHealth = 100.0f;
	healthPoints = maxHealth;
	baseWidth = 200.0f;
	bottomBar = game->makeObject();
	topBar = game->makeObject();

	bottomBarShape = &bottomBar->getShapeComponent();
	bottomBarTransform = &bottomBar->getTransform();

	topBarShape = &topBar->getShapeComponent();
	topBarTransform = &topBar->getTransform();

	bottomBarShape->positionOffset = sf::Vector2f(0, 0);
	bottomBarShape->origin = sf::Vector2f(0, 0);
	bottomBarShape->rectSize = sf::Vector2f(baseWidth, 25.0f);
	bottomBarTransform->position = sf::Vector2f(10, 10);
	bottomBarShape->fillColor = sf::Color(150,0,0);

	topBarShape->positionOffset = sf::Vector2f(0, 0);
	topBarShape->origin = sf::Vector2f(0, 0);
	topBarShape->rectSize = sf::Vector2f(baseWidth, 25.0f);
	topBarTransform->position = sf::Vector2f(10, 10);
	topBarShape->fillColor = sf::Color(255, 0, 0);
}

HealthBar::HealthBar(Game *engine) {
	game = engine;
	init();
}

void HealthBar::update() {
	// Update health bar
	topBar->getShapeComponent().rectSize = sf::Vector2f(baseWidth*healthPoints/maxHealth,25.0f);
}

void HealthBar::changeMaxHealth(float amount) {
	maxHealth += amount;
	if (healthPoints < 0) { healthPoints = 0; }; // Make sure hp doesnt go under 0 and that the bar doesn't strech backwards
}

void HealthBar::changeHealth(float amount) {
	healthPoints += amount;
	if (healthPoints > maxHealth) { // Clamp hp to maximum health
		healthPoints = maxHealth;
	}
}

/* PlayerControl definitions */

void PlayerControl::updatePlayer() {
	// For some reason this isn't setting the position
	playerObj->getTransform().setPosition(this->position);
	sf::Vector2f v = playerObj->getTransform().getPosition();
	
	/* Update Collider */
	Collider* col = playerObj->getCollider();
	if (col != nullptr) {
		Rect newCollider;

		sf::Vector2f topLeft = playerObj->getTransform().getPosition();
		sf::Vector2f size = playerObj->getTransform().getSize();

		newCollider.left = topLeft.x;
		newCollider.top = topLeft.y;
		newCollider.right = topLeft.x + size.x;
		newCollider.bottom = topLeft.y + size.y;
		static_cast<RectCollider*>(col)->setCollider(newCollider);
	}
}

void PlayerControl::onWDown() {
	inputMovement.y--;
}

void PlayerControl::onADown() {
	inputMovement.x--;
}

void PlayerControl::onSDown() {
	inputMovement.y++;
}

void PlayerControl::onDDown() {
	inputMovement.x++;
}
void PlayerControl::init() {
	inputForce = 0.5f;
	drag = 0.1;
	position = sf::Vector2f(80.0f,200.0f);
	playerObj = game->makeObject();
}

PlayerControl::PlayerControl(Game *engine) {
	game = engine;
	init();
}

void PlayerControl::update() {
	// Handle inputs

	inputMovement = sf::Vector2f(0.0f,0.0f);
	if (game->controller.getKeyDown(KEYCODEW) || game->controller.getKeyDown(KEYCODEUPARROW)) {
		onWDown();
	}
	if (game->controller.getKeyDown(KEYCODEA) || game->controller.getKeyDown(KEYCODELEFTARROW)) {
		onADown();
	}
	if (game->controller.getKeyDown(KEYCODES) || game->controller.getKeyDown(KEYCODEDOWNARROW)) {
		onSDown();
	}
	if (game->controller.getKeyDown(KEYCODED) || game->controller.getKeyDown(KEYCODERIGHTARROW)) {
		onDDown();
	}

	inputMovement = vmath::normalizeVector(inputMovement) * inputForce;

	// Crouching
	if (game->controller.getKeyDown(KEYCODELCNTRL) || game->controller.getKeyDown(KEYCODERCNTRL)) {
		playerObj->getTransform().setSize(sf::Vector2f(1.0f, 0.5f));
	}
	else {
		// Sprinting
		playerObj->getTransform().setSize(sf::Vector2f(1.0f, 1.0f));
		if (game->controller.getKeyDown(KEYCODELSHIFT) || game->controller.getKeyDown(KEYCODERSHIFT)) {
			inputForce = 1.50f;
		}
		else {
			inputForce = 0.5f;
		}
	}

	// Scale velocity down by a factor of drag
	velocity = (velocity + inputMovement) * (1.0f-drag);
	position += velocity;
	updatePlayer();
}

Player::Player(PlayerControl* c, HealthBar* hb) {
	controller = c;
	healthBar = hb;
}

void Player::damagePlayer(float damage) {
	healthBar->changeHealth(0 - damage);
}

void Player::healPlayer(float health) {
	healthBar->changeHealth(health);
}

Zombie::Zombie(Game* engine, Player* p) {
	game = engine;
	player = p;

	healthPoints = 100.0f;
	maxHealth = 100.0f;
}