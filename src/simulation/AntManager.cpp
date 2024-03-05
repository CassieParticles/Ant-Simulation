#include "AntManager.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "../graphics/AntRenderer.h"
#include "../graphics/FoodRenderer.h"
#include "../TaskParallelism/ThreadRandom.h"

AntManager::AntManager(int initialAntCount, sf::Vector2f initialPosition,sf::Vector2i worldSize) :ants{},worldSize{worldSize}
{
	foodArray.resize(worldSize.x * worldSize.y,false);
	ants.clear();
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
	ants.push_back(Ant{ position,moveDirection,sf::Vector2f(-1,-1)});
}

void AntManager::moveAnt(int index, float deltaTime, int threadId)
{

	//Get ant from list (race condition should be prevented in the task farm, so mutex not needed here)
	Ant& ant = ants.at(index);

	//std::cout << threadId << '\n';


	if (ant.desiredLocation.x == -1)	//if no desired location, just meandering
	{
		constexpr float turnSpeed = 15.f;	//In degrees cause that's what sfml uses

		float randomNumber = ThreadRandom::getThreadRandom()->getRandomNumber(threadId);
		float rotationOffset = (randomNumber - 0.5f) * turnSpeed;

		sf::Transform t{};
		t.rotate(rotationOffset);

		ant.moveDirection = t * ant.moveDirection;

		//Check for food
		constexpr int foodDetectRange = 5;

		for (int y = ant.position.y - foodDetectRange; y < ant.position.y + foodDetectRange; ++y)
		{
			for (int x = ant.position.x - foodDetectRange; x < ant.position.x + foodDetectRange; ++x)
			{
				if (getFood(x, y))
				{
					ant.desiredLocation = sf::Vector2f(x, y);
					sf::Vector2f desiredDirection = (ant.desiredLocation - ant.position);
					float mag = sqrt(desiredDirection.x * desiredDirection.x + desiredDirection.y * desiredDirection.y);
					desiredDirection /= mag;
					ant.moveDirection = desiredDirection;
				}
			}
		}
	}




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


	if (getFood(ant.position.x, ant.position.y))	//If ant is over food, take it
	{
		takeFood(ant.position.x, ant.position.y);
		ant.desiredLocation = sf::Vector2f(-1, -1);
	}

	antRenderer->updateAntPosition(index, ant.position);
}

void AntManager::addFoodChunk(int x, int y, int width, int height)
{
	//Update the array in manager
	foodMutex.lock();
	for (int localY = y; localY < y + height; ++localY)
	{
		for (int localX = x; localX < x + width; ++localX)
		{
			int index = localY * worldSize.x + localX;	//Get index from position
			foodArray[index] = true;
		}
	}
	foodMutex.unlock();

	//Update the texture in the renderer
	foodRenderer->updateFoodChunk(x, y, width, height, true);
}

void AntManager::takeFood(int x, int y)
{
	//foodMutex.lock();
	foodArray[y * worldSize.x + x] = false;
	//foodMutex.unlock();
	foodRenderer->updateFoodPixel(x, y, false);
}

bool AntManager::getFood(int index)
{
	bool data=foodArray.at(index);
	return data;
}