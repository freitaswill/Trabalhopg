#pragma once

#include "Shader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

enum Tela{ tJogo, tMenu };

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void initialize(GLuint width, GLuint height);
	void initializeGraphics();

	void addShader(string vFilename, string fFilename);

	//GLFW callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouseCursorCallback(GLFWwindow* window, double x, double y);
	static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	static void resize(GLFWwindow* window, int width, int height);

	void do_movement();
	void render();
	void run();
	void finish();


	// scene
	void setupScene();
	void setupCamera2D();
	int setupTexture(GLchar *path);
	void draw(glm::vec3 transform, int index, GLfloat offset, glm::vec3 scale);
	void telaJogo();
	void telaMenu();
	//static void tMenu();
	bool checkCollision(int a, int b);

	Tela telaAtual;

private:
	GLchar *path;
	GLfloat characterPositionX=0, characterPositionY=0, offsetBG1;
	double xpos, ypos;
	bool jumping = false, caindo = false;

	//GFLW window
	GLFWwindow *window;

	//our shader program
	Shader *shader;

	//scene attributes
	GLuint VAO;

	//Transformations - Model Matrix
	glm::mat4 model[2];

	//2D Camera - Projection matrix
	glm::mat4 projection;

	//Texture index
	unsigned int texture[2];

	float offsetX = 0, offset[2];

	//Transform index
	glm::vec3 transform[2];
	glm::vec3 scale[2];

	int size[2][2];

};

