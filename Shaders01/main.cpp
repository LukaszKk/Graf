#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GL\glew.h"
#include "GL\freeglut.h"

char * shaderLoadSource(const char *filePath);


// Program bazuje na tutorialu ze strony: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/


//struktura
typedef struct Point3
{
	float x;
	float y;
	float z;
}Point3;

//Wymiary okna
int screen_width = 640;
int screen_height = 480;

float vertices[] = {
	0, 0, 0,
	-0.25, 0, 0,
	0, 0.25, 0,		//1
	0, 0.25, 0,
	-0.25, 0.25, 0,
	-0.25, 0, 0,		//2
	-0.25, 0, 0,
	-0.5, 0, 0,
	-0.25, 0.25, 0,	//3
	-0.25, 0.25, 0,
	-0.5, 0.25, 0,
	-0.5, 0, 0,		//4
	-0.5, 0, 0,
	-0.75, 0, 0,
	-0.5, 0.25, 0,		//5
	-0.5, 0.25, 0,	
	-0.25, 0.25, 0,	
	-0.25, 0.5, 0,		//6	
	-0.25, 0.5, 0,	
	-0.25, 0.25, 0,	
	0, 0.5, 0,			//7	
	0, 0.5, 0,	
	-0.25, 0.25, 0,	
	0, 0.25, 0,			//8	
	0, 0.5, 0,
	-0.25, 0.5, 0,
	0, 0.75, 0,			//9
	
	0, 0, 0,
	0.25, 0, 0,
	0, 0.25, 0,		//1
	0, 0.25, 0,
	0.25, 0.25, 0,
	0.25, 0, 0,		//2
	0.25, 0, 0,
	0.5, 0, 0,
	0.25, 0.25, 0,	//3
	0.25, 0.25, 0,
	0.5, 0.25, 0,
	0.5, 0, 0,		//4
	0.5, 0, 0,
	0.75, 0, 0,
	0.5, 0.25, 0,		//5
	0.5, 0.25, 0,	
	0.25, 0.25, 0,	
	0.25, 0.5, 0,		//6	
	0.25, 0.5, 0,	
	0.25, 0.25, 0,	
	0, 0.5, 0,			//7	
	0, 0.5, 0,	
	0.25, 0.25, 0,	
	0, 0.25, 0,			//8	
	0, 0.5, 0,
	0.25, 0.5, 0,
	0, 0.75, 0,			//9

	0, 0, 0,
	-0.25, 0, 0,
	0, -0.25, 0,		//1
	0, -0.25, 0,
	-0.25, -0.25, 0,
	-0.25, 0, 0,		//2
	-0.25, 0, 0,
	-0.5, 0, 0,
	-0.25, -0.25, 0,	//3
	-0.25, -0.25, 0,
	-0.5, -0.25, 0,
	-0.5, 0, 0,		//4
	-0.5, 0, 0,
	-0.75, 0, 0,
	-0.5, -0.25, 0,		//5
	-0.5, -0.25, 0,	
	-0.25, -0.25, 0,	
	-0.25, -0.5, 0,		//6	
	-0.25, -0.5, 0,	
	-0.25, -0.25, 0,	
	0, -0.5, 0,			//7	
	0, -0.5, 0,	
	-0.25, -0.25, 0,	
	0, -0.25, 0,			//8	
	0, -0.5, 0,
	-0.25, -0.5, 0,
	0, -0.75, 0,			//9

	0, 0, 0,
	0.25, 0, 0,
	0, -0.25, 0,		//1
	0, -0.25, 0,
	0.25, -0.25, 0,
	0.25, 0, 0,		//2
	0.25, 0, 0,
	0.5, 0, 0,
	0.25, -0.25, 0,	//3
	0.25, -0.25, 0,
	0.5, -0.25, 0,
	0.5, 0, 0,		//4
	0.5, 0, 0,
	0.75, 0, 0,
	0.5, -0.25, 0,		//5
	0.5, -0.25, 0,	
	0.25, -0.25, 0,	
	0.25, -0.5, 0,		//6	
	0.25, -0.5, 0,	
	0.25, -0.25, 0,	
	0, -0.5, 0,			//7	
	0, -0.5, 0,	
	0.25, -0.25, 0,	
	0, -0.25, 0,			//8	
	0, -0.5, 0,
	0.25, -0.5, 0,
	0, -0.75, 0			//9
};

float color[] = {
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,	//1
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	//2
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,	//3
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	//4
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,	//5
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,	//6
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	//7
	1.0f, 0.5f, 0.0f,
	1.0f, 0.5f, 0.0f,
	1.0f, 0.5f, 0.0f,	//8
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,	//9

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,	//1
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	//2
	1.0f, 0.5f, 0.0f,
	1.0f, 0.5f, 0.0f,
	1.0f, 0.5f, 0.0f,	//3
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	//4
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,	//5
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,	//6
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	//7
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,	//8
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,	//9

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,	//1
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	//2
	1.0f, 0.5f, 0.0f,
	1.0f, 0.5f, 0.0f,
	1.0f, 0.5f, 0.0f,	//3
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	//4
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,	//5
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,	//6
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	//7
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,	//8
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,	//9

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,	//1
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	//2
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,	//3
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	//4
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,	//5
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,	//6
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	//7
	1.0f, 0.5f, 0.0f,
	1.0f, 0.5f, 0.0f,
	1.0f, 0.5f, 0.0f,	//8
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f	//9
};

bool VBO = false;

/*###############################################################*/

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Pierwszy prog");

	glewInit(); //init rozszerzeszeñ OpenGL z biblioteki GLEW
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glutDisplayFunc(rysuj);			// def. funkcji rysuj¦cej
	//glutIdleFunc(idle);			// def. funkcji rysuj¦cej w czasie wolnym procesoora (w efekcie: ci¦gle wykonywanej)
	//glutReshapeFunc(rozmiar);		// def. obs-ugi zdarzenia resize (GLUT)

	//glutKeyboardFunc(klawisz);		// def. obs³ugi klawiatury
	//glutMouseFunc(mysz); 		// def. obs³ugi zdarzenia przycisku myszy (GLUT)
	//glutMotionFunc(mysz_ruch); // def. obs³ugi zdarzenia ruchu myszy (GLUT)

	
	//tworzenie bufora wierzcholków---------------------------------------------------------------------------
	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint VBO_2;
	glGenBuffers(1, &VBO_2);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	


	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	char *vertexShaderSource = shaderLoadSource("vertex_shader.glsl");
	if (!vertexShaderSource)
		return 0;
	glShaderSource(vertexShader, 1, (const char **)&vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	//sprawdzenie b³êdów kompilacji....
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
	}
	
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);


    // wczytanie kodu shadera
	char *fragmentShaderSource = shaderLoadSource("fragment_shader.glsl");
	if (!fragmentShaderSource)
		return 0;

	glShaderSource(fragmentShader, 1, (const char **)&fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);
	
	
	/*glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/


	glDrawArrays(GL_TRIANGLES, 0, 36*3);

	glutSwapBuffers();

	glutMainLoop();					// start

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	return(0);
}


/*
* Returns a string containing the text in
* a vertex/fragment shader source file.
*/
char *shaderLoadSource(const char *filePath)
{
	const size_t blockSize = 512;
	FILE *fp;
	char buf[512];
	char *source = NULL;
	size_t tmp, sourceLength = 0;

	/* open file */
	fopen_s(&fp, filePath, "r");
	if (!fp) {
		fprintf(stderr, "shaderLoadSource(): Unable to open %s for reading\n", filePath);
		return NULL;
	}

	/* read the entire file into a string */
	while ((tmp = fread(buf, 1, blockSize, fp)) > 0) {
		char *newSource = (char*)malloc(sourceLength + tmp + 1);
		if (!newSource) {
			fprintf(stderr, "shaderLoadSource(): malloc failed\n");
			if (source)
				free(source);
			return NULL;
		}

		if (source) {
			memcpy(newSource, source, sourceLength);
			free(source);
		}
		memcpy(newSource + sourceLength, buf, tmp);

		source = newSource;
		sourceLength += tmp;
	}

	/* close the file and null terminate the string */
	fclose(fp);
	if (source)
		source[sourceLength] = '\0';

	return source;
}

