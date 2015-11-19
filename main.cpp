/*
 * main.cpp
 *
 *  Created on: 12/nov/2015
 *      Author: stefano
 */

#include "Bitmap.h"
#include "GraphicsEngine.h"
#include "TileMapper.h"
#include "Timer.h"

#include <iostream>

int main()
{
	if (!GraphicsEngine::Initialize()) {
		std::cerr << "Cannot initialize Graphics Engine!" << std::endl;
		return -1;
	}

	GraphicsEngine::Get()->SetVideoMode(600, 400,
			16, GraphicsEngine::VIDEOMODE_WINDOWED);
	TileMapper tileMapper;
	if (!tileMapper.Load("./resources/terrainmap.json.map")) {
		GraphicsEngine::Destroy();
		return -1;
	}
	GFX::rect rect(0, 0, tileMapper.TileWidth(), tileMapper.TileHeight());
	for (int x = 0; x < 10; x++) {
		rect.y = 0;
		for (int y = 0; y < 10; y++) {
			Bitmap* tile = tileMapper.GetTileAt(x, y);
			if (tile != NULL) {
				GraphicsEngine::Get()->BlitToScreen(tile, NULL, &rect);
				tile->Release();
			}
			rect.y += 32;
		}
		rect.x += 32;
	}
	GraphicsEngine::Get()->Flip();
	Timer::Wait(5000);

	GraphicsEngine::Destroy();
}
