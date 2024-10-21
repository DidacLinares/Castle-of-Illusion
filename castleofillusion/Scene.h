#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include <irrKlang.h>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "TreeEnemy.h"
#include "Cake.h"
#include "Block.h"
#include "FlowerEnemy.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene {

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	TileMap* map;
	TileMap* layer_0;
	TileMap* layer_1;
	TileMap* layer_2;
	Player* player;
	irrklang::ISoundEngine* soundEngine;
	std::vector<NonPlayerEntity*> entityArray;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::mat4 view;
};


#endif // _SCENE_INCLUDE

