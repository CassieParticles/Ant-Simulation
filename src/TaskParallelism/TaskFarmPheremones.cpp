#include "TaskFarmPheremones.h"

#include <chrono>

#include "../simulation/PheremoneManager.h"

TaskFarmPheremones::TaskFarmPheremones(int threadCount, sf::Vector2i worldSize, PheremoneManager* pheremoneManager):threadCount{threadCount},pheremoneManager{ pheremoneManager }
{
	for (int i = 0; i < threadCount; ++i)
	{
		pheremoneTasks.push_back(std::queue<int>());

		std::thread t = std::thread(&TaskFarmPheremones::workerTask, this, i);
	}
	
}

TaskFarmPheremones::~TaskFarmPheremones()
{
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
		//Get task
		int task = pheremoneTasks[threadId].front();
		pheremoneTasks[threadId].pop();

		//Run task
		pheremoneManager->evaporatePheremone(task,0.01f);

	}
} 
