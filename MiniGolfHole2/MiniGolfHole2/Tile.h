#ifndef _TILE_H_
#define _TILE_H_
#pragma once
#include <vector>
class Tile
{
private: 
	int tileID;
	int numNeighbors;
	std::vector < float > vertices;
	std::vector < int > neighbors;
public:
	Tile(void);
	~Tile(void);
	void setTileID(int);
	int getTileID(void);
	void setNumNeighbors(int);
	int getNumNeighbors(void);
	void setVerts(std::vector < float >);
	std::vector < float > getVerts(void);
	void setNeighbors(std::vector < int >);
	std::vector < int > getNeighbors(void);
	std::vector < float > orderedVerts;
};
#endif

