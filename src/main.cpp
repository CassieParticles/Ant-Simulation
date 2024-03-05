#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

#include "TaskParallelism/TaskFarmAnts.h"
#include "Simulation/AntManager.h"
#include "graphics/WindowManager.h"
#include "graphics/AntRenderer.h"
#include "graphics/FoodRenderer.h"
#include "TaskParallelism/ThreadRandom.h"

#include <chrono>

constexpr int antCount = 160;

int main()
{
	AntManager antManager(antCount, sf::Vector2f(400, 400),sf::Vector2i(800,800));
	TaskFarm farm(16,&antManager,antCount);
	WindowManager windowManager;
	AntRenderer antRenderer(antCount, &windowManager);
	FoodRenderer foodRenderer(&windowManager, sf::Color(255, 0, 0, 255),sf::Vector2i(800,800));
	ThreadRandom::getThreadRandom(16);	//Initialize the thread random class so it can be accessed elsewhere

	antManager.addAntRenderer(&antRenderer);
	antManager.addFoodRenderer(&foodRenderer);

	farm.start();	//Start running threads

	antManager.addFoodChunk(256, 256, 64, 64);

	auto time = std::chrono::high_resolution_clock::now();

	while (windowManager.getWindowOpen())
	{
		windowManager.windowLoop();
		if (farm.isTaskListEmpty())
		{
			auto newTime= std::chrono::high_resolution_clock::now();
			long timeDelta = std::chrono::duration_cast<std::chrono::milliseconds>(newTime-time).count();
			time = newTime;
			std::cout << timeDelta << '\n';
			farm.addAnts(antCount);
		}
	}

	return 0;
}