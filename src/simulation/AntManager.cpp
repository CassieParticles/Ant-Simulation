#include "AntManager.h"

#include <iostream>
#include <SFML/Graphics.hpp>

AntManager::AntManager(int initialAntCount, sf::Vector2f initialPosition) :ants{}
{
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

void AntManager::moveAnt(int index, float deltaTime)
{
	//Get ant from list (race condition should be prevented in the task farm, so mutex not needed here)
	Ant& ant = ants.at(index);

	ant.position += moveSpeed * deltaTime * ant.moveDirection;

	//std::cout << "Ant " << index << " has been moved\n";
}
