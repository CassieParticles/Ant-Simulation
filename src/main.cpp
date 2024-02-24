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
	AntManager antManager(64000, sf::Vector2f(512, 512));
	TaskFarm farm(16,&antManager);
	WindowManager windowManager;
	AntRenderer antRenderer(64000, &windowManager);
	

	//farm.addAntManager(&manager);

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
			//std::cout << "Adding more ant\n";
			farm.addAnts(64000);
		}
	}

	return 0;
}