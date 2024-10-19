#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Entity.h"


// Mirar que fa aixo
#define SCREEN_X 32
#define SCREEN_Y 16

// S'ha de canviar al necesari segons el nivell
//#define INIT_PLAYER_X_TILES 4
//#define INIT_PLAYER_Y_TILES 25

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 8


#define INIT_ENEMY_X_TILES 8
#define INIT_ENEMY_Y_TILES 8

Scene::Scene() {
	map = NULL;
	player = NULL;
}

Scene::~Scene() {
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init() {
	initShaders();
	//map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	layer_0 = TileMap::createTileMap("levels/sky_background_0.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	layer_1 = TileMap::createTileMap("levels/sky_background_1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	layer_2 = TileMap::createTileMap("levels/trees.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map = TileMap::createTileMap("levels/map.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	entityArray.push_back(new TreeEnemy());
	entityArray[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	entityArray[0]->setPlayer(player);
	entityArray[0]->setPosition(glm::vec2(INIT_ENEMY_X_TILES * map->getTileSize(), INIT_ENEMY_Y_TILES * map->getTileSize()));
	entityArray[0]->setTileMap(map);

	entityArray.push_back(new Cake());
	entityArray[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	entityArray[1]->setPlayer(player);
	entityArray[1]->setPosition(glm::vec2(INIT_ENEMY_X_TILES * map->getTileSize(), (INIT_ENEMY_Y_TILES + 1) * map->getTileSize()));
	entityArray[1]->setTileMap(map);

	entityArray.push_back(new Block());
	entityArray[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	entityArray[2]->setPlayer(player);
	entityArray[2]->setPosition(glm::vec2((INIT_ENEMY_X_TILES + 1) * map->getTileSize(), (INIT_ENEMY_Y_TILES + 1) * map->getTileSize()));
	entityArray[2]->setTileMap(map);

	entityArray.push_back(new FlowerEnemy());
	entityArray[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	entityArray[3]->setPlayer(player);
	entityArray[3]->setPosition(glm::vec2((INIT_ENEMY_X_TILES + 1) * map->getTileSize(), (INIT_ENEMY_Y_TILES + 1) * map->getTileSize()));
	entityArray[3]->setTileMap(map);

	// View at player position
	glm::vec2 pos = player->getPosition();

	projection = glm::ortho(pos.x - float(SCREEN_WIDTH) / 2, pos.x + float(SCREEN_WIDTH) / 2, pos.y + float(SCREEN_HEIGHT) / 2, pos.y - float(SCREEN_HEIGHT) / 2);
	
	currentTime = 0.0f;
}

void Scene::update(int deltaTime) {
	currentTime += deltaTime;
	player->update(deltaTime);
	int size = entityArray.size();
	for (int i = 0; i < size; ++i) {
		if (entityArray[i] != nullptr) {
			entityArray[i]->update(deltaTime);
			if (entityArray[i]->isDead()) {
				delete entityArray[i];
				entityArray[i] = nullptr;
			}
		}
	}
}

void Scene::render() {
	glm::mat4 modelview;

	texProgram.use();
	// Center the camera at player position
	glm::vec2 pos = player->getPosition();
	int zoom = 8;
	projection = glm::ortho(pos.x - float(SCREEN_WIDTH) / zoom, pos.x + float(SCREEN_WIDTH) / zoom, pos.y + float(SCREEN_HEIGHT) / zoom, pos.y - float(SCREEN_HEIGHT) / zoom);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);


	layer_0->render();
	layer_1->render();
	layer_2->render();
	map->render();
	player->render();
	int size = entityArray.size();
	for (int i = 0; i < size; ++i) {
		if (entityArray[i] != nullptr) {
			entityArray[i]->render();
		}
	}
}

void Scene::initShaders() {
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled()) {
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}

	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	
	if (!fShader.isCompiled()) {
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}

	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked()) {
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}

	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



