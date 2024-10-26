#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"

void Game::init() {
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	soundEngine = irrklang::createIrrKlangDevice();
	titleScreen = new TitleScreen();
	titleScreen->init();
}

bool Game::update(int deltaTime) {
	updateScene(status, deltaTime);

	return bPlay;
}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderScene(status);
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

void Game::changeScene(int newStatus) {
	switch (status) {
	case TITLE:
		delete titleScreen;
		break;
	case MAIN_MENU:
		delete mainMenu;
		break;
	case PRACTICE_LEVEL:
		delete scene;
		break;
	}

	status = newStatus;

	switch (status) {
		case TITLE:
			titleScreen = new TitleScreen();
			titleScreen->init();
			break;
		case MAIN_MENU:
			mainMenu = new MainMenu();
			mainMenu->init();
			break;
		case PRACTICE_LEVEL:
			scene = new Scene();
			scene->init(soundEngine);
			break;
	}
}

void Game::renderScene(int status) {
	switch (status) {
		case TITLE:
			titleScreen->render();
			break;
		case MAIN_MENU:
			mainMenu->render();
			break;
		case PRACTICE_LEVEL:
			scene->render();
			break;
	}
}

void Game::updateScene(int status, int deltaTime) {
	switch (status) {
	case TITLE:
		titleScreen->update(deltaTime);
		break;
	case MAIN_MENU:
		mainMenu->update(deltaTime);
		break;
	case PRACTICE_LEVEL:
		scene->update(deltaTime);
		break;
	}
}