#include "Tile.h"


Tile::Tile()
{
	things = NULL;
}


Tile::~Tile()
{
	if (things){
		delete[] things;
	}
}
