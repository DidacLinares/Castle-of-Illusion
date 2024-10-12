#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Entity.h"
#include "Game.h"


void Entity::render() {
	sprite->render();
}

void Entity::setTileMap(TileMap* tileMap) {
	map = tileMap;
}

void Entity::setPosition(const glm::vec2& pos) {
	this->pos = pos;
	sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
}

glm::vec2 Entity::getPosition()
{
	return pos;
}

bool Entity::isDead() {
	return dead;
}

