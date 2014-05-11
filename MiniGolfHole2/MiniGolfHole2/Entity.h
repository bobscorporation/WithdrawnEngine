#pragma once

#include <vector>
#include "glm\glm.hpp"

//Entities can collide with other Entities or Solids
//Entities can only be spheres
class Entity
{

public:
	Entity(void);
	~Entity(void);
	virtual void Update();
	int RayIntersection(glm::vec3 &intersectPos, glm::vec3 &intersectNormal, glm::vec3 rayPos, glm::vec3 rayDirec);
	glm::vec3 PhysicalCollision(glm::vec3 intersectPos, glm::vec3 intersectNormal, glm::vec3 rayPos, glm::vec3 rayDirec, int entityId);
	void ApplyForce(float x, float y, float z);
	void ApplyForce(glm::vec3 force);
	static std::vector < Entity* > &GetRefEntityList(void);

	void setVerts(std::vector < float >);
	std::vector<float> getVerts(void);

	int id;
	bool hasGravity;
	float radius;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 lastTickForces;
	glm::mat4 transformMatrix;

	float mass;
	float bounceDampening;

	static std::vector < Entity* > EntityList;

private:
	std::vector < float > vertices;


};

