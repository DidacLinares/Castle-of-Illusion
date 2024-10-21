#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program) {
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program) {
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap() {
	if(map != NULL)
		delete map;
}


void TileMap::render() const {
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free() {
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile) {
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if (!fin.is_open()) return false;
	
	getline(fin, line);
	
	if (line.compare(0, 7, "TILEMAP") != 0) return false;
	
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];

	//Reimplemented per a llegir ids i no chars individuals
	for (int j = 0; j < mapSize.y; j++) {
		getline(fin, line);
		sstream.clear();
		sstream.str(line);

		for (int i = 0; i < mapSize.x; i++) {
			int tileId;
			sstream >> tileId;
			/*if (!(sstream >> tileId)) {
				std::cerr << "Error: Unable to read tile ID at (" << j << ", " << i << ")" << std::endl;
				exit(EXIT_FAILURE); 
			}*/

			map[j * mapSize.x + i] = tileId;
		}
	}

	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program) {
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j=0; j<mapSize.y; j++) {
		for (int i=0; i<mapSize.x; i++) {
			tile = map[j * mapSize.x + i];
			if (tile != 0) {
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(glm::vec2& pos, const glm::vec2& size) const
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++) {
		int tile = map[y * mapSize.x + x];
		if (tile != 0) {
			pos.x = (x * tileSize) + tileSize;
			return true;
		}
	}

	return false;
}

bool TileMap::collisionMoveRight(glm::vec2& pos, const glm::vec2& size) const {
	int x, y0, y1;

	x = (pos.x + size.x) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++) {
		int tile = map[y * mapSize.x + x];
		if (tile != 0) {
			pos.x = x * tileSize - size.x;
			return true;
		}
	}

	return false;
}
//&& tile != 7 && tile != 31 && tile != 8 && tile != 10 && tile != 11 && tile != 37 ramp
bool TileMap::collisionMoveDown(const glm::vec2& pos, const glm::vec2& size, float* posY) const {
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++) {
		int tile = map[y * mapSize.x + x] != 0;
		if (tile != 0) {
			if(*posY - tileSize * y + size.y <= 5) {
					*posY = tileSize * y - size.y;
					return true;
			}
		}
	}
	return false;
}

/*Rampa: Detectar a MoveLeft/right tiles de tipus rampa, i tambe fer que per cada pixel 
que es mou esquerra o dreta, puja o baixa un pixel. Acabar sprites mickey i fer Block*/
void TileMap::raycastDown(const glm::vec2& pos, const glm::vec2& size, std::vector<bool>& collisions) const {
	
	glm::vec2 leftBottomRayStart = glm::vec2(pos.x, pos.y + size.y - 1);
	int leftY = (leftBottomRayStart.y + 1) / tileSize; 
	int leftX = leftBottomRayStart.x / tileSize;

	glm::vec2 middleBottomRayStart = glm::vec2(pos.x + size.x / 2, pos.y + size.y - 1);
	int middleY = (middleBottomRayStart.y + 1) / tileSize; 
	int middleX = middleBottomRayStart.x / tileSize;

	glm::vec2 rightBottomRayStart = glm::vec2(pos.x + size.x - 1, pos.y + size.y - 1);
	int rightY = (rightBottomRayStart.y + 1) / tileSize; 
	int rightX = rightBottomRayStart.x / tileSize;

	if (map[leftY * mapSize.x + leftX] != 0) {
		collisions[0] = true; 
	}
	if (map[middleY * mapSize.x + middleX] != 0) {
		collisions[1] = true; 
	}
	if (map[rightY * mapSize.x + rightX] != 0) {
		collisions[2] = true; 
	}
}

bool TileMap::setTileAsBlock(int tileX, int tileY,int value) {
	if (tileX >= 0 && tileX < mapSize.x && tileY >= 0 && tileY < mapSize.y && (map[tileY * mapSize.x + tileX] == 0 || map[tileY * mapSize.x + tileX] == 1000)) {
		map[tileY * mapSize.x + tileX] = value;
		return true;
	}
	return false;
}

bool TileMap::isTileValidAsBlock(int tileX, int tileY) {
	if (tileX >= 0 && tileX < mapSize.x && tileY >= 0 && tileY < mapSize.y && (map[tileY * mapSize.x + tileX] == 0 || map[tileY * mapSize.x + tileX] == 1000)) {
		return true;
	}
	return false;
}


















