/*
 * TileMapper.cpp
 *
 *  Created on: 13/nov/2015
 *      Author: stefano
 */

#include "TileMapper.h"

#include "Bitmap.h"
#include "FileStream.h"

#include "Types.h"

#include <iostream>
#include <sstream>

#include <string.h>

TileMapper::TileMapper()
	:
	fMapWidth(0),
	fMapHeight(0),
	fTileMap(NULL)
{
}


TileMapper::~TileMapper()
{
	delete[] fTileMap;
}


bool
TileMapper::Load(const char* fileName)
{
	std::cout << "TileMapper::Load(";
	std::cout << fileName << ")" << std::endl;
	FileStream file(fileName);
	if (!file.IsValid())
		return false;

	file.Read(&fMapWidth, sizeof(fMapWidth));
	file.Read(&fMapHeight, sizeof(fMapHeight));

	int mapSize = fMapWidth * fMapHeight;
	fTileMap = new uint8[mapSize];
	file.Read(fTileMap, mapSize);

	return true;
}


int
TileMapper::TileWidth() const
{
	return 32;
}


int
TileMapper::TileHeight() const
{
	return 32;
}


int
TileMapper::MapWidth() const
{
	return fMapWidth;
}


int
TileMapper::MapHeight() const
{
	return fMapHeight;
}


Bitmap*
TileMapper::GetTileAt(int x, int y)
{
	if (_TileAt(x, y) == 1)
		return Bitmap::Load("./resources/terrain.bmp");
	return NULL;
}


int
TileMapper::_TileAt(int x, int y) const
{
	return fTileMap[y + x * MapWidth()];
}
