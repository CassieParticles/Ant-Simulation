#pragma once

#include <thread>
#include <array>
#include <queue>
#include <mutex>
#include "../simulation/PheremoneManager.h"

class PheremoneManager;

class TaskFarmPheremones
{
public:
	TaskFarmPheremones(int threadCount, sf::Vector2i worldSize, PheremoneManager* pheremoneManager);
	~TaskFarmPheremones();

	void cleanup() { end = true; }

	void pause() { doTasks = false; }
	void run() { doTasks = true; }
protected:
	PheremoneManager* pheremoneManager;

	std::vector<std::queue<int>> pheremoneTasks; //Vector of task queues, one for each thread
	int threadCount;

	void addTasks(int taskCount);

	void workerTask(int threadId);

	bool doTasks{ false };
	bool end{ false };
};