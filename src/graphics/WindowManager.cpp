#include "WindowManager.h"

WindowManager::WindowManager():window{sf::VideoMode(800,800),"Ant simulation"}
{
	
}

WindowManager::~WindowManager()
{

}

void WindowManager::windowLoop()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
	}

	window.clear();
	window.display();
	
}
