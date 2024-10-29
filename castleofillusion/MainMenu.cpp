#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "MainMenu.h"
#include "Game.h"


// Mirar que fa aixo
#define SCREEN_X 32
#define SCREEN_Y 16

// S'ha de canviar al necesari segons el nivell
//#define INIT_PLAYER_X_TILES 4
//#define INIT_PLAYER_Y_TILES 25



MainMenu::MainMenu() {
}

MainMenu::~MainMenu() {
	if (arrow != nullptr) delete arrow;
}

enum Animation {
	DUMMY, ARROW
};

void MainMenu::init() {
	initShaders();
	//glViewport(0, 0, 1280, 720);
	imageTexture.loadFromFile("images/main_menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(1280, 720), glm::vec2(1, 1), &imageTexture, &texProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(1, 1);
	sprite->addKeyframe(1, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(1);
	sprite->setPosition(glm::vec2(0, 0));
	arrowTexture.loadFromFile("images/arrow.png", TEXTURE_PIXEL_FORMAT_RGBA);
	arrow = Sprite::createSprite(glm::ivec2(60, 40), glm::vec2(0.5f, 1.0f), &arrowTexture, &texProgram);
	arrow->setNumberAnimations(2);

	arrow->setAnimationSpeed(DUMMY, 1);
	arrow->addKeyframe(DUMMY, glm::vec2(0.f, 0.f));

	arrow->setAnimationSpeed(ARROW, 2);
	arrow->addKeyframe(ARROW, glm::vec2(0.f, 0.f));
	arrow->addKeyframe(ARROW, glm::vec2(0.5f, 0.f));

	arrow->changeAnimation(ARROW);

	arrowX = 1280 / 2;
	arrowY = 720 / 2;

	arrow->setPosition(glm::vec2(float(arrowX), float(arrowY)));
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

	if (Game::instance().getTries() == 0) {
		Game::instance().setTries(3);
	}
}

void MainMenu::update(int deltaTime) {
	arrow->update(deltaTime);
	sprite->update(deltaTime);
	if (Game::instance().getKey(GLFW_KEY_ENTER) || Game::instance().getKey(GLFW_KEY_SPACE)) {
		
		switch (selectedOption) {
			case 0:
				Game::instance().changeScene(Game::SELECT_LEVEL);
				break;
			case 1:
				//Game::instance().changeScene(Game::INSTRUCTIONS);
				break;
			case 2:
				Game::instance().changeScene(Game::CREDITS_MENU);
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
		arrowX = 1280 / 3;//33;
		arrowY = 720 / 4;//26;
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
		arrowX = 240;//33;
		arrowY = 280;//26;
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

		arrowX = 1280/3 - 40;
		arrowY = 370;
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
	// Center the camera at player position
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	sprite->render();
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
