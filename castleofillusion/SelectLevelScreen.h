
#ifndef _SELECT_LEVEL_INCLUDE
#define _SELECT_LEVEL_INCLUDE

#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <irrKlang.h>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Sprite.h"

class SelectLevelScreen {

public:
	SelectLevelScreen();
	~SelectLevelScreen();

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

#endif // _SELECT_LEVEL_INCLUDE