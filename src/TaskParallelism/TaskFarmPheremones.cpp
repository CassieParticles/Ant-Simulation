#include "TaskFarmPheremones.h"

#include <chrono>
#include <iostream>

#include "../simulation/PheremoneManager.h"

TaskFarmPheremones::TaskFarmPheremones(int threadCount, sf::Vector2i worldSize, PheremoneManager* pheremoneManager):threadCount{threadCount},pheremoneManager{ pheremoneManager }
{
	for (int i = 0; i < threadCount; ++i)
	{
		pheremoneTasks.push_back(std::queue<int>());

		std::thread t = std::thread(&TaskFarmPheremones::workerTask, this, i);
		t.detach();
	}
	
}

TaskFarmPheremones::~TaskFarmPheremones()
{
	cleanup();
}

void TaskFarmPheremones::start()
{
	doTasks = true;
	readyToStart.notify_all();
}

void TaskFarmPheremones::addTasks(int taskCount)
{
	for (int i = 0; i < taskCount; ++i)	//Push tasks equally to each thread
	{
		pheremoneTasks[i % threadCount].push(i);	
	}
}

bool TaskFarmPheremones::isTaskListEmpty()
{
	for (int i = 0; i < threadCount; ++i)	//Go through each pool, if any has tasks, don't end
	{
		if (pheremoneTasks[i].size() != 0)
		{
			return false;
		}
	}
	return true;
}

void TaskFarmPheremones::workerTask(int threadId)
{
	std::unique_lock<std::mutex> rLock(startMutex);			//Don't allow threads to start until TaskFarmPheremones::start() is called
	readyToStart.wait(rLock, [this] {return doTasks; });
	rLock.unlock();

	while (!end)	//Keep running until closing program
	{
		if (!doTasks)	//Check if threads should be doing tasks
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));	//Pause thread for 10 millisonds (will be doing this when ants are running)
			continue;
		}	

		if (pheremoneTasks[threadId].size() == 0)	//Prevent task farm taking tasks when none are available
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;	
		}

		//Get task
		std::queue<int>& taskQueue = pheremoneTasks.at(threadId);
		int task = taskQueue.front();
		taskQueue.pop();

		//Run task
		pheremoneManager->evaporatePheremone(task,0.05f);

	}
} 
