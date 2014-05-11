#include "Tile.h"
#include <vector>
using namespace std;

Tile::Tile(void)
{
}

Tile::~Tile(void)
{
}

void Tile::setTileID(int ID)
{
	tileID = ID;
}

int Tile::getTileID(void)
{
	return tileID;
}

void Tile::setNumNeighbors(int num)
{
	numNeighbors = num;
}

int Tile::getNumNeighbors(void)
{
	return numNeighbors;
}

void Tile::setVerts(vector < float > verts)
{
	vertices = verts;
}

vector < float > Tile::getVerts(void)
{
	return vertices;
}

void Tile::setNeighbors(vector < int > neighborIDs)
{
	neighbors = neighborIDs;
}

vector < int > Tile::getNeighbors(void)
{
	return neighbors;
}
