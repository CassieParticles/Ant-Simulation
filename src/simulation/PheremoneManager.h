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

	void evaporatePheremone(int index, float evapStrength);
protected:
	sf::Vector2i worldSize;
	std::vector<float> toFoodPheremone;
	std::vector<float> toHomePheremone;

	std::mutex foodMutex;
	std::mutex homeMutex;
};