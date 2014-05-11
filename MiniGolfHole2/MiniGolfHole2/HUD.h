#ifndef _HUD_H_
#define _HUD_H_
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "GL/glew.h" //must include this before gl.h
#include "GL/freeglut.h"
#include <GL/gl.h>

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
#pragma once
class HUD
{
private:
	int parValue;
public:
	HUD(void);
	~HUD(void);
	static HUD& Instance(void);
	void setParValue(int);
	int getParValue(void);
	std::string convertParVal(int);
	void drawText(float, float, void*, const char*);
};

#endif _HUD_H