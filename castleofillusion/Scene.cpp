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

#define REMOVE_AT 60

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
	soundEngine = irrklang::createIrrKlangDevice();
	//map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	layer_0 = TileMap::createTileMap("levels/sky_background_0.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	layer_1 = TileMap::createTileMap("levels/sky_background_1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	layer_2 = TileMap::createTileMap("levels/trees.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map = TileMap::createTileMap("levels/map.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	player = new Player();
	player->setSoundEngine(soundEngine);
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y - 5), texProgram); // canviat per alinear la hitbox al numberSprite, reajustar si dona problemes
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
	entityArray[2]->setTileMap(map);
	entityArray[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	entityArray[2]->setPlayer(player);
	entityArray[2]->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 1) * map->getTileSize(), (13) * map->getTileSize()));

	entityArray.push_back(new FlowerEnemy());
	entityArray[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	entityArray[3]->setPlayer(player);
	entityArray[3]->setPosition(glm::vec2((INIT_ENEMY_X_TILES + 1) * map->getTileSize(), (INIT_ENEMY_Y_TILES + 1) * map->getTileSize()));
	entityArray[3]->setTileMap(map);

	spritesheetinterfaceBackground.loadFromFile("images/interface.png", TEXTURE_PIXEL_FORMAT_RGBA);
	interfaceBackgroundSprite = Sprite::createSprite(glm::ivec2(1528, 161), glm::vec2(1, 1), &spritesheetinterfaceBackground, &texProgram);
	interfaceBackgroundSprite->setNumberAnimations(2);

	interfaceBackgroundSprite->setAnimationSpeed(0, 1);
	interfaceBackgroundSprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	interfaceBackgroundSprite->setAnimationSpeed(1, 1);
	interfaceBackgroundSprite->addKeyframe(1, glm::vec2(0.f, 0.f));

	interfaceBackgroundSprite->changeAnimation(1);
	interfaceBackgroundSprite->setPosition(glm::vec2(0.f, 0.f));

	spritesheetStar.loadFromFile("images/star.png", TEXTURE_PIXEL_FORMAT_RGBA);
	starSprite = Sprite::createSprite(glm::ivec2(91, 79), glm::vec2(0.5f, 1), &spritesheetStar, &texProgram);
	starSprite->setNumberAnimations(4);

	starSprite->setAnimationSpeed(0, 1);
	starSprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	starSprite->setAnimationSpeed(1, 1);
	starSprite->addKeyframe(1, glm::vec2(0.f, 0.f));

	starSprite->setAnimationSpeed(2, 1);
	starSprite->addKeyframe(2, glm::vec2(0.f, 0.f));

	starSprite->setAnimationSpeed(3, 1);
	starSprite->addKeyframe(3, glm::vec2(0.5f, 0.f));
	
	starSprite->changeAnimation(1);
	initNumbers();

	// View at player position
	glm::vec2 pos = player->getPosition();

	projection = glm::ortho(pos.x - float(SCREEN_WIDTH) / 2, pos.x + float(SCREEN_WIDTH) / 2, pos.y + float(SCREEN_HEIGHT) / 2, pos.y - float(SCREEN_HEIGHT) / 2);
	//glViewport(0, 300, 1280, 720 - 300);  // UI

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

	if (nextRemove++ >= REMOVE_AT) {
		nextRemove = 0;

		// Remove null pointers to avoid memory leaks
		entityArray.erase(std::remove(entityArray.begin(), entityArray.end(), nullptr), entityArray.end());
	}
}

void Scene::render() {
	glm::mat4 modelview;

	texProgram.use();
	// Center the camera at player position
	glm::vec2 pos = player->getPosition();
	int zoom = 8;
	projection = glm::ortho(pos.x - float(SCREEN_WIDTH) / zoom, pos.x + float(SCREEN_WIDTH) / zoom, pos.y + float(SCREEN_HEIGHT) / zoom, pos.y - float(SCREEN_HEIGHT - 200) / zoom);
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

	renderInterface();
}

void Scene::renderInterface() {
	glViewport(0, 0, 1280, 200);
	texProgram.use();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(200), 0.f);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	glm::mat4 modelview;
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	interfaceBackgroundSprite->render();
	int lives = player->getLives();
	//Estrelles
	for (int i = 0; i < 3; ++i) {
		if ((i + 1) - lives > 0) starSprite->changeAnimation(3);
		else starSprite->changeAnimation(1);
		starSprite->setPosition(glm::vec2(i * 103 + 66, 49));
		starSprite->render();
	}
	//TriesS
	int tries = 98;//player->getTries();
	for (int i = 0; i < 2;++i) {
		numberSprite->setPosition(glm::vec2(730+ i*65 , 65));
		int n;
		if (i == 1) n = tries % 10;
		else n = tries / 10;
		numberSprite->changeAnimation(numberMaping[n]);
		numberSprite->render();
	}

	glViewport(0, 200, 1280, 720 - 200);
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

void Scene::addEntity(NonPlayerEntity* entity) {
	entityArray.push_back(entity);
}

std::vector<NonPlayerEntity*>& Scene::getEnemies() {
	return entityArray;
}

void Scene::initNumbers() {
	spritesheetNumbers.loadFromFile("images/font.png", TEXTURE_PIXEL_FORMAT_RGBA);
	numberSprite = Sprite::createSprite(glm::ivec2(59, 85), glm::vec2(0.1f, 1.f), &spritesheetNumbers, &texProgram);
	numberSprite->setNumberAnimations(20);

	for (int i = 0; i < 10; ++i) {
		numberMaping.push_back(2* i + 1);
	}
	
	numberSprite->setAnimationSpeed(1, 1);
	numberSprite->addKeyframe(1, glm::vec2(0.f, 0.f));
	numberSprite->setAnimationSpeed(3, 1);
	numberSprite->addKeyframe(3, glm::vec2(0.1f, 0.f));
	numberSprite->setAnimationSpeed(5, 1);
	numberSprite->addKeyframe(5, glm::vec2(0.2f, 0.f));
	numberSprite->setAnimationSpeed(7, 1);
	numberSprite->addKeyframe(7, glm::vec2(0.3f, 0.f));
	numberSprite->setAnimationSpeed(9, 1);
	numberSprite->addKeyframe(9, glm::vec2(0.4f, 0.f));
	numberSprite->setAnimationSpeed(11, 1);
	numberSprite->addKeyframe(11, glm::vec2(0.5f, 0.f));
	numberSprite->setAnimationSpeed(13, 1);
	numberSprite->addKeyframe(13, glm::vec2(0.6f, 0.f));
	numberSprite->setAnimationSpeed(15, 1);
	numberSprite->addKeyframe(15, glm::vec2(0.7f, 0.f));
	numberSprite->setAnimationSpeed(17, 1);
	numberSprite->addKeyframe(17, glm::vec2(0.8f, 0.f));
	numberSprite->setAnimationSpeed(19, 1);
	numberSprite->addKeyframe(19, glm::vec2(0.9f, 0.f));

	initDummies();
	numberSprite->changeAnimation(numberMaping[0]);
	numberSprite->setPosition(glm::vec2(0, 0));
}

void Scene::initDummies() {

	numberSprite->setAnimationSpeed(0, 1);
	numberSprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	numberSprite->setAnimationSpeed(2, 1);
	numberSprite->addKeyframe(2, glm::vec2(0.f, 0.f));
	numberSprite->setAnimationSpeed(4, 1);
	numberSprite->addKeyframe(4, glm::vec2(0.f, 0.f));
	numberSprite->setAnimationSpeed(6, 1);
	numberSprite->addKeyframe(6, glm::vec2(0.f, 0.f));
	numberSprite->setAnimationSpeed(8, 1);
	numberSprite->addKeyframe(8, glm::vec2(0.f, 0.f));
	numberSprite->setAnimationSpeed(10, 1);
	numberSprite->addKeyframe(10, glm::vec2(0.f, 0.f));
	numberSprite->setAnimationSpeed(12, 1);
	numberSprite->addKeyframe(12, glm::vec2(0.f, 0.f));
	numberSprite->setAnimationSpeed(14, 1);
	numberSprite->addKeyframe(14, glm::vec2(0.f, 0.f));
	numberSprite->setAnimationSpeed(16, 1);
	numberSprite->addKeyframe(16, glm::vec2(0.f, 0.f));
	numberSprite->setAnimationSpeed(18, 1);
	numberSprite->addKeyframe(18, glm::vec2(0.f, 0.f));


}