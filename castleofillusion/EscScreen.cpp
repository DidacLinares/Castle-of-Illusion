#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "EscScreen.h"
#include "Game.h"


EscScreen::EscScreen() {
}

EscScreen::~EscScreen() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

enum Animation {
	DUMMY, ARROW
};

void EscScreen::init() {
	initShaders();
	//glViewport(0, 0, 1280, 720);
	imageTexture.loadFromFile("images/esc_menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
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

void EscScreen::update(int deltaTime) {
	arrow->update(deltaTime);
	background->update(deltaTime);

	if (Game::instance().getKey(GLFW_KEY_ESCAPE)) {
		Game::instance().setPaused(false);
		Game::instance().keyReleased(GLFW_KEY_ESCAPE);
		return;
	}

	if (Game::instance().getKey(GLFW_KEY_ENTER) || Game::instance().getKey(GLFW_KEY_SPACE)) {

		switch (selectedOption) {
		case 0:
			Game::instance().setPaused(false);
			break;
		case 1:
			Game::instance().setPaused(false);
			Game::instance().changeScene(Game::instance().getSceneId());
			break;
		case 2:
			Game::instance().setPaused(false);

			if (Game::instance().getSceneId() == Game::MAIN_MENU) {
				Game::instance().close();
			}
			else {
				Game::instance().changeScene(Game::MAIN_MENU);
			}
			break;
		}

		Game::instance().keyReleased(GLFW_KEY_ENTER);
		Game::instance().keyReleased(GLFW_KEY_SPACE);
		return;
	}

	if (selectedOption == 0) {
		// Continuar
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
		arrowX = 320;//33;
		arrowY = 720 / 4;//26;
	}
	else if (selectedOption == 1) {
		// Reiniciar
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
		arrowX = 340;//33;
		arrowY = 330;//26;
	}
	else {
		// Sortir
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

		arrowX = 400;//33;
		arrowY = 480;//26;
	}

	Game::instance().keyReleased(GLFW_KEY_W);
	Game::instance().keyReleased(GLFW_KEY_S);
	Game::instance().keyReleased(GLFW_KEY_UP);
	Game::instance().keyReleased(GLFW_KEY_DOWN);

	arrow->setPosition(glm::vec2(float(arrowX), float(arrowY)));
}

void EscScreen::render() {
	if (Game::instance().getSceneId() == Game::PRACTICE_LEVEL || Game::instance().getSceneId() == Game::MAIN_LEVEL) {
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

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

void EscScreen::initShaders() {
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
