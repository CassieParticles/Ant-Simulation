#include "TaskFarmAnts.h"

#include <chrono>
#include <iostream>

#include "../Simulation/AntManager.h"

void workerThreadFunction(TaskFarm* farm)
{
	while (!farm->end)
	{
		farm->taskMutex.lock();	//Lock the task mutex then get a task

		if (farm->antTasks.size() == 0)	//No tasks, take a 100ms break to prevent using a lot of CPU bandwidth looping
		{
			farm->taskMutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			//std::cout << "Done tasks\n";
			continue;
		}

		int task = farm->antTasks.front();	//Get the task
		farm->antTasks.pop();

		farm->taskMutex.unlock();	//unlock the mutex

		farm->antManager->moveAnt(task, 1.f / 60.f);

	}
	return;
}


TaskFarm::TaskFarm(int threadCount,AntManager* antManager):antManager{antManager}
{
	taskMutex.lock();
	for (int i = 0; i < threadCount; ++i)	//Create worker threads, don't need to be stored since they won't be joined
	{
		std::thread t = std::thread(workerThreadFunction, this);
		t.detach();
	}

	//Add tasks
	for (int i = 0; i < 32; ++i)
	{
		antTasks.push(i);
	}

	taskMutex.unlock();	//Let worker threads execute tasks

	std::cout << "Tasks added\n";
}

TaskFarm::~TaskFarm()
{
	cleanup();
}

void TaskFarm::addAnts(int antCount)
{
	taskMutex.lock();
	for (int i = 0; i < antCount; ++i)
	{
		antTasks.push(i);
	}
	taskMutex.unlock();
}


