#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "MainScene.h"
#include "Game.h"


// Mirar que fa aixo
#define SCREEN_X 32
#define SCREEN_Y 16

// S'ha de canviar al necesari segons el nivell
//#define INIT_PLAYER_X_TILES 4
//#define INIT_PLAYER_Y_TILES 25



MainScene::MainScene() {
	menu = NULL;
}

MainScene::~MainScene() {
	if (menu != NULL)
		delete menu;
}


void MainScene::init() {
	initShaders();
	soundEngine = irrklang::createIrrKlangDevice();

	menu = TileMap::createTileMap("levels/main_scene/main_scene.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
}

void MainScene::update(int deltaTime) {

}

void MainScene::render() {
	glm::mat4 modelview;

	texProgram.use();

	// Ajustamos la proyección al tamaño completo del viewport
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.0f);

	// Tamaño de la portada, ajustar según el tamaño real de tu imagen
	float imageWidth = 248.0f; // Ajusta según el ancho real de la imagen
	float imageHeight = 192.0f; // Ajusta según el alto real de la imagen
	

	// Calculamos la traslación para centrar la imagen en el viewport
	float xOffset = (SCREEN_WIDTH - imageWidth) / 2.0f;
	float yOffset = (SCREEN_HEIGHT - imageHeight) / 2.0f;

	// Aplicamos la traslación
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(xOffset, yOffset, 0.0f));
	modelview = glm::scale(modelview, glm::vec3(2, 2, 1.0f));
	modelview = glm::translate(modelview, glm::vec3(0.0f, 0.0f, 0.0f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	

	menu->render();
}

void MainScene::initShaders() {
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
