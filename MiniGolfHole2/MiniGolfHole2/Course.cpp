#include "Course.h"
using namespace std;

Course::Course(void)
{
}

Course::~Course(void)
{
}

Course& Course::Instance(void)
{
	static Course C;
	return C;
}

void Course::setCourse(vector < Level > levels)
{
	course = levels;
}

vector < Level > &Course::getCourse(void)
{
	return course;
}

void Course::setCurrentLevel(int hole)
{
	currentLevel = hole;
	//Clear entities
	Entity::GetRefEntityList().clear();

	Ball *ball = new Ball();
	ball->position = glm::vec3(Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getTee().getVerts()[0],Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getTee().getVerts()[1]+0.1f, Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getTee().getVerts()[2]);
	Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).setBall(*ball);
}

int Course::getCurrentLevel(void)
{
	return currentLevel;
}