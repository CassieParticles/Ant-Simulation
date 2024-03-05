#include "FoodRenderer.h"

#include "WindowManager.h"

FoodRenderer::FoodRenderer(WindowManager* windowManager, sf::Color foodColour):foodColour{foodColour},windowManager{windowManager}
{
	rect.setPosition(0, 0);
	rect.setSize({ 800,800 });
	rect.setFillColor(foodColour);
	rect.setTexture(&foodTexture);
}

FoodRenderer::~FoodRenderer()
{
}

void FoodRenderer::renderFood()
{
	windowManager->getRenderWindow()->draw(rect);
}

void FoodRenderer::updateFoodChunk(int x, int y, int width, int height, bool food)
{
	sf::Uint8* pixels = new sf::Uint8[width * height*4];
	for (int i = 0; i < width*height*4; ++i)
	{
		pixels[i] = food * 255;	//Set pixels to true or false
	}
	foodTexture.update(pixels, width, height, x, y);
	delete[] pixels;
}
