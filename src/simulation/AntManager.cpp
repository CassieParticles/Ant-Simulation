#include "AntManager.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "../graphics/AntRenderer.h"
#include "../graphics/FoodRenderer.h"
#include "../TaskParallelism/ThreadRandom.h"

AntManager::AntManager(int initialAntCount, sf::Vector2f initialPosition,sf::Vector2i worldSize) :ants{},worldSize{worldSize}
{
	foodArray.resize(worldSize.x * worldSize.y,false);
	float angle{};
	for (int i = 0; i < initialAntCount; ++i)
	{
		addAnt(initialPosition, sf::Vector2f(cos(angle), sin(angle)));
		angle += 2 * 3.14159f / initialAntCount;
	}
	
}

AntManager::~AntManager()
{
}

void AntManager::addAnt(sf::Vector2f position, sf::Vector2f moveDirection)
{
	ants.push_back(Ant{ position,moveDirection });
}

void AntManager::moveAnt(int index, float deltaTime, int threadId)
{
	//Get ant from list (race condition should be prevented in the task farm, so mutex not needed here)
	Ant& ant = ants.at(index);

	constexpr float turnSpeed = 15.f;	//In degrees cause that's what sfml uses

	float rotationOffset = (ThreadRandom::getThreadRandom()->getRandomNumber(threadId) - 0.5f) * turnSpeed;

	sf::Transform t{};
	t.rotate(rotationOffset);

	ant.moveDirection = t * ant.moveDirection;

	ant.position += moveSpeed * deltaTime * ant.moveDirection;

	if (ant.position.x<0 || ant.position.x>worldSize.x)	//Ant bounces off wall
	{
		ant.moveDirection.x *= -1;
		ant.position.x += moveSpeed * deltaTime * ant.moveDirection.x;
	}
	if (ant.position.y<0 || ant.position.y>worldSize.y)
	{
		ant.moveDirection.y *= -1;
		ant.position.y += moveSpeed * deltaTime * ant.moveDirection.y;
	}

	antRenderer->updateAntPosition(index, ant.position);

	//std::cout << "Ant " << index << " has been moved\n";
}

void AntManager::addFoodChunk(int x, int y, int width, int height)
{
	//Update the array in manager
	for (int localY = y; localY < y + height; ++localY)
	{
		for (int localX = x; localX < x + width; ++localX)
		{
			int index = localY * worldSize.x + localX;	//Get index from position
			foodArray[index] = true;
		}
	}

	//Update the texture in the renderer
	foodRenderer->updateFoodChunk(x, y, width, height, true);
}
