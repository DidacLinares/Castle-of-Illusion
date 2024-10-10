#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

// Mirar que fa aixo
#define SCREEN_X 32
#define SCREEN_Y 16

// S'ha de canviar al necesari segons el nivell
//#define INIT_PLAYER_X_TILES 4
//#define INIT_PLAYER_Y_TILES 25

#define INIT_PLAYER_X_TILES 1
#define INIT_PLAYER_Y_TILES 8
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
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	// View at player position
	glm::vec2 pos = player->getPosition();

	projection = glm::ortho(pos.x - float(SCREEN_WIDTH) / 2, pos.x + float(SCREEN_WIDTH) / 2, pos.y + float(SCREEN_HEIGHT) / 2, pos.y - float(SCREEN_HEIGHT) / 2);
	
	currentTime = 0.0f;
}

void Scene::update(int deltaTime) {
	currentTime += deltaTime;
	player->update(deltaTime);
}

void Scene::render() {
	glm::mat4 modelview;

	texProgram.use();
	// Center the camera at player position
	glm::vec2 pos = player->getPosition();
	projection = glm::ortho(pos.x - float(SCREEN_WIDTH) / 2, pos.x + float(SCREEN_WIDTH) / 2, pos.y + float(SCREEN_HEIGHT) / 2, pos.y - float(SCREEN_HEIGHT) / 2);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
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



