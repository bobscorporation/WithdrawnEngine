#ifndef _TEE_H_
#define _TEE_H_
#pragma once
#include <vector>
class Tee
{
private:
	int teeID;
	std::vector < float > vertices;
public:
	Tee(void);
	~Tee(void);
	void setTeeID(int);
	int getTeeID(void);
	void setVerts(std::vector < float >);
	std::vector < float > getVerts(void);
};
#endif