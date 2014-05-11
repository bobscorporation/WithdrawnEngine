#include "Wall.h"
#include <vector>
using namespace std;

Wall::Wall(void)
{
}

Wall::~Wall(void)
{
}

void Wall::setWallVerts(vector < float > wallVerts)
{
	vertices = wallVerts;
}

vector < float > Wall::getWallVerts(void)
{
	return vertices;
}