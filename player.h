#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"

class player
{
private:
	float xPos, yPos, zPos;

public:
	player();
	glm::vec3 getCoordinates();
};
