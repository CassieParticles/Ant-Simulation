#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <mutex>

class PheremoneManager
{
public:
	PheremoneManager(sf::Vector2i worldSize);
	~PheremoneManager();

	void addFoodPheremone(int x, int y, float strength) { addFoodPheremone(y * worldSize.x + x, strength); }
	void addFoodPheremone(int index, float strength);

	void addHomePheremone(int x, int y, float strength) { addHomePheremone(y * worldSize.x + x, strength); }
	void addHomePheremone(int index, float strength);

	float getFoodPheremone(int x, int y) { return getFoodPheremone(y * worldSize.x + x); }
	float getFoodPheremone(int index);
	float getHomePheremone(int x, int y) { return getHomePheremone(y * worldSize.x + x); }
	float getHomePheremone(int index);


	void evaporatePheremone(int index, float evapStrength);
protected:
	sf::Vector2i worldSize;

	float* toFoodPheremone;
	float* toHomePheremone;
	int size;

	std::mutex foodMutex;
	std::mutex homeMutex;
};