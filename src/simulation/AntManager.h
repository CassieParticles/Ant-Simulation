#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <mutex>
#include <vector>


struct Ant
{
	sf::Vector2f position;
	sf::Vector2f moveDirection;
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

protected:

	sf::Vector2i worldSize;

	std::vector<Ant> ants;

	AntRenderer* antRenderer;	//Access renderers, to update their data
	FoodRenderer* foodRenderer;	

	std::vector<bool> foodArray;	//Array of pixels where there is food

	constexpr static float moveSpeed=64.f;
};