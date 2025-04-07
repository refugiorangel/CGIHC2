//Laboratorio Computacion Grafica Grupo 06
//Practica 9: Fuentes de Luz
//Rangel de la Rosa Jos� Refugio
//num. de cuenta: 420054559
//Fecha elaboraci�n: 06 de Abril de 2025
//Fecha entrega: 06 de Abril de 2025

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime> 
// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 lighthousePos(2.0f, 2.0f, 0.0f); 
float rotationSpeed = 0.5f;
bool active;

float angle = 0.0f; // El �ngulo de rotaci�n inicial
float lightCycleDuration = 2.0f; // Duraci�n del ciclo en segundos
float timeElapsed = 0.0f; // Acumulador de tiempo
float lightRadius = 5.0f; // Radio de la luz (distancia desde el origen)

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(2.0f, -0.4f, 2.0f)
};
float baseIntensity = 1.0f;
float flickerIntensity = 0.0f;

// Car Spot Light
bool isLightOn = true;
bool wasPKeyPressedLastFrame = false;

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Rangel de la Rosa Jose Refugio", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	
	//Model Dog((char*)"Models/ball.obj");
	Model Piso((char*)"Models/piso.obj");
	Model bottle((char*)"Models/Bott/bott.obj");
	Model lighthouse((char*)"Models/Lighthouse/lighthouse.obj");
	Model candle((char*)"Models/Candle/candle.obj");
	Model civilian((char*)"Models/Civilian/civilian.obj");



	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		
		
		//Load Model
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.5f,0.5f,0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.3f, 0.3f, 0.3f);


		// Point light 1
		//std::cout << "Test: " << std::endl;
		//flickerIntensity = randomFloat(baseIntensity - 0.3f, baseIntensity + 0.3f);
		glm::vec3 lightColor(1.0f, 0.9f, 0.7f);
		flickerIntensity = (baseIntensity - 0.3f) + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (baseIntensity + 0.6f - (baseIntensity - 0.6f)));
		//std::cout << "Flicker Intensity: " << flickerIntensity << std::endl;
		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x * 0.2f, lightColor.y * 0.2f, lightColor.z * 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x * flickerIntensity, lightColor.y * flickerIntensity, lightColor.z * flickerIntensity);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), lightColor.x * 0.3f, lightColor.y * 0.3f, lightColor.z * 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		// SpotLight 1
		timeElapsed += deltaTime; // Acumulamos el tiempo que ha pasado

		// Hacemos que el ciclo se repita constantemente
		if (timeElapsed > lightCycleDuration) {
			timeElapsed -= lightCycleDuration; // Reinicia el ciclo
		}

		// Actualiza el �ngulo de rotaci�n
		angle += 30.0f * deltaTime; // Aumenta el �ngulo (30 grados por segundo)

		// Aseg�rate de que el �ngulo no pase de 360 grados
		if (angle > 360.0f) {
			angle -= 360.0f; // Reseteamos el �ngulo si pasa de 360 grados
		}

		// Calculamos la direcci�n de la luz
		glm::vec3 spotlightDirection = glm::vec3(
			lightRadius * cos(glm::radians(angle)),  // x
			0.0f,                                  // No cambiamos la altura (eje Y)
			lightRadius * sin(glm::radians(angle))   // z
		);

		//std::cout << "Spotlight Direction: ("
		//	<< spotlightDirection.x << ", "
		//	<< spotlightDirection.y << ", "
		//	<< spotlightDirection.z << ")"
		//	<< std::endl;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].position"), lighthousePos[0], lighthousePos[1], lighthousePos[2]);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].direction"), spotlightDirection.x, spotlightDirection.y, spotlightDirection.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].ambient"), 1.0f, 1.0f, 1.0f);  // Luz blanca tenue
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].diffuse"), 1.0f, 1.0f, 1.0f);  // Luz blanca brillante
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].specular"), 0.2f, 0.2f, 0.2f); // Especular m�s suave
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].constant"), 1.0f);  // Efecto de ca�da suave
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].linear"), 0.08f);  // Menos atenuaci�n
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].quadratic"), 0.02f);  // Atenuaci�n muy suave
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].cutOff"), glm::cos(glm::radians(8.0f))); // Haz estrecho
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].outerCutOff"), glm::cos(glm::radians(12.0f))); // �ngulo exterior m�s grande

		// SpotLight 2
		if (isLightOn) {
			// Luz encendida (roja)
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].position"), 2.0f, 0.08f, -4.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].direction"), 0.0f, 0.0f, 1.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].ambient"), 0.2f, 0.0f, 0.0f);  // Rojo tenue
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].diffuse"), 1.0f, 0.0f, 0.0f);   // Rojo brillante
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].specular"), 0.1f, 0.0f, 0.0f); // Poco especular
		}
		else {
			// Luz apagada
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].ambient"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].specular"), 0.0f, 0.0f, 0.0f);
		}
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].linear"), 0.1f);   // Asegura que no se apague demasiado r�pido
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].quadratic"), 0.04f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		 //Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		
		glm::mat4 model(1);
		//Carga de modelo 
        view = camera.GetViewMatrix();	
		model = glm::mat4(1);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		glm::mat4 modelCivilian(1);
		modelCivilian= glm::mat4(1);
		modelCivilian = glm::translate(modelCivilian, glm::vec3(2.0f, 0.08f, -4.0f));
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCivilian));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
	    civilian.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
	
		glm::mat4 modelLhouse(1);
		modelLhouse = glm::mat4(1);
		modelLhouse = glm::translate(modelLhouse, glm::vec3(2.0f, -0.35f, 0.0f));
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLhouse));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		lighthouse.Draw(lightingShader);
		

		glm::mat4 modelCandle(1);
		modelCandle = glm::mat4(1);
//		modelCandle = glm::scale(modelCandle, glm::vec3(0.1f, 0.1f, 0.1f));
		modelCandle = glm::translate(modelCandle, glm::vec3(pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z));
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCandle));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		candle.Draw(lightingShader);

		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.001f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.001f;
	}
	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.001f;
	}
	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.001f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.001f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.001f;
	}
	if (keys[GLFW_KEY_P])
	{
		isLightOn = !isLightOn;  // Alterna el estado de la luz
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}

}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}