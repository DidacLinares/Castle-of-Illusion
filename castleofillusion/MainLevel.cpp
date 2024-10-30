#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "MainLevel.h"
#include "Game.h"
#include "Entity.h"
#include "DragonBoss.h"
#include "illusionGem.h"
#include "BreakeableBlock.h"
#include "Armadillo.h"


// Mirar que fa aixo
#define SCREEN_X 32
#define SCREEN_Y 16

// S'ha de canviar al necesari segons el nivell
//#define INIT_PLAYER_X_TILES 4
//#define INIT_PLAYER_Y_TILES 25

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 8

#define REMOVE_AT 60

enum ids
{
	PLAYER, TREE, FLOWER, OBJECT
};

MainLevel::MainLevel() {
	map = nullptr;
	player = nullptr;
	dyingMusic = nullptr;
	music = nullptr;
	levelCompleteMusic = nullptr;
	starSprite = nullptr;
	numberSprite = nullptr;
	interfaceBackgroundSprite = nullptr;
	layer_0 = nullptr;
	TileMap* layer_1 = nullptr;
	TileMap* layer_2 = nullptr;
	soundEngine = nullptr;
	jumpSound = nullptr;
	boxBreaking = nullptr;
	levelMusic = nullptr;
	dead = nullptr;
	levelComplete = nullptr;
}

MainLevel::~MainLevel() {
	int size = entityArray.size();
	for (int i = 0; i < size; ++i) {
		if (entityArray[i] != nullptr) {
			delete entityArray[i];
		}
	}
	if (map != nullptr) delete map;
	if (player != nullptr) delete player;
	if (layer_0 != nullptr) delete layer_0;
	if (layer_1 != nullptr) delete layer_1;
	if (layer_2 != nullptr) delete layer_2;
	if (numberSprite != nullptr) delete numberSprite;
	if (interfaceBackgroundSprite != nullptr) delete interfaceBackgroundSprite;
	if (starSprite != nullptr) delete starSprite;
	if (music != nullptr) {
		music->stop();
		music->drop();
	}
	if (dyingMusic != nullptr) {
		dyingMusic->stop();
		dyingMusic->drop();
	}
	if (levelCompleteMusic != nullptr) {
		levelCompleteMusic->stop();
		levelCompleteMusic->drop();
	}
}


void MainLevel::init(irrklang::ISoundEngine* soundEngine, irrklang::ISoundSource* jumpSound, irrklang::ISoundSource* levelMusic, irrklang::ISoundSource* boxBreaking, irrklang::ISoundSource* dead,
	irrklang::ISoundSource* levelComplete, irrklang::ISoundSource* hit) {
	initShaders();
	this->soundEngine = soundEngine;
	this->jumpSound = jumpSound;
	this->boxBreaking = boxBreaking;
	this->levelComplete = levelComplete;
	this->dead = dead;
	this->hit = hit;

	//map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	layer_0 = TileMap::createTileMap("levels/main_level/sky_1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	layer_1 = TileMap::createTileMap("levels/main_level/sky_2.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	layer_2 = TileMap::createTileMap("levels/main_level/decoration.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map = TileMap::createTileMap("levels/main_level/map.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	player = new Player();
	player->setTileMap(map);
	player->setSoundEngineAndSounds(soundEngine, jumpSound, hit);
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y - 5), texProgram); // canviat per alinear la hitbox al numberSprite, reajustar si dona problemes
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setId(PLAYER);

	TreeEnemy* treeEnemy = new TreeEnemy(false);
	treeEnemy->setTileMap(map);
	treeEnemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	treeEnemy->setPlayer(player);
	treeEnemy->setPosition(glm::vec2(18 * map->getTileSize(), 9 * map->getTileSize()));
	treeEnemy->setTileMap(map);
	entityArray.push_back(treeEnemy);

	FlowerEnemy* flowerEnemy = new FlowerEnemy();
	flowerEnemy->setTileMap(map);
	flowerEnemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	flowerEnemy->setPlayer(player);
	flowerEnemy->setPosition(glm::vec2(36 * map->getTileSize(), 6 * map->getTileSize()));
	entityArray.push_back(flowerEnemy);

	Chest* chest = new Chest();
	chest->setTileMap(map);
	chest->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, soundEngine, boxBreaking);
	chest->setPlayer(player);
	chest->setPosition(glm::vec2(22 * map->getTileSize(), 13 * map->getTileSize()));
	chest->setObjectToSpawn(0); // Cake
	entityArray.push_back(chest);

	Armadillo* armadillo = new Armadillo();
	armadillo->setTileMap(map);
	armadillo->init(glm::ivec2(SCREEN_X, SCREEN_Y - 15), texProgram);
	armadillo->setPlayer(player);
	armadillo->setPosition(glm::vec2(31 * map->getTileSize(), 28 * map->getTileSize()));
	entityArray.push_back(armadillo);

	flowerEnemy = new FlowerEnemy();
	flowerEnemy->setTileMap(map);
	flowerEnemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	flowerEnemy->setPlayer(player);
	flowerEnemy->setPosition(glm::vec2(47 * map->getTileSize(), 11 * map->getTileSize()));
	entityArray.push_back(flowerEnemy);

	BreakeableBlock* breakeableBlock = new BreakeableBlock();
	breakeableBlock->setTileMap(map);
	breakeableBlock->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, soundEngine, boxBreaking);
	breakeableBlock->setPlayer(player);
	breakeableBlock->setPosition(glm::vec2(70 * map->getTileSize(), 13 * map->getTileSize()));
	entityArray.push_back(breakeableBlock);

	treeEnemy = new TreeEnemy(false);
	treeEnemy->setTileMap(map);
	treeEnemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	treeEnemy->setPlayer(player);
	treeEnemy->setPosition(glm::vec2(64 * map->getTileSize(), 11 * map->getTileSize()));
	treeEnemy->setTileMap(map);
	entityArray.push_back(treeEnemy);

	flowerEnemy = new FlowerEnemy();
	flowerEnemy->setTileMap(map);
	flowerEnemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	flowerEnemy->setPlayer(player);
	flowerEnemy->setPosition(glm::vec2(76 * map->getTileSize(), 28 * map->getTileSize()));
	entityArray.push_back(flowerEnemy);

	chest = new Chest();
	chest->setTileMap(map);
	chest->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, soundEngine, boxBreaking);
	chest->setPlayer(player);
	chest->setPosition(glm::vec2(90 * map->getTileSize(), 28 * map->getTileSize()));
	chest->setObjectToSpawn(1); // Coin
	entityArray.push_back(chest);

	flowerEnemy = new FlowerEnemy();
	flowerEnemy->setTileMap(map);
	flowerEnemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	flowerEnemy->setPlayer(player);
	flowerEnemy->setPosition(glm::vec2(48 * map->getTileSize(), 41 * map->getTileSize()));
	entityArray.push_back(flowerEnemy);

	Block* block = new Block();
	block->setTileMap(map);
	block->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	block->setPlayer(player);
	block->setPosition(glm::vec2(65 * map->getTileSize(), 41 * map->getTileSize()));
	entityArray.push_back(block);

	armadillo = new Armadillo();
	armadillo->setTileMap(map);
	armadillo->init(glm::ivec2(SCREEN_X, SCREEN_Y - 15), texProgram);
	armadillo->setPlayer(player);
	armadillo->setPosition(glm::vec2(79 * map->getTileSize(), 40 * map->getTileSize()));
	entityArray.push_back(armadillo);

	chest = new Chest();
	chest->setTileMap(map);
	chest->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, soundEngine, boxBreaking);
	chest->setPlayer(player);
	chest->setPosition(glm::vec2(2 * map->getTileSize(), 27 * map->getTileSize()));
	chest->setObjectToSpawn(1); // Coin
	entityArray.push_back(chest);

	chest = new Chest();
	chest->setTileMap(map);
	chest->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, soundEngine, boxBreaking);
	chest->setPlayer(player);
	chest->setPosition(glm::vec2(12 * map->getTileSize(), 29 * map->getTileSize()));
	chest->setObjectToSpawn(1); // Coin
	entityArray.push_back(chest);

	chest = new Chest();
	chest->setTileMap(map);
	chest->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, soundEngine, boxBreaking);
	chest->setPlayer(player);
	chest->setPosition(glm::vec2(14 * map->getTileSize(), 41 * map->getTileSize()));
	chest->setObjectToSpawn(0); // Coin
	entityArray.push_back(chest);

	breakeableBlock = new BreakeableBlock();
	breakeableBlock->setTileMap(map);
	breakeableBlock->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, soundEngine, boxBreaking);
	breakeableBlock->setPlayer(player);
	breakeableBlock->setPosition(glm::vec2(20 * map->getTileSize(), 59 * map->getTileSize()));
	entityArray.push_back(breakeableBlock);

	treeEnemy = new TreeEnemy(false);
	treeEnemy->setTileMap(map);
	treeEnemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	treeEnemy->setPlayer(player);
	treeEnemy->setPosition(glm::vec2(8 * map->getTileSize(), 57 * map->getTileSize()));
	treeEnemy->setTileMap(map);
	entityArray.push_back(treeEnemy);

	chest = new Chest();
	chest->setTileMap(map);
	chest->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, soundEngine, boxBreaking);
	chest->setPlayer(player);
	chest->setPosition(glm::vec2(70 * map->getTileSize(), 59 * map->getTileSize()));
	chest->setObjectToSpawn(0); // Cake
	entityArray.push_back(chest);

	chest = new Chest();
	chest->setTileMap(map);
	chest->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, soundEngine, boxBreaking);
	chest->setPlayer(player);
	chest->setPosition(glm::vec2(86 * map->getTileSize(), 59 * map->getTileSize()));
	chest->setObjectToSpawn(0); // Cake
	entityArray.push_back(chest);

	block = new Block();
	block->setTileMap(map);
	block->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	block->setPlayer(player);
	block->setPosition(glm::vec2(83 * map->getTileSize(), 59 * map->getTileSize()));
	entityArray.push_back(block);

	DragonBoss* dragonBoss = new DragonBoss();
	dragonBoss->setTileMap(map);
	dragonBoss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	dragonBoss->setPlayer(player);
	dragonBoss->setPosition(glm::vec2(84 * map->getTileSize(), 52 * map->getTileSize()));
	entityArray.push_back(dragonBoss);

	this->levelMusic = levelMusic;
	music = soundEngine->play2D(levelMusic, true, true, true);
	music->setVolume(0.1f);
	if (music->getIsPaused()) music->setIsPaused(false);

	initInterface();
	// View at player position
	glm::vec2 pos = player->getPosition();

	projection = glm::ortho(pos.x - float(SCREEN_WIDTH) / 2, pos.x + float(SCREEN_WIDTH) / 2, pos.y + float(SCREEN_HEIGHT) / 2, pos.y - float(SCREEN_HEIGHT) / 2);
	//glViewport(0, 300, 1280, 720 - 300);  // UI

	currentTime = 0.0f;
}

void MainLevel::initInterface() {
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

void MainLevel::update(int deltaTime) {
	if (endLevel) {
		glViewport(0, 0, 1280, 720);

		if (player->isDead()) {
			Game::instance().removeTry();
			Game::instance().changeScene(Game::GAME_OVER);
			return;
		}
		if (counter <= 0) {
			music->stop();
			levelCompleteMusic = soundEngine->play2D(levelComplete, false, true, true);
			levelCompleteMusic->setVolume(0.1f);
			if (levelCompleteMusic->getIsPaused()) levelCompleteMusic->setIsPaused(false);
			player->changeAnim(0);
		}
		++counter;
		if (counter >= 350) {
			Game::instance().changeScene(Game::MAIN_MENU);
			return;
		}
	}
	else {

		currentTime += deltaTime;
		player->update(deltaTime);
		if (player->getPosition().y >= 970) player->setDying(true);
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

		if (nextRemove++ >= REMOVE_AT) {
			nextRemove = 0;
			if (time > 0) --time;

			// Remove null pointers to avoid memory leaks
			entityArray.erase(std::remove(entityArray.begin(), entityArray.end(), nullptr), entityArray.end());
		}
	}

}

void MainLevel::render() {
	glm::mat4 modelview;
	glViewport(0, 161, 1280, 720 - 161);
	texProgram.use();
	// Center the camera at player position
	glm::vec2 pos = player->getPosition();
	int zoom = 8;
	float x = pos.x;
	float y = pos.y;
	if (pos.x <= 194) x = 194;
	else if (pos.x >= 1350) x = 1350;
	if (pos.y <= 85) y = 86;
	else if (pos.y >= 928) y = 928;
	projection = glm::ortho(x - float(SCREEN_WIDTH) / zoom, x + float(SCREEN_WIDTH) / zoom, y + float(SCREEN_HEIGHT) / zoom, y - float(SCREEN_HEIGHT - 161) / zoom);
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
	glViewport(0, 0, 1280, 720);
}

void MainLevel::renderInterface() {
	glViewport(0, 0, 1280, 161);
	texProgram.use();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(161), 0.f);
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
	int tries = Game::instance().getTries();
	for (int i = 0; i < 2; ++i) {
		numberSprite->setPosition(glm::vec2(680 + i * 35, 100));
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

}

void MainLevel::initShaders() {
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

void MainLevel::addEntity(NonPlayerEntity* entity) {
	entityArray.push_back(entity);
}

std::vector<NonPlayerEntity*>& MainLevel::getEnemies() {
	return entityArray;
}

void MainLevel::initNumbers() {
	spritesheetNumbers.loadFromFile("images/font.png", TEXTURE_PIXEL_FORMAT_RGBA);
	numberSprite = Sprite::createSprite(glm::ivec2(30, 42), glm::vec2(0.1f, 1.f), &spritesheetNumbers, &texProgram);
	numberSprite->setNumberAnimations(20);

	for (int i = 0; i < 10; ++i) {
		numberMaping.push_back(2 * i + 1);
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

void MainLevel::initDummies() {

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

void MainLevel::changeMusicToDying() {
	music->stop();
	dyingMusic = soundEngine->play2D(dead, false, true, true);
	dyingMusic->setVolume(0.1f);
	if (dyingMusic->getIsPaused()) dyingMusic->setIsPaused(false);
}