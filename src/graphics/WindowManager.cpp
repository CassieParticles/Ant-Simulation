#include "WindowManager.h"
#include "AntRenderer.h"
#include "FoodRenderer.h"
#include "PheremoneRenderer.h"

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

	window.clear(sf::Color(255,255,255,255));
	pheremoneRenderer->renderPheremones();
	foodRenderer->renderFood();
	antRenderer->renderAnts();

	window.display();
	
}
