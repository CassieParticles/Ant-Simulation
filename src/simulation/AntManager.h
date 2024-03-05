#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <mutex>
#include <vector>


struct Ant
{
	sf::Vector2f position;
	sf::Vector2f moveDirection;
	sf::Vector2f desiredLocation;	//Will be set to a position if food is availible 
	bool hasFood=false;
};

class AntRenderer;
class FoodRenderer;

class AntManager
{
public:
	AntManager(int initialAntCount, sf::Vector2f initialPosition,sf::Vector2i worldSize);
	~AntManager();

	void addAntRenderer(AntRenderer* antRenderer) { this->antRenderer = antRenderer; }
	void addFoodRenderer(FoodRenderer* foodRenderer) { this->foodRenderer = foodRenderer; }

	void addAnt(sf::Vector2f position, sf::Vector2f moveDirection);

	void moveAnt(int index, float deltaTime, int threadId);

	void addFoodChunk(int x, int y, int width=1, int height=1);
	void takeFood(int x, int y);

	bool getFood(int x, int y) { return getFood(y * worldSize.x + x); }	
	bool getFood(int index);
protected:

	sf::Vector2i worldSize;

	std::vector<Ant> ants;

	AntRenderer* antRenderer;	//Access renderers, to update their data
	FoodRenderer* foodRenderer;	

	std::vector<bool> foodArray;	//Array of pixels where there is food
	std::mutex foodMutex;

	constexpr static float moveSpeed=64.f;
};