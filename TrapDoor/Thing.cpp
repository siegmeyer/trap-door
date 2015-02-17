#include "Thing.h"

Thing::Thing(float startingY, float inc)
{
	x = lx = -0.05f;
	y = ly = startingY;
	radius = inc / 2;
	orientation = DOWN;
	speed = 0.01f;
}


Thing::~Thing()
{
}

void Thing::update_matrix(GLuint uniTrans){
	
	//
	// Get difference between previous position and new position and translate that amount.
	//
	glm::vec3 transVec = glm::vec3(
		this->get_x() - this->get_lx(),
		this->get_y() - this->get_ly(),
		0.0f
	);
	glm::mat4 transMat;
	
	transMat = glm::translate(transMat, transVec);

	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(transMat));
}

