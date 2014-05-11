#include "Ball.h"
#include "Entity.h"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

#include <vector>
using namespace std;

Ball::Ball(void)
{
	hasGravity = true;


	//Create ball
	const double PI = 3.1415926535897;
	//Initial triangle number factor
	const float space = 20;
	//Scale
	radius = 0.1;
	//mass
	mass = 1;

	vector <float> vertices = vector<float>();
	for(double b=0;b<=180-space;b+=space)
	{
		for(double a = 0; a <= 360-space; a+=space)
		{
			vertices.push_back(radius * sin((a) / 180 * PI) * sin((b) / 180 * PI));
			vertices.push_back(radius * cos((a) / 180 * PI) * sin((b) / 180 * PI));
			vertices.push_back(radius * cos((b) / 180 * PI));

			vertices.push_back(radius * sin((a) / 180 * PI) * sin((b+space) / 180 * PI));
			vertices.push_back(radius * cos((a) / 180 * PI) * sin((b+space) / 180 * PI));
			vertices.push_back(radius * cos((b+space) / 180 * PI));

			vertices.push_back(radius * sin((a+space) / 180 * PI) * sin((b) / 180 * PI));
			vertices.push_back(radius * cos((a+space) / 180 * PI) * sin((b) / 180 * PI));
			vertices.push_back(radius * cos((b) / 180 * PI));

			vertices.push_back(radius * sin((a+space) / 180 * PI) * sin((b) / 180 * PI));
			vertices.push_back(radius * cos((a+space) / 180 * PI) * sin((b) / 180 * PI));
			vertices.push_back(radius * cos((b) / 180 * PI));

			vertices.push_back(radius * sin((a+space) / 180 * PI) * sin((b+space) / 180 * PI));
			vertices.push_back(radius * cos((a+space) / 180 * PI) * sin((b+space) / 180 * PI));
			vertices.push_back(radius * cos((b+space) / 180 * PI));

			vertices.push_back(radius * sin((a) / 180 * PI) * sin((b+space) / 180 * PI));
			vertices.push_back(radius * cos((a) / 180 * PI) * sin((b+space) / 180 * PI));
			vertices.push_back(radius * cos((b+space) / 180 * PI));
		}
	}
	setVerts(vertices);
}


Ball::~Ball(void)
{
}


void Ball::Update()
{

	//Update super class
	Entity::Update();
}

bool Ball::insidePointA(glm::vec3 cupCoords, glm::vec3 ballCoords)
{
	if((ballCoords.x < cupCoords.x) /*&& (ballCoords.y < cupCoords.y) */&& (ballCoords.z < cupCoords.z)){
		//cout << "A\n";
		return true;
	}
	return false;
}

bool Ball::insidePointB(glm::vec3 cupCoords, glm::vec3 ballCoords)
{
	if((ballCoords.x < cupCoords.x)/* && (ballCoords.y < cupCoords.y)*/ && (ballCoords.z > cupCoords.z)){
		//cout << "B\n";
		return true;
	}
	return false;
}

bool Ball::insidePointC(glm::vec3 cupCoords, glm::vec3 ballCoords)
{
	if((ballCoords.x > cupCoords.x)/* && (ballCoords.y < cupCoords.y)*/ && (ballCoords.z > cupCoords.z)){
		//cout << "C\n";
		return true;
	}
	return false;
}

bool Ball::insidePointD(glm::vec3 cupCoords, glm::vec3 ballCoords)
{
	if((ballCoords.x > cupCoords.x)/* && (ballCoords.y < cupCoords.y) */&& (ballCoords.z < cupCoords.z)){
		//cout << "D\n";
		return true;
	}
	return false;
}

bool Ball::inHole(vector < float > cupVerts, glm::vec3 ball)
{
	glm::vec3 cupA, cupB, cupC, cupD;
	
	float cupOffset = 0.1;

	//make an invisible box that the ball has to be in in order to get in the hole
	//store into vec3's for easier checking
	cupA.x = cupVerts[0] + cupOffset;
	cupA.y = cupVerts[1];
	cupA.z = cupVerts[2] + cupOffset;

	cupB.x = cupVerts[0] + cupOffset;
	cupB.y = cupVerts[1];
	cupB.z = cupVerts[2] - cupOffset;

	cupC.x = cupVerts[0] - cupOffset;
	cupC.y = cupVerts[1];
	cupC.z = cupVerts[2] - cupOffset;

	cupD.x = cupVerts[0] - cupOffset;
	cupD.y = cupVerts[1];
	cupD.z = cupVerts[2] + cupOffset;

	//get the ball vertices and store into a vec3 for easier checking

	float totalVelocity = glm::length(velocity);
	//cout << "check\n";
	if(totalVelocity < 1.04 && insidePointA(cupA, ball) && insidePointB(cupB, ball)
		&& insidePointC(cupC, ball) && insidePointD(cupD, ball)){
			return true;
	}
	return false;
}



