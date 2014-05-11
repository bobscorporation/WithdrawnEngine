#include "Render.h"
#include "FileInput.h"
#include "Level.h"
#include "Ball.h"
#include "Course.h"
#include "HUD.h"
#include "GL/glew.h" //must include this before gl.h
#include "GL/freeglut.h"
#include <GL/gl.h>
#include <iostream>
#include <vector>
using namespace std;

static string& implode(const vector < string >& elems, const string& delim, string& s)
{
	for(vector < string >::const_iterator ii = elems.begin(); ii != elems.end(); ++ii){
		s += (*ii);
		if(ii + 1 != elems.end()){
			s += delim;
		}
	}
	return s;
}

static string implode(const vector < string >& elems, const string& delim)
{
	string s;
	return implode(elems, delim, s);
}

void print(void)
{
		//begin printing tiles information
	cout << "Course size: " << Course::Instance().getCourse().size() << endl;
	for(int i = 0; i < Course::Instance().getCourse().size(); i++){
		string holeName = implode(Course::Instance().getCourse().at(i).getHoleName(), " ");
		cout << "Hole Name: " << holeName << endl;
		cout << "Par Value: " << Course::Instance().getCourse().at(i).getParValue() << endl;
	}
		/*
	//begin printing tiles information
	for(int i = 0; i < Level::Instance().getTiles().size(); i++){
		cout << "Tile ID: " << Level::Instance().getTiles()[i].getTileID() << endl;
		cout << "Number of Neighbors: " << Level::Instance().getTiles()[i].getNumNeighbors() << endl;
		for(int j = 0; j < Level::Instance().getTiles()[i].getVerts().size(); j++){
			cout << "Tile Verts: " << Level::Instance().getTiles()[i].getVerts()[j] << endl;
		}
		for(int j = 0; j < Level::Instance().getTiles()[i].getNeighbors().size(); j++){
			cout << "Neighbor IDs: " << Level::Instance().getTiles()[i].getNeighbors()[j] << endl;
		}
	}
	//end printing tiles information
	
	//being printing tee information
	cout << "Tee ID: " << Level::Instance().getTee().getTeeID() << endl;
	for(int i = 0; i < Level::Instance().getTee().getVerts().size(); i++){
		cout << "Tee Verts: " << Level::Instance().getTee().getVerts()[i] << endl;
	}
	//end printing tee information

	//begin printing cup information
	cout << "Cup ID: " << Level::Instance().getCup().getCupID() << endl;
	for(int i = 0; i < Level::Instance().getCup().getVerts().size(); i++){
		cout << "Cup Verts: " << Level::Instance().getCup().getVerts()[i] << endl;
	}
	//end printing cup information

	//print ordered information
	//tiles
	for(int i = 0; i < Level::Instance().getOrderedTileVerts().size(); i++){
		cout << "Ordered Tile Verts: " << Level::Instance().getOrderedTileVerts()[i] << endl;
	}
	//tee
	for(int i = 0; i < Level::Instance().getOrderedTeeVerts().size(); i++){
		cout << "Ordered Tee Verts: " << Level::Instance().getOrderedTeeVerts()[i] << endl;
	}
	//cup
	for(int i = 0; i < Level::Instance().getOrderedCupVerts().size(); i++){
		cout << "Ordered Cup Verts: " << Level::Instance().getOrderedCupVerts()[i] << endl;
	}
	//wall
	for(int i = 0; i < Level::Instance().getOrderedWallVerts().size(); i++){
		cout << "Ordered Wall Verts: " << Level::Instance().getOrderedWallVerts()[i] << endl;
	}*/
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	FileInput f;
	f.readFile();
	Course::Instance().setCurrentLevel(0);
/*
	for(int i = 0; i < Level::Instance().getTiles().size(); i++){
		Level::Instance().setOrderedTileVerts(Level::Instance().getTiles()[i].getVerts());
	}
	Level::Instance().setOrderedTeeVerts(Level::Instance().getTee().getVerts());
	Level::Instance().setOrderedCupVerts(Level::Instance().getCup().getVerts());
	for(int i = 0; i < Level::Instance().getWalls().size(); i++){
		Level::Instance().setOrderedWallVerts(Level::Instance().getWalls()[i].getWallVerts());
	}*/
	//print();
	Render r;
	r.setupGlut();
	r.setupGL();

	glewInit();
	
	r.setupShaders();

	glutMainLoop();

	r.deleteShader();

	return 0;
}