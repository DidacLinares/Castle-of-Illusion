#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Entity.h"
#include "Game.h"


Entity::~Entity()
{
	if (sprite != nullptr) delete sprite;
}

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

glm::vec4 Entity::getCollisionBox() {
	return glm::vec4(pos.x, pos.y, hitbox_x, hitbox_y);
}

void Entity::setId(int id) {
	this->id = id;
}

int Entity::getId() {
	return id;
}


