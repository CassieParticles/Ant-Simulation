#pragma once

#include <SFML/Graphics.hpp>

class WindowManager;

class PheremoneRenderer
{
public:
	PheremoneRenderer(WindowManager* window, sf::Vector2i worldSize);
	~PheremoneRenderer();

	void renderPheremones();

	void addFoodPheremone(int x, int y, float pheremoneStrength);
	void addHomePheremone(int x, int y, float pheremoneStrength);
protected:
	WindowManager* windowManager;

	sf::Vector2i worldSize;

	sf::Texture pheremoneTexture;
	sf::Image pheremoneImage;	//Used to read other pixel data

	sf::RectangleShape screen;
};