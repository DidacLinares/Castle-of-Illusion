#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include <iostream>

void Game::init() {
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	soundEngine = irrklang::createIrrKlangDevice();
	jumpSound = soundEngine->addSoundSourceFromFile("sound/jump.wav");
	titleScreen = new TitleScreen();
	titleScreen->init();
}

bool Game::update(int deltaTime) {
	if (!paused && getKey(GLFW_KEY_ESCAPE)) {
		setPaused(true);

		keyReleased(GLFW_KEY_ESCAPE);
	}

	if (paused) {
		if (escScreen == nullptr) {
			escScreen = new EscScreen();
			escScreen->init();
		}

		escScreen->update(deltaTime);
		
		return bPlay;
	}

	updateScene(status, deltaTime);

	return bPlay;
}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (paused) {
		escScreen->render();
		return;
	}

	renderScene(status);
}

void Game::keyPressed(int key) {
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
	case SELECT_LEVEL:
		delete selectLevelScreen;
		break;
	case CREDITS_MENU:
		delete creditsScreen;
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
		case SELECT_LEVEL:
			selectLevelScreen = new SelectLevelScreen();
			selectLevelScreen->init();
			break;
		case MAIN_MENU:
			mainMenu = new MainMenu();
			mainMenu->init();
			break;
		case CREDITS_MENU:
			creditsScreen = new CreditsScreen();
			creditsScreen->init();
			break;
		case PRACTICE_LEVEL:
			scene = new Scene();
			scene->init(soundEngine,jumpSound);
			break;
	}
}

void Game::renderScene(int status) {
	switch (status) {
		case TITLE:
			titleScreen->render();
			break;
		case SELECT_LEVEL:
			selectLevelScreen->render();
			break;
		case MAIN_MENU:
			mainMenu->render();
			break;
		case CREDITS_MENU:
			creditsScreen->render();
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
	case SELECT_LEVEL:
		selectLevelScreen->update(deltaTime);
		break;
	case MAIN_MENU:
		mainMenu->update(deltaTime);
		break;
	case CREDITS_MENU:
		creditsScreen->update(deltaTime);
		break;
	case PRACTICE_LEVEL:
		scene->update(deltaTime);
		break;
	}
}

void Game::setPaused(bool paused) {
	if (!paused) {
		delete escScreen;
		escScreen = nullptr;
	}

	this->paused = paused;
}