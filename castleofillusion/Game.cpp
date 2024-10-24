#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init() {
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	//scene.init();
	mainScene.init();
}

bool Game::update(int deltaTime) {
	mainScene.update(deltaTime);

	return bPlay;
}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mainScene.render();
}

void Game::keyPressed(int key) {
	if (key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key) {
	keys[key] = false;
}

void Game::mouseMove(int x, int y) {
}

void Game::mousePress(int button) {
}

void Game::mouseRelease(int button) {
}

bool Game::getKey(int key) const {
	return keys[key];
}