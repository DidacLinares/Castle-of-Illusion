#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Entity.h"
#include "DragonBoss.h"
#include "illusionGem.h"


// Mirar que fa aixo
#define SCREEN_X 32
#define SCREEN_Y 16

// S'ha de canviar al necesari segons el nivell
//#define INIT_PLAYER_X_TILES 4
//#define INIT_PLAYER_Y_TILES 25

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 8


#define INIT_ENEMY_X_TILES 14
#define INIT_ENEMY_Y_TILES 8

#define REMOVE_AT 60

enum ids
{
	PLAYER,TREE,FLOWER,OBJECT
};

Scene::Scene() {
	map = NULL;
	player = NULL;
}

Scene::~Scene() {
	int size = entityArray.size();
	for (int i = 0; i < size; ++i) {
		if (entityArray[i] != nullptr) {
			delete entityArray[i];
		}
	}
	if(map != NULL) delete map;
	if(player != NULL) delete player;
	if (layer_0 != nullptr) delete layer_0;
	if (layer_1 != nullptr) delete layer_1;
	if (layer_2 != nullptr) delete layer_2;
	if(numberSprite != nullptr) delete numberSprite;
	if (interfaceBackgroundSprite != nullptr) delete interfaceBackgroundSprite;
	if (starSprite != nullptr) delete starSprite;
}


void Scene::init(irrklang::ISoundEngine* soundEngine,irrklang::ISoundSource* jumpSound) {
	initShaders();
	this->soundEngine = soundEngine;
	this->jumpSound = jumpSound;
	//map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	layer_0 = TileMap::createTileMap("levels/level01/sky_background_0.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	layer_1 = TileMap::createTileMap("levels/level01/sky_background_1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	layer_2 = TileMap::createTileMap("levels/level01/trees.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map = TileMap::createTileMap("levels/level01/map.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	player = new Player();
	player->setSoundEngineAndSounds(soundEngine,jumpSound);
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y - 5), texProgram); // canviat per alinear la hitbox al numberSprite, reajustar si dona problemes
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	player->setId(PLAYER);

	entityArray.push_back(new TreeEnemy());
	entityArray[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	entityArray[0]->setPlayer(player);
	entityArray[0]->setPosition(glm::vec2(INIT_ENEMY_X_TILES * map->getTileSize(), INIT_ENEMY_Y_TILES * map->getTileSize()));
	entityArray[0]->setTileMap(map);
	entityArray[0]->setId(TREE);

	entityArray.push_back(new Block());
	entityArray[1]->setTileMap(map);
	entityArray[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	entityArray[1]->setPlayer(player);
	entityArray[1]->setPosition(glm::vec2((10) * map->getTileSize(), (12) * map->getTileSize()));
	entityArray[1]->setId(OBJECT);

	Chest* chest = new Chest();
	chest->setObjectToSpawn(0);
	entityArray.push_back(chest);
	entityArray[2]->setTileMap(map);
	entityArray[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	entityArray[2]->setPlayer(player);
	entityArray[2]->setPosition(glm::vec2((32) * map->getTileSize(), (8) * map->getTileSize()));
	entityArray[2]->setId(OBJECT);

	chest = new Chest();
	chest->setObjectToSpawn(1);
	entityArray.push_back(chest);
	entityArray[3]->setTileMap(map);
	entityArray[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	entityArray[3]->setPlayer(player);
	entityArray[3]->setPosition(glm::vec2((75) * map->getTileSize(), (13) * map->getTileSize()));
	entityArray[3]->setId(OBJECT);

	entityArray.push_back(new FlowerEnemy());
	entityArray[4]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	entityArray[4]->setPlayer(player);
	entityArray[4]->setPosition(glm::vec2((66) * map->getTileSize(), (INIT_ENEMY_Y_TILES + 1) * map->getTileSize()));
	entityArray[4]->setTileMap(map);
	entityArray[4]->setId(FLOWER);

	entityArray.push_back(new Block());
	entityArray[5]->setTileMap(map);
	entityArray[5]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	entityArray[5]->setPlayer(player);
	entityArray[5]->setPosition(glm::vec2((49) * map->getTileSize(), (13) * map->getTileSize()));
	entityArray[5]->setId(OBJECT);

	entityArray.push_back(new IllusionGem());
	entityArray[6]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	entityArray[6]->setPlayer(player);
	entityArray[6]->setPosition(glm::vec2((91) * map->getTileSize(), (5) * map->getTileSize()));
	entityArray[6]->setTileMap(map);
	entityArray[6]->setId(OBJECT);


	initInterface();
	// View at player position
	glm::vec2 pos = player->getPosition();

	projection = glm::ortho(pos.x - float(SCREEN_WIDTH) / 2, pos.x + float(SCREEN_WIDTH) / 2, pos.y + float(SCREEN_HEIGHT) / 2, pos.y - float(SCREEN_HEIGHT) / 2);
	//glViewport(0, 300, 1280, 720 - 300);  // UI

	currentTime = 0.0f;
}

void Scene::initInterface() {
	spritesheetinterfaceBackground.loadFromFile("images/interface.png", TEXTURE_PIXEL_FORMAT_RGBA);
	interfaceBackgroundSprite = Sprite::createSprite(glm::ivec2(1280, 161), glm::vec2(1, 1), &spritesheetinterfaceBackground, &texProgram);
	interfaceBackgroundSprite->setNumberAnimations(2);

	interfaceBackgroundSprite->setAnimationSpeed(0, 1);
	interfaceBackgroundSprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	interfaceBackgroundSprite->setAnimationSpeed(1, 1);
	interfaceBackgroundSprite->addKeyframe(1, glm::vec2(0.f, 0.f));

	interfaceBackgroundSprite->changeAnimation(1);
	interfaceBackgroundSprite->setPosition(glm::vec2(0.f, 0.f));

	spritesheetStar.loadFromFile("images/star.png", TEXTURE_PIXEL_FORMAT_RGBA);
	starSprite = Sprite::createSprite(glm::ivec2(85, 79), glm::vec2(0.5f, 1), &spritesheetStar, &texProgram);
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

}

void Scene::update(int deltaTime) {
	if (endLevel) {
		glViewport(0, 0, 1280, 720);
		Game::instance().changeScene(Game::MAIN_MENU);
		return;
	}

	currentTime += deltaTime;
	player->update(deltaTime);
	bool treeAlive = false;
	bool flowerAlive = false;
	int size = entityArray.size();
	for (int i = 0; i < size; ++i) {
		if (entityArray[i] != nullptr) {
			entityArray[i]->update(deltaTime);
			if (entityArray[i]->getId() == TREE) treeAlive = true;
			else if (entityArray[i]->getId() == FLOWER) flowerAlive = true;
			if (entityArray[i]->isDead()) {
				delete entityArray[i];
				entityArray[i] = nullptr;
			}
		}
	}

	if (!treeAlive) {
		glm::vec2 playerPos = player->getPosition();
		if (std::abs(playerPos.x - (INIT_ENEMY_X_TILES * 16) > (224))) {
			TreeEnemy* tree = new TreeEnemy();
			tree->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			tree->setPlayer(player);
			tree->setPosition(glm::vec2(INIT_ENEMY_X_TILES * map->getTileSize(), INIT_ENEMY_Y_TILES * map->getTileSize()));
			tree->setTileMap(map);
			tree->setId(TREE);
			entityArray.push_back(tree);
		}
	}

	if (nextRemove++ >= REMOVE_AT) {
		nextRemove = 0;
		if(time > 0) --time;

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
	float x = pos.x;
	float y = pos.y;
	if (pos.x <= 194) x = 194;
	else if (pos.x >= 1350) x = 1350;
	if (pos.y <= 80) y = 81;
	else if (pos.y >= 200) y = 199;
	projection = glm::ortho(x - float(SCREEN_WIDTH) / zoom, x + float(SCREEN_WIDTH) / zoom, y + float(SCREEN_HEIGHT) / zoom, y - float(SCREEN_HEIGHT - 200) / zoom);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	layer_0->render();
	layer_1->render();
	layer_2->render();
	map->render();

	int size = entityArray.size();
	for (int i = 0; i < size; ++i) {
		if (entityArray[i] != nullptr) {
			entityArray[i]->render();
		}
	}
	
	player->render();
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
		starSprite->setPosition(glm::vec2(i * 95 + 60, 49));
		starSprite->render();
	}
	//TriesS
	int tries = player->getTries();
	for (int i = 0; i < 2;++i) {
		numberSprite->setPosition(glm::vec2(680+ i * 35 , 100));
		int n;
		if (i == 1) n = tries % 10;
		else n = tries / 10;
		numberSprite->changeAnimation(numberMaping[n]);
		numberSprite->render();
	}
	//time
	for (int i = 0; i < 3; ++i) {
		numberSprite->setPosition(glm::vec2(1160 + i * 35, 100));
		int n;
		if (i == 2) n = time % 10;
		else if (i == 1) n = (time / 10) % 10;
		else n = time / 100;
		numberSprite->changeAnimation(numberMaping[n]);
		numberSprite->render();
	}
	int score = player->getScore();
	for (int i = 0; i < 6; ++i) {
		numberSprite->setPosition(glm::vec2(840 + i * 35, 100));
		int n;
		switch (i) {
		case 5:
			n = score % 10;
			break;
		case 4:
			n = (score / 10) % 10;
			break;
		case 3:
			n = (score / 100) % 10;
			break;
		case 2:
			n = (score / 1000) % 10;
			break;
		case 1:
			n = (score / 10000) % 10;
			break;
		default:
			n = (score / 100000) % 10;
			break;
		}

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
	numberSprite = Sprite::createSprite(glm::ivec2(30, 42), glm::vec2(0.1f, 1.f), &spritesheetNumbers, &texProgram);
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