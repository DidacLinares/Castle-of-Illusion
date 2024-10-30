#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "SelectLevelScreen.h"
#include "Game.h"

SelectLevelScreen::SelectLevelScreen() {
}

SelectLevelScreen::~SelectLevelScreen() {
	if (arrow != nullptr) delete arrow;
	if (background != nullptr) delete background;
}

enum Animation {
	DUMMY, ARROW
};

void SelectLevelScreen::init() {
	initShaders();

	imageTexture.loadFromFile("images/select_levels.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::ivec2(1280, 720), glm::vec2(1, 1), &imageTexture, &texProgram);
	background->setNumberAnimations(2);

	background->setAnimationSpeed(0, 1);
	background->addKeyframe(0, glm::vec2(0.f, 0.f));

	background->setAnimationSpeed(1, 1);
	background->addKeyframe(1, glm::vec2(0.f, 0.f));
	background->changeAnimation(1);
	background->setPosition(glm::vec2(0, 0));
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
}

void SelectLevelScreen::update(int deltaTime) {
	arrow->update(deltaTime);

	if (Game::instance().getKey(GLFW_KEY_ENTER) || Game::instance().getKey(GLFW_KEY_SPACE)) {

		switch (selectedOption) {
		case 0:
			Game::instance().changeScene(Game::PRACTICE_LEVEL);
			break;
		case 1:
			//Game::instance().changeScene(Game::FINAL_LEVEL);
			break;
		}

		Game::instance().keyReleased(GLFW_KEY_ENTER);
		Game::instance().keyReleased(GLFW_KEY_SPACE);
		return;
	}

	if (selectedOption == 0) {
		// PRACTICE LEVEL
		if (Game::instance().getKey(GLFW_KEY_W) || Game::instance().getKey(GLFW_KEY_UP)) {
			selectedOption = 0;
			arrow->changeAnimation(ARROW);
		}
		else {
			if (Game::instance().getKey(GLFW_KEY_S) || Game::instance().getKey(GLFW_KEY_DOWN)) {
				selectedOption = 1;
				arrow->changeAnimation(ARROW);
			}
		}
		arrowX = 165;
		arrowY = 100;
	}
	else if (selectedOption == 1) {
		// BOSS LEVEL
		if (Game::instance().getKey(GLFW_KEY_W) || Game::instance().getKey(GLFW_KEY_UP)) {
			selectedOption = 0;
			arrow->changeAnimation(ARROW);
		}
		else {
			if (Game::instance().getKey(GLFW_KEY_S) || Game::instance().getKey(GLFW_KEY_DOWN)) {
				selectedOption = 0;
				arrow->changeAnimation(ARROW);
			}
		}
		arrowX = 165;
		arrowY = 210;
	}
	else {

	}

	Game::instance().keyReleased(GLFW_KEY_W);
	Game::instance().keyReleased(GLFW_KEY_S);
	Game::instance().keyReleased(GLFW_KEY_UP);
	Game::instance().keyReleased(GLFW_KEY_DOWN);

	arrow->setPosition(glm::vec2(float(arrowX), float(arrowY)));
}

void SelectLevelScreen::render() {
	glm::mat4 modelview;

	texProgram.use();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render();
	arrow->render();
}

void SelectLevelScreen::initShaders() {
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
