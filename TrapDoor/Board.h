#ifndef BOARD_H
#define BOARD_H

//#define DEBUG_ON TRUE

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include <SFML\Window.hpp>

#include "Thing.h"
#include "Tile.h"
#include "TileTypes.h"
#include "Shader.h"

class Board
{
public:
	Tile** tiles;
	Thing* player;

	Board(__int16 width, __int16 height);
	virtual ~Board();
	
	int board_setup_opengl();
	void board_handle_input();
	
	//
	// Do transformations for all Things, handle collisions etc
	//
	void board_update();

	//
	// Draw all vbos
	//
	void board_draw();
	
private:
	GLint uniTex;					
	GLint uniTrans;
	GLfloat* bgTileVerts;

	GLuint textures[5];
	GLuint shaderPrograms[2];
	GLuint bg_fs;				// Background fragment shader name
	GLuint bg_vs;				// Background vertex shader name
	GLuint bg_vao;				// Background vertex array buffer
	GLuint bg_vbo;				// Background vertex buffer object
	GLuint player_vao;			// Player sprite vertex array buffer
	GLuint player_vbo;			// Player sprite vertex buffer object

	void board_setup_vertex_arrays(GLuint shaderProgram);
	void board_load_textures();
};
#endif