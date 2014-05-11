#include "Render.h"
#include "Level.h"
#include "Course.h"
#include "HUD.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>

#include "GL/glew.h" //must include this before gl.h
#include "GL/freeglut.h"
#include <GL/gl.h>

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"

using namespace std;

class Shader {
public:
    Shader(string vertFile, string fragFile) { fromFiles(vertFile, fragFile); }

    /**
     * Creates a shader program based on vertex and fragment source.
     *
     * @param vertFile Path to vertex source
     * @param fragFile Path to fragment source
     */
    void fromFiles(string vertFile, string fragFile) {
        //These are shader objects containing the shader source code
        GLint vSource = setShaderSource(vertFile, GL_VERTEX_SHADER);
        GLint fSource = setShaderSource(fragFile, GL_FRAGMENT_SHADER);

        //Create a new shader program
        program = glCreateProgram();

        //Compile the source code for each shader and attach it to the program.
        glCompileShader(vSource);
        printLog("vertex compile log: ", vSource);
        glAttachShader(program, vSource);

        glCompileShader(fSource);
        printLog("fragment compile log: ", fSource);
        glAttachShader(program, fSource);

        //we could attach more shaders, such as a geometry or tessellation
        //shader here.

        //link all of the attached shader objects
        glLinkProgram(program);
    }

    /**
     * Helper method for reading in the source for a shader and creating a
     * shader object.
     *
     * @param file Filename of shader source
     * @param type Type of shader-> Only GL_VERTEX_SHADER and GL_FRAGMENT_SHADER
     *   are supported here.
     */
    GLint setShaderSource(string file, GLenum type) {
        //read source code
        ifstream fin(file.c_str());
        if (fin.fail()) {
            cerr << "Could not open " << file << " for reading" << endl;
            return -1;
        }
        fin.seekg(0, ios::end);
        int count  = fin.tellg();
        char *data = NULL;
        if (count > 0) {
            fin.seekg(ios::beg);
            data = new char[count+1];
            fin.read(data,count);
            data[count] = '\0';
        }
        fin.close();

        //create the shader
        GLint s = glCreateShader(type);
        glShaderSource(s, 1, const_cast<const char **>(&data), NULL);
        delete [] data;
        return s;
    }

    /**
     * Helper function used for debugging.
     */
    void printLog(std::string label, GLint obj) {
        int infologLength = 0;
        int maxLength = 256;

        if(glIsShader(obj)) {
            glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
        } else {
            glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
        }

        char infoLog[256];

        if (glIsShader(obj)) {
            glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
        } else {
            glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);
        }

        if (infologLength > 0) {
            cerr << label << infoLog << endl;
        }
    }

    GLint program; //shader program
    GLint modelViewLoc; //location of the modelview matrix in the program (M)
    GLint projectionLoc; //location of the projection matrix in the program (P)
    GLint normalMatrixLoc; //location of the normal matrix in the program (M_n)
    GLint vertexLoc, normalLoc; //vertex attribute locations (pos and norm)
      //respectively
    GLint timeLoc; //location of time variable
	GLint colorLoc;	// location color
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint directionLoc;
	GLuint lightcolorLoc;
};

//Global camera manipulation variables
bool press_x = false;
bool press_y = false;
bool press_z = false;
bool press_s = false;
bool press_c = false;
bool press_o = false;
bool press_r = false;
bool press_t = false;
bool press_left = false;
bool press_right = false;
bool press_up = false;
bool press_down = false;
bool press_powerup = false;
bool press_powerdown = false;

bool left_Mouse = false;
bool right_Mouse = false;
int clickinit = 0;
int clickcurr = 0;
float clickchange_x = 0;
float clickchange_y = 0;
float clickchange_z = 0;
float clickchange_s = 200;
float clickchange_rx = 0;
float clickchange_ry = 0;
float clickchange_rz = 0;
float camchange_rx = 0;
float camchange_ry = 0;
float camchange_rz = 0;
float camChange_x = 0;
float camChange_y = 0;
float camChange_z = 0;
float camchange_s = 0;
float camrot_x = 0;
float camrot_y = 0;
float camrot_z = 0;
float camzoom = 0;

int currentLevel = 0;


/* Constructors and Destructors*/
Render::Render(void)
{
}

Render::~Render(void)
{
}

int WIN_WIDTH = 512, WIN_HEIGHT = 512;
int mainWindow;
glm::mat4 projection, modelView, cameraView, camera;
Shader *shader = NULL;
float animTime = 0.0f, deltaT = 0.0001; //animation variables


glm::vec3 clubDirec = glm::vec3(0,0,0);
glm::vec3 clubTo = glm::vec3(1,1,1);;
float clubVAngle = 0;
float clubHAngle = 180;
float clubPower = 1;

bool massSpawn = false;

void reloadBuffers(Shader *passed, vector < float > objectVerts, vector < float > objectNorms)
{

	//reloads the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, passed->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		objectVerts.size() * sizeof(float),
		objectVerts.data(),
		GL_STATIC_DRAW);
	
	//same for normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, passed->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		objectNorms.size() * sizeof(float),
		objectNorms.data(),
		GL_STATIC_DRAW);

	//which buffer we want to use
	glBindBuffer(GL_ARRAY_BUFFER, passed->vertexBuffer);
	glEnableVertexAttribArray(passed->vertexLoc); //enable attribute
	glVertexAttribPointer(passed->vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//same procedure for normals
	glBindBuffer(GL_ARRAY_BUFFER, passed->normalBuffer);
	glEnableVertexAttribArray(passed->normalLoc); //enable attribute
	glVertexAttribPointer(passed->normalLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL);

}

void update(float dt)
{

	float max_tot = abs(20);
	animTime += dt;
	
	//Update all entites
	for(int i=0;i<Entity::GetRefEntityList().size();i++)
	{
		Entity::GetRefEntityList()[i]->Update();
	}

	camera = glm::lookAt(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	projection = glm::perspective(
		glm::float_t(45),
		glm::float_t(WIN_WIDTH) / glm::float_t(WIN_HEIGHT),
		glm::float_t(0.1),
		glm::float_t(1000.0));


	if (press_up)
		clubVAngle += 0.02;
	if (press_down)
		clubVAngle -= 0.02;
	if (press_left)
		clubHAngle += 0.02;
	if (press_right)
		clubHAngle -= 0.02;
	if (press_powerup)
		clubPower += 0.01;
	if (press_powerdown)
		clubPower -= 0.01;

	clubTo.x = sin(clubHAngle);
	clubTo.y = sin(clubVAngle);
	clubTo.z = cos(clubHAngle);

	if (reinterpret_cast<Ball*>(Entity::GetRefEntityList()[0])->inHole(Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getOrderedCupVerts(),Entity::GetRefEntityList()[0]->position))
	{
		currentLevel++;
		Course::Instance().setCurrentLevel(currentLevel);
	}
}

void topDownCamera(float dt)
{
	float max_tot = abs(20);
	animTime += dt;

	camera = glm::lookAt(glm::vec3(0, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
}


void display(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cameraView = glm::mat4();
	glm::mat4 modelCam = camera*modelView;
	glm::mat3 normalMatrix(modelCam);
	normalMatrix = glm::inverse(normalMatrix);
	normalMatrix = glm::transpose(normalMatrix);

	//set the current shader
	glUseProgram(shader->program);
	
	//pass the matrices and animation time to GPU
	glUniformMatrix4fv(shader->modelViewLoc, 1, GL_FALSE,
		glm::value_ptr(modelCam));
	glUniformMatrix4fv(shader->projectionLoc, 1, GL_FALSE,
		glm::value_ptr(projection));
	glUniformMatrix4fv(shader->normalMatrixLoc, 1, GL_FALSE,
		glm::value_ptr(normalMatrix));
	glUniform1f(shader->timeLoc, animTime);

	glUniform3f(shader->directionLoc, 0.0f, 1000.0f, 0.0f);
	glUniform3f(shader->lightcolorLoc, 1.0f, 1.0f, 1.0f);
	

	//draw some hud
	/*HUD::Instance().setParValue(Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getParValue());
	//cout << "Par Value: " << HUD::Instance().getParValue() << endl;
	string par = HUD::Instance().convertParVal(HUD::Instance().getParValue());
	//cout << "par value from render: " << par << endl;
	HUD::Instance().drawText(1, 3, GLUT_BITMAP_TIMES_ROMAN_24, par.c_str());
	
	*/
	//draw the holes
	//draw the tiles
	glUniform4f(shader->colorLoc, 0.0f, 1.0f, 0.0f, 0.0f);
	reloadBuffers(shader, Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getRefOrderedTileVerts(), Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getRefTileNorms());
	glDrawArrays(GL_TRIANGLES, 0, (Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getRefOrderedTileVerts().size()/3));
	
	//draw the tee
	glUniform4f(shader->colorLoc, 0.0f, 0.0f, 1.0f, 0.0f);
	reloadBuffers(shader, Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getOrderedTeeVerts(), Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getTeeNorms());
	glDrawArrays(GL_TRIANGLES, 0, (Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getOrderedTeeVerts().size()/3));
	
	//draw the cup
	glUniform4f(shader->colorLoc, 1.0f, 0.0f, 0.0f, 0.0f);
	reloadBuffers(shader, Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getOrderedCupVerts(), Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getCupNorms());
	glDrawArrays(GL_TRIANGLES, 0, (Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getOrderedCupVerts().size()/3));
	
	//draw the walls
	glUniform4f(shader->colorLoc, 0.54f, 0.27f, 0.074f, 0.0f);
	reloadBuffers(shader, Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getOrderedWallVerts(), Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getWallNorms());
	glDrawArrays(GL_TRIANGLES, 0, (Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getOrderedWallVerts().size()/3));

	//draw club vector
	vector<float>  clubVector;
	Entity *b = Entity::GetRefEntityList()[0];
	clubVector.push_back(b->position.x);
	clubVector.push_back(b->position.y);
	clubVector.push_back(b->position.z);

	glm::vec3 v = clubTo - b->position;
	v = v*clubPower;
	v += b->position;
	v = clubTo * clubPower + b->position;
	clubVector.push_back(v.x);
	clubVector.push_back(v.y);
	clubVector.push_back(v.z);

	glUniform4f(shader->colorLoc, 1.0f, 0.0f, 0.0, 0.0f);
	reloadBuffers(shader, clubVector, Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getWallNorms());
	glDrawArrays(GL_LINES, 0, (clubVector.size()/3));


	//draw entities
	for(int i=0;i<Entity::GetRefEntityList().size();i++)
	{
		Entity *e = Entity::GetRefEntityList()[i];
		glUniformMatrix4fv(shader->modelViewLoc, 1, GL_FALSE,
			glm::value_ptr(camera*e->transformMatrix));
		glUniform4f(shader->colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
		reloadBuffers(shader, e->getVerts(), e->getVerts());
		glDrawArrays(GL_TRIANGLES, 0, (e->getVerts().size()/3));
	}
	
	
	modelView = glm::translate(glm::mat4(), glm::vec3(clickchange_x / 100, -clickchange_y /200, -clickchange_z / 200));
	modelView = glm::scale(modelView, glm::vec3(clickchange_s/200, clickchange_s/200, clickchange_s/200));
	modelView = glm::rotate(modelView, clickchange_rx, glm::vec3(0.20f, 0.0f, 0.0f));
	modelView = glm::rotate(modelView, clickchange_ry, glm::vec3(0.0f, 0.20f, 0.0f));
	modelView = glm::rotate(modelView, clickchange_rz, glm::vec3(0.0f, 0.0f, 0.20f));


	camera = glm::translate(glm::mat4(), glm::vec3(camChange_x/100, camChange_y/100, camChange_z/100));
	camera = camera * glm::lookAt(glm::vec3(10,5,10), glm::vec3(0,0,0), glm::vec3(0,1,0));
	camera = glm::scale(camera, glm::vec3(camchange_s, camchange_s, camchange_s));
	camera = glm::rotate(camera, camchange_rx, glm::vec3(0.20f, 0.0f, 0.0f));
	camera = glm::rotate(camera, camchange_ry, glm::vec3(0.0f, 0.20f, 0.0f));
	camera = glm::rotate(camera, camchange_rz, glm::vec3(0.0f, 0.0f, 0.20f));


	modelCam = camera * modelView;

	//update animation variables.
	//have time oscillate between 0.0 and 1.0
	if((animTime >= 1.0 && deltaT > 0.0) ||
			(animTime <= 0.0 && deltaT < 0.0)){
			deltaT = -deltaT;
	}

	if(press_t == true){
		topDownCamera(deltaT);
	}
	else{
		update(deltaT);
	}

	glutSwapBuffers();
	
	short d = 0x12345678;

}

void reshape(int w, int h)
{
	WIN_WIDTH = w;
	WIN_HEIGHT = h;

	projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
}

void idle(void)
{
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	cout << "Current Hole: " << Course::Instance().getCurrentLevel() << endl;
	switch(key){
	case 27:
		exit(0);
		break;
	case 'x':
		press_powerup = true;
		break;
	case'y':
		press_y = true;
		break;
	case 'z':
		press_powerdown = true;
		break;
	case 's':
		press_down = true;
		break;
	case 'c':
		press_c = true;
		break;
	case 'o':
		press_o = true;
		break;
	case 'r':
		press_r = true;
		break;
	case 'w':
		press_up = true;
		break;
	case 'a':
		press_left = true;
		break;
	case 'd':
		press_right = true;
		break;
	case 't':
		if(press_t == false){
			press_t = true;
		}else{
			press_t = false;
		}
		break;
	case 'n':
		currentLevel++;
		Course::Instance().setCurrentLevel(currentLevel);		
		break;
	case 'p':
		currentLevel--;
		Course::Instance().setCurrentLevel(currentLevel);
		break;
	
	case ' ':
		Ball b = Course::Instance().getCourse().at(Course::Instance().getCurrentLevel()).getBall();
		Entity::GetRefEntityList()[0]->ApplyForce( (clubTo)*clubPower*(0.1f));
		//Entity::GetRefEntityList()[0]->ApplyForce(1,1,1);
		//Entity::GetRefEntityList()[0]->position = glm::vec3(1,1,1);

		break;
	}
}

void keyRel(unsigned char key, int x, int y)
{
	if(key == 'x'){
		press_powerup = false;
		left_Mouse = false;
	}
	if(key == 'y'){
		press_y = false;
		left_Mouse = false;
	}
	if(key == 'z'){
		press_powerdown = false;
		left_Mouse = false;
	}
	if(key == 's'){
		press_down = false;
	}
	if(key == 'w'){
		press_up = false;
	}
	if(key == 'a'){
		press_left = false;
	}
	if(key == 'd'){
		press_right = false;
	}
	if(key == '`' && massSpawn == false){
		for (int i=0;i<10;i++)
		{
			for(int j=0;j<10;j++)
			{
				Ball *b = new Ball();
				b->position = glm::vec3((i-5)*0.25,-0.5,(j-5)*0.25);
				b->velocity = glm::vec3((i-5)*0.01,0,(j-5)*0.01);
			}
		}
		massSpawn = true;
	}
	if(key == '1'){
		for(int i=0;i<10;i++)
		{
		Ball *b = new Ball();
		b->position = glm::vec3(0,0.5,0);
		b->velocity = glm::vec3(rand()%100*0.0005,0,rand()%100*0.0005);
		}
	}
	if(key == '2'){
		Ball *b = new Ball();
		b->position = glm::vec3(0,1,0.55);
		b->velocity = glm::vec3(0,0,0);
	}
	if(key == '3'){
		Ball *b = new Ball();
		b->position = glm::vec3(0,1,0);
		b->velocity = glm::vec3(0.001,0,0);
	}
	if (key == '4'){
		Level::Instance().getRefEntityList().clear();
	}
}

void mousePress(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		left_Mouse = true;
		clickinit = x;
	}else{
		left_Mouse = false;
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		right_Mouse = true;
		clickinit = x;
	}else{
		right_Mouse = false;
	}
}

void move(int x, int y)
{
	if(press_o == true){
		if(left_Mouse == true){
			if(press_x == true){
				clickcurr = x - clickinit;
				clickchange_x += clickcurr;
				clickinit = x;
			}
			if(press_y == true){
				clickcurr = x - clickinit;
				clickchange_y += clickcurr;
				clickinit = x;
			}
			if(press_z == true){
				clickcurr = x - clickinit;
				clickchange_z += clickcurr;
				clickinit = x;
			}
		}
		if(right_Mouse == true){
			if(press_x == true){
				clickcurr = x - clickinit;
				clickchange_rx += clickcurr;
				clickinit = x;
			}
			if(press_y == true){
				clickcurr = x - clickinit;
				clickchange_ry += clickcurr;
				clickinit = x;
			}
			if(press_z == true){
				clickcurr = x - clickinit;
				clickchange_rz += clickcurr;
				clickinit = x;
			}
		}
		if(press_s == true){
			clickcurr = x - clickinit;
			clickchange_s += clickcurr;
			clickinit = x;
			if(clickchange_s <= 1.0){
				clickchange_s = 1.0;
			}
		}
	}
	if(press_c == true){
		if(left_Mouse == true){
			if(press_x == true){
				clickcurr = x - clickinit;
				camChange_x += clickcurr;
				clickinit = x;
			}
			if(press_y == true){
				clickcurr = x - clickinit;
				camChange_y += clickcurr;
				clickinit = x;
			}
			if(press_z == true){
				clickcurr = x - clickinit;
				camChange_z += clickcurr;
				clickinit = x;
			}
		}
		if(right_Mouse == true){
			if(press_x == true){
				clickcurr = x - clickinit;
				camchange_rx += clickcurr;
				clickinit = x;
			}
			if(press_y == true){
				clickcurr = x - clickinit;
				camchange_ry += clickcurr;
				clickinit = x;
			}
			if(press_z == true){
				clickcurr = x - clickinit;
				camchange_rz += clickcurr;
				clickinit = x;
			}
		}
		if(press_s == true){
			clickcurr = y - clickinit;
			camchange_s += clickcurr;
			clickinit = y;
			if(camchange_s <= 1.0){
				camchange_s = 1.0;
			}

		}
	}
}

void Render::setupGlut(void)
{
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	mainWindow = glutCreateWindow("Mini Golf Level");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	
	glutKeyboardUpFunc(keyRel);

	glutMouseFunc(mousePress);

	glutMotionFunc(move);
	
	glutIdleFunc(idle);
}

void Render::setupGL(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	camera = glm::lookAt(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
}

void Render::setupShaders(void)
{

	shader = new Shader("shaders/light.vert", "shaders/light.frag");
	shader->modelViewLoc = glGetUniformLocation(shader->program, "M");
	shader->projectionLoc = glGetUniformLocation(shader->program, "P");
	shader->normalMatrixLoc = glGetUniformLocation(shader->program, "M_n");
	shader->timeLoc = glGetUniformLocation(shader->program, "time");

	shader->vertexLoc = glGetAttribLocation(shader->program, "pos");
	shader->normalLoc = glGetAttribLocation(shader->program, "norm");
	shader->colorLoc = glGetUniformLocation(shader->program, "color");
	shader->directionLoc = glGetUniformLocation(shader->program, "light_dir");
	shader->lightcolorLoc = glGetUniformLocation(shader->program, "light_color");

	GLuint buffers[2];
	glGenBuffers(2, buffers);

	shader->vertexBuffer = buffers[0];
	shader->normalBuffer = buffers[1];

	glCullFace(GL_BACK);
	glDepthRangef(0.0,1.0);
	glEnable(GL_DEPTH_TEST);


	//Initially load the buffers
	glBindBuffer(GL_ARRAY_BUFFER, shader->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		Level::Instance().getOrderedTileVerts().size() * sizeof(float),
		Level::Instance().getOrderedTileVerts().data(),
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, shader->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		Level::Instance().getTileNorms().size() * sizeof(float),
		Level::Instance().getTileNorms().data(),
		GL_STATIC_DRAW);
}

void Render::deleteShader(void)
{
	if(shader) delete shader;
}