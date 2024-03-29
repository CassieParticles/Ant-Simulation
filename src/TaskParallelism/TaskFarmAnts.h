#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <SFML/System/Vector2.hpp>

#include "../Simulation/AntManager.h"

class AntManager;

class TaskFarmAnts
{
	
public:
	TaskFarmAnts(int threadCount, AntManager* antManager, int initialAntCount);
	~TaskFarmAnts();

	void addAntManager(AntManager* antManager) { this->antManager = antManager; }

	void cleanup() { end = true; }

	bool isTaskListEmpty();
	void addAnts(int antCount);

	void workerThreadFunction(int threadIndex);

	void start();

	void pause() { ready = false; }
	void unpause() { ready = true; }
protected:
	//std::queue<int> antTasks;

	std::vector<std::queue<int>> antTasks;
	int threadCount;

	std::mutex taskMutex;

	std::mutex readyMutex;
	std::condition_variable readyToStart;
	bool ready{ false };

	std::thread managerThread;

	AntManager* antManager;

	bool end{ false };
};