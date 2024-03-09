#include "FoodRenderer.h"

#include <iostream>

#include "WindowManager.h"

FoodRenderer::FoodRenderer(WindowManager* windowManager, sf::Color foodColour,sf::Vector2i worldSize):foodColour{foodColour},windowManager{windowManager}
{
	windowManager->setFoodRenderer(this);

	rect.setPosition(0, 0);
	rect.setSize({ 800,800 });
	rect.setFillColor(foodColour);

	//sf::Uint8* pixels = new sf::Uint8[worldSize.x * worldSize.y * 4];
	//for (int i = 0; i < worldSize.x * worldSize.y;++i)
	//{
	//	pixels[i*4+0] = 0;
	//	pixels[i*4+1] = 0;
	//	pixels[i*4+2] = 0;
	//	pixels[i*4+3] = 0;
	//}

	sf::Image image;
	image.create(800, 800, sf::Color(0,0,0,0));

	foodTexture.loadFromImage(image);

	rect.setTexture(&foodTexture);
	//delete[] pixels;
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
