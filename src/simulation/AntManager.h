#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <mutex>

struct Ant
{
	sf::Vector2f position;
	sf::Vector2f moveDirection;
};

class AntRenderer;

class AntManager
{
public:
	AntManager(int initialAntCount, sf::Vector2f initialPosition);
	~AntManager();

	void addAntRenderer(AntRenderer* antRenderer) { this->antRenderer = antRenderer; }

	void addAnt(sf::Vector2f position, sf::Vector2f moveDirection);

	void moveAnt(int index, float deltaTime);

protected:
	std::vector<Ant> ants;
	AntRenderer* antRenderer;

	constexpr static float moveSpeed=64.f;
};