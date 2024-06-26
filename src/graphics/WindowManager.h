#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class AntRenderer;
class FoodRenderer;
class PheremoneRenderer;

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	void setAntRenderer(AntRenderer* antRenderer) { this->antRenderer = antRenderer; }
	void setFoodRenderer(FoodRenderer* foodRenderer) { this->foodRenderer = foodRenderer; }
	void setPheremoneRenderer(PheremoneRenderer* pheremoneRenderer) { this->pheremoneRenderer = pheremoneRenderer; }

	void windowLoop();

	bool getWindowOpen() { return window.isOpen(); }
	sf::RenderWindow* getRenderWindow() { return &window; }
protected:
	sf::RenderWindow window;

	AntRenderer* antRenderer;
	FoodRenderer* foodRenderer;
	PheremoneRenderer* pheremoneRenderer;
};