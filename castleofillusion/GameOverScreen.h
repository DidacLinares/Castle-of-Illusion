#ifndef _GAME_OVER_SCREEN_INCLUDE
#define _GAME_OVER_SCREEN_INCLUDE

#include <glm/glm.hpp>
#include <vector>
#include <irrKlang.h>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Sprite.h"

class GameOverScreen {

public:
	GameOverScreen();
	~GameOverScreen();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	Texture imageTexture;

	Texture arrowTexture;
	Sprite* arrow;

	int arrowX, arrowY;

	int selectedOption = 0;

	glm::mat4 projection;
	GLuint vao, vbo;
};

#endif // _GAME_OVER_SCREEN_INCLUDE