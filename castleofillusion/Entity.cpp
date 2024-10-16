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

void Entity::setLives(int lives)
{
	this->lives = lives;
}

int Entity::getLives()
{
	return lives;
}

bool Entity::isInvulnerable()
{
	return invulnerable;
}

void Entity::setInvulnerable(bool invulnerable)
{
	this->invulnerable = invulnerable;
	this->invulnerableTimeLeft = invulnerableDuration;
}

bool Entity::isDead()
{
	return dead;
}

void Entity::setDead(bool dead)
{
	this->dead = dead;
}

void Entity::setPlayer(Entity* player) {

}


