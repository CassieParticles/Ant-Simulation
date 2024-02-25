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

#include <chrono>


int main()
{
	AntManager antManager(6400, sf::Vector2f(400, 400));
	TaskFarm farm(16,&antManager);
	WindowManager windowManager;
	AntRenderer antRenderer(6400, &windowManager);
	antManager.addAntRenderer(&antRenderer);

	//farm.addAntManager(&manager);

	farm.start();	//Start running threads

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
			farm.addAnts(6400);
		}
	}

	return 0;
}