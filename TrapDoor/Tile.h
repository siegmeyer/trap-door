#ifndef TILE_H
#define TILE_H

#include <iostream>
#include "Thing.h"

class Tile
{
public:
	Tile();
	virtual ~Tile();

	// Determines background
	unsigned short tileType;

	// Determines decorative/impassable object on that tile
	unsigned short tileDecor;

	float tlX;	// top left x coordinate
	float tlY;
	float trX;
	float trY;
	float brX;
	float brY;
	float blX;
	float blY;
	
	// Array of Things that are on this tile
	Thing* things;
};

#endif