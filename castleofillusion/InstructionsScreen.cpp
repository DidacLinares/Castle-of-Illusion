#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "TitleScreen.h"
#include "Game.h"


InstructionsScreen::InstructionsScreen() {

}

InstructionsScreen::~InstructionsScreen() {
	if (background != nullptr) delete background;
}


void InstructionsScreen::init() {
	initShaders();
	//map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	imageTexture.loadFromFile("images/instruccions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::ivec2(1280, 720), glm::vec2(1, 1), &imageTexture, &texProgram);
	background->setNumberAnimations(2);

	background->setAnimationSpeed(0, 1);
	background->addKeyframe(0, glm::vec2(0.f, 0.f));

	background->setAnimationSpeed(1, 1);
	background->addKeyframe(1, glm::vec2(0.f, 0.f));

	background->setPosition(glm::vec2(0, 0));
	background->changeAnimation(1);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
}

void InstructionsScreen::update(int deltaTime) {
	background->update(deltaTime);
	if (Game::instance().getKey(GLFW_KEY_ENTER) || Game::instance().getKey(GLFW_KEY_SPACE)) {
		Game::instance().changeScene(Game::MAIN_MENU);
	}

	Game::instance().keyReleased(GLFW_KEY_ENTER);
	Game::instance().keyReleased(GLFW_KEY_SPACE);
}

void InstructionsScreen::render() {
	glm::mat4 modelview;

	texProgram.use();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render();
}

void InstructionsScreen::initShaders() {
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