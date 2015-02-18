#include "Thing.h"

Thing::Thing(float startingX, float startingY, float inc)
{
	x = lx = -0.05f;
	y = ly = startingY;
	thingWidth = inc;
	orientation = DOWN;
	speed = 0.01f;
	boardTopLeftX = startingX;
	boardTopLeftY = startingY;
}


Thing::~Thing()
{
}

void Thing::update_matrix(GLuint uniTrans){
	
	//
	// Detect collisions with the edges of the board
	//
	if (x < boardTopLeftX){
		x = boardTopLeftX;
	}
	else if (x + thingWidth > -boardTopLeftX){
		x = -boardTopLeftX - thingWidth;
	}

	if (y > boardTopLeftY){
		y = boardTopLeftY;
	}
	else if (y - thingWidth < -boardTopLeftY){
		y = -boardTopLeftY + thingWidth;	
	}

	//
	// Get difference between previous position and new position and translate that amount.
	//
	glm::vec3 transVec = glm::vec3(
		x - lx,
		y - ly,
		0.0f
	);
	glm::mat4 transMat;
	
	transMat = glm::translate(transMat, transVec);

	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(transMat));
}


