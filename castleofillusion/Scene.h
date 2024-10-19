#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "TreeEnemy.h"
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
	TreeEnemy *enemy; // potser canviar a un array? depenent de la escena, i fer una superclasse de enemy
	FlowerEnemy* flower;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::mat4 view;
};


#endif // _SCENE_INCLUDE

