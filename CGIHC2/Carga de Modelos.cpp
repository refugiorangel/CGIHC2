//Laboratorio Computacion Grafica Grupo 06
//Práctica 6. Carga de modelos
//Aumno: Rangel de la Rosa José Refugio
//num. de cuenta: 420054559
//Fecha elaboración: 16 de Marzo de 2025
//Fecha entrega: 16 de Marzo de 2025

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void Inputs(GLFWwindow* window);
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );



// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

float	escalar = 0.01f;
float	rotar = 0.0f;


int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Rangel de la Rosa Jose Refugio", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    
    // Load models
    Model dog((char*)"Models/RedDog.obj");

    //Model pepsiman((char*)"Models/Pepsiman/pepsiman.obj");

    //Model fuente((char*)"Models/Fountain/fountain.obj");

    //Model templo((char*)"Models/Temple/temple.obj");

    Model venus((char*)"Models/Venu/Venu.obj");

    Model tower((char*)"Models/Tower/tower.obj");

    Model house((char*)"Models/House/house.obj");


    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
  

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        Inputs(window);
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        
        dog.Draw(shader);
        
        glm::mat4 modelTower = glm::mat4(1.0f);
        modelTower = glm::translate(modelTower, glm::vec3(10.0f, -0.7f, 0.0f));
        modelTower = glm::scale(modelTower, glm::vec3(2.0, 2.0, 2.0));
        modelTower = glm::rotate(modelTower, glm::radians(rotar), glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTower));
        tower.Draw(shader);

        glm::mat4 modelTower2 = glm::mat4(1.0f);
        modelTower2 = glm::translate(modelTower2, glm::vec3(-10.0f, -0.7f, 0.0f));
        modelTower2 = glm::scale(modelTower2, glm::vec3(2.0, 2.0, 2.0));
        modelTower2 = glm::rotate(modelTower2, glm::radians(rotar), glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTower2));
        tower.Draw(shader);

        glm::mat4 modelVenus = glm::mat4(1.0f);
        modelVenus = glm::translate(modelVenus, glm::vec3(-5.0f, -0.5f, 3.0f));
        modelVenus = glm::scale(modelVenus, glm::vec3(0.03, 0.03, 0.03));
        modelVenus = glm::rotate(modelVenus, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelVenus));
        venus.Draw(shader);

        glm::mat4 modelHouse = glm::mat4(1.0f);
        modelHouse = glm::translate(modelHouse, glm::vec3(0.0f, 2.2f, 0.0f));
        modelHouse = glm::scale(modelHouse, glm::vec3(25.0f, 25.0f, 25.0f));
        //modelHouse = glm::rotate(modelHouse, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelHouse));
        house.Draw(shader);

        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

   
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

 

 
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

void Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        escalar += 0.000005f;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        escalar -= 0.000005f;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        rotar += 0.005f;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        rotar -= 0.005f;
}