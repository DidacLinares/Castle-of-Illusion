#include "Chest.h"
#include "Game.h"
#include "Cake.h"
#include "Coin.h"

#define OFFSET_X 0.125
#define OFFSET_Y 0.5

void Chest::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	dead = false;
	spritesheet.loadFromFile("images/blocks.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(OFFSET_X, OFFSET_Y), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(1, 1);
	sprite->addKeyframe(1, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(1);
	tileMapDispl = glm::vec2(tileMapPos);
	hitbox_x = 16;
	hitbox_y = 16;
	tileSize = map->getTileSize();
	tileX = pos.x / tileSize;
	tileY = pos.y / tileSize;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Chest::update(int deltatime) {
	Block::update(deltatime);

	if (player->checkCollision(glm::vec4(pos.x,pos.y - 4,hitbox_x,hitbox_y)) && player->isPlayerGroundPounding()) {
		tileX = pos.x / tileSize;
		tileY = pos.y / tileSize;
		map->setTileAsBlock(tileX, tileY, 0);
		onEntityHit();
	}
}

void Chest::onEntityHit(bool isPlayer) {
	dead = true;
	player->addScore(10);
	Cake* cake;
	Coin* coin;
	switch (objectToSpawn) {
	case 0:
		cake = new Cake();
		cake->init(glm::ivec2(tileMapDispl), Game::instance().getScene()->getShaderProgram());
		cake->setTileMap(map);
		Game::instance().getScene()->addEntity(cake);
		cake->setPlayer(player);
		cake->setPosition(pos);
		break;
	case 1:
		coin = new Coin();
		coin->init(glm::ivec2(tileMapDispl), Game::instance().getScene()->getShaderProgram());
		coin->setTileMap(map);
		Game::instance().getScene()->addEntity(coin);
		coin->setPlayer(player);
		coin->setPosition(pos);
	default:
		break;
	}

}

void Chest::setObjectToSpawn(int id) {
	objectToSpawn = id;
}

