#pragma once
#ifndef _WALL_H_
#define _WALL_H_
#include <vector>
class Wall
{
private:
	std::vector < float > vertices;
public:
	Wall(void);
	~Wall(void);
	void setWallVerts(std::vector < float >);
	std::vector < float > getWallVerts(void);
};
#endif