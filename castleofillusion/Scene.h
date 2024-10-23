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
#include "Chest.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene {

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void renderInterface();

	void addEntity(NonPlayerEntity* entity);

	std::vector<NonPlayerEntity*>& getEnemies();
	ShaderProgram& getShaderProgram() { return texProgram; }

private:
	void initShaders();
	void initInterface();
	void initNumbers();
	void initDummies();

private:
	TileMap* map;
	TileMap* layer_0;
	TileMap* layer_1;
	TileMap* layer_2;
	Player* player;
	irrklang::ISoundEngine* soundEngine;
	std::vector<NonPlayerEntity*> entityArray;

	std::vector<int> numberMaping;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::mat4 view;
	Sprite* numberSprite;
	Texture spritesheetNumbers;
	Sprite* interfaceBackgroundSprite;
	Texture spritesheetinterfaceBackground;
	Sprite* starSprite;
	Texture spritesheetStar;
	int nextRemove = 0,time = 300;
};


#endif // _SCENE_INCLUDE

