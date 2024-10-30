#ifndef _MAIN_LEVEL_INCLUDE
#define _MAIN_LEVEL_INCLUDE


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
#include "Scene.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class MainLevel : Scene {

public:
	MainLevel();
	~MainLevel();

	void init(irrklang::ISoundEngine* soundEngine, irrklang::ISoundSource* jumpSound, irrklang::ISoundSource* levelMusic, irrklang::ISoundSource* boxBreaking, irrklang::ISoundSource* dead,
				irrklang::ISoundSource* levelComplete,irrklang::ISoundSource* hit);
	void update(int deltaTime);
	void render();
	void renderInterface();

	void addEntity(NonPlayerEntity* entity);

	std::vector<NonPlayerEntity*>& getEnemies();
	ShaderProgram& getShaderProgram() { return texProgram; }
	void changeScene() { endLevel = true; }
	void changeMusicToDying();

private:
	void initShaders();
	void initInterface();
	void initNumbers();
	void initDummies();

private:

};


#endif // _MAIN_LEVEL_INCLUDE

