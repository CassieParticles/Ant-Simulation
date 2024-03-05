#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class WindowManager;

class FoodRenderer
{
public:
	FoodRenderer(WindowManager* windowManager,sf::Color foodColour,sf::Vector2i worldSize);
	~FoodRenderer();

	void renderFood();
	void updateFoodPixel(int x, int y, bool food) { updateFoodChunk(x, y, 1, 1, food); }
	void updateFoodChunk(int x, int y, int width, int height, bool food);
protected:
	WindowManager* windowManager;

	sf::Texture foodTexture;

	sf::Color foodColour;
	sf::RectangleShape rect;
};