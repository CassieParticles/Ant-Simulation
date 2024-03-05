#include "ThreadRandom.h"

#include <iostream>
#include <random>


//Initialize static
bool ThreadRandom::initialized = false;
std::mutex ThreadRandom::initialisationMutex;
std::unique_ptr<ThreadRandom> ThreadRandom::instance;

ThreadRandom::ThreadRandom(int threadCount):threadCount{threadCount}
{
	std::mt19937 randGen;
	randGen.seed(time(0));
	
	randomNumbers = new float*[threadCount];
	indices = new int[threadCount];

	for (int t = 0; t < threadCount; ++t)
	{
		randomNumbers[t] = new float[randomListSize];
		indices[t] = 0;
		for (int n = 0; n < randomListSize; ++n)
		{
			float num= static_cast<float>(randGen()) / randGen.max();
			randomNumbers[t][n] = num;
		}
	}
}

ThreadRandom* ThreadRandom::getThreadRandom(int threadCount)
{
	if (!initialized)
	{
		std::lock_guard<std::mutex> lock(initialisationMutex);	//Protect initialization
		instance = std::make_unique<ThreadRandom>(threadCount);
		initialized = true;
	}
	return instance.get();
}

ThreadRandom::~ThreadRandom()
{
	for (int i = 0; i < threadCount;++i)
	{
		delete[] randomNumbers[i];
	}
	delete[] randomNumbers;
	delete[] indices;
}

float ThreadRandom::getRandomNumber(int threadId)
{
	indices[threadId] = (indices[threadId]++) % (randomListSize-1);	//Increment index, and spin back to 0 if it reaches 1000
	return randomNumbers[threadId][indices[threadId]];
}
