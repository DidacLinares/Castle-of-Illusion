#ifndef _MAIN_SCENE_INCLUDE
#define _MAIN_SCENE_INCLUDE

#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <irrKlang.h>
#include "ShaderProgram.h"
#include "TileMap.h"


class MainScene {

public:
	MainScene();
	~MainScene();

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
};

#endif // _MAIN_SCENE_INCLUDE

