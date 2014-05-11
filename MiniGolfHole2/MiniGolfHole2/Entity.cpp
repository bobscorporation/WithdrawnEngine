#include "Entity.h"
#include "Level.h"
#include "Course.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

using namespace std;

const double PI = 3.14159265358;
const double Gravity = -0.002;

 vector<Entity*> Entity::EntityList = vector<Entity*>();

Entity::Entity(void)
{
	Entity::GetRefEntityList().push_back(this);

	transformMatrix = glm::mat4(1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								0,0,0,1);

	id = Entity::GetRefEntityList().size()-1;
	hasGravity = false;
	mass = 0;
	bounceDampening = 0.3f;
}


Entity::~Entity(void)
{
}

void Entity::setVerts(std::vector < float > teeVerts)
{
	vertices = teeVerts;
}

vector < float > Entity::getVerts(void)
{
	return vertices;
}

//Updates this entity
void Entity::Update()
{
	if (hasGravity)
		ApplyForce(0,Gravity,0);

	//Apply all forces since last tick
	acceleration = lastTickForces;
	lastTickForces = glm::vec3(0);

	//Check for a collision given current force. If so, PhysicalCollision will handle the next tick's movement
	glm::vec3 intPos, intNorm, nextVel;
	nextVel = velocity+acceleration;
	//radius = 0;
	int collision = RayIntersection(intPos, intNorm, position, nextVel);
	if (collision > -2)
	{
		PhysicalCollision(intPos, intNorm, position,nextVel,collision);
		velocity += acceleration;
		//position += velocity;
	}
	else
	{
		velocity += acceleration;
		position += velocity;
		
	}


	transformMatrix = glm::translate(glm::mat4(), position);
}


//Casts a ray from rayPos and fills intersectPos and intersectNormal if there was an intersection given the magnitude and direction of rayDirec
//Return -2 = no intersection
//Return -1 = intersection with solid
//Return 0-n = intersection with Entity. Entity ID is returned
int Entity::RayIntersection(glm::vec3 &intersectPos, glm::vec3 &intersectNormal, glm::vec3 rayPos, glm::vec3 rayDirec)
{
	vector<float> distList;
	vector<glm::vec3> intPosList, intNormalList;
	vector<int> idList;
	float distClosest = 9999999999;
	glm::vec3 intPosClosest, intNormClosest;
	int idClosest;

	//Loop through all tiles and walls

	//Reference the list of vertices rather than copy them by accessing constantly
	std::vector<float> tileVerts = (Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getOrderedTileVerts());
	std::vector<float> tileNorms = (Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getTileNorms());
	for(int loop = 0; loop < 2; loop++)
	{
	int vertsSize = tileVerts.size()/9;
	for(int i=0;i<vertsSize;i++)
	{
		//Extend the triangle to a plane
		//Ax + By + Cz + D = 0
		//D is the dot product of a point on the plane and the nmal to the plane
		glm::vec3 planeNormal;
		float planeD;
		planeNormal.x = tileNorms[i*9];
		planeNormal.y = tileNorms[i*9+1];
		planeNormal.z = tileNorms[i*9+2];
		planeD = -glm::dot(glm::vec3(tileVerts[i*9], tileVerts[i*9+1], tileVerts[i*9+2]),
									glm::vec3(planeNormal.x,planeNormal.y,planeNormal.z));
		

		//Find out if the ray crossed the plane
		enum Loc {PlaneFront, PlaneBack, OnPlane};
		Loc pStartLoc, pEndLoc;
		float p;

		p = glm::dot(planeNormal,rayPos) + planeD;
		if (p > 0)
			pStartLoc = PlaneFront;
		else if (p < 0)
			pStartLoc = PlaneBack;
		else
			pStartLoc = OnPlane;

		p = glm::dot(planeNormal,rayPos + rayDirec) + planeD;
		if (p > 0)
			pEndLoc = PlaneFront;
		else if (p < 0)
			pEndLoc = PlaneBack;
		else
			pEndLoc = OnPlane;

		//No collision with this triangle
		if (pStartLoc == pEndLoc)
			continue;

		//Find the point of collision
		float t;
		glm::vec3 intersect;
		t = - (glm::dot(planeNormal, rayPos) + planeD) / glm::dot(planeNormal, glm::normalize(rayDirec));
		intersect = rayPos + glm::normalize(rayDirec)*t;

		//Determine if collision point is on the triangle
		//If the sum of all angles to the triangles vertices is 360, it is on the triangle
		glm::vec3 vt1,vt2,vt3,v1,v2,v3;
		float thetaSum;
		vt1 = glm::vec3(tileVerts[i*9],tileVerts[i*9+1],tileVerts[i*9+2]);
		vt2 = glm::vec3(tileVerts[i*9+3],tileVerts[i*9+4],tileVerts[i*9+5]);
		vt3 = glm::vec3(tileVerts[i*9+6],tileVerts[i*9+7],tileVerts[i*9+8]);

		v1 = glm::normalize(intersect - vt1);
		v2 = glm::normalize(intersect - vt2);
		v3 = glm::normalize(intersect - vt3);
		thetaSum = acos(glm::dot(v1,v2)) + acos(glm::dot(v2,v3)) + acos(glm::dot(v3,v1));

		if (std::fabs(thetaSum - (2 * PI)) < 0.1)
		{
			intersectPos = intersect;
			intersectNormal = planeNormal;
			//cout << "-- " << loop << '\n';
			//Collision
			if (distClosest > glm::length(intersectPos - rayPos)+radius)
			{
				intPosClosest = intersectPos;
				intNormClosest = intersectNormal;
				idClosest = -1;
				distClosest = glm::length(intersectPos - rayPos)+radius;
			}
			
		}
	}
	tileVerts = (Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getOrderedWallVerts());
	tileNorms = (Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getWallNorms());

	}


	//Loop through all entities
	glm::vec3 newPos = rayPos + rayDirec;
	vector<Entity*> entityList = Entity::GetRefEntityList();
	for(int i=0; i<entityList.size(); i++)
	{
		if (id == i)
			continue;

		//cout << "Entity check " << id << " -> " << i << "\n";

		Entity* e = entityList[i];
		glm::vec3 betweenVector = e->position - rayPos;
		glm::vec3 nRayDirec = glm::normalize(rayDirec);
		//Early escape 1
		//If distance between entities is too large
		if (glm::length(rayDirec) < (glm::length(betweenVector) - radius - e->radius))
			continue;

		//cout << "Entity check pass 1 " << id << " -> " << i << "\n";


		//Early escape 2
		if (glm::dot(betweenVector, rayDirec) <= 0)
			continue;

		//cout << "Entity check pass 2 " << id << " -> " << i << "\n";

		//Early escape 3
		//If closest point on path is < radii combined
		float d = glm::dot(nRayDirec, betweenVector);
		float f = (pow(glm::length(betweenVector),2) - pow(d,2));
		if ( f > pow(radius+e->radius,2))
			continue;

		//cout << "Entity check pass 3 " << id << " -> " << i << "\n";

		float dist = d-sqrt( pow(radius + e->radius, 2));
		if (dist > glm::length(rayDirec))
			continue;

		//Collision with entity i
		if (distClosest > dist)
		{
			//Move entity to potential contact point
			intPosClosest = rayPos + nRayDirec * (dist - f);
			distClosest = dist;
			idClosest = i;
		}
	}

	//No collision
	if (distClosest > 9999999)
		return -2;
	else
	{
		intersectPos = intPosClosest;
		intersectNormal = intNormClosest;
		return idClosest;
	}

}

//Does a collision based on given information
//Returns the new velocity
glm::vec3 Entity::PhysicalCollision(glm::vec3 intersectPos, glm::vec3 intersectNormal, glm::vec3 rayPos, glm::vec3 rayDirec, int entityId)
{
	//cout << "Physical Collision call\n";
	if (entityId < 0)
	{
		glm::vec3 nRayDirec = glm::normalize(rayDirec);	
		glm::vec3 nIntersectNormal = glm::normalize(intersectNormal);
		float bounceScalar = glm::length(rayDirec) - glm::length((intersectPos - rayPos));
		glm::vec3 reflectVector = glm::normalize(-2*glm::dot(nRayDirec,nIntersectNormal)*nIntersectNormal+nRayDirec);
	
		//Set position to just before the intersection position
		position = (intersectPos + nIntersectNormal*0.002f);//*radius*0.01f*/);

		glm::vec3 newVel = reflectVector * ((glm::length(rayDirec)));
		newVel -= newVel*glm::normalize(nIntersectNormal+glm::vec3(0.1,0.1,0.1))*bounceDampening;

		acceleration += newVel-rayDirec;

		//cout << "rayDirec " << glm::length(rayDirec) << '\n';
		/*cout << "rayDirec " << rayDirec.x << ", " << rayDirec.y << ", " << rayDirec.z << '\n';
		cout << "intersectNormal " << intersectNormal.x << ", " << intersectNormal.y << ", " << intersectNormal.z << '\n';
		cout << " newVel-rayDirec " <<  (newVel-rayDirec).x << ", " << (newVel-rayDirec).y << ", " << (newVel-rayDirec).z << '\n';
		*/
		//Check if new position will collide
		int collision = RayIntersection(intersectPos, intersectNormal, position, reflectVector * (bounceScalar));
		if (collision > -2)
		{
			cout << "---------------------RECURSION\n";
			PhysicalCollision(intersectPos, intersectNormal, position, reflectVector * (bounceScalar), collision);
		}
		else
		{
			//Move to the reflected position
			position += reflectVector * (bounceScalar);
		}
	
		/*cout <<  "velocity " << velocity.y << '\n';
		cout <<  "newVel " << newVel.y << '\n';
		cout <<  "acceleration " << acceleration.y << '\n';
		cout <<  "finaly " << (velocity+acceleration).y << '\n';
		cout <<  "bounceScalar " << (bounceScalar) << '\n';
		cout <<  "reflectVector " << (reflectVector).y << '\n';*/
	
		return newVel;
	}
	else
	{
		Entity* e = Entity::GetRefEntityList()[entityId];
		position = intersectPos;

		// First, find the normalized vector n from the center of 
		// circle1 to the center of circle2
		glm::vec3 n = rayPos - e->position;
		n = glm::normalize(n);
		// Find the length of the component of each of the movement
		// vectors along n. 
		float a1 = glm::dot(rayDirec,n);
		float a2 = glm::dot(e->velocity,n);

		// Using the optimized version, 
		// optimizedP =  2(a1 - a2)
		//              -----------
		//                m1 + m2
		float optimizedP = (2.0 * (a1 - a2)) / (mass + e->mass);

		// Calculate v1', the new movement vector of circle1
		// v1' = v1 - optimizedP * m2 * n
		glm::vec3 newV1 = rayDirec - optimizedP * mass * n;

		// Calculate v1', the new movement vector of circle1
		// v2' = v2 + optimizedP * m1 * n
		glm::vec3 newV2 = e->velocity + optimizedP * e->mass * n;

		//acceleration += rayDirec - newV1;
		//e->ApplyForce(e->velocity - newV2);
		velocity = newV1;
		e->velocity = newV2;

		return velocity;
	}
}


void Entity::ApplyForce(float x, float y, float z)
{
	lastTickForces += glm::vec3(x,y,z);
}

void Entity::ApplyForce(glm::vec3 force)
{
	lastTickForces += force;
}

vector < Entity* > &Entity::GetRefEntityList(void)
{
	return Entity::EntityList;
}