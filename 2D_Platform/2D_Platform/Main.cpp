#include <iostream>								// C++ streams
#include <stdlib.h>								// C++ stuff
#include <time.h>								// C++ stuff
#define GLEW_STATIC								// GLEW
#include <glew.h>								// GLEW
#include <glfw3.h>								// GLFW
#include <SOIL\SOIL.h>							// SOIL
#include <glm/glm/glm.hpp>						// GLM Mathematics
#include <glm/glm/gtc/matrix_transform.hpp>		// GLM Mathematics
#include <glm/glm/gtc/type_ptr.hpp>				// GLM Mathematics
#include "shaderclass.h"						// Shader Class

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void initPositions(void);
void doMovement(void);

const GLuint WIDTH = 1920, HEIGHT = 1080;		// Window dimensions
bool keys[1024];							// Movement Globals

// Texture Variables
glm::vec3 groundPositions[1800];
glm::vec2 groundTexCoords[1800];
glm::vec3 alphaPositions[1800];
glm::vec2 alphaTexCoords[1800];

int main()
{
	glfwInit();									// init GLFW
	srand(time(NULL));							// Random Generator Seed

	// Good spot for Initialization functions.
	initPositions();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);	// Create a GLFWwindow object that we can use for GLFW functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);		// Set the required callback functions
	glewExperimental = GL_TRUE;						// GLEW - modern function pointers and extensions
	glewInit();										// Initialize GLEW to setup the OpenGL Function pointers
	glViewport(0, 0, WIDTH, HEIGHT);				// Define the viewport dimensions
	Shader ourShader("vertex_shader.txt", "fragment_shader.txt");	// Build and compiler shader program

	GLfloat coin_verts[] = {						// Initialize Geometry
		-0.0156f, -0.0278, 0.0f, 0.3334f, 0.8333f,	// bottom left
		0.0156f, -0.0278f, 0.0f, 0.5f, 0.8333f,		// bottom right
		0.0156f, 0.0278f, 0.0f, 0.5f, 1.0,			// top right
		-0.0156f, 0.0278f, 0.0f, 0.3334f, 1.0f		// top left
	};

	// file is 126x190 (8x12)  = (15.75ea x 15.8333ea)  on screen (x 60, y 68)
	GLfloat alpha_verts[] = {
		-0.0082f, -0.0146f, 0.0f, 0.0f, 0.0f,
		0.0082f, -0.0146f, 0.0f, 0.125f, 0.0f,
		0.0082f, 0.0146f, 0.0f, 0.125f, 0.083f,
		-0.0082f, 0.0146f, 0.0f, 0.0f, 0.083f
	};

	GLuint indices[] = {				// Note that we start from 0!
		0, 1, 2,						// First Triangle
		2, 3, 0							// Second Triangle
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);				// Bind VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coin_verts), coin_verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);				// Unbind VAO

	int width, height;					// Load and Create a Texture
	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* image2 = SOIL_load_image("wallsprites1.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint VBO2, VAO2, EBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);
	glBindVertexArray(VAO2);				// Bind VAO2
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(alpha_verts), alpha_verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);				// Unbind VAO2

	GLuint texture3;					// Load and Create another Texture
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* image3 = SOIL_load_image("alphabet.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image3);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image3);
	glBindTexture(GL_TEXTURE_2D, 0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();							// Check for events, call related callback functions
		doMovement();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		// Clear Back Buffer
		glClear(GL_COLOR_BUFFER_BIT);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);		// Link to Shader Variable
		ourShader.Use();							// Activate Shader Program

		// Drawing with 1st texture here...
		glActiveTexture(GL_TEXTURE0);				// Set Active Texture Unit
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		for (int i = 0; i < 1800; i++)
		{
			glm::mat4 transform;
			transform = glm::translate(transform, groundPositions[i]);
			GLint transformLoc = glGetUniformLocation(ourShader.Program, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
			GLint texCoordLoc = glGetUniformLocation(ourShader.Program, "texDecide");
			glUniform2fv(texCoordLoc, 1, glm::value_ptr(groundTexCoords[i]));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);

		// Drawing with 2nd Texture here...
		glActiveTexture(GL_TEXTURE0);				// Set Active Texture Unit
		glBindTexture(GL_TEXTURE_2D, texture3);
		glBindVertexArray(VAO2);
		for (int i = 0; i < 1800; i++)
		{
			glm::mat4 transform2;
			transform2 = glm::translate(transform2, alphaPositions[i]);
			GLint transformLoc2 = glGetUniformLocation(ourShader.Program, "transform");
			glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(transform2));
			GLint texCoordLoc2 = glGetUniformLocation(ourShader.Program, "texDecide");
			glUniform2fv(texCoordLoc2, 1, glm::value_ptr(alphaTexCoords[i]));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);					// De-allocate resources
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();								// Terminate GLFW (it will clear GLFW allocations...)
	return 0;										// exit program
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void initPositions(void)
{
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			groundPositions[i + (j * 30)].x = -0.95 + (i*0.03125f);
			groundPositions[i + (j * 30)].y = -1.33 + (j*0.05556f);
			groundPositions[i + (j * 30)].z = 0.0f;
			groundTexCoords[i + (j * 30)].x = (rand() % 6) * 0.16667f;
			groundTexCoords[i + (j * 30)].y = (rand() % 6) * 0.16667f;
		}
	}
	// x 60, y 68
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			alphaPositions[i + (j * 30)].x = i * 0.03125f;
			alphaPositions[i + (j * 30)].y = -1.33 + j * 0.05556f;
			alphaPositions[i + (j * 30)].z = 0.0f;
			alphaTexCoords[i + (j * 30)].x = (rand() % 8) * 0.125f;
			alphaTexCoords[i + (j * 30)].y = (rand() % 12) * 0.083f;
		}
	}
}

void doMovement(void)
{
	GLfloat moveDX = 0.0f;
	GLfloat moveDY = 0.0f;

	if (keys[GLFW_KEY_LEFT])
		moveDX += 0.00105f;
	if (keys[GLFW_KEY_RIGHT])
		moveDX -= 0.00105f;
	if (keys[GLFW_KEY_UP])
		moveDY -= 0.00185f;
	if (keys[GLFW_KEY_DOWN])
		moveDY += 0.00185f;

	for (int i = 0; i < 1800; i++)
	{
		groundPositions[i].x += moveDX;
		groundPositions[i].y += moveDY;
		alphaPositions[i].x += moveDX;
		alphaPositions[i].y += moveDY;
	}
}
