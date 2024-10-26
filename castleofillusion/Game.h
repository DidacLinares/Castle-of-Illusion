#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "Scene.h"
#include "TitleScreen.h"
#include "MainMenu.h"
#include "irrKlang.h"


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

private:
	void renderScene(int status);
	void updateScene(int status, int deltaTime);

private:
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST+1]; // Store key states so that 
							    // we can have access at any time
	Scene* scene;
	TitleScreen* titleScreen;
	MainMenu* mainMenu;
	irrklang::ISoundEngine* soundEngine;

	int status = 0;
};


#endif // _GAME_INCLUDE


