#include "WindowManager.h"
#include "AntRenderer.h"

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

	antRenderer->renderAnts();

	window.clear(sf::Color(200,200,255,255));
	window.display();
	
}
