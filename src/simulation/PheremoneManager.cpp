#include "PheremoneManager.h"

PheremoneManager::PheremoneManager(sf::Vector2i worldSize):worldSize{worldSize}
{
	toFoodPheremone.resize(worldSize.x * worldSize.y);
	toHomePheremone.resize(worldSize.x * worldSize.y);
}

PheremoneManager::~PheremoneManager()
{
}

void PheremoneManager::addFoodPheremone(int index, float strength)
{
	
	if (index < 0 || index >= toFoodPheremone.size()) { return; }	//Prevent array out of bounds exceptions
	std::lock_guard<std::mutex> lock(foodMutex);	//Prevent race conditions (Race conditions would be inconsiquential)
	toFoodPheremone[index] = std::min(strength + toFoodPheremone[index], 1.f);

}

void PheremoneManager::addHomePheremone(int index, float strength)
{
	if (index < 0 || index >= toHomePheremone.size()) { return; }	//Prevent array out of bounds exceptions
	std::lock_guard<std::mutex> lock(homeMutex);	//Prevent race conditions (Race conditions would be inconsiquential)
	toHomePheremone[index] = std::min(strength+toHomePheremone[index],1.f);
}

void PheremoneManager::evaporatePheremone(int index, float evapStrength)
{
	if (index < 0 || index >= toHomePheremone.size()) { return; }	//Both vectors are same size, so this is fine
	std::lock_guard<std::mutex> foodLock(foodMutex);
	std::lock_guard<std::mutex> homeLock(homeMutex);
	toHomePheremone[index] = std::max(toHomePheremone[index] - evapStrength, 0.f);
	toFoodPheremone[index] = std::max(toFoodPheremone[index] - evapStrength, 0.f);
}
