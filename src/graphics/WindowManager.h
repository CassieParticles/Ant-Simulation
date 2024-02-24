#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class AntRenderer;

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	void setAntRenderer(AntRenderer* antRenderer) { this->antRenderer = antRenderer; }

	void windowLoop();

	bool getWindowOpen() { return window.isOpen(); }
	sf::RenderWindow* getRenderWindow() { return &window; }
protected:
	sf::RenderWindow window;
	AntRenderer* antRenderer;
};