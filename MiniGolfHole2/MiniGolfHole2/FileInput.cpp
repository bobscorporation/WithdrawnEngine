#include "FileInput.h"
#include "Tile.h"
#include "Tee.h"
#include "Cup.h"
#include "Level.h"
#include "Wall.h"
#include "Course.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
using namespace std;

/*Constructors and Destructors*/
FileInput::FileInput(void)
{
}

FileInput::~FileInput(void)
{
}

/* taken from the internet
*  This function takes a string and delimits
*  the string by the parameter specified and
*  stores each substring into a vector of 
*  strings. Returns this vector.
*/
vector<string> tokenize(const string & str, const string & delim){
	vector<string> tokens;
	size_t p0 = 0, p1 = string::npos;

	while(p0 != string::npos){
		p1 = str.find_first_of(delim, p0);
		if(p1 != p0){
			string token = str.substr(p0, p1 -p0);
			tokens.push_back(token);
		}
		p0 = str.find_first_not_of(" ", p1);
	}
	return tokens;
}

/*Reads in the file and parses it accordingly*/
void FileInput::readFile(void)
{
	vector < Level > course;
	vector < string > tokens;
	vector < Tile > golfTiles;
	vector < Wall > golfWalls;
	ifstream in;
	string fileName = "course.db";
	/*cout << "Enter the hole to play: ";
	cin >> fileName;*/
	string typeID, courseName, holeName;
	string line = "";
	int tileID, numNeighbors, neighborID;
	int teeID, cupID, numberOfHoles, parValue;
	float tileVertLoc, teeVertLoc, cupVertLoc;
	in.open(fileName);
	while(!in.eof()){
		getline(in, line);
		//tokenize each line in the file
		tokens = tokenize(line, " ");
		//specifies the course name and number of holes
		if(tokens[0].compare("course") == 0){
			courseName = tokens[1].c_str();
			numberOfHoles = atoi(tokens[2].c_str());
		}
		//beginning of hole defiition, consists of tiles, tee, cup,
		//par value and name of each hole
		if(tokens[0].compare("begin_hole") == 0){
			vector < Tile > golfTiles;
			vector < Wall > golfWalls;
			vector < string > name;
			getline(in, line);
			tokens = tokenize(line, " ");
			while(tokens[0].compare("end_hole") != 0){
				//store name into variable
				if(tokens[0].compare("name") == 0){
					for(int i = 1; i < tokens.size(); i++){
						holeName = tokens[i].c_str();
						name.push_back(holeName);
					}
					Level::Instance().setHoleName(name);
				}
				getline(in, line);
				tokens = tokenize(line, " ");
				if(tokens[0].compare("par") == 0){
					parValue = atoi(tokens[1].c_str());
					Level::Instance().setParValue(parValue);
				}
				//parsing the lines representing tiles
				if(tokens[0].compare("tile") == 0){
					Tile tempTile = Tile();
					//store the tile id into the tile object
					tempTile.setTileID(atoi(tokens[1].c_str()));
					//store the number of neighbors into the tile object
					tempTile.setNumNeighbors(atoi(tokens[2].c_str()));
					numNeighbors = tempTile.getNumNeighbors();
					vector < float > tempVerts;
					vector < int > tempNeighborIDs;
					//get all of the verts and store them into the tile object
					for(int i = 3; i < numNeighbors*3+3; i++){
						tileVertLoc = atof(tokens[i].c_str());
						tempVerts.push_back(tileVertLoc);
						tempTile.setVerts(tempVerts);
					}
					Level::Instance().setOrderedTileVerts(tempVerts);
					//get all of the neighbor id's and store them into the tile object
					for(int i = numNeighbors*3+3; i < tokens.size(); i++){
						neighborID = atoi(tokens[i].c_str());
						tempNeighborIDs.push_back(neighborID);
						tempTile.setNeighbors(tempNeighborIDs);
					}
					//set the walls according to the neighbor id's
					for(int i = 0; i < tempNeighborIDs.size(); i++){
						vector < float > tempWallVerts;
						if(tempNeighborIDs[i] == 0){
							Wall tempWall = Wall();
							if(i == tempNeighborIDs.size()-1){
								tempWallVerts.push_back(tempVerts[tempVerts.size()-3]);
								tempWallVerts.push_back(tempVerts[tempVerts.size()-2]);
								tempWallVerts.push_back(tempVerts[tempVerts.size()-1]);
								tempWallVerts.push_back(tempVerts[0]);
								tempWallVerts.push_back(tempVerts[1]);
								tempWallVerts.push_back(tempVerts[2]);						
								tempWall.setWallVerts(tempWallVerts);
							}
							else{
								for(int j = i*3; j < i*3+6; j++){
									tempWallVerts.push_back(tempVerts[j]);
								}								
								tempWall.setWallVerts(tempWallVerts);
							}
							Level::Instance().setOrderedWallVerts(tempWallVerts);
							Level::Instance().setWall(tempWall);
							golfWalls.push_back(tempWall);
						}
					}
					Level::Instance().setTile(tempTile);
					golfTiles.push_back(tempTile);
				}
				Level::Instance().setWalls(golfWalls);
				Level::Instance().setTiles(golfTiles);
				//parsing the line representing a tee
				if(tokens[0].compare("tee") == 0){
					Tee tempTee = Tee();
					//store tee id into tee object
					tempTee.setTeeID(atoi(tokens[1].c_str()));
					teeID = tempTee.getTeeID();
					vector < float > tempTeeVerts;
					//store tee verts into tee object
					for(int i = 2; i < tokens.size(); i++){
						teeVertLoc = atof(tokens[i].c_str());
						tempTeeVerts.push_back(teeVertLoc);
						tempTee.setVerts(tempTeeVerts);
					}
					

					Level::Instance().setOrderedTeeVerts(tempTeeVerts);
					Level::Instance().setTee(tempTee);
				}
				//parsing the line representing a cup
				if(tokens[0].compare("cup") == 0){
					Cup tempCup = Cup();
					//store cup id into cup object
					tempCup.setCupID(atoi(tokens[1].c_str()));
					cupID = tempCup.getCupID();
					vector < float > tempCupVerts;
					//store cup verts into cup object
					for(int i = 2; i < tokens.size(); i++){
						cupVertLoc = atof(tokens[i].c_str());
						tempCupVerts.push_back(cupVertLoc);
						tempCup.setVerts(tempCupVerts);
					}
					Level::Instance().setOrderedCupVerts(tempCupVerts);
					Level::Instance().setCup(tempCup);
				}
			}
			

			course.push_back(Level::Instance());
			Level::Instance().deleteOrderedTileVerts();
			Level::Instance().deleteOrderedTeeVerts();
			Level::Instance().deleteOrderedCupVerts();
			Level::Instance().deleteOrderedWallVerts();
		}
	}
	Course::Instance().setCourse(course);
}