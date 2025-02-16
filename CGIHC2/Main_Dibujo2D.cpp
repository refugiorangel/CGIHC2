#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Dibujo de Primitivas en 2D", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		//Puntos Cabeza
		-0.571514379f, 0.943139226f, 0.00f,  0.5f, 0.0f, 0.5f,//C=0
		-0.572411733f, 0.585448842f, 0.00f,  0.5f, 0.0f, 0.5f,//D=1
		-0.928394144f, 0.586944576f, 0.00f,  0.5f, 0.0f, 0.5f,//E=2

		//Puntos pecho
		-0.570733978f, 0.73884424f, 0.00f, 1.0f, 0.5f, 0.0f,//F=3
		-0.569932484f, 0.01383689f, 0.00f, 1.0f, 0.5f, 0.0f,//G=4
		0.153827133f, 0.014666212f, 0.00f, 1.0f, 0.5f, 0.0f,//H=5

		//Punyos Piernas
		0.153282769f, -0.71008335f, 0.00f, 0.0f, 0.5f, 1.0f,//L=
		-0.569932484f, 0.01383689f, 0.00f, 0.0f, 0.5f, 1.0f,//G=
		0.153827133f, 0.014666212f, 0.00f, 0.0f, 0.5f, 1.0f,//H=5

		//Puntos Manos
		-0.569593663f, 0.162211236f, 0.00f, 1.0f, 0.0f, 0.0f,//I=
		-0.824822469f, -0.091923715f, 0.00f, 1.0f, 0.0f, 0.0f,//J=
		-0.572212468f, -0.344915839f, 0.00f, 1.0f, 0.0f, 0.0f,//K=

		//Puntos Pies
		0.047394509f, -0.604714471f, 0.00f, 1.0f, 0.5f, 0.7f,//M=
		-0.3111912f, -0.966878432f, 0.00f, 1.0f, 0.5f, 0.7f,//Q=
		0.408119728f, -0.967526624f, 0.00f, 1.0f, 0.5f, 0.7f,//N=

		//Puntos Bolsa 1
		-0.465181101f, -0.604049668f, 0.00f, 0.0f, 1.0f, 0.0f, //S=
		-0.208359753f, -0.348447583f, 0.00f, 0.0f, 1.0f, 0.0f, //T=
		0.047394509f, -0.604714471f, 0.00f, 0.0f, 1.0f, 0.0f, //M=

		//Puntos Bolsa 2
		0.047394509f, -0.604714471f, 0.00f, 0.0f, 1.0f, 0.0f, //M=
		-0.465181101f, -0.604049668f, 0.00f, 0.0f, 1.0f, 0.0f, //S=
		-0.209504786f, -0.859894314f, 0.00f, 0.0f, 1.0f, 0.0f, //R=

		////Puntos Cola 1
		0.667445795f, -0.711771171f, 0.00f, 1.0f, 1.0f, 0.0f,//O=
		0.920127276f, -0.966665767f, 0.00f, 1.0f, 1.0f, 0.0f,//P=
		0.153282769f, -0.71008335f, 0.00f, 1.0f, 1.0f, 0.0f,//L=

		//Puntos Cola 2
		0.153282769f, -0.71008335f, 0.00f, 1.0f, 1.0f, 0.0f,//L=
		0.408119728f, -0.967526624f, 0.00f, 1.0f, 1.0f, 0.0f,//N=
		0.920127276f, -0.966665767f, 0.00f, 1.0f, 1.0f, 0.0f,//P=
	};
	unsigned int indices[] = {  // note that we start from 0!
		0,1,2, //Triangulo Cabeza
		3,4,5, //Triangulo Pecho
		6,7,8, //Triangulo Piernas
		9,10,11, //Triangulo manos
		12,13,14, //Triangulo pies
		15,16,17, //Triangulo bolsa 1
		18,19,20, //Triangulo bolsa 2
		21,22,23, //Triangulo cola 1
		24,25,26, //Triangulo cola 2
	};



	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
        ourShader.Use();
        glBindVertexArray(VAO);


        //glPointSize(3);
        //glDrawArrays(GL_POINTS,0,1);
        
        //glDrawArrays(GL_LINES,0,2);
        //glDrawArrays(GL_LINE_LOOP,0,4);
        
        //glDrawArrays(GL_TRIANGLES,0,3);
        glDrawElements(GL_TRIANGLES, 27,GL_UNSIGNED_INT,0);

        
        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}