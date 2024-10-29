#ifndef _TITLE_SCENE_INCLUDE
#define _TITLE_SCENE_INCLUDE

#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <irrKlang.h>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "sprite.h"


class TitleScreen {

public:
	TitleScreen();
	~TitleScreen();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	Texture imageTexture;
	glm::mat4 projection;
	GLuint vao, vbo;
	Sprite* sprite;
};

#endif // _TITLE_SCENE_INCLUDE

