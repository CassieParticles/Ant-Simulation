#include "AntManager.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "../graphics/AntRenderer.h"
#include "../graphics/FoodRenderer.h"
#include "../graphics/PheremoneRenderer.h"
#include "../TaskParallelism/ThreadRandom.h"
#include "PheremoneManager.h"

AntManager::AntManager(int initialAntCount, sf::Vector2f initialPosition,sf::Vector2i worldSize, PheremoneManager* pheremoneManager) :ants{},worldSize{worldSize},pheremoneManager{pheremoneManager},colonyPos{initialPosition}
{
	foodArray.resize(worldSize.x * worldSize.y,false);
	ants.clear();
	float angle{};
	for (int i = 0; i < initialAntCount; ++i)
	{
		addAnt(initialPosition, sf::Vector2f(cos(angle), sin(angle)));
		angle += 2 * 3.14159f / initialAntCount;
	}

	colonyRadius = 10;
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
	constexpr float turnSpeed = 15.f;	//In degrees cause that's what sfml uses
	constexpr float offsetScalar = 1.5f;	//Scalar for random offset, lets you fine tune offset effect
	constexpr float pheremoneOverride = 1.f;	//How much pheremone needs to be read in either direction to override the ant
	constexpr float pheremoneStrengthHome{ 0.05f };	// How much of a "to home" pheremone trail an ant leaves
	constexpr float pheremoneStrengthFood{ 0.45f };	// How much of a "to food" pheremone trail an ant leaves
	//Get ant from list (race condition should be prevented in the task farm, so mutex not needed here)
	Ant& ant = ants.at(index);

	//if no desired location, just meandering, now with pheremones!
	if (ant.desiredLocation.x == -1)	
	{


		//Get random numbers for turn direction, and for pheremone bias
		float randomNumber = ThreadRandom::getThreadRandom()->getRandomNumber(threadId);
		float randomBias = ThreadRandom::getThreadRandom()->getRandomNumber(threadId);
		
		

		constexpr int pheremoneDetectRange = 10;	//Range the ant can smell pheremone

		sf::Vector2f antRight = sf::Vector2f{ ant.moveDirection.y,-ant.moveDirection.x };	//Get the right direction for ant

		//Get overall pheremones around the ant (-ve is left, +ve is right)
		float pheremoneDirection{ 0 };	//Cumulative pheremone effect on ant
		for (int y = -pheremoneDetectRange; y < pheremoneDetectRange; ++y)
		{
			for (int x = -pheremoneDetectRange; x < pheremoneDetectRange; ++x)
			{
				if (x == 0 && y == 0) { continue; }	//Don't check about pheremone directly underneath ant, since it isn't relevant

				float pheremoneStrength;	//Get relevant pheremone strength
				if (ant.hasFood)	
				{
					pheremoneStrength = pheremoneManager->getHomePheremone(x + ant.position.x, y + ant.position.y);
				}
				else
				{
					pheremoneStrength = pheremoneManager->getFoodPheremone(x + ant.position.x, y + ant.position.y);
				}


				//Pheremone calculation
				//Direction to pixel is dotted with right direction for ant, +ve is pixel is to the right, -ve if pixel is to the left, divide by dist^2 as it is faster 
				//and further away pheremones are more less effective


				float distSqr = x * x + y * y;	//Get distance from ant squared, will make pixels closer affect direction more, as they "smell" more
				float dotDir = (x * antRight.x) + (y * antRight.y);	//Get dot product, -ve if to left
				float pheremoneEffect = dotDir * pheremoneStrength / distSqr;	//The effect this pixel of pheremone will have
				pheremoneDirection += pheremoneEffect;
			}
		}
		

		if (abs(pheremoneDirection) > pheremoneOverride)	//Pheremone is strong enough to override random wander
		{
			randomNumber = signbit(pheremoneDirection);	//Set ant to turn at max speed in that direction, pheremone affects in this way, not randomly
			pheremoneDirection = 0;
		}

		//Get random 
		float rotationOffset = (randomNumber - 0.5f) * turnSpeed;

		float randomAngleOffset = pheremoneDirection * randomBias * offsetScalar;	//Add offset from pheremones
		rotationOffset += randomAngleOffset;
		

		sf::Transform t{};
		t.rotate(rotationOffset);

		ant.moveDirection = t * ant.moveDirection;

		//Check in area for nearby food

		//Range ant can smell food
		constexpr int foodDetectRange = 5;

		//Look for food
		if (!ant.hasFood)
		{
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

		//Range ant can find home
		constexpr int homeDetectRange = 35;
		//If ant has food, and is close to colony, head there
		if (ant.hasFood)
		{
			float distSqr = (ant.position.x-colonyPos.x) * (ant.position.x-colonyPos.x) + (ant.position.y-colonyPos.y) * (ant.position.y-colonyPos.y);
			if (distSqr < (colonyRadius + homeDetectRange) * (colonyRadius + homeDetectRange))
			{
				sf::Vector2f direction = colonyPos - ant.position;
				float directionMag = sqrt(direction.x * direction.x + direction.y * direction.y);
				direction /= directionMag;
				ant.moveDirection = direction;
			}
		}
	}

	//Move ant
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

	//If ant is over food, take it
	if (getFood(ant.position.x, ant.position.y)&&!ant.hasFood)	
	{
		takeFood(ant.position.x, ant.position.y);
		ant.desiredLocation = sf::Vector2f(-1, -1);
		ant.hasFood = true;
		antRenderer->updateAntFood(index, ant.hasFood);
		ant.moveDirection *= -1.f;	//Make ant turn around
	}

	//If ant is in colony with food, drop it off
	if (ant.hasFood)
	{
		float distSqr = (ant.position.x - colonyPos.x) * (ant.position.x - colonyPos.x) + (ant.position.y - colonyPos.y) * (ant.position.y - colonyPos.y);
		if (distSqr < colonyRadius * colonyRadius)
		{
			ant.desiredLocation = sf::Vector2f(-1, -1);
			ant.hasFood = false;
			ant.moveDirection *= -1.f;	//Make ant turn around
			std::cout << "Ant dropped off food!\n";
			antRenderer->updateAntFood(index, ant.hasFood);
		}
	}

	//Ant leaves a trail of pheremone

	if (ant.hasFood)
	{
		pheremoneManager->addFoodPheremone(ant.position.x, ant.position.y, pheremoneStrengthFood);
		//pheremoneRenderer->addFoodPheremone(ant.position.x, ant.position.y, pheremoneStrength);	//Pheremone renderer too slow to be usable
	}
	else
	{
		pheremoneManager->addHomePheremone(ant.position.y, ant.position.y, pheremoneStrengthHome);
		//pheremoneRenderer->addHomePheremone(ant.position.x, ant.position.y, pheremoneStrength);	//Pheremone renderer too slow to be usable
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
	foodMutex.lock();
	foodArray[y * worldSize.x + x] = false;	//Update food, then update renderer
	foodMutex.unlock();
	foodRenderer->updateFoodPixel(x, y, false);
}

bool AntManager::getFood(int index)
{
	if (index<0 || index>=foodArray.size()) { return false; }
	bool data=foodArray.at(index);
	return data;
}