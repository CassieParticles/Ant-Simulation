#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class WindowManager;

class AntRenderer
{
public:
	AntRenderer(int antCount, WindowManager* windowManager);
	~AntRenderer();

	void renderAnts();
	void updateAntPosition(int antIndex, sf::Vector2f antPos);
	void updateAntFood(int antIndex, bool hasFood);
protected:
	sf::VertexArray ants;
	WindowManager* windowManager;

	sf::CircleShape colonyShape;
};