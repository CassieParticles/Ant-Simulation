#include "PheremoneRenderer.h"
#include "WindowManager.h"

PheremoneRenderer::PheremoneRenderer(WindowManager* window, sf::Vector2i worldSize):windowManager{window},worldSize{worldSize}
{
	window->setPheremoneRenderer(this);
	pheremoneImage.create(worldSize.x, worldSize.y, sf::Color(0, 0, 0, 0));

	pheremoneTexture.loadFromImage(pheremoneImage);

	screen.setPosition(0, 0);
	screen.setSize(sf::Vector2f(800, 800));
	screen.setTexture(&pheremoneTexture);
}

PheremoneRenderer::~PheremoneRenderer()
{
}

void PheremoneRenderer::renderPheremones()
{
	windowManager->getRenderWindow()->draw(screen);
}

void PheremoneRenderer::addFoodPheremone(int x, int y,float pheremoneStrength)
{
	if (x < 0 || x >= worldSize.x || y<0 || y>worldSize.y) { return; }	//Ensure pixel is within bounds

	//Get new image colour in pixel
	pheremoneStrength *= 255.f;				
	sf::Color currentColour = pheremoneImage.getPixel(x, y);
	currentColour.g += pheremoneStrength;
	currentColour.a += pheremoneStrength;

	pheremoneImage.setPixel(x, y, currentColour);	//Upload data to pheremone texture
	sf::Uint8 data[4]
	{
		currentColour.r,
		currentColour.g,
		currentColour.b,
		currentColour.a
	};
	pheremoneTexture.update(data, 1, 1, x, y);
}

void PheremoneRenderer::addHomePheremone(int x, int y, float pheremoneStrength)
{
	if (x < 0 || x >= worldSize.x || y<0 || y>worldSize.y) { return; }	//Ensure pixel is within bounds

	//Get new image colour in pixel
	pheremoneStrength *= 255.f;				
	sf::Color currentColour = pheremoneImage.getPixel(x, y);
	currentColour.b += pheremoneStrength;
	currentColour.a += pheremoneStrength;

	pheremoneImage.setPixel(x, y, currentColour);	//Upload data to pheremone texture
	sf::Uint8 data[4]
	{
		currentColour.r,
		currentColour.g,
		currentColour.b,
		currentColour.a
	};
	pheremoneTexture.update(data, 1, 1, x, y);
}
