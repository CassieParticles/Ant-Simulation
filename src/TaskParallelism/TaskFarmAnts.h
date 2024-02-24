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
	friend void workerThreadFunction(TaskFarm* farm);
public:
	TaskFarm(int threadCount, AntManager* antManager);
	~TaskFarm();

	void addAntManager(AntManager* antManager) { this->antManager = antManager; }

	void cleanup() { end = true; }

	bool isTaskListEmpty() { return antTasks.size() == 0; }
	void addAnts(int antCount);

protected:
	std::queue<int> antTasks;
	std::mutex taskMutex;

	std::thread managerThread;

	AntManager* antManager;

	bool end{ false };
};