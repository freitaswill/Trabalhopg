#include "SceneManager.h"


//static controllers for mouse and keyboard

static bool keys[1024];
static bool resized;
static GLuint width, height;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = 800;
	height = 600;
	
	telaAtual = tMenu;

	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();

}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Hello Transform", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	//Position Mouse callback
	glfwSetCursorPosCallback(window, mouseCursorCallback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Build and compile our shader program
	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE 
	// CONFIGURAÇÃO

	resized = true; //para entrar no setup da câmera na 1a vez

	setupScene();
}

void SceneManager::addShader(string vFilename, string fFilename)
{
	shader = new Shader (vFilename.c_str(), fFilename.c_str());
}


void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::mouseCursorCallback(GLFWwindow* window, double x, double y)
{
	cout << x << ": " << y << endl;
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);

}


void SceneManager::do_movement()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

}

void SceneManager::render()
{
	srand(time(0));



	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render scene
	shader->Use();

	// Create transformations 
	model = glm::mat4();
	offsetX = float();

	// Get their uniform location
	

	// Pass them to the shaders

	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	if ((glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT)) == GLFW_PRESS) {
		characterPositionX += 0.05f;
	}
	if ((glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT)) == GLFW_PRESS) {
		characterPositionX -= 0.05f;
	}
	if ((glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP)) == GLFW_PRESS) {
		characterPositionY += 0.05f;
	}
	if ((glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN)) == GLFW_PRESS) {
		characterPositionY -= 0.05f;
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && jumping == false) {
		jumping = true;
		characterPositionX += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) {
		jumping = false;
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS /*&& jumping == false*/) {
		/*jumping = true;*/
		for(int i = 0; i < 7; i++){
			obstaculoX[i] += 0.035f;
		}
	}
	//if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE) {
	//	jumping = false;
	//}

	/*if (jumping == true && caindo == false) {
		characterPositionY += 0.03f;
		if (characterPositionY >= 0.5f){
			caindo = true; jumping = false;
		}
	}
	else if (caindo) {
		characterPositionY -= 0.03f;
		if (characterPositionY <= 0) {
			characterPositionY = 0;
			caindo = false;
		}
	}*/
	// bind Texture
	// Bind Textures using texture units
	
	if (telaAtual == tJogo)
	{
		offsetBG1 += 0.008f;

		offsetBG2 += 0.005f;


		draw(glm::vec3(0, 0, 0), texture[0], 0, glm::vec3(1, 1, 1));

		draw(glm::vec3(0, 0, 0), texture[7], offsetBG1, glm::vec3(1, 1, 1));

		draw(glm::vec3(0, 0, 0), texture[8], offsetBG2, glm::vec3(1, 1, 1));

		draw(glm::vec3(characterPositionX - 0.5f, characterPositionY, 0), texture[1], 0, glm::vec3(1.5f, 1.5f, 1));


		

		for (int i = 0; i < 7; i++) {
			obstaculoX[i] -= 0.04f;
			if (obstaculoX[i] <= -1) {
				obstaculoX[i] = rand() % 4 + 2;
				obstaculoY[i] = (rand() % 16) / 10.0f + -0.5f;
				cout << "ZIP ZOP NA PARADA\n";
			}
		}

		for(int i = 0; i < 7; i++)
		draw(glm::vec3(obstaculoX[i], obstaculoY[i], 0), texture[2], 0, glm::vec3(1, 1, 1));

		for(int i = 0; i <7; i++)
		if (checkCollision(texture[2] - 1, texture[1] - 1, glm::vec3(obstaculoX[i], obstaculoY[i], 1)))
		{
			telaAtual = tGameOver;
		}

	}
	else if (telaAtual == tMenu)
	{
		draw(glm::vec3(0, 0, 0), texture[3], 0, glm::vec3(1, 1, 1));

		glfwGetCursorPos(window, &xpos, &ypos);

		if (xpos > 660 && xpos < 788 && ypos > 444 && ypos < 473 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			telaAtual = tJogo;
		}

		if (xpos > 639 && xpos < 788 && ypos > 515 && ypos < 545 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			telaAtual = tCreditos;
		}

		if (xpos > 627 && xpos < 788 && ypos > 480 && ypos < 509 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			telaAtual = tInstrucoes;
		}

		if (xpos > 670 && xpos < 788 && ypos > 552 && ypos < 580 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			telaAtual = sair;
		}
	}
	else if (telaAtual == tGameOver)
	{
		draw(glm::vec3(0, 0, 0), texture[4], 0, glm::vec3(1, 1, 1));

		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			for (int i = 3; i < 10; i++)
				obstaculoX[i-2] = i;

			telaAtual = tMenu;
		}
	}

	if (telaAtual == tCreditos)
	{
		draw(glm::vec3(0, 0, 0), texture[5], 0, glm::vec3(1, 1, 1));
	}

	if (telaAtual == tInstrucoes)
	{
		draw(glm::vec3(0, 0, 0), texture[6], 0, glm::vec3(1, 1, 1));
	}

	/*if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &xpos, &ypos);
		if (checkButton(xpos, ypos, texture[0]) == 0) {
			cout << "colidiu\n";
		}
		if (checkButton(xpos, ypos, texture[0]) == 1) {
			cout << " NNN  colidiu\n";
		}
	}*/
}

void SceneManager::run()
{
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	do {
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames+=1;
		if (currentTime - lastTime >= 0.016f) { // If last prinf() was more than 1 sec ago
											 // printf and reset timer
			cout << "frames\n" << 1000.0 / double(nbFrames);
			nbFrames = 0;
			lastTime += 0.016f;
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();

			//Update method(s)
			do_movement();

			//Render scene
			render();

			// Swap the screen buffers
			glfwSwapBuffers(window);
		}
		}while (!glfwWindowShouldClose(window) && telaAtual != sair);
	
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


		texture[0] = setupTexture("../textures/tJogo.png");

		texture[1] = setupTexture("../textures/capWhats.png");

		texture[2] = setupTexture("../textures/zap.png");

		texture[3] = setupTexture("../textures/tMenu.png");

		texture[4] = setupTexture("../textures/tGameOver.png");

		texture[5] = setupTexture("../textures/tCreditos.png");

		texture[6] = setupTexture("../textures/tInstrucoes.png");

		texture[7] = setupTexture("../textures/tJogo1.png");

		texture[8] = setupTexture("../textures/tJogo2.png");

}

void SceneManager::setupCamera2D()
{
	//corrigindo o aspecto
	float ratio;
	float xMin = -1.0, xMax = 1.0, yMin = -1.0, yMax = 1.0, zNear = -1.0, zFar = 1.0;


	// Get their uniform location
	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

int SceneManager::setupTexture(GLchar *path)
{
	unsigned int text;
	
	// load and create a texture 
	// -------------------------
	glGenTextures(1, &text);
	glBindTexture(GL_TEXTURE_2D, text); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	
	size[text-1][0] = width;
	size[text-1][1] = height;
	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return text;
}

void SceneManager::draw(glm::vec3 transform, int index, GLfloat offset, glm::vec3 scale)
{
	this->transform[index-1] = transform;
	this->scale[index - 1].x = scale.x;
	this->scale[index - 1].y = scale.y;
	this->scale[index - 1].z = 1;

	this->multScale[index - 1].x = size[index - 1][0] / 400.0f * scale.x;
	this->multScale[index - 1].y = size[index - 1][1] / 300.0f * scale.y;
	this->multScale[index - 1].z = 1;

	this->offset[index-1] = offset;

	GLint modelLoc = glGetUniformLocation(shader->Program, "model");
	model = glm::translate(model, glm::vec3(this->transform[index-1]));
	model = glm::scale(model, glm::vec3(this->multScale[index-1]));

	GLint offsetXx = glGetUniformLocation(shader->Program, "offsetX");
	offsetX = this->offset[index-1];

	glBindTexture(GL_TEXTURE_2D, index);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1f(offsetXx, offsetX);
	// render container
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	model = glm::scale(model, glm::vec3(1.0f/ this->multScale[index - 1].x, 1.0f / this->multScale[index - 1].y, 1.0f / this->multScale[index - 1].z));
	model = glm::translate(model, glm::vec3(-this->transform[index-1]));
	glBindTexture(GL_TEXTURE_2D, index);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

bool SceneManager::checkCollision(int a, int b)
{
	if ((transform[a].x + 1) * 400 + size[a][0] / 2 * scale[a].x <= (transform[b].x + 1) * 400 - size[b][0] / 2 * scale[b].x ||
		(transform[a].x + 1) * 400 - size[a][0] / 2 * scale[a].x >= (transform[b].x + 1) * 400 + size[b][0] / 2 * scale[b].x ||
		(transform[a].y - 1) * -300 + size[a][1] / 2 * scale[a].y <= (transform[b].y - 1) * -300 - size[b][1] / 2 * scale[b].y ||
		(transform[a].y - 1) * -300 - size[a][1] / 2 * scale[a].y >= (transform[b].y - 1) * -300 + size[b][1] / 2 * scale[b].y) {
		return false;
	}
	else
	return true;
}

bool SceneManager::checkCollision(int a, int b, glm::vec3 trans)
{
	if ((trans.x + 1) * 400 + size[a][0] / 2 * scale[a].x <= (transform[b].x + 1) * 400 - size[b][0] / 2 * scale[b].x ||
		(trans.x + 1) * 400 - size[a][0] / 2 * scale[a].x >= (transform[b].x + 1) * 400 + size[b][0] / 2 * scale[b].x ||
		(trans.y - 1) * -300 + size[a][1] / 2 * scale[a].y <= (transform[b].y - 1) * -300 - size[b][1] / 2 * scale[b].y ||
		(trans.y - 1) * -300 - size[a][1] / 2 * scale[a].y >= (transform[b].y - 1) * -300 + size[b][1] / 2 * scale[b].y) {
		return false;
	}
	else
		return true;
}

int SceneManager::checkButton(double x, double y, int id)
{
	if ((x >= transform[id].x * 800 - size[id][0] / 2 * scale[id].x &&
		x <= transform[id].x * 800 + size[id][0] / 2 * scale[id].x) &&
		(y >= transform[id].y * 600 - size[id][1] / 2 * scale[id].y &&
		y <= transform[id].y * 600 + size[id][1] / 2 * scale[id].y)) {
		return 1;
	}
	else
		return 0;
}
