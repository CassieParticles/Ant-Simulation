#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <mutex>

struct Ant
{
	sf::Vector2f position;
	sf::Vector2f moveDirection;
};

class AntManager
{
public:
	AntManager(int initialAntCount, sf::Vector2f initialPosition);
	~AntManager();

	void addAnt(sf::Vector2f position, sf::Vector2f moveDirection);

	void moveAnt(int index, float deltaTime);

protected:
	std::vector<Ant> ants;

	constexpr static float moveSpeed=16.f;
};