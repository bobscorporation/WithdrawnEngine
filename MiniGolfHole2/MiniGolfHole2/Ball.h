#pragma once

#include "Entity.h"
#include "glm/glm.hpp"
#include <vector>

class Ball: public Entity
{
private:
	std::vector < float > vertices;
	bool insidePointA(glm::vec3, glm::vec3);
	bool insidePointB(glm::vec3, glm::vec3);
	bool insidePointC(glm::vec3, glm::vec3);
	bool insidePointD(glm::vec3, glm::vec3);
public:
	Ball(void);
	~Ball(void);
	void Update();
	bool inHole(std::vector < float >, glm::vec3);
};