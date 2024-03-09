#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

#include "TaskParallelism/TaskFarmAnts.h"
#include "TaskParallelism/TaskFarmPheremones.h"
#include "TaskParallelism/ThreadRandom.h"

#include "Simulation/AntManager.h"
#include "simulation/PheremoneManager.h"

#include "graphics/WindowManager.h"
#include "graphics/AntRenderer.h"
#include "graphics/FoodRenderer.h"

#include <chrono>

constexpr int antCount = 1600;

sf::Vector2i worldSize= sf::Vector2i(800, 800);
sf::Vector2f antStartPos = sf::Vector2f(400, 400);

int antThreads = 12;
int pheremoneThreads = 4;

int main()
{
	PheremoneManager pheremoneManager(worldSize);
	AntManager antManager(antCount, antStartPos,worldSize,&pheremoneManager);
	TaskFarmAnts antFarm(antThreads,&antManager,antCount);
	TaskFarmPheremones pheremoneFarm(pheremoneThreads, worldSize, &pheremoneManager);
	WindowManager windowManager;
	AntRenderer antRenderer(antCount, &windowManager);
	FoodRenderer foodRenderer(&windowManager, sf::Color(255, 0, 0, 255),worldSize);
	ThreadRandom::getThreadRandom(antThreads);	//Initialize the thread random class so it can be accessed elsewhere

	antManager.addAntRenderer(&antRenderer);
	antManager.addFoodRenderer(&foodRenderer);

	antFarm.start();	//Start running threads

	antManager.addFoodChunk(64, 64, 64, 64);

	auto time = std::chrono::high_resolution_clock::now();

	while (windowManager.getWindowOpen())
	{
		windowManager.windowLoop();
		if (antFarm.isTaskListEmpty())
		{
			auto newtime= std::chrono::high_resolution_clock::now();
			long timedelta = std::chrono::duration_cast<std::chrono::milliseconds>(newtime-time).count();
			time = newtime;
			std::cout << timedelta << '\n';
			antfarm.addants(antcount);
		}

		if (pheremoneFarm.isTaskListEmpty())
		{

		}
	}

	return 0;
}