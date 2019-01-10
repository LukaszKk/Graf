#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "GL\glew.h"
#include "GL\freeglut.h"

#include "shaderLoader.h" //narzŕdzie do │adowania i kompilowania shaderˇw z pliku


//funkcje algebry liniowej
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective


//Wymiary okna
int screen_width = 640;
int screen_height = 480;


int pozycjaMyszyX; // na ekranie
int pozycjaMyszyY;
int mbutton; // wcisiety klawisz myszy

double kameraX= 50.0;
double kameraZ = 0.0;
double kameraD = -3.0;
double kameraPredkosc;
double kameraKat = 20;
double kameraPredkoscObrotu;
double poprzednie_kameraX;
double poprzednie_kameraZ;
double poprzednie_kameraD;

double rotation = 0;


//macierze
glm::mat4 MV; //modelview - macierz modelu i świata
glm::mat4 P;  //projection - macierz projekcji, czyli naszej perspektywy


GLuint elements1[] = {
	0, 2, 1, 
	0, 1, 3, 
	0, 3, 2, 
	1, 2, 4, 
	3, 1, 4, 
	2, 3, 4 
};

GLuint elements2[] = {
	6, 2, 	1 ,
	2, 7,	1 ,
	5, 4,	3 ,
	8, 3,	4 ,
	11, 5,	 6,
	10, 6,	 5,
	2, 10,	 9,
	3, 9,	10,
	9, 8,	7 ,
	0, 7,	8 ,
	1, 0,	11,
	4, 11,	 0,
	10, 2,	 6,
	11, 6,	 1,
	10, 5,	 3,
	11, 4,	 5,
	9, 7,	2 ,
	0, 1,	7 ,
	8, 9,	3 ,
	0, 8,	4 
};

float vertices1[] = {
	1.000000, 1.000000, 6, 
	3.000000, 0.000000, 4, 
	0.000000, 0.000000, 4, 
	0.000000, 3.000000, 4, 
	1.500000, 0.500000, 0 
};

float vertices2[] = {
	0			,	-0.525731		,	0.850651 	   ,
	0.850651	,	0				,	0.525731 	   ,
	0.850651	,	0				,	-0.525731 	   ,
	-0.850651	,	 0				,	 -0.525731 	   ,
	-0.850651	,	 0				,	 0.525731 	   ,
	-0.525731	,	 0.850651		,	 0 			   ,
	0.525731	,	0.850651		,	0 			   ,
	0.525731	,	-0.850651		,	 0 			   ,
	-0.525731	,	 -0.850651		,	0 			   ,
	0			,	-0.525731		,	 -0.850651 	   ,
	0			,	0.525731		,	-0.850651 	   ,
	0			,	0.525731		,	0.850651 

};


//shaders
GLuint programID = 0;

unsigned int VBO, VBO2;
unsigned int VAO[3];

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

double dx = 0;

void klawisz(GLubyte key, int x, int y)
{
	switch (key) {

	case 27:    /* Esc - koniec */
		exit(1);
		break;

	case 'x':
		dx += 0.1;
		break;
	case '1':

		break;
	case '2':

		break;
	}
	
	
}
/*###############################################################*/
void rysuj(void)
{

	//GLfloat color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	//glClearBufferfv(GL_COLOR, 0, color);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Kasowanie ekranu

	glUseProgram(programID); //u┐yj programu, czyli naszego shadera	

	MV = glm::mat4(1.0f);  //macierz jednostkowa
	MV = glm::translate(MV, glm::vec3(dx, 0, kameraD)); 
	MV = glm::rotate(MV, (float)glm::radians(kameraZ), glm::vec3(1, 0, 0)); 
	MV = glm::rotate(MV, (float)glm::radians(kameraX), glm::vec3(0, 1, 0));

	glm::mat4 MVP = P * MV;

	/*Zmienne   jednorodne   (ang. uniform variable), zwane  także  zmiennym  globalnymi,sązmiennymi,
		których  wartośc  jest  stała  w  obrębie  obiektu programu. Shadery  mogą zmienne jednorodne tylko  odczytywac */
	
	GLuint MVP_id = glGetUniformLocation(programID, "MVP"); // pobierz lokalizację zmiennej 'uniform' "MV" w programie
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));	   // wyślij tablicę mv do lokalizacji "MV", która jest typu mat4	


	glPolygonMode(GL_FRONT_AND_BACK,  GL_FILL);

	glBindVertexArray( VAO[0] );
	glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);

	glBindVertexArray( VAO[1] );
	glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);

	MV = glm::translate(MV, glm::vec3(15, 0, 0)); 
	MVP = P * MV;
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));

	glBindVertexArray( VAO[2] );
	glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);

	//glDrawArrays(GL_TRIANGLES, 0, 6); // Zaczynamy od 0 i rysujemy wszystkie wierzcho│ki

	glFlush();
	glutSwapBuffers();

}
/*###############################################################*/
void rozmiar(int width, int height)
{
	screen_width = width;
	screen_height = height;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
GLfloat k = 0.05f;
GLfloat ad = 0.0;

void timer(int value) {

	ad+= k;
	
	if(ad>1 || ad<0)
	k=-k;

	//GLfloat attrib[] = { ad, 0.0f,0.0f };
	// Aktualizacja wartości atrybutu wejściowego 1.
	//glVertexAttrib3fv(1, attrib);

	glutTimerFunc(20, timer, 0);
}
/*###############################################################*/
int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Przyklad 3");

	glewInit(); //init rozszerzeszeń OpenGL z biblioteki GLEW

	glutDisplayFunc(rysuj);			// def. funkcji rysuj¦cej
    glutIdleFunc(idle);			// def. funkcji rysuj¦cej w czasie wolnym procesoora (w efekcie: ci¦gle wykonywanej)
	glutTimerFunc(20, timer, 0);
	glutReshapeFunc(rozmiar); // def. obs-ugi zdarzenia resize (GLUT)
									
	glutKeyboardFunc(klawisz);		// def. obsługi klawiatury
	glutMouseFunc(mysz); 		// def. obsługi zdarzenia przycisku myszy (GLUT)
	glutMotionFunc(mysz_ruch); // def. obsługi zdarzenia ruchu myszy (GLUT)


	glEnable(GL_DEPTH_TEST);

	//glPointSize(3.0f);

	//==================================================================================================
	
	GLuint ebo;
	glGenBuffers(1,&ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements1), elements1, GL_STATIC_DRAW);

	GLuint ebo2;
	glGenBuffers(1,&ebo2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements2), elements2, GL_STATIC_DRAW);

	//tworzenie bufora wierzcholków
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	/* úadowanie shadera, tworzenie programu *************************/
	/* i linkowanie go oraz sprawdzanie b│ŕdˇw! **********************/
	programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");

	glGenVertexArrays(3, VAO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0); // pierwszy buform atrybuˇw: wierzcho│ki
	glVertexAttribPointer(
		0,                  // atrybut 0. musi odpowiadaŠ atrybutowi w programie shader
		3,                  // wielkoťŠ (x,y,z)
		GL_FLOAT,           // typ
		GL_FALSE,           // czy znormalizowany [0-1]?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
	glEnableVertexAttribArray(0); // pierwszy buform atrybuˇw: wierzcho│ki
	glVertexAttribPointer( 
		0,                  // atrybut 0. musi odpowiadaŠ atrybutowi w programie shader
		3,                  // wielkoťŠ (x,y,z)
		GL_FLOAT,           // typ
		GL_FALSE,           // czy znormalizowany [0-1]?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
	glEnableVertexAttribArray(0); // pierwszy buform atrybuˇw: wierzcho│ki
	glVertexAttribPointer( 
		0,                  // atrybut 0. musi odpowiadaŠ atrybutowi w programie shader
		3,                  // wielkoťŠ (x,y,z)
		GL_FLOAT,           // typ
		GL_FALSE,           // czy znormalizowany [0-1]?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glutMainLoop();					// start
	
	glDeleteBuffers(1,&VBO);

	return(0);
}

