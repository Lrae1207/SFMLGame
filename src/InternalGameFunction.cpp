#include "InternalGameFunction.hpp"
//https://stackoverflow.com/questions/54518519/c-setter-doesnt-change-variable-value

/* Create the pause menu and its objects */
intern::PauseMenu::PauseMenu(engine::Game* engine) {

	game = engine;
	engine::Rect backgroundShape;

	sf::Vector2f windowSize = game->getWindowSize();
	sf::Vector2f margin;
	margin.x = 300.0f;
	margin.y = 200.0f;

	backgroundShape.top = margin.y;
	backgroundShape.bottom = windowSize.y - margin.y;
	backgroundShape.left = margin.x;
	backgroundShape.right = windowSize.x - margin.x;
	background = game->makeObject(1,engine::rectToPolygon(backgroundShape),true);
	background->objName = "pauseBackground";

	background->setVisibility(false);
	background->getShapeComponent()->fillColor = engine::changeAlpha(C_DGRAY1,200);

	background->getTransform()->setPosition(sf::Vector2f(0,0));
	background->removeCollider();
	game->registerObject(background,true);

	if (pausedFont.loadFromFile("Raleway-Regular.ttf")) {
		game->debugLog("Loaded \"Raleway-Regular.ttf\"\n", LOG_GREEN);
	} else {
		game->debugLog("Failed load \"Raleway-Regular.ttf\"\n", LOG_RED);
	}

	sf::Text temp;
	temp.setFont(pausedFont);
	temp.setCharacterSize(24);
	temp.setString("Paused");

	sf::FloatRect textRect = temp.getLocalBounds();
	sf::Vector2f localOffset = sf::Vector2f(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	pausedMessage = new engine::Text(sf::Color(255, 0, 0), sf::Vector2f(windowSize.x / 2, margin.y + 15), localOffset, "Paused", 24, &pausedFont);

	pausedMessage->layer = 0;
	pausedMessage->cullThis = true;
	game->drawText(pausedMessage, true);
}

/* Show if the game is paused; Hide otherwise */
void intern::PauseMenu::update() {
 	if (game->isPaused()) { // Show menu objects
		background->getShapeComponent()->isVisible = (true);
		pausedMessage->cullThis = false;
	} else {
		background->getShapeComponent()->isVisible = (false);
		pausedMessage->cullThis = true;
	}
}