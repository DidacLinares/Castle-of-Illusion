#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "MainMenu.h"
#include "Game.h"


// Mirar que fa aixo
#define SCREEN_X 100
#define SCREEN_Y 100

// S'ha de canviar al necesari segons el nivell
//#define INIT_PLAYER_X_TILES 4
//#define INIT_PLAYER_Y_TILES 25



MainMenu::MainMenu() {
}

MainMenu::~MainMenu() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

enum Animation {
	DUMMY, ARROW
};

void MainMenu::init() {
	initShaders();

	imageTexture.loadFromFile("images/main_menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
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

	arrowTexture.loadFromFile("images/arrow.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	arrow = Sprite::createSprite(glm::ivec2(5, 5), glm::vec2(0.5f, 1.0f), &arrowTexture, &texProgram);
	arrow->setNumberAnimations(2);

	arrow->setAnimationSpeed(DUMMY, 1);
	arrow->addKeyframe(DUMMY, glm::vec2(0.f, 0.f));

	arrow->setAnimationSpeed(ARROW, 2);
	arrow->addKeyframe(ARROW, glm::vec2(0.f, 0.f));
	arrow->addKeyframe(ARROW, glm::vec2(0.5f, 0.f));

	arrow->changeAnimation(ARROW);

	arrowX = SCREEN_X / 2;
	arrowY = SCREEN_Y / 2;

	arrow->setPosition(glm::vec2(float(arrowX), float(arrowY)));
}

void MainMenu::update(int deltaTime) {
	arrow->update(deltaTime);

	if (Game::instance().getKey(GLFW_KEY_ENTER) || Game::instance().getKey(GLFW_KEY_SPACE)) {
		
		switch (selectedOption) {
			case 0:
				Game::instance().changeScene(Game::PRACTICE_LEVEL);
				break;
			case 1:
				//Game::instance().changeScene(Game::INSTRUCTIONS);
				break;
			case 2:
				//Game::instance().changeScene(Game::CREDITS);
				break;
		}

		Game::instance().keyReleased(GLFW_KEY_ENTER);
		Game::instance().keyReleased(GLFW_KEY_SPACE);
		return;
	}

	if (selectedOption == 0) {
		// Jugar
		if (Game::instance().getKey(GLFW_KEY_W) || Game::instance().getKey(GLFW_KEY_UP)) {
			selectedOption = 2;
			arrow->changeAnimation(ARROW);
		}
		else {
			if (Game::instance().getKey(GLFW_KEY_S) || Game::instance().getKey(GLFW_KEY_DOWN)) {
				selectedOption = 1;
				arrow->changeAnimation(ARROW);
			}
		}
		arrowX = 33;
		arrowY = 26;
	}
	else if (selectedOption == 1) {
		// Instruccions
		if (Game::instance().getKey(GLFW_KEY_W) || Game::instance().getKey(GLFW_KEY_UP)) {
			selectedOption = 0;
			arrow->changeAnimation(ARROW);
		}
		else {
			if (Game::instance().getKey(GLFW_KEY_S) || Game::instance().getKey(GLFW_KEY_DOWN)) {
				selectedOption = 2;
				arrow->changeAnimation(ARROW);
			}
		}
		arrowX = 18;
		arrowY = 38;
	}
	else {
		// Credits
		if (Game::instance().getKey(GLFW_KEY_W) || Game::instance().getKey(GLFW_KEY_UP)) {
			selectedOption = 1;
			arrow->changeAnimation(ARROW);
		}
		else {
			if (Game::instance().getKey(GLFW_KEY_S) || Game::instance().getKey(GLFW_KEY_DOWN)) {
				selectedOption = 0;
				arrow->changeAnimation(ARROW);
			}
		}

		arrowX = 30;
		arrowY = 52;
	}

	Game::instance().keyReleased(GLFW_KEY_W);
	Game::instance().keyReleased(GLFW_KEY_S);
	Game::instance().keyReleased(GLFW_KEY_UP);
	Game::instance().keyReleased(GLFW_KEY_DOWN);

	arrow->setPosition(glm::vec2(float(arrowX), float(arrowY)));
}

void MainMenu::render() {
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

	arrow->render();
}

void MainMenu::initShaders() {
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
