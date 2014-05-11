#include "HUD.h"
using namespace std;

HUD::HUD(void)
{
}

HUD::~HUD(void)
{
}

HUD& HUD::Instance(void)
{
	static HUD H;
	return H;
}

void HUD::setParValue(int val)
{
	parValue = val;
}

int HUD::getParValue(void)
{
	return parValue;
}

string HUD::convertParVal(int val)
{
	stringstream str;
	str << val;
	return str.str();
}

void HUD::drawText(float xPos, float yPos, void* font, const char* text)
{
	cout << "par value from drawText: " << text << endl;
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glRasterPos2f(xPos, yPos);
	glutBitmapString(font, (const unsigned char*)text);
}
