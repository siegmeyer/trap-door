#include "Board.h"

// Shader sources
const GLchar* bg_vs_source =
"#version 150 core\n"
"in vec2 position;"
"in vec3 color;"
"in vec2 texcoord;"
"out vec3 Color;"
"out vec2 Texcoord;"
"uniform int uniTex;"
"uniform mat4 uniTrans;"
"void main() {"
"   Color = color;"
"   Texcoord = texcoord;"
"	if(uniTex != 4){"
"		gl_Position = uniTrans * vec4(position, 0.0, 1.0);"
"	} else {"
"		gl_Position = vec4(position, 0.0, 1.0);"
"	}"
"}";
const GLchar* bg_fs_source =
"#version 150 core\n"
"in vec3 Color;"
"in vec2 Texcoord;"
"out vec4 outColor;"
"uniform int uniTex;"
"uniform sampler2D texStone;"
"uniform sampler2D texMortyFront;"
"uniform sampler2D texMortyBack;"
"uniform sampler2D texMortyRight;"
"uniform sampler2D texMortyLeft;"
"void main() {"
"	switch(uniTex){"
"	case 0:"
"		outColor = texture(texMortyBack, Texcoord);"
"		break;"
"	case 1:"
"		outColor = texture(texMortyRight, Texcoord);"
"		break;"
"	case 2:"
"		outColor = texture(texMortyFront, Texcoord);"
"		break;"
"	case 3:"
"		outColor = texture(texMortyLeft, Texcoord);"
"		break;"
"	case 4:"
"		outColor = texture(texStone, Texcoord);"
"		break;"
"	}"
"}";

//Temporary
int numTiles;
const int attributeLength = 16; // Four corners and 2 texture coordinates
GLfloat startingX = 0.0f;
GLfloat startingY = 0.0f;

GLfloat inc = 0.1f;

Board::Board(__int16 tilesAcross, __int16 tilesDown)
{
	numTiles = tilesAcross * tilesDown;

	startingX = -1.0f - (tilesAcross / 2.0f - 10.0f) / 10.0f;
	startingY = 1.0f - (tilesDown / 2.0f - 10.0f) / -10.0f;

	std::cout << "Starting coordinates: " << startingX << ", " << startingY << std::endl;
	std::cout << "Total tiles to draw: " << numTiles << std::endl;

	// Create the columns
	tiles = new Tile*[tilesDown];
	for (int k = 0; k < tilesDown; k++){
		// Create the rows
		tiles[k] = new Tile[tilesAcross];
	}

	bgTileVerts = new GLfloat[numTiles * attributeLength];

	int vCount = 0; // walks along array

	float offX = 0.0f; // offsetX
	float offY = 0.0f; // offsetY

	for (int y = 0; y < tilesDown; ++y){
		offY = y * 0.1f;
		for (int x = 0; x < tilesAcross; ++x){
			offX = x * 0.1f;														//std::cout << "x = " << x << " y = " << y << std::endl;
			bgTileVerts[vCount++] = tiles[y][x].tlX = startingX + offX;				//std::cout << bgTileVerts[vCount - 1] << std::endl;
			bgTileVerts[vCount++] = tiles[y][x].tlY = startingY - offY;				//std::cout << bgTileVerts[vCount - 1] << std::endl;
			bgTileVerts[vCount++] = 0.0f;// tl x texcoord							
			bgTileVerts[vCount++] = 0.0f;// tl y texcoord	
			bgTileVerts[vCount++] = tiles[y][x].trX = startingX + offX + inc;		//std::cout << bgTileVerts[vCount - 1] << std::endl;
			bgTileVerts[vCount++] = tiles[y][x].trY = startingY - offY;				//std::cout << bgTileVerts[vCount - 1] << std::endl;
			bgTileVerts[vCount++] = 1.0f; // tr x texcoord
			bgTileVerts[vCount++] = 0.0f; // tr y texcoord	
			bgTileVerts[vCount++] = tiles[y][x].brX = startingX + offX + inc;		//std::cout << bgTileVerts[vCount - 1] << std::endl;
			bgTileVerts[vCount++] = tiles[y][x].brY = startingY - offY - inc;		//std::cout << bgTileVerts[vCount - 1] << std::endl;
			bgTileVerts[vCount++] = 1.0f; // br x texcoord
			bgTileVerts[vCount++] = 1.0f; // br y texcoord	
			bgTileVerts[vCount++] = tiles[y][x].blX = startingX + offX;				//std::cout << bgTileVerts[vCount - 1] << std::endl;
			bgTileVerts[vCount++] = tiles[y][x].blY = startingY - offY - inc;		//std::cout << bgTileVerts[vCount - 1] << std::endl;
			bgTileVerts[vCount++] = 0.0f; // bl x texcoord
			bgTileVerts[vCount++] = 1.0f; // bl y texcoord
		}
	}//std::cout << "map size: " << sizeof(bgTileVerts)* attributeLength * numTiles << std::endl;

	player = new Thing(startingY, inc);
}

int Board::board_setup_opengl(){
	//
	// All x coordinates are at 0.0 because I want player to start in top center
	//
	GLfloat player_start_verts[] = {
		//  Position	Texcoords
		-0.05f,			startingY,			0.0f, 0.0f, // Top-left
		-0.05f + inc,	startingY,			1.0f, 0.0f, // Top-right
		-0.05f + inc,	startingY - inc,	1.0f, 1.0f, // Bottom-right
		-0.05f,			startingY - inc,	0.0f, 1.0f  // Bottom-left
	};

	//
	// Initialize GLEW
	//
	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create and compile the vertex shader
	bg_vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(bg_vs, 1, &bg_vs_source, NULL);
	glCompileShader(bg_vs);

	// Create and compile the fragment shader
	bg_fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(bg_fs, 1, &bg_fs_source, NULL);
	glCompileShader(bg_fs);

	// Link the vertex and fragment shader into a shader program
	shaderPrograms[0] = glCreateProgram();
	glAttachShader(shaderPrograms[0], bg_vs);
	glAttachShader(shaderPrograms[0], bg_fs);
	glBindFragDataLocation(shaderPrograms[0], 0, "outColor");
	glLinkProgram(shaderPrograms[0]);
	glUseProgram(shaderPrograms[0]);

	board_setup_vertex_arrays(shaderPrograms[0]);

	glBindBuffer(GL_ARRAY_BUFFER, bg_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bgTileVerts)* attributeLength * numTiles, bgTileVerts, GL_STATIC_DRAW);

	// Player start data
	glBindBuffer(GL_ARRAY_BUFFER, player_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(player_start_verts), player_start_verts, GL_STATIC_DRAW);

	board_load_textures();
	
	uniTex = glGetUniformLocation(shaderPrograms[0], "uniTex");
	uniTrans = glGetUniformLocation(shaderPrograms[0], "uniTrans");

	return 0;
}

void Board::board_draw(){
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw background tiles
	glUniform1i(uniTex, 4); 
	glBindVertexArray(bg_vao);
	glDrawArrays(GL_QUADS, 0, numTiles * 4);
	
	// Draw player tile
	glUniform1i(uniTex, player->get_orientation());
	glBindVertexArray(player_vao);
	glDrawArrays(GL_QUADS, 0, 4);
}

void Board::board_update(){
	player->update_matrix(uniTrans);
	#ifdef DEBUG_ON 
		printf("%f \t %f\n", player->get_x(), player->get_y());
	#endif
}

void Board::board_setup_vertex_arrays(GLuint shaderProgram){
	//
	// Create Vertex Array Object <- (holds vertex buffers, saves state)
	//
	glGenVertexArrays(1, &bg_vao);
	glGenVertexArrays(1, &player_vao);

	glBindVertexArray(bg_vao);
	glGenBuffers(1, &bg_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, bg_vbo);
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	
	glBindVertexArray(player_vao);
	glBindBuffer(GL_ARRAY_BUFFER, player_vbo);
	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}

void Board::board_handle_input(){	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
		player->set_orientation(UP);
		player->up();
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
		player->set_orientation(DOWN);
		player->down();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
		player->set_orientation(LEFT);
		player->left();
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
		player->set_orientation(RIGHT);
		player->right();
	}
}

void Board::board_load_textures(){
	glGenTextures(5, textures);

	int width, height;
	unsigned char* image = SOIL_load_image("./res/img/blue_floor_32.png", &width, &height, 0, SOIL_LOAD_RGBA);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shaderPrograms[0], "texStone"), 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//
	// Player textures
	//
	image = SOIL_load_image("./res/img/morty_front_32.png", &width, &height, 0, SOIL_LOAD_RGBA);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shaderPrograms[0], "texMortyFront"), 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("./res/img/morty_rear_32.png", &width, &height, 0, SOIL_LOAD_RGBA);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shaderPrograms[0], "texMortyBack"), 2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("./res/img/morty_right_32.png", &width, &height, 0, SOIL_LOAD_RGBA);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shaderPrograms[0], "texMortyRight"), 3);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("./res/img/morty_left_32.png", &width, &height, 0, SOIL_LOAD_RGBA);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shaderPrograms[0], "texMortyLeft"), 4);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Board::~Board()
{
	glDeleteTextures(1, textures);
	glDeleteProgram(shaderPrograms[0]);
	glDeleteShader(bg_fs);
	glDeleteShader(bg_vs);
	glDeleteBuffers(1, &bg_vbo);
	glDeleteBuffers(1, &player_vbo);

	glDeleteVertexArrays(1, &bg_vao);
	glDeleteVertexArrays(1, &player_vao);

	if (tiles){
		delete[] tiles;
	}

	if (player){
		delete player;
	}
}

