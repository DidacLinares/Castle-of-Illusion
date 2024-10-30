#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "Scene.h"
#include "TitleScreen.h"
#include "MainMenu.h"
#include "irrKlang.h"
#include "CreditsScreen.h"
#include "SelectLevelScreen.h"
#include "EscScreen.h"
#include "GameOverScreen.h"
#include "MainLevel.h"


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


// Game is a singleton (a class with a single instance) that represents our whole application


class Game {

private:
	Game() {}

public:

	enum GameStatus {
		TITLE,
		MAIN_MENU,
		CREDITS_MENU,
		SELECT_LEVEL,
		PRACTICE_LEVEL,
		MAIN_LEVEL,
		PAUSE,
		GAME_OVER
	};

	static Game& instance()
	{
		static Game G;

		return G;
	}

	void init();
	bool update(int deltaTime);
	void render();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;

	Scene* getScene() { return scene; }
	
	void changeScene(int newStatus);
	int getSceneId() { return status; }
	int getLastScene() { return lastScene; }

	void setPaused(bool paused);
	bool isPaused() { return paused; }

	void close() { bPlay = false; }

	void setTries(int tries) { this->tries = tries; }
	void addTry() { tries++; }
	void removeTry() { tries--; }
	int getTries() { return tries; }

private:
	void renderScene(int status);
	void updateScene(int status, int deltaTime);

private:
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST + 1]; // Store key states so that 
	// we can have access at any time
	Scene* scene = nullptr;
	SelectLevelScreen* selectLevelScreen = nullptr;
	TitleScreen* titleScreen = nullptr;
	MainMenu* mainMenu = nullptr;
	CreditsScreen* creditsScreen = nullptr;
	EscScreen* escScreen = nullptr;
	GameOverScreen* gameOverScreen = nullptr;
	MainLevel* mainLevel = nullptr;
	irrklang::ISoundEngine* soundEngine;
	irrklang::ISoundSource* jumpSound;
	irrklang::ISoundSource* boxBreaking;
	irrklang::ISoundSource* dead;
	irrklang::ISoundSource* levelComplete;
	irrklang::ISoundSource* levelMusic;
	irrklang::ISoundSource* hit;
	irrklang::ISoundSource* mainMenuMusicSource;
	irrklang::ISound* mainMenuMusic;

	int status = 0;
	int lastScene = status;
	bool paused = false;

	int tries = 3;
};


#endif // _GAME_INCLUDE


