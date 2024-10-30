#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include <iostream>

void Game::init() {
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	soundEngine = irrklang::createIrrKlangDevice();
	jumpSound = soundEngine->addSoundSourceFromFile("sound/jump.wav");
	boxBreaking = soundEngine->addSoundSourceFromFile("sound/box_breaking.wav");
	levelComplete = soundEngine->addSoundSourceFromFile("sound/level_complete.wav");
	dead = soundEngine->addSoundSourceFromFile("sound/lose_live.wav");
	hit = soundEngine->addSoundSourceFromFile("sound/hit.wav");
	levelMusic = soundEngine->addSoundSourceFromFile("sound/level.wav");
	mainMenuMusicSource = soundEngine->addSoundSourceFromFile("sound/main_menu.wav");
	mainMenuMusic = soundEngine->play2D(mainMenuMusicSource, true, true, true);
	mainMenuMusic->setVolume(0.1f);
	mainMenuMusic->setIsPaused(false);

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
	case GAME_OVER:
		delete gameOverScreen;
		break;
	case PRACTICE_LEVEL:
		delete scene;
		break;
	case MAIN_LEVEL:
		delete scene;
		break;
	case PAUSE:
		if (escScreen != nullptr) delete escScreen;
		break;
	}

	lastScene = status;
	status = newStatus;

	switch (status) {
		case TITLE:
			if (!soundEngine->isCurrentlyPlaying(mainMenuMusicSource->getName())) {
				mainMenuMusic = soundEngine->play2D(mainMenuMusicSource, true, true, true);
				mainMenuMusic->setVolume(0.1f);
				mainMenuMusic->setIsPaused(false);
			}
			titleScreen = new TitleScreen();
			titleScreen->init();
			break;
		case SELECT_LEVEL:
			if (!soundEngine->isCurrentlyPlaying(mainMenuMusicSource->getName())) {
				mainMenuMusic = soundEngine->play2D(mainMenuMusicSource, true, true, true);
				mainMenuMusic->setVolume(0.1f);
				mainMenuMusic->setIsPaused(false);
			}
			selectLevelScreen = new SelectLevelScreen();
			selectLevelScreen->init();
			break;
		case MAIN_MENU:
			if (!soundEngine->isCurrentlyPlaying(mainMenuMusicSource->getName())) {
				mainMenuMusic = soundEngine->play2D(mainMenuMusicSource, true, true, true);
				mainMenuMusic->setVolume(0.1f);
				mainMenuMusic->setIsPaused(false);
			}
			mainMenu = new MainMenu();
			mainMenu->init();
			break;
		case CREDITS_MENU:
			if (!soundEngine->isCurrentlyPlaying(mainMenuMusicSource->getName())) {
				mainMenuMusic = soundEngine->play2D(mainMenuMusicSource, true, true, true);
				mainMenuMusic->setVolume(0.1f);
				mainMenuMusic->setIsPaused(false);
			}
			creditsScreen = new CreditsScreen();
			creditsScreen->init();
			break;
		case PRACTICE_LEVEL:
			if (soundEngine->isCurrentlyPlaying(mainMenuMusicSource->getName())) {
				mainMenuMusic->stop();
				mainMenuMusic->drop();
			}
			scene = new Scene();
			scene->init(soundEngine,jumpSound,levelMusic,boxBreaking,dead,levelComplete,hit);
			break;
		case GAME_OVER:
			gameOverScreen = new GameOverScreen();
			gameOverScreen->init();
			break;
		case MAIN_LEVEL:
			if (soundEngine->isCurrentlyPlaying(mainMenuMusicSource->getName())) {
				mainMenuMusic->stop();
				mainMenuMusic->drop();
			}
			mainLevel = new MainLevel();
			mainLevel->init(soundEngine, jumpSound, levelMusic, boxBreaking, dead, levelComplete,hit);
			scene = (Scene*) mainLevel;
			break;
		case PAUSE:
			if (escScreen != nullptr) delete escScreen;
			escScreen = new EscScreen();
			escScreen->init();
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
		case MAIN_LEVEL:
			scene->render();
			break;
		case GAME_OVER:
			gameOverScreen->render();
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
	case MAIN_LEVEL:
		scene->update(deltaTime);
		break;
	case GAME_OVER:
		gameOverScreen->update(deltaTime);
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