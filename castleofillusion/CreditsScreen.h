
#ifndef _CREDITS_SCREEN_INCLUDE
#define _CREDITS_SCREEN_INCLUDE

#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <irrKlang.h>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Sprite.h"

class CreditsScreen {

public:
	CreditsScreen();
	~CreditsScreen();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	Texture imageTexture;

	Texture arrowTexture;

	glm::mat4 projection;
	GLuint vao, vbo;
};

#endif // _MAIN_MENU_INCLUDE