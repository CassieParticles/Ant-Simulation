#include "AntRenderer.h"

#include "WindowManager.h"

AntRenderer::AntRenderer(int antCount,WindowManager* windowManager):windowManager{windowManager}
{
	ants = sf::VertexArray(sf::Points, antCount);
	windowManager->setAntRenderer(this);
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
