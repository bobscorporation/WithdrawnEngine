#include "Level.h"
#include <iostream>
#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
using namespace std;

Level::Level(void)
{
	 currTime = time(0);
}

Level::~Level(void)
{
}

Level& Level::Instance(void)
{
	static Level L;
	return L;
}

void Level::setTee(Tee T)
{
	tee.setTeeID(T.getTeeID());
	tee.setVerts(T.getVerts());
}

Tee Level::getTee(void)
{
	return tee;
}

void Level::setCup(Cup C)
{
	cup.setCupID(C.getCupID());
	cup.setVerts(C.getVerts());
}

Cup Level::getCup(void)
{
	return cup;
}

void Level::setTiles(vector < Tile > Tiles){
	tiles = Tiles;
}

vector < Tile > Level::getTiles(void)
{
	return tiles;
}

void Level::setBall(Ball B)
{
	//ball.setVerts(B.getVerts());
	ball = B;
}

Ball &Level::getBall(void)
{
	return ball;
}

void Level::setTile(Tile T)
{
	tile.setTileID(T.getTileID());
	tile.setNumNeighbors(T.getNumNeighbors());
	tile.setVerts(T.getVerts());
	tile.setNeighbors(T.getNeighbors());
}

Tile Level::getTile(void)
{
	return tile;
}

void Level::setWall(Wall w)
{
	wall.setWallVerts(w.getWallVerts());
}

Wall Level::getWall(void)
{
	return wall;
}

void Level::setWalls(vector < Wall > Walls)
{
	walls = Walls;
}

vector < Wall > Level::getWalls(void)
{
	return walls;
}

/* This function orders all of the vertices
*  for each tile so that we can draw each tile
*  in triangles.
*/
void Level::setOrderedTileVerts(vector < float > tileVerts)
{
	glm::vec3 firstpt;
	glm::vec3 secondpt;
	glm::vec3 thirdpt;

	glm::vec3 cross1;
	glm::vec3 cross2;
	glm::vec3 crossproduct;

	for(int i = 0; i < tileVerts.size()/3; i++){
		if(i == 0){
			firstpt.x = tileVerts[0];
			firstpt.y = tileVerts[1];
			firstpt.z = tileVerts[2];
		}else if(i == 1){
			secondpt.x = tileVerts[3];
			secondpt.y = tileVerts[4];
			secondpt.z = tileVerts[5];
		}else{
			thirdpt.x = tileVerts[i*3];
			thirdpt.y = tileVerts[i*3+1];
			thirdpt.z = tileVerts[i*3+2];

			//push on vertices
			orderedTileVerts.push_back(firstpt.x);
			orderedTileVerts.push_back(firstpt.y);
			orderedTileVerts.push_back(firstpt.z);

			orderedTileVerts.push_back(secondpt.x);
			orderedTileVerts.push_back(secondpt.y);
			orderedTileVerts.push_back(secondpt.z);

			orderedTileVerts.push_back(thirdpt.x);
			orderedTileVerts.push_back(thirdpt.y);
			orderedTileVerts.push_back(thirdpt.z);

			//calculate normals
			cross1.x = secondpt.x - firstpt.x;
			cross2.x = thirdpt.x - firstpt.x;

			cross1.y = secondpt.y - firstpt.y;
			cross2.y = thirdpt.y - firstpt.y;

			cross1.z = secondpt.z - firstpt.z;
			cross2.z = thirdpt.z - firstpt.z;

			crossproduct = glm::cross(cross1, cross2);

			//push on normals
			tileNorms.push_back(crossproduct.x);
			tileNorms.push_back(crossproduct.y);
			tileNorms.push_back(crossproduct.z);
			
			tileNorms.push_back(crossproduct.x);
			tileNorms.push_back(crossproduct.y);
			tileNorms.push_back(crossproduct.z);
			
			tileNorms.push_back(crossproduct.x);
			tileNorms.push_back(crossproduct.y);
			tileNorms.push_back(crossproduct.z);

			//assign second pt to third pt
			secondpt.x = thirdpt.x;
			secondpt.y = thirdpt.y;
			secondpt.z = thirdpt.z;
		}
	}
}

void Level::deleteOrderedTileVerts(void)
{
	orderedTileVerts.clear();
	tileNorms.clear();
}

void Level::deleteOrderedTeeVerts(void)
{
	orderedTeeVerts.clear();
}

void Level::deleteOrderedCupVerts(void)
{
	orderedCupVerts.clear();
}

void Level::deleteOrderedWallVerts(void)
{
	orderedWallVerts.clear();
	wallNorms.clear();
}

vector < float > Level::getOrderedTileVerts(void)
{
	return orderedTileVerts;
}

vector < float > Level::getTileNorms(void)
{
	return tileNorms;
}

vector < float > &Level::getRefOrderedTileVerts(void)
{
	return orderedTileVerts;
}

vector < float > &Level::getRefTileNorms(void)
{
	return tileNorms;
}

/* This function orders all of the vertices
*  for the tee so that we can draw the tee
*  in triangles. Tee will ALWAYS be a square
*  so we push on six different points in order
*  to draw the tee in triangles.
*/
void Level::setOrderedTeeVerts(vector < float > teeVertices)
{
	float offset = 0.1;
	//push on first point
	orderedTeeVerts.push_back(teeVertices[0]-offset);
	orderedTeeVerts.push_back(teeVertices[1]+0.01);
	orderedTeeVerts.push_back(teeVertices[2]-offset);

	//push on second point
	orderedTeeVerts.push_back(teeVertices[0]-offset);
	orderedTeeVerts.push_back(teeVertices[1]+0.01);
	orderedTeeVerts.push_back(teeVertices[2]+offset);

	//push on third point
	orderedTeeVerts.push_back(teeVertices[0]+offset);
	orderedTeeVerts.push_back(teeVertices[1]+0.01);
	orderedTeeVerts.push_back(teeVertices[2]+offset);

	//need to push on first point again in order
	//to draw tee in triangles correctly
	orderedTeeVerts.push_back(teeVertices[0]-offset);
	orderedTeeVerts.push_back(teeVertices[1]+0.01);
	orderedTeeVerts.push_back(teeVertices[2]-offset);

	//need to push on third point as second point
	//for second triangle
	orderedTeeVerts.push_back(teeVertices[0]+offset);
	orderedTeeVerts.push_back(teeVertices[1]+0.01);
	orderedTeeVerts.push_back(teeVertices[2]+offset);

	//push on fourth and final point
	orderedTeeVerts.push_back(teeVertices[0]+offset);
	orderedTeeVerts.push_back(teeVertices[1]+0.01);
	orderedTeeVerts.push_back(teeVertices[2]-offset);

	//push on norms
	teeNorms.push_back(1);teeNorms.push_back(1);teeNorms.push_back(1);
}

vector < float > &Level::getOrderedTeeVerts(void)
{
	return orderedTeeVerts;
}

vector < float > &Level::getTeeNorms(void)
{
	return teeNorms;
}

/* This function orders all of the vertices
*  for the cup so that we can draw the cup
*  in triangles. Cup will be represented as
*  a hexagon.
*/
void Level::setOrderedCupVerts(vector < float > cupVertices)
{
	float offset = 0.05;
	//push on first point
	orderedCupVerts.push_back(cupVertices[0]-offset);
	orderedCupVerts.push_back(cupVertices[1]+0.01);
	orderedCupVerts.push_back(cupVertices[2]-offset);

	//push on second point
	orderedCupVerts.push_back(cupVertices[0]+offset);
	orderedCupVerts.push_back(cupVertices[1]+0.01);
	orderedCupVerts.push_back(cupVertices[2]-offset);

	//push on third point
	orderedCupVerts.push_back(cupVertices[0]+offset*2);
	orderedCupVerts.push_back(cupVertices[1]+0.01);
	orderedCupVerts.push_back(cupVertices[2]);

	//push on fourth point, which is same as first
	orderedCupVerts.push_back(cupVertices[0]-offset);
	orderedCupVerts.push_back(cupVertices[1]+0.01);
	orderedCupVerts.push_back(cupVertices[2]-offset);

	//push on fifth point, which is same as third
	orderedCupVerts.push_back(cupVertices[0]+offset*2);
	orderedCupVerts.push_back(cupVertices[1]+0.01);
	orderedCupVerts.push_back(cupVertices[2]);

	//push on sixth point
	orderedCupVerts.push_back(cupVertices[0]+offset);
	orderedCupVerts.push_back(cupVertices[1]+0.01);
	orderedCupVerts.push_back(cupVertices[2]+offset);

	//push on seventh point, which is same as first
	orderedCupVerts.push_back(cupVertices[0]-offset);
	orderedCupVerts.push_back(cupVertices[1]+0.01);
	orderedCupVerts.push_back(cupVertices[2]-offset);

	//push on eighth point, which is same as sixth
	orderedCupVerts.push_back(cupVertices[0]+offset);
	orderedCupVerts.push_back(cupVertices[1]+0.01);
	orderedCupVerts.push_back(cupVertices[2]+offset);

	//push on ninth point
	orderedCupVerts.push_back(cupVertices[0]-offset);
	orderedCupVerts.push_back(cupVertices[1]+0.01);
	orderedCupVerts.push_back(cupVertices[2]+offset);

	//push on tenth point, which is same as first
	orderedCupVerts.push_back(cupVertices[0]-offset);
	orderedCupVerts.push_back(cupVertices[1]+0.01);
	orderedCupVerts.push_back(cupVertices[2]-offset);

	//push on eleventh point, which is same as ninth
	orderedCupVerts.push_back(cupVertices[0]-offset);
	orderedCupVerts.push_back(cupVertices[1]+0.01);
	orderedCupVerts.push_back(cupVertices[2]+offset);

	//push on twelfth point
	orderedCupVerts.push_back(cupVertices[0]-offset*2);
	orderedCupVerts.push_back(cupVertices[1]+0.01);
	orderedCupVerts.push_back(cupVertices[2]);

	//push on norms
	cupNorms.push_back(1);cupNorms.push_back(1);cupNorms.push_back(1);
}

vector < float > &Level::getOrderedCupVerts(void)
{
	return orderedCupVerts;
}

vector < float > &Level::getCupNorms(void)
{
	return cupNorms;
}

void Level::setOrderedWallVerts(vector < float > wallVertices)
{
	float offset = 0.15;
	//push on first point
	orderedWallVerts.push_back(wallVertices[0]);
	orderedWallVerts.push_back(wallVertices[1]);
	orderedWallVerts.push_back(wallVertices[2]);

	//push on second point
	orderedWallVerts.push_back(wallVertices[3]);
	orderedWallVerts.push_back(wallVertices[4]);
	orderedWallVerts.push_back(wallVertices[5]);

	//push on third point
	orderedWallVerts.push_back(wallVertices[3]);
	orderedWallVerts.push_back(wallVertices[4]+offset);
	orderedWallVerts.push_back(wallVertices[5]);

	//push on first point again
	orderedWallVerts.push_back(wallVertices[0]);
	orderedWallVerts.push_back(wallVertices[1]);
	orderedWallVerts.push_back(wallVertices[2]);

	//push on fifth point, which is third point
	orderedWallVerts.push_back(wallVertices[3]);
	orderedWallVerts.push_back(wallVertices[4]+offset);
	orderedWallVerts.push_back(wallVertices[5]);

	//push on sixth point
	orderedWallVerts.push_back(wallVertices[0]);
	orderedWallVerts.push_back(wallVertices[1]+offset);
	orderedWallVerts.push_back(wallVertices[2]);

	//calculate normals
	glm::vec3 cross1, cross2, firstpt, secondpt, thirdpt, crossproduct;
	firstpt = glm::vec3(wallVertices[0],wallVertices[1]-offset,wallVertices[2]);
	secondpt = glm::vec3(wallVertices[3],wallVertices[4]-offset,wallVertices[5]);
	thirdpt = glm::vec3(wallVertices[3],wallVertices[4]+offset,wallVertices[5]);

			cross1.x = secondpt.x - firstpt.x;
			cross2.x = thirdpt.x - firstpt.x;

			cross1.y = secondpt.y - firstpt.y;
			cross2.y = thirdpt.y - firstpt.y;

			cross1.z = secondpt.z - firstpt.z;
			cross2.z = thirdpt.z - firstpt.z;

			crossproduct = -glm::cross(cross1, cross2);

			//push on normals
			wallNorms.push_back(crossproduct.x);
			wallNorms.push_back(crossproduct.y);
			wallNorms.push_back(crossproduct.z);
			
			wallNorms.push_back(crossproduct.x);
			wallNorms.push_back(crossproduct.y);
			wallNorms.push_back(crossproduct.z);
			
			wallNorms.push_back(crossproduct.x);
			wallNorms.push_back(crossproduct.y);
			wallNorms.push_back(crossproduct.z);

			wallNorms.push_back(crossproduct.x);
			wallNorms.push_back(crossproduct.y);
			wallNorms.push_back(crossproduct.z);
			
			wallNorms.push_back(crossproduct.x);
			wallNorms.push_back(crossproduct.y);
			wallNorms.push_back(crossproduct.z);
			
			wallNorms.push_back(crossproduct.x);
			wallNorms.push_back(crossproduct.y);
			wallNorms.push_back(crossproduct.z);

	//push on norms
	//wallNorms.push_back(1);wallNorms.push_back(1);wallNorms.push_back(1);
}

vector < float > Level::getOrderedWallVerts(void)
{
	return orderedWallVerts;
}

vector < float > Level::getWallNorms(void)
{
	return wallNorms;
}

void Level::setHoleName(vector < string > name)
{
	holeName = name;
}

void Level::setParValue(int value)
{
	parValue = value;
}

vector < string > Level::getHoleName(void)
{
	return holeName;
}

int Level::getParValue(void)
{
	return parValue;
}

vector < float > &Level::getRefOrderedWallVerts(void)
{
	return orderedWallVerts;
}

vector < float > &Level::getRefWallNorms(void)
{
	return wallNorms;
}

vector < Entity* > &Level::getRefEntityList(void)
{
	return entityList;
}