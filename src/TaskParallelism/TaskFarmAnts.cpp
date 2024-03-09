#include "TaskFarmAnts.h"

#include <chrono>
#include <iostream>

#include "../Simulation/AntManager.h"

void TaskFarmAnts::workerThreadFunction(int threadIndex)
{
	std::unique_lock<std::mutex>rLock(readyMutex);
	std::cout << "Thread " << threadIndex << " is ready to go\n";
	readyToStart.wait(rLock, [this] {return ready; });	//Wait until ready
	std::cout << "Thread " << threadIndex << " is going!\n";
	rLock.unlock();	//Allows all threads to activate

	while (!end)
	{
		if (!ready) 
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));	//Pause thread for 10 millisonds (will be doing this when pheremones are evapourating)
			continue;
		}
		taskMutex.lock();	//Lock the task mutex then get a task

		if (antTasks.size() == 0)	//No tasks, take a 100ms break to prevent using a lot of CPU bandwidth looping
		{
			taskMutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			//std::cout << "Done tasks\n";
			continue;
		}

		int task = antTasks.front();	//Get the task
		antTasks.pop();

		taskMutex.unlock();	//unlock the mutex

		

		antManager->moveAnt(task, 1.f / 60.f,threadIndex);

	}
	return;
}

void TaskFarmAnts::start()
{
	ready = true;
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "Threads notified\n";
	readyToStart.notify_all();
}


TaskFarmAnts::TaskFarmAnts(int threadCount,AntManager* antManager, int initialAntCount):antManager{antManager}
{
	taskMutex.lock();
	for (int i = 0; i < threadCount; ++i)	//Create worker threads, don't need to be stored since they won't be joined
	{
		std::thread t = std::thread(&TaskFarmAnts::workerThreadFunction, this,i);
		t.detach();
	}

	//Add tasks
	for (int i = 0; i < initialAntCount; ++i)
	{
		antTasks.push(i);
	}

	taskMutex.unlock();	//Let worker threads execute tasks

	std::cout << "Tasks added\n";
}

TaskFarmAnts::~TaskFarmAnts()
{
	cleanup();
}

void TaskFarmAnts::addAnts(int antCount)
{
	taskMutex.lock();
	for (int i = 0; i < antCount; ++i)
	{
		antTasks.push(i);
	}
	taskMutex.unlock();
}


