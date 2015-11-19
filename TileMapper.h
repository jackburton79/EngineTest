/*
 * TileMapper.h
 *
 *  Created on: 13/nov/2015
 *      Author: stefano
 */

#ifndef TILEMAPPER_H_
#define TILEMAPPER_H_

#include "SupportDefs.h"

class Bitmap;
class TileMapper {
public:
	TileMapper();
	virtual ~TileMapper();

	bool Load(const char* fileName);

	// in pixels
	int TileWidth() const;
	int TileHeight() const;

	// in tiles
	int MapWidth() const;
	int MapHeight() const;

	Bitmap* GetTileAt(int x, int y);
private:
	int _TileAt(int x, int y) const;

	uint8 fMapWidth;
	uint8 fMapHeight;
	uint8 *fTileMap;
};

#endif /* TILEMAPPER_H_ */
