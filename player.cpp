#include "player.h"

/******************************************************************************

File        : player.cpp

Date        : 2023

Author      : Joey Keputa/Stephen Laycock

Description : Code for a player/camera/obj. Code from Graphics 2 module.
******************************************************************************/

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
