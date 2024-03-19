#include "AntRenderer.h"

#include "WindowManager.h"

AntRenderer::AntRenderer(int antCount,WindowManager* windowManager):windowManager{windowManager}
{
	ants = sf::VertexArray(sf::Points, antCount);
	windowManager->setAntRenderer(this);
	for (int i = 0; i < antCount; ++i)
	{
		ants[i].color = sf::Color(0, 0, 0, 255);
	}

	colonyShape.setRadius(10);
	colonyShape.setOrigin(10, 10);
	colonyShape.setPosition(400, 400);
	colonyShape.setFillColor(sf::Color(125, 0, 255, 255));
}

AntRenderer::~AntRenderer()
{
}

void AntRenderer::renderAnts()
{
	windowManager->getRenderWindow()->draw(colonyShape);
	windowManager->getRenderWindow()->draw(ants);
}

void AntRenderer::updateAntPosition(int antIndex, sf::Vector2f antPos)
{
	if (!windowManager->getWindowOpen()) { return; }
	ants[antIndex].position = antPos;
}

void AntRenderer::updateAntFood(int antIndex, bool hasFood)
{
	if (!windowManager->getWindowOpen()) { return; }
	ants[antIndex].color = hasFood ? sf::Color(0, 125, 125, 255) : sf::Color(0, 0, 0, 255);
}
