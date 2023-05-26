#include <iostream>
//#include <GLFW/glfw3.h>
using namespace std;

/*
* 
* 
* 
* For different light sources i need to add shader info from another light and add it onto the main shader and change the variable names,
* refer to notes week 3 multiple light sources
* 
* 
* 
* 
*/

//--- OpenGL ---
#include "GL\glew.h"
#include "GL\wglew.h"
#pragma comment(lib, "glew32.lib")
//--------------

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"

#include "GL\freeglut.h"

#include "Images\FreeImage.h"

#include "shaders\Shader.h"
#include "sphere\Sphere.h"

#include "printToFile.h"

CShader* myShader;  ///shader object 
CShader* myBasicShader;
CShader* myTextureShader;
CShader* myPointLightShader;

//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj/OBJLoader.h"
#include <future>

float amount = 0;
float temp = 0.002f;
	
COBJLoader objLoader;
CThreeDModel sphereModel;
Sphere sphere0;
///END MODEL LOADING

//file object
printToFile printFile;

glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing

glm::mat4 objectRotation;
glm::vec3 translation = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 pos = glm::vec3(0.0f,0.0f,0.0f); //vector for the position of the object.

//Material properties
float Material_Ambient[4] = {1.f, 1.f, 1.f, 1.0f};
float Material_Diffuse[4] = {0.99f, 0.99f, 0.99f, 1.0f};
float Material_Specular[4] = {1.0f,1.0f,1.0f,1.0f};
float Material_Shininess = 40;

//Light Properties
float Light_Ambient_And_Diffuse[4] = {0.5f, 0.5f, 0.5f, 1.0f};
float Light_Specular[4] = {0.9f,0.9f,0.9f,1.0f};
float LightPos[4] = {0.0f, 50.0f, 1.0f, 0.0f};
float spotLightPos[4];
float spotLightDirection[4] = { 0.0, 0.0, -1.0, 0.0f };


//
int	mouse_x=0, mouse_y=0;
bool LeftPressed = false;
int screenWidth=600, screenHeight=600;

int cam = 0;

//variables for movement
//float spin=180.0f;
//float speed=0.0f;
//float strafe = 0.0f;
//float ymovement = 0.0f;
//float rotateRadians = 0.0f;

/*Variables for timing*/
float fDeltaTime = 0.0f;
float currentTicks = 0.0f;
float PreviousTicks = 0.0f;
float timetest = 0.0f;
float timeOfTest = 0.0f;
int framerate = 0;

//float objColor[4] = { 0.5,0.2,0.8,1.0 };

glm::vec3 sphereCoordinates = glm::vec3(0.0, 0.0, 0.0);

//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void reshape(int width, int height);				//called when the window is resized
void init();				//called in winmain when the program starts.
void processKeys();         //called in winmain to process keyboard input
void idle();		//idle function
void updateTransform(float xinc, float yinc, float zinc);

/*************    START OF OPENGL FUNCTIONS   ****************/
void display()
{
	//obtain the ticks from the clock and find difference with previous time.
	currentTicks = std::clock();
	float deltaTicks = (float)(currentTicks - PreviousTicks);
	PreviousTicks = currentTicks;

	//now find the actual time passed between frames
	fDeltaTime = deltaTicks / (float)CLOCKS_PER_SEC;

	timeOfTest += fDeltaTime;

	//Finding the amount of frames in one second and then calling the printing method in
	//the printToFile class.
	if (timetest < 1.0)
	{
		timetest += fDeltaTime;
		framerate++;
	}
	else
	{
		//cout << "framerate: " << framerate << " " << timeOfTest << endl;
		printFile.printingMethod(framerate);
		if (timeOfTest >= 63.0f)
		{
			printFile.closeFile();
			glutLeaveMainLoop();
		}
		framerate = 0;
		timetest = 0.0f;
	}


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(myPointLightShader->GetProgramObjID());  // use the shader

	vector<glm::vec3> pointLightColors = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 0.5f, 0.5f)
	};

	for (int k = 0; k < pointLightColors.size(); k++)
	{
		/*glUniform3f(glGetUniformLocation(myPointLightShader->GetProgramObjID(), "SpotLightPos"), 1.f, 1.f, 1.f);
		glUniform3f(glGetUniformLocation(myPointLightShader->GetProgramObjID(), "light_ambient"), pointLightColors[k].x * 0.1, pointLightColors[k].y * 0.1, pointLightColors[k].z * 0.1);
		glUniform3f(glGetUniformLocation(myPointLightShader->GetProgramObjID(), "light_diffuse"), pointLightColors[k].x, pointLightColors[k].y, pointLightColors[k].z);
		glUniform3f(glGetUniformLocation(myPointLightShader->GetProgramObjID(), "light_specular"), pointLightColors[k].x, pointLightColors[k].y, pointLightColors[k].z);
		glUniform1f(glGetUniformLocation(myPointLightShader->GetProgramObjID(), "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(myPointLightShader->GetProgramObjID(), "pointLights[0].linear"), 0.09);
		glUniform1f(glGetUniformLocation(myPointLightShader->GetProgramObjID(), "pointLights[0].quadratic"), 0.032);*/
	}

	glUseProgram(myShader->GetProgramObjID());  // use the shader

	//glCullFace(GL_BACK);

	//Part for displacement shader.
	//amount += temp;
	//if (amount > 1.0f || amount < -1.5f)
	//	temp = -temp;
	//amount = 0;

	//Set the projection matrix in the shader
	GLuint projMatLocation = glGetUniformLocation(myShader->GetProgramObjID(), "ProjectionMatrix");
	glUniformMatrix4fv(projMatLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

	glm::mat4 viewingMatrix = glm::mat4(1.0f);

	//translation and rotation for view
	viewingMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0));

	glm::vec3 modelCoords = pos;

	viewingMatrix = glm::lookAt(glm::vec3(pos.x-2.5, pos.y -2.5, pos.z - 2.5), glm::vec3(pos.x, pos.y, pos.z), glm::vec3(0.0f, 1.0f, 0.0));

	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	glm::mat4 modelmatrix = glm::translate(glm::mat4(1.0f), pos);
	//modelmatrix = glm::rotate(modelmatrix, rotateRadians, glm::vec3(0.0, 1.0, 0.0));
	ModelViewMatrix = viewingMatrix * modelmatrix;
	glUniformMatrix4fv(glGetUniformLocation(myShader->GetProgramObjID(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);



	sphere0.render();

	glFlush();
	glutSwapBuffers();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth=width; screenHeight = height;           // to ensure the mouse coordinates match 
														// we will use these values to set the coordinate system

	glViewport(0,0,width,height);						// Reset The Current Viewport

	//Set the projection matrix
	ProjectionMatrix = glm::perspective(glm::radians(60.0f), (GLfloat)screenWidth/(GLfloat)screenHeight, 1.0f, 1000000.0f);
}

void init()
{
	glClearColor(0.1,0.1,0.1,0.1);						
														//glClear(GL_COLOR_BUFFER_BIT) in the display function
														//will clear the buffer to this colour
	glEnable(GL_DEPTH_TEST);
	
	//Enable or disable culling of polygons
	//glEnable(GL_CULL_FACE);


	myShader = new CShader();
	//if(!myShader->CreateShaderProgram("BasicView", "glslfiles/basicTransformationsWithDisplacement.vert", "glslfiles/basicTransformationsWithDisplacement.frag"))
	if(!myShader->CreateShaderProgram("BasicView", "glslfiles/basic.vert", "glslfiles/basic.frag"))
	{
		cout << "failed to load shader" << endl;
	}		

	myBasicShader = new CShader();
	if(!myBasicShader->CreateShaderProgram("Basic", "glslfiles/basic.vert", "glslfiles/basic.frag"))
	{
		cout << "failed to load shader" << endl;
	}	
	myTextureShader = new CShader();
	if (!myTextureShader->CreateShaderProgram("TextureOnly", "glslfiles/basicTransformations.vert", "glslfiles/basicTransformations.frag"))
	{
		cout << "failed to load texture only shader" << endl;
	}
	myPointLightShader = new CShader();
	if (!myPointLightShader->CreateShaderProgram("PointLight", "glslfiles/basicSpecularSpotlight.vert", "glslfiles/basicSpecularSpotlight.frag"))
	{
		cout << "failed to load shader for point light" << endl;
	}



	glUseProgram(myShader->GetProgramObjID());  // use the shader

	glEnable(GL_TEXTURE_2D);

	//lets initialise our object's rotation transformation 
	//to the identity matrix
	objectRotation = glm::mat4(1.0f);

	cout << " loading model " << endl;

	sphere0.setRadius(2);
	sphere0.setCentre(0.0, 0.0, 0.0);
	sphere0.constructGeometry(myShader, 3000);
}

void special(int key, int x, int y)
{

}

void specialUp(int key, int x, int y)
{

}

void keyboard(unsigned char key, int x, int y)
{

}

void keyboardUp(unsigned char key, int x, int y)
{

}

void processKeys()
{


}


void updateTransform(float xinc, float yinc, float zinc)
{

}

void idle()
{
	//spin += speed;
	//if(spin > 360)
	//	spin = 0;

	//processKeys();

	//call the display function again on idle
	glutPostRedisplay();
}
/**************** END OPENGL FUNCTIONS *************************/

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL FreeGLUT Example: Obj loading");

	//This initialises glew - it must be called after the window is created.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << " GLEW ERROR" << endl;
	}

	//Check the OpenGL version being used
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;


	//initialise the objects for rendering
	init();
	wglSwapIntervalEXT(0);

	glutReshapeFunc(reshape);
	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);

	glutIdleFunc(idle);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	glutMainLoop();

	return 0;
}
