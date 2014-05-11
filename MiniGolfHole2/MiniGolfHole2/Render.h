#ifndef _RENDER_H_
#define _RENDER_H_
#pragma once
class Render
{
public:
	Render(void);
	~Render(void);
	void setupGlut(void);
	void setupGL(void);
	void setupShaders(void);
	void deleteShader(void);
};

#endif