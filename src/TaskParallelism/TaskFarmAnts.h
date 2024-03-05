#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <SFML/System/Vector2.hpp>

#include "../Simulation/AntManager.h"

class AntManager;

class TaskFarm
{
	
public:
	TaskFarm(int threadCount, AntManager* antManager, int initialAntCount);
	~TaskFarm();

	void addAntManager(AntManager* antManager) { this->antManager = antManager; }

	void cleanup() { end = true; }

	bool isTaskListEmpty() { return antTasks.size() == 0; }
	void addAnts(int antCount);

	void workerThreadFunction(int threadIndex);

	void start();
protected:
	std::queue<int> antTasks;
	std::mutex taskMutex;

	std::mutex readyMutex;
	std::condition_variable readyToStart;
	bool ready{ false };

	std::thread managerThread;

	AntManager* antManager;

	bool end{ false };
};