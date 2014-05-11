#include "Tee.h"
#include <vector>
using namespace std;

Tee::Tee(void)
{
}

Tee::~Tee(void)
{
}

void Tee::setTeeID(int ID)
{
	teeID = ID;
}

int Tee::getTeeID(void)
{
	return teeID;
}

void Tee::setVerts(std::vector < float > teeVerts)
{
	vertices = teeVerts;
}

vector < float > Tee::getVerts(void)
{
	return vertices;
}