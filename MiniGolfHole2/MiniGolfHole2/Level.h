#pragma once
#include "Ball.h"
#include "Tile.h"
#include "Cup.h"
#include "Tee.h"
#include "Wall.h"
#include "Entity.h"
#include <vector>
#include <ctime>
#ifndef _LEVEL_H_
#define _LEVEL_H_
class Level
{
private:
	Ball ball;
	Cup cup;
	Tee tee;
	Tile tile;
	Wall wall;
	std::vector < std::string > holeName;
	int parValue;
	std::vector < Tile > tiles;
	std::vector < Wall > walls;
	std::vector < float > orderedTileVerts;
	std::vector < float > tileNorms;
	std::vector < float > orderedTeeVerts;
	std::vector < float > teeNorms;
	std::vector < float > orderedCupVerts;
	std::vector < float > cupNorms;
	std::vector < float > orderedWallVerts;
	std::vector < float > wallNorms;
	std::vector < Entity* > entityList;
public:
	Level(void);
	~Level(void);
	static Level& Instance(void);
	void setTee(Tee);
	void setCup(Cup);
	void setTiles(std::vector < Tile >);
	void setBall(Ball);
	void setTile(Tile);
	void setWall(Wall);
	void setWalls(std::vector < Wall >);
	Tee getTee(void);
	Cup getCup(void);
	Tile getTile(void);
	std::vector < Tile > getTiles(void);
	Ball &getBall(void);
	Wall getWall(void);
	std::vector < Wall > getWalls(void);
	void setOrderedTileVerts(std::vector < float >);
	void setOrderedCupVerts(std::vector < float >);
	void setOrderedTeeVerts(std::vector < float >);
	void setOrderedWallVerts(std::vector < float >);
	void setHoleName(std::vector < std::string >);
	void setParValue(int);
	void deleteOrderedTileVerts(void);
	void deleteOrderedTeeVerts(void);
	void deleteOrderedWallVerts(void);
	void deleteOrderedCupVerts(void);
	std::vector < float > getOrderedTileVerts(void);
	std::vector < float > &getRefOrderedTileVerts(void);
	std::vector < float > &getOrderedCupVerts(void);
	std::vector < float > &getOrderedTeeVerts(void);
	std::vector < float > getOrderedWallVerts(void);
	std::vector < float > &getRefOrderedWallVerts(void);
	std::vector < float > getTileNorms(void);
	std::vector < float > &getRefTileNorms(void);
	std::vector < float > &getCupNorms(void);
	std::vector < float > &getTeeNorms(void);
	std::vector < float > getWallNorms(void);
	std::vector < float > &getRefWallNorms(void);
	std::vector < Entity* > &getRefEntityList(void);
		std::vector < std::string > getHoleName(void);
	int getParValue(void);

	time_t currTime;
};
#endif