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
}

AntRenderer::~AntRenderer()
{
}

void AntRenderer::renderAnts()
{
	windowManager->getRenderWindow()->draw(ants);
}

void AntRenderer::updateAntPosition(int antIndex, sf::Vector2f antPos)
{
	ants[antIndex].position = antPos;
}

void AntRenderer::updateAntFood(int antIndex, bool hasFood)
{
	ants[antIndex].color = hasFood ? sf::Color(0, 125, 0, 255) : sf::Color(0, 0, 0, 255);
}
