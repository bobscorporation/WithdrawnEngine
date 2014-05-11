#ifndef _CUP_H_
#define _CUP_H_
#pragma once
#include <vector>
class Cup
{
private:
	int cupID;
	std::vector < float > vertices;
public:
	Cup(void);
	~Cup(void);
	void setCupID(int);
	int getCupID(void);
	void setVerts(std::vector < float >);
	std::vector < float > getVerts(void);
};

#endif