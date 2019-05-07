#pragma once

#include "Shader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ctime>


enum Tela{ tJogo, tMenu, tCreditos, tInstrucoes, tGameOver, sair };

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
	static void resize(GLFWwindow* window, int width, int height);

	void do_movement();
	void render();
	void run();
	void finish();


	// scene
	void setupScene();
	void setupCamera2D();
	int setupTexture(GLchar *path);
	void draw(glm::vec3 transform, int index, GLfloat offset, glm::vec3 scale, GLfloat qtdSpritesX, GLfloat qtdSpritesY);
	//static void tMenu();
	bool checkCollision(int a, int b);
	bool checkCollision(int a, int b, glm::vec3 trans);
	int checkButton(double x, double y, int id);

	Tela telaAtual;

private:
	int telaGameOver;
	GLchar *path;
	GLfloat characterPositionX=0, characterPositionY=0, offsetBG1, offsetBG2; 
	GLfloat obstaculoX[7]{ 7, 8, 9, 3, 4, 5, 6 }, obstaculoY[7] { 0, 1, 2, 3, 4, 5, 6 };
	double xpos, ypos;
	bool jumping = false, caindo = false;
	GLfloat qtdSpritesX[9];
	int velSprites = 0;

	//GFLW window
	GLFWwindow *window;

	//our shader program
	Shader *shader;

	//scene attributes
	GLuint VAO;

	//Transformations - Model Matrix
	glm::mat4 model;

	//2D Camera - Projection matrix
	glm::mat4 projection;

	//Texture index
	unsigned int texture[9];

	float offsetX = 0, offset[9]{};

	//Transform index
	glm::vec3 transform[9];
	glm::vec3 scale[9];

	glm::vec3 multScale[9];

	int size[9][2];

};

