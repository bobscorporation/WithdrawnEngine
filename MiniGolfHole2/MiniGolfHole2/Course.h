#ifndef _COURSE_H_
#define _COURSE_H_
#include "Level.h"
#pragma once
class Course
{
private:
	std::vector < Level > course;
	int currentLevel;
public:
	Course(void);
	~Course(void);
	static Course& Instance(void);
	void setCourse(std::vector < Level >);
	std::vector < Level > &getCourse(void);
	void setCurrentLevel(int);
	int getCurrentLevel(void);
};

#endif