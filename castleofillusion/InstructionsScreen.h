#ifndef _INSTRUCTIONS_SCENE_INCLUDE
#define _INSTRUCTIONS_SCENE_INCLUDE

#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <irrKlang.h>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "sprite.h"


class InstructionsScreen {

public:
	InstructionsScreen();
	~InstructionsScreen();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	Texture imageTexture;
	glm::mat4 projection;
	Sprite* background;
};

#endif // _INSTRUCTIONS_SCENE_INCLUDE

