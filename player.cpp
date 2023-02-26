#include "player.h"

player::player()
{
	xPos = 0.0;
	yPos = 0.0;
	zPos = 0.0;
}

glm::vec3 player::getCoordinates()
{
	return glm::vec3(xPos, yPos, zPos);
}
