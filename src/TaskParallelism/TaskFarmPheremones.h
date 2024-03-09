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

	void start();

	void cleanup() { end = true; }

	void pause() { doTasks = false; }
	void run() { doTasks = true; }

	void addTasks(int taskCount);

	bool isTaskListEmpty();
protected:
	PheremoneManager* pheremoneManager;

	std::vector<std::queue<int>> pheremoneTasks; //Vector of task queues, one for each thread
	int threadCount;

	std::mutex startMutex;
	std::condition_variable readyToStart;

	bool doTasks{ false };
	bool end{ false };



	void workerTask(int threadId);
};