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
#include "graphics/PheremoneRenderer.h"

#include <chrono>

constexpr int antCount = 1600;

sf::Vector2i worldSize= sf::Vector2i(800, 800);
sf::Vector2f antStartPos = sf::Vector2f(400, 400);

int antThreads = 12;
int pheremoneThreads = 4;

int main()
{
	PheremoneManager pheremoneManager(worldSize);	//World systems
	AntManager antManager(antCount, antStartPos,worldSize,&pheremoneManager);

	TaskFarmAnts antFarm(antThreads,&antManager,antCount);	//Task farms
	TaskFarmPheremones pheremoneFarm(pheremoneThreads, worldSize, &pheremoneManager);

	WindowManager windowManager;	//Renderers
	AntRenderer antRenderer(antCount, &windowManager);
	FoodRenderer foodRenderer(&windowManager, sf::Color(255, 0, 0, 255),worldSize);
	PheremoneRenderer pheremoneRenderer(&windowManager, worldSize);

	ThreadRandom::getThreadRandom(antThreads);	//Initialize the thread random class so it can be accessed elsewhere

	antManager.addAntRenderer(&antRenderer);
	antManager.addFoodRenderer(&foodRenderer);
	antManager.addPheremoneRenderer(&pheremoneRenderer);


	antFarm.addAnts(antCount);
	pheremoneFarm.addTasks(worldSize.x * worldSize.y);

	antFarm.start();	//Start running threads

	antManager.addFoodChunk(64, 64, 64, 64);

	auto time = std::chrono::high_resolution_clock::now();

	long long sumTime{ 0 };
	int count{ 0 };

	while (windowManager.getWindowOpen())
	{
		windowManager.windowLoop();

		if (antFarm.isTaskListEmpty())	//If all ants are done, swap to pheremone
		{
			antFarm.pause();
			antFarm.addAnts(antCount);
			pheremoneFarm.start();	//Start and unpause are similar, but this also notifies the condition variable
		}

		if (pheremoneFarm.isTaskListEmpty())	//If all pheremones are done, swap to ants
		{
			pheremoneFarm.pause();
			pheremoneFarm.addTasks(worldSize.x * worldSize.y);
			antFarm.unpause();

			auto newTime = std::chrono::high_resolution_clock::now();	//Print time for that frame
			long timeDelta = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - time).count();
			time = newTime;
			sumTime += timeDelta;
			std::cout << "Time for tick: " << timeDelta << "\t average time per tick: "<< sumTime / ++count <<'\n';
		}
	}

	return 0;
}