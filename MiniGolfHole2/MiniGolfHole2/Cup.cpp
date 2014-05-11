#include "Cup.h"
#include <vector>
using namespace std;

Cup::Cup(void)
{
}

Cup::~Cup(void)
{
}

void Cup::setCupID(int ID)
{
	cupID = ID;
}

int Cup::getCupID(void)
{
	return cupID;
}

void Cup::setVerts(vector < float > verts)
{
	vertices = verts;
}

vector < float > Cup::getVerts(void)
{
	return vertices;
}
