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

// Program bazuje na tutorialu ze strony: https://learnopengl.com/


//Wymiary okna
int screen_width = 640;
int screen_height = 480;


int pozycjaMyszyX; // na ekranie
int pozycjaMyszyY;
int mbutton; // wcisiety klawisz myszy

double kameraX=-20.0;
double kameraZ =10.0;
double kameraD = -800;
double kameraPredkosc;
double kameraKat = -20;
double kameraPredkoscObrotu;
double poprzednie_kameraX;
double poprzednie_kameraZ;
double poprzednie_kameraD;

double rotation = 0;

//macierze

glm::mat4 MV; //modelview - macierz modelu i świata
glm::mat4 P;  //projection - macierz projekcji, czyli naszej perspektywy
glm::vec3 lightPos(300, 100.0f, 300.0f);
GLuint objectColor_id = 0;
GLuint lightColor_id = 0;
GLuint lightPos_id = 0;
GLuint viewPos_id = 0;

float *vertices;
float *normals;
GLuint *elements;


//shaders
GLuint programID = 0;
GLuint lamp_ID = 0;

unsigned int VBO, sphereVAO,VBO_normals, ebo;
unsigned int lightVAO;

int n_v, n_el;

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


void klawisz(GLubyte key, int x, int y)
{
	switch (key) {

	case 27:    /* Esc - koniec */
		exit(1);
		break;

	case '1':
		lightPos[1] += 10;
		break;
	case '2':
		lightPos[1] += -10;
		break;
	}
	
	
}
/*###############################################################*/
void rysuj(void)
{

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID); //u┐yj programu, czyli naszego shadera	
	

	GLuint MVP_id = glGetUniformLocation(programID, "MVP"); // pobierz lokalizację zmiennej 'uniform' "MV" w programie


	MV = glm::mat4(1.0f);  //macierz jednostkowa
	MV = glm::translate(MV, glm::vec3(0, -1, kameraD ));

	MV = glm::rotate(MV, (float)glm::radians(kameraZ ), glm::vec3(1, 0, 0));
	MV = glm::rotate(MV, (float)glm::radians(kameraX ), glm::vec3(0, 1, 0));
	
	glm::mat4 MVP = P * MV;
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));

	////////////////////////////////////////////////////

	glUniform3f(objectColor_id, 1.0f, 0.5f, 0.31f);
	glUniform3f(lightColor_id, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPos_id,lightPos[0], lightPos[1], lightPos[2]);

	/////////////////////////////////////////////////
	
	// render the sphere
	glBindVertexArray(sphereVAO);

	glDrawElements(GL_TRIANGLES, n_el, GL_UNSIGNED_INT, 0);

	
	glUseProgram(lamp_ID);
	GLuint MVPlamp_id = glGetUniformLocation(lamp_ID, "MVP");
	MV = glm::translate(MV, lightPos);
	MV = glm::scale(MV, glm::vec3(0.1f, 0.1f, 0.1f));
	MV = glm::rotate(MV, (float)glm::radians(-30.0), glm::vec3(0, 1, 0));
	 MVP = P * MV;
	glUniformMatrix4fv(MVPlamp_id, 1, GL_FALSE, &(MVP[0][0]));
	glBindVertexArray(lightVAO);
	
	glDrawElements(GL_TRIANGLES, n_el, GL_UNSIGNED_INT, 0);


	glFlush();
	glutSwapBuffers();

}
/*###############################################################*/
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
void idle()
{

	glutPostRedisplay();
}

/*###############################################################*/


void timer(int value) {

	
glutTimerFunc(20, timer, 0);
}
/*###############################################################*/



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Przyklad 6");

	glewInit(); //init rozszerzeszeń OpenGL z biblioteki GLEW

	glutDisplayFunc(rysuj);			// def. funkcji rysuj¦cej
	glutIdleFunc(idle);			// def. funkcji rysuj¦cej w czasie wolnym procesoora (w efekcie: ci¦gle wykonywanej)
	//glutTimerFunc(20, timer, 0);
	glutReshapeFunc(rozmiar); // def. obs-ugi zdarzenia resize (GLUT)

	glutKeyboardFunc(klawisz);		// def. obsługi klawiatury
	glutMouseFunc(mysz); 		// def. obsługi zdarzenia przycisku myszy (GLUT)
	glutMotionFunc(mysz_ruch); // def. obsługi zdarzenia ruchu myszy (GLUT)


	glEnable(GL_DEPTH_TEST);


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
		
		vertices = (float*)calloc(n_v, sizeof(float));
		normals = (float*)calloc(n_el*3, sizeof(float));
		elements = (GLuint*)calloc(n_el, sizeof(int));

		for(int i=0;i<n_v;i++)
			file >> vertices[i];

		for (int i = 0; i<n_el; i++)
			file >> elements[i];
	

		//obliczenie wektorów normalnych




		}
	

	
	programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");
 

	glUseProgram(programID);

	objectColor_id = glGetUniformLocation(programID, "objectColor");
	lightColor_id = glGetUniformLocation(programID, "lightColor");
    lightPos_id = glGetUniformLocation(programID, "lightPos");
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n_v * sizeof(float), vertices, GL_STATIC_DRAW);

	/* 
	glGenBuffers(1, &VBO_normals);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, n_el * 3* sizeof(float), normals, GL_STATIC_DRAW);

	*/
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_el * sizeof(int), elements, GL_STATIC_DRAW);



	glGenVertexArrays(1, &sphereVAO);

	glBindVertexArray(sphereVAO);
	/* glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	*/
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute



	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D sphere)
	lamp_ID = loadShaders("lamp_vshader.glsl", "lamp_fshader.glsl");
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glutMainLoop();					// start

	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	return(0);

}

