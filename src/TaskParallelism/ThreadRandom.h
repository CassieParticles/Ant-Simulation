#pragma once
#include <vector>
#include <mutex>

class ThreadRandom
{
public:
	static ThreadRandom* getThreadRandom(int threadCount = 0);
	~ThreadRandom();

	ThreadRandom(int threadCount);
	
	float getRandomNumber(int threadId);
protected:
	static std::mutex initialisationMutex;	//Used to ensure the singleton isn't initialized twice
	static std::unique_ptr<ThreadRandom> instance;
	static bool initialized;	//Once we know the singleton is initialized, we don't need to use a mutex to protect it anymore


	constexpr static int randomListSize=1347;	//1347 values per thread * 16 threads * 4 bytes per thread
	int threadCount;

	float** randomNumbers;
	int* indices;

};
