#ifndef THING_H
#define THING_H

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum {
	UP,
	RIGHT,
	DOWN,
	LEFT,

	NUM_ORIENTATIONS
};


class Thing
{
public:
	// inc: tile size (length of one side)
	Thing(float startingY, float inc);
	virtual ~Thing();

	inline float get_x(){ return x; }
	inline float get_y(){ return y; }
	inline float get_lx(){ return lx; }
	inline float get_ly(){ return ly; }
	inline float get_radius(){ return radius; }
	inline unsigned char get_orientation(){ return orientation; }

	inline void set_x(float x){ this->x = x; }
	inline void set_y(float y){ this->y = y; }
	inline void set_orientation(unsigned char orientation){ this->orientation = orientation; }

	inline void up(){ y += speed; }
	inline void down(){ y -= speed; }
	inline void left(){ x -= speed; }
	inline void right(){ x += speed; }

	void update_matrix(GLuint uniTrans);
private:
	float x;
	float y;
	float lx;
	float ly;
	float radius;
	float speed;
	short thingType;
	unsigned char orientation;
};
#endif
