#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	void windowLoop();

	bool getWindowOpen() { return window.isOpen(); }
protected:
	sf::RenderWindow window;

};