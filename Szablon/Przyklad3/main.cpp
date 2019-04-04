#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <fstream>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include "shaderLoader.h" //narzŕdzie do │adowania i kompilowania shaderˇw z pliku
#include "tekstura.h"

//funkcje algebry liniowej
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective


/*###############################################################*/

//Wymiary okna
int screen_width = 640;
int screen_height = 480;

GLint uniformTex0, uniformTex1;
int pozycjaMyszyX; // na ekranie
int pozycjaMyszyY;
int mbutton; // wcisiety klawisz myszy

/*###############################################################*/

double kameraX = 10.0;
double kameraZ = 20.0;
double kameraD = -3;
double kameraPredkosc;
double kameraKat = -20;
double kameraPredkoscObrotu;
double poprzednie_kameraX;
double poprzednie_kameraZ;
double poprzednie_kameraD;

double rotation = 0;
double blend = 0.5;
double blend2 = 0;

/*###############################################################*/

//macierze
glm::mat4 MV; //modelview - macierz modelu i świata
glm::mat4 P;  //projection - macierz projekcji, czyli naszej perspektywy
//Light
glm::vec3 lightPos(300, 100.0f, 300.0f);
GLuint objectColor_id = 0;
GLuint lightColor_id = 0;
GLuint lightPos_id = 0;
GLuint viewPos_id = 0;

/*###############################################################*/

//shaders
GLuint programID[6] = { 0 };
GLuint lamp_ID = 0;

//
unsigned int VAO[6];
unsigned int VBO[6];
unsigned int ebo[6];
unsigned int vtex[4];
GLuint		 tex_id[4];
unsigned int lightVAO;

int n_v;
int n_el;
int n_vFace;

double ax = 0;
float onOff = 1;

/*###############################################################*/

/*
float verticesFloor[] = {
	-5, 1, 4,
	 5, 1, 4,
	 5, 1, -8,
	-5, 1, -8
};

GLuint elementsFloor[] = {
	0, 1, 2, 3
};

float textureCoordsFloor[] = {
	0, 0,
	5, 0,
	5, 5,
	0, 5
};
*/

/*
float verticesCube[] = {
	// front
    -1.0, -1.0,  1.0,	//0
     1.0, -1.0,  1.0,	//1
     1.0,  1.0,  1.0,	//2
    -1.0,  1.0,  1.0,	//3
    // back
    -1.0, -1.0, -1.0,	//4
     1.0, -1.0, -1.0,	//5
     1.0,  1.0, -1.0,	//6
    -1.0,  1.0, -1.0	//7
};

GLuint elementsCube[] = {
		// front
		0, 1, 2, 3,
		// back
		4, 5, 6, 7,
		// bottom
		0, 1, 5, 4,
		// right/left
		1, 5, 6, 2,
		// top
		2, 6, 7, 3,
		// right/left
		3, 7, 4, 0

		//Triangle way
		// front
		//2, 3, 0,
		// right
		//1, 5, 6,
		//6, 2, 1,
		// back
		//7, 6, 5,
		//5, 4, 7,
		// left
		//4, 0, 3,
		//3, 7, 4,
		// bottom
		//4, 5, 1,
		//1, 0, 4,
		// top
		//3, 2, 6,
		//6, 7, 3
};

float textureCoordsCube[] = {
	-1, 1,
	-1, -1,
	1, -1,
	1, 1
};
*/

/*
float verticesGrass[] = {
	0, 0, 0,
	0, 1, 0,
	1, 1, 0,
	1, 0, 0
};

GLuint elementsGrass[] = {
	0, 1, 2, 3
};

float textureCoordsGrass[] = {
	0, 0,
	0, 1,
	1, 1,
	1, 0
};
*/

/*
float verticesWall[] = {
	-1, 0.5, 0.5,
	 0, 0.5, 0.5,
	 0, 0.5, -0.5,
	-1, 0.5, -0.5
};

GLuint elementsWall[] = {
	0, 1, 2, 3
};

float textureCoordsWall[] = {
	0, 1,
	1, 1,
	1, 0,
	0, 0,
};
*/

/*
float *verticesSphere;
float *normalsSphere;
GLuint *elementsSphere;
*/

/*
float *verticesFace;
*/

/*###############################################################*/

void mysz(int button, int state, int x, int y)
{
	mbutton = button;
	switch (state)
	{
	case GLUT_UP:
		break;
	case GLUT_DOWN:
		pozycjaMyszyX = x;
		pozycjaMyszyY = y;
		poprzednie_kameraX = kameraX;
		poprzednie_kameraZ = kameraZ;
		poprzednie_kameraD = kameraD;
		break;

	}
}
/*******************************************/
void mysz_ruch(int x, int y)
{
	if (mbutton == GLUT_LEFT_BUTTON)
	{
		kameraX = poprzednie_kameraX - (pozycjaMyszyX - x) * 0.1;
		kameraZ = poprzednie_kameraZ - (pozycjaMyszyY - y) * 0.1;
	}
	if (mbutton == GLUT_RIGHT_BUTTON)
	{
		kameraD = poprzednie_kameraD + (pozycjaMyszyY - y) * 0.1;
	}

}
/******************************************/

void rozmiar(int width, int height)
{
	screen_width = width;
	screen_height = height;

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screen_width, screen_height);

	P = glm::perspective(glm::radians(60.0f), (GLfloat)screen_width / (GLfloat)screen_height, 1.0f, 1000.0f);

	glutPostRedisplay(); // Przerysowanie sceny
}

/*###############################################################*/

double dx = -5;

/*###############################################################*/

void klawisz(GLubyte key, int x, int y)
{
	switch (key) {

	case 27:    /* Esc - koniec */
		exit(1);
		break;

	case 'x':
		dx += 0.1;
		break;
	case 'c':
		dx -= 0.1;
		break;

	case 't':
		onOff = !onOff;
		break;
	
	case '1':

		break;
	case '2':

		break;
	}
	
	
}

/*###############################################################*/
/*###############################################################*/

void rysuj(void)
{
	// Kasowanie ekranu
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//MVP
	glm::mat4 MVP;
	GLuint MVP_id[6];

	//ustawienia poczatkowe
	MV = glm::mat4(1.0f);  //macierz jednostkowa
	MV = glm::translate(MV, glm::vec3(1, -1, kameraD + dx));
	MV = glm::rotate(MV, (float)glm::radians(kameraZ ), glm::vec3(1, 0, 0));
	MV = glm::rotate(MV, (float)glm::radians(kameraX ), glm::vec3(0, 1, 0));


	/*
	//===================================

	//Floor fragmentshader
	glUseProgram(programID[0]);								//uzyj programu, czyli naszego shadera	
	MVP_id[0] = glGetUniformLocation(programID[0], "MVP");	// pobierz lokalizację zmiennej 'uniform' "MV" w programie

	MVP = P * MV;
	glUniformMatrix4fv(MVP_id[0], 1, GL_FALSE, &(MVP[0][0]));

	//draw floor
	glBindVertexArray(VAO[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//active texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id[0]);
	glUniform1i(uniformTex0, 0);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_QUADS, 0, 4);
	*/


	/*
	//===================================
	
	//Cube fragmentshader
	glUseProgram(programID[1]);
	MVP_id[1] = glGetUniformLocation(programID[1], "MVP");

	MV = glm::translate(MV, glm::vec3(0, 2, 0));

	MVP = P * MV;
	glUniformMatrix4fv(MVP_id[1], 1, GL_FALSE, &(MVP[0][0]));

	//draw cube
	glBindVertexArray(VAO[1]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//active texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id[2]);
	glUniform1i(uniformTex0, 0);

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);

	//rysowanie krawedzi
	glPolygonMode(GL_FRONT, GL_LINE);
	
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);
	*/


	/*
	//===================================
	
	//Grass fragmentshader
	glUseProgram(programID[2]);
	MVP_id[2] = glGetUniformLocation(programID[2], "MVP");

	MV = glm::translate(MV, glm::vec3(-1, -1, 2));

	MVP = P * MV;
	glUniformMatrix4fv(MVP_id[2], 1, GL_FALSE, &(MVP[0][0]));

	//draw grass
	glBindVertexArray(VAO[2]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	//active texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id[1]);
	glUniform1i(uniformTex0, 0);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	
	//glVertexAttrib3fv(1, attrib);

	glVertexAttrib1f(3, onOff);
	*/


	/*
	//===================================

	//Sphere fragmentshader
	glUseProgram(programID[3]);
	MVP_id[3] = glGetUniformLocation(programID[3], "MVP");

	MV = glm::translate(MV, glm::vec3(-2, 2, -5));
	MV = glm::scale(MV, glm::vec3(0.01, 0.01, 0.01));

	MVP = P * MV;
	glUniformMatrix4fv(MVP_id[3], 1, GL_FALSE, &(MVP[0][0]));

	//Light
	////////////////////////////////////////////////////
	
	//lightPos.x = 100+(float)(100*sin(ax));
	//lightPos.y = 100;
	//lightPos.z = 300+(float)(100*cos(ax));

	glUniform3f(objectColor_id, 1.0f, 0.5f, 0.31f);
	glUniform3f(lightColor_id, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPos_id,lightPos[0], lightPos[1], lightPos[2]);

	/////////////////////////////////////////////////

	//draw sphere
	glBindVertexArray(VAO[3]);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, n_el, GL_UNSIGNED_INT, 0);

	//draw shinning object
	/*
	glUseProgram(lamp_ID);
	GLuint MVPlamp_id = glGetUniformLocation(lamp_ID, "MVP");
	MV = glm::translate(MV, lightPos);
	MV = glm::scale(MV, glm::vec3(0.1f, 0.1f, 0.1f));
	MV = glm::rotate(MV, (float)glm::radians(-30.0), glm::vec3(0, 1, 0));
	 MVP = P * MV;
	glUniformMatrix4fv(MVPlamp_id, 1, GL_FALSE, &(MVP[0][0]));
	glBindVertexArray(lightVAO);
	
	glDrawElements(GL_TRIANGLES, n_el, GL_UNSIGNED_INT, 0);
	*/
	


	/*
	//===================================

	//Face fragmentshader
	glUseProgram(programID[4]);
	MVP_id[4] = glGetUniformLocation(programID[4], "MVP");

	MV = glm::translate(MV, glm::vec3(0, 0, 0));
	MV = glm::scale(MV, glm::vec3(1000, 1000, 1000));
	MV = glm::rotate(MV, (float)glm::radians( -90.0f ), glm::vec3(1, 0, 0));
	MV = glm::rotate(MV, (float)glm::radians( 90.0f ), glm::vec3(0, 0, 1));
	MV = glm::translate(MV, glm::vec3(-0.7, -0.7, 0));

	MVP = P * MV;
	glUniformMatrix4fv(MVP_id[4], 1, GL_FALSE, &(MVP[0][0]));

	//draw face
	glBindVertexArray(VAO[4]);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_POINTS, 0, n_vFace);
	*/


	/*
	//===================================

	//Wall fragmentshader
	glUseProgram(programID[5]);
	MVP_id[5] = glGetUniformLocation(programID[5], "MVP");

	MVP = P * MV;
	glUniformMatrix4fv(MVP_id[5], 1, GL_FALSE, &(MVP[0][0]));

	//draw wall
	glBindVertexArray(VAO[5]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//active texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id[3]);
	glUniform1i(uniformTex0, 0);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	*/


	//===================================




	glFlush();
	glutSwapBuffers();

}

/*###############################################################*/

void idle()
{
	ax += 0.005;
	glutPostRedisplay();
}

/*###############################################################*/

void timer(int value)
{

	/////////////////////////
	glutTimerFunc(20, timer, 0);
}

/*###############################################################*/
/*###############################################################*/

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Przyklad 4");

	glewInit();						// init rozszerzeszeń OpenGL z biblioteki GLEW

	glutDisplayFunc(rysuj);			// def. funkcji rysuj¦cej
	glutIdleFunc(idle);				// def. funkcji rysuj¦cej w czasie wolnym procesoora (w efekcie: ciagle wykonywanej)
	glutTimerFunc(20, timer, 0);
	glutReshapeFunc(rozmiar);		// def. obs-ugi zdarzenia resize (GLUT)

	glutKeyboardFunc(klawisz);		// def. obsługi klawiatury
	glutMouseFunc(mysz); 			// def. obsługi zdarzenia przycisku myszy (GLUT)
	glutMotionFunc(mysz_ruch);		// def. obsługi zdarzenia ruchu myszy (GLUT)


	glEnable(GL_DEPTH_TEST);


	//wlaczenie przezroczystosci
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//

	//VAO
	glGenVertexArrays(6, VAO);
	//

	/*
	//======================== Floor
	programID[0] = loadShaders("vertex_shader.glsl", "fragment_shader0.glsl");
	glUseProgram(programID[0]);

	glBindVertexArray(VAO[0]);

	//Bind buffers Floor
	glGenBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFloor), verticesFloor, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementsFloor), elementsFloor, GL_STATIC_DRAW);

	glGenBuffers(1, &vtex[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vtex[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordsFloor), textureCoordsFloor, GL_STATIC_DRAW);
	//

	//Load texture Floor
	tex_id[0] = WczytajTeksture("stone.bmp");
	if (tex_id[0] == -1)
	{
		MessageBox(NULL, "Nie znaleziono pliku z teksturą", "Problem", MB_OK | MB_ICONERROR);
		exit(0);
	}
	//

	//Active texture Floor
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id[0]);
	uniformTex0 = glGetUniformLocation(programID[0], "tex0");
	glUniform1i(uniformTex0, 0); 
	//

	//Send Floor
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vtex[0]);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,  GL_FALSE, 0, (void*)0 );
	//
	//========================
	*/



	/*
	//======================== Cube
	programID[1] = loadShaders("vertex_shader.glsl", "fragment_shader1.glsl");
	glUseProgram(programID[1]);

	glBindVertexArray(VAO[1]);
	
	//Bind buffers Cube
	glGenBuffers(1, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementsCube), elementsCube, GL_STATIC_DRAW);

	glGenBuffers(1, &vtex[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vtex[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordsCube), textureCoordsCube, GL_STATIC_DRAW);
	//

	//Load texture Cube
	tex_id[2] = WczytajTeksture("monalisa.bmp");
	if( tex_id[2] == -1 )
	{
		MessageBox(NULL, "Nie znaleziono pliku z teksturą", "Problem", MB_OK | MB_ICONERROR);
		exit(0);
	}
	//

	//Active texture Cube
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id[2]);
	uniformTex0 = glGetUniformLocation(programID[1], "tex0");
	glUniform1i(uniformTex0, 0);
	//

	//Send Cube
	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, vtex[2]);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,  GL_FALSE, 0, (void*)0 );
	//
	//========================
	*/



	/*
	//======================== Grass
	programID[2] = loadShaders("vertex_shader.glsl", "fragment_shader2.glsl");
	glUseProgram(programID[2]);

	glBindVertexArray(VAO[2]);

	//Bind buffers Grass
	glGenBuffers(1, &VBO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesGrass), verticesGrass, GL_STATIC_DRAW);
	
	glGenBuffers(1, &ebo[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementsGrass), elementsGrass, GL_STATIC_DRAW);
	
	glGenBuffers(1, &vtex[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vtex[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordsGrass), textureCoordsGrass, GL_STATIC_DRAW);
	//
	
	//Load texture Grass
	tex_id[1] = WczytajTeksture("grass.bmp");
	if (tex_id[1] == -1)
	{
		MessageBox(NULL, "Nie znaleziono pliku z teksturą", "Problem", MB_OK | MB_ICONERROR);
		exit(0);
	}
	//

	//Active texture Grass
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id[1]);
	uniformTex0 = glGetUniformLocation(programID[2], "tex0");
	glUniform1i(uniformTex0, 0);
	//

	//Send Grass
	glBindVertexArray(VAO[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vtex[1]);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,  GL_FALSE, 0, (void*)0 );
	//
	//========================
	*/


	/*
	//======================== Sphere
	programID[3] = loadShaders("vertex_shader.glsl", "fragment_shader3.glsl");
	glUseProgram(programID[3]);
	
	glBindVertexArray(VAO[3]);

	//Load from file
	std::ifstream file("sphere.txt");
	if (file.fail())
	{
		printf("Cannot open this file or is not in this directory ! \n");
		system("pause");
		exit(-4);
	}

	if (file.is_open())
	{
		file >> n_v;
		file >> n_el;
		n_v = n_v * 3;
		
		verticesSphere = (float*)calloc(n_v, sizeof(float));
		normalsSphere = (float*)calloc(n_el*3, sizeof(float));
		elementsSphere = (GLuint*)calloc(n_el, sizeof(int));

		for( int i = 0; i < n_v; i++ )
			file >> verticesSphere[i];

		for( int i = 0; i < n_el; i++ )
			file >> elementsSphere[i];
	}
	//

	//Light
	objectColor_id = glGetUniformLocation(programID[3], "objectColor");
	lightColor_id = glGetUniformLocation(programID[3], "lightColor");
    lightPos_id = glGetUniformLocation(programID[3], "lightPos");
	//

	//Bind buffers Sphere
	glGenBuffers(1, &VBO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, n_v * sizeof(float), verticesSphere, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo[3]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_el * sizeof(int), elementsSphere, GL_STATIC_DRAW); //n_el * 3 ?

	glGenVertexArrays(1, &VAO[3]);
	//

	//Send Sphere
	glBindVertexArray(VAO[3]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);
	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//

	//Light - zrodlo swiatla
	lamp_ID = loadShaders("lamp_vshader.glsl", "lamp_fshader.glsl");
	glUseProgram(lamp_ID);
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//
	//========================
	*/




	/*
	//======================== Face
	programID[4] = loadShaders("vertex_shader.glsl", "fragment_shader4.glsl");
	glUseProgram(programID[4]);
	
	glBindVertexArray(VAO[4]);

	//Load from file
	std::ifstream file2("dane3d.txt");
	if (file2.fail())
	{
		printf("Cannot open this file or is not in this directory ! \n");
		system("pause");
		exit(-4);
	}

	if (file2.is_open())
	{
		file2 >> n_vFace;
		n_vFace = n_vFace * 3;
		
		verticesFace = (float*)calloc(n_vFace, sizeof(float));

		for( int i = 0; i < n_vFace; i++ )
			file2 >> verticesFace[i];
	}
	//

	//Bind buffers Face
	glGenBuffers(1, &VBO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, n_vFace * sizeof(float), verticesFace, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO[4]);
	//

	//Send Face
	glBindVertexArray(VAO[4]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//
	//========================
	*/



	/*
	//======================== Wall
	programID[5] = loadShaders("vertex_shader.glsl", "fragment_shader5.glsl");
	glUseProgram(programID[5]);

	glBindVertexArray(VAO[5]);

	//Bind buffers Wall
	glGenBuffers(1, &VBO[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesWall), verticesWall, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo[5]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[5]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementsWall), elementsWall, GL_STATIC_DRAW);

	glGenBuffers(1, &vtex[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vtex[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordsWall), textureCoordsWall, GL_STATIC_DRAW);
	//

	//Load texture Wall
	tex_id[3] = WczytajTeksture("monalisa.bmp");
	if (tex_id[1] == -1)
	{
		MessageBox(NULL, "Nie znaleziono pliku z teksturą", "Problem", MB_OK | MB_ICONERROR);
		exit(0);
	}
	//

	//Active texture Wall
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id[3]);
	uniformTex0 = glGetUniformLocation(programID[5], "tex0");
	glUniform1i(uniformTex0, 0);
	//

	//Send Wall
	glBindVertexArray(VAO[5]);

	glBindBuffer(GL_ARRAY_BUFFER, vtex[3]);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[5]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,  GL_FALSE, 0, (void*)0 );
	//
	//========================
	*/




	glutMainLoop();					// start


	glDeleteBuffers(3, VAO);
	glDeleteBuffers(3, VBO);
	glDeleteBuffers(3, ebo);
	glDeleteBuffers(2, vtex);
	glDeleteVertexArrays(1, &lightVAO);

	return(0);
}
