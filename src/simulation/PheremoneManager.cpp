#include "PheremoneManager.h"

PheremoneManager::PheremoneManager(sf::Vector2i worldSize):worldSize{worldSize},size{worldSize.x*worldSize.y}
{
	toFoodPheremone=new float[worldSize.x * worldSize.y];
	toHomePheremone = new float[worldSize.x * worldSize.y];
	for (int i = 0; i < worldSize.x * worldSize.y; ++i)
	{
		toFoodPheremone[i] = 0;
		toHomePheremone[i] = 0;
	}
}

PheremoneManager::~PheremoneManager()
{
	delete[] toFoodPheremone;
	delete[] toHomePheremone;
}

void PheremoneManager::addFoodPheremone(int index, float strength)
{
	
	if (index < 0 || index >= size) { return; }	//Prevent array out of bounds exceptions
	std::lock_guard<std::mutex> lock(foodMutex);	//Prevent race conditions (Race conditions would be inconsiquential)
	toFoodPheremone[index] = std::min(strength + toFoodPheremone[index], 1.f);

}

void PheremoneManager::addHomePheremone(int index, float strength)
{
	if (index < 0 || index >= size) { return; }	//Prevent array out of bounds exceptions
	std::lock_guard<std::mutex> lock(homeMutex);	//Prevent race conditions (Race conditions would be inconsiquential)
	toHomePheremone[index] = std::min(strength + toHomePheremone[index], 1.f);
}

float PheremoneManager::getFoodPheremone(int index)
{
	if (index < 0 || index >= size) { return 0; }	//Prevent array out of bounds exceptions
	return toFoodPheremone[index];
}

float PheremoneManager::getHomePheremone(int index)
{
	if (index < 0 || index >= size) { return 0; }	//Prevent array out of bounds exceptions
	return toHomePheremone[index];
}

void PheremoneManager::evaporatePheremone(int index, float evapStrength)
{
	if (index < 0 || index >= size) { return; }	//Both vectors are same size, so this is fine
	//Pheremones shouldn't evapourate when ants are running, so guard isn't needed
	toHomePheremone[index] = std::max(toHomePheremone[index] - evapStrength, 0.f);
	toFoodPheremone[index] = std::max(toFoodPheremone[index] - evapStrength, 0.f);
}
