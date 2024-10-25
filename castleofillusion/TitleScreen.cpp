#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "TitleScreen.h"
#include "Game.h"


// Mirar que fa aixo
#define SCREEN_X 32
#define SCREEN_Y 16

// S'ha de canviar al necesari segons el nivell
//#define INIT_PLAYER_X_TILES 4
//#define INIT_PLAYER_Y_TILES 25



TitleScreen::TitleScreen() {
}

TitleScreen::~TitleScreen() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}


void TitleScreen::init() {
	initShaders();

	imageTexture.loadFromFile("images/main_screen.png", TEXTURE_PIXEL_FORMAT_RGBA);
	imageTexture.setMinFilter(GL_NEAREST);
	imageTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.0f, float(SCREEN_X), float(SCREEN_Y), 0.0f);

	GLfloat vertices[] = { 0.0f,  0.0f, 0.0f, 0.0f, float(SCREEN_X), 0.0f, 1.0f, 0.0f, float(SCREEN_X), float(SCREEN_Y),  1.0f, 1.0f, 0.0f, float(SCREEN_Y),  0.0f, 1.0f };

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void TitleScreen::update(int deltaTime) {
	if (Game::instance().getKey(GLFW_KEY_ENTER) || Game::instance().getKey(GLFW_KEY_SPACE)) {
		Game::instance().changeScene(Game::MAIN_MENU);
	}

	Game::instance().keyReleased(GLFW_KEY_ENTER);
	Game::instance().keyReleased(GLFW_KEY_SPACE);
}

void TitleScreen::render() {
	glm::mat4 modelview;

	texProgram.use();

	// Configuración de la proyección y la vista
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	// Bind de la textura
	imageTexture.use();

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}

void TitleScreen::initShaders() {
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled()) {
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}

	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");

	if (!fShader.isCompiled()) {
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}

	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked()) {
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}

	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
