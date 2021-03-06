#include <assert.h>
#include <algorithm>
#include <iostream>

#include "Bitmap.h"
#include "GraphicsEngine.h"

#include <SDL.h>

static GraphicsEngine *sGraphicsEngine = NULL;

GraphicsEngine::GraphicsEngine()
	:
	fScreen(NULL),
	fFlags(0),
	fOldDepth(0),
	fOldFlags(0)
{
	fOldRect.w = fOldRect.h = fOldRect.x = fOldRect.y = 0;
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw "SDL Error";
	SDL_ShowCursor(0);
}


GraphicsEngine::~GraphicsEngine()
{
	fScreen->Release();
	SDL_Quit();
}


/* static */
GraphicsEngine*
GraphicsEngine::Get()
{
	return sGraphicsEngine;
}


/* static */
bool
GraphicsEngine::Initialize()
{
	std::cout << "Initializing Graphics Engine... ";
	std::flush(std::cout);
	try {
		sGraphicsEngine = new GraphicsEngine();
	} catch (...) {
		std::cout << "Failed!" << std::endl;
		sGraphicsEngine = NULL;
		return false;
	}

	std::cout << "OK!" << std::endl;
	return true;
}


/* static */
void
GraphicsEngine::Destroy()
{
	std::cout << "GraphicsEngine::Destroy()" << std::endl;
	delete sGraphicsEngine;
}


void
GraphicsEngine::SetClipping(const GFX::rect* rect)
{
	SDL_SetClipRect(fScreen->Surface(), (const SDL_Rect*)rect);
}


void
GraphicsEngine::BlitToScreen(const Bitmap* source, GFX::rect *sourceRect,
		GFX::rect *destRect)
{
	SDL_BlitSurface(source->Surface(), (SDL_Rect*)sourceRect,
				fScreen->Surface(), (SDL_Rect*)destRect);
}


/*static*/
void
GraphicsEngine::BlitBitmap(const Bitmap* bitmap, GFX::rect *source,
		Bitmap *surface, GFX::rect *dest)
{
	SDL_BlitSurface(bitmap->Surface(), (SDL_Rect*)(source),
			surface->Surface(), (SDL_Rect*)(dest));
}


/*static*/
void
GraphicsEngine::BlitBitmapWithMask(const Bitmap* bitmap,
		GFX::rect *source, Bitmap *destBitmap, GFX::rect *dest,
		const Bitmap* mask, GFX::rect *maskRect)
{
	uint32 xStart = 0;
	uint32 yStart = 0;
	if (dest->x < 0) {
		xStart -= dest->x;
		dest->x = 0;
	}
	if (dest->y < 0) {
		yStart -= dest->y;
		dest->y = 0;
	}

	mask->Lock();

	uint8* maskPixels = (uint8*)mask->Pixels()
				+ (maskRect->y * mask->Pitch()) + maskRect->x;

	for (uint32 y = yStart; y < bitmap->Height(); y++) {
		for (uint32 x = xStart; x < bitmap->Width(); x++) {
			SDL_Rect sourceRect = {0, 0, 1, 1};
			SDL_Rect destRect = {0, 0, 1, 1};
			if (maskPixels[x] != MASK_COMPLETELY) {
				if (maskPixels[x] == MASK_SHADE) {
					//if (y % 2 != 0 || x % 2 != 0)
						continue;
				}
				sourceRect.x = x;
				sourceRect.y = y;
				destRect.x = x - xStart + dest->x;
				destRect.y = y - yStart + dest->y;

				SDL_BlitSurface(bitmap->Surface(), &sourceRect,
						destBitmap->Surface(), &destRect);
			}
		}
		maskPixels += mask->Pitch();
	}
	mask->Unlock();
}


/* static */
void
GraphicsEngine::BlitBitmapScaled(const Bitmap* bitmap, GFX::rect* sourceRect,
							Bitmap* surface, GFX::rect* destRect)
{
	SDL_SoftStretch(bitmap->Surface(), (SDL_Rect*)sourceRect,
			surface->Surface(), (SDL_Rect*)destRect);
}


void
GraphicsEngine::SetVideoMode(uint16 x, uint16 y, uint16 depth,
		uint16 flags)
{
	std::cout << "GraphicsEngine::SetVideoMode(";
	std::cout << std::dec << x << ", " << y << ", " << depth;
	std::cout << ")" << std::endl;

	int sdlFlags = 0;
	if (flags & VIDEOMODE_FULLSCREEN)
		sdlFlags |= SDL_FULLSCREEN;
	SDL_Surface* surface = SDL_SetVideoMode(x, y, depth, sdlFlags);
	fScreen = new Bitmap(surface, false);
	fFlags = flags;

	std::cout << "GraphicsEngine::SetVideoMode(): Got mode ";
	std::cout << fScreen->Width() << "x";
	std::cout << fScreen->Height() << "x" << fScreen->BitsPerPixel();
	std::cout << std::endl;

/*	std::vector<Listener*>::iterator i;
	for (i = fListeners.begin(); i != fListeners.end(); i++) {
		(*i)->VideoModeChanged(x, y, depth);
	}*/
}


void
GraphicsEngine::SaveCurrentMode()
{
	if (fScreen != NULL) {
		SDL_Surface* surface = fScreen->Surface();
		fOldRect.x = fOldRect.y = 0;
		fOldRect.w = surface->w;
		fOldRect.h = surface->h;
		fOldDepth = surface->format->BitsPerPixel;
		fOldFlags = fFlags;
	}
}


void
GraphicsEngine::RestorePreviousMode()
{
	if (fOldDepth != 0) {
		SetVideoMode(fOldRect.w, fOldRect.h, fOldDepth, fOldFlags);
		fOldDepth = 0;
	}
}


GFX::rect
GraphicsEngine::ScreenFrame() const
{
	return fScreen->Frame();
}


void
GraphicsEngine::SetWindowCaption(const char* caption)
{
	SDL_WM_SetCaption(caption, NULL);
}


Bitmap*
GraphicsEngine::ScreenBitmap()
{
	return fScreen;
}


void
GraphicsEngine::Flip()
{
	fScreen->Update();
}


/* static */
void
GraphicsEngine::CreateGradient(const GFX::Color& start, const GFX::Color& end,
		GFX::Palette& palette)
{
	GFX::Color* colors = palette.colors;
	uint8 rFactor = (end.r - start.r) / 255;
	uint8 gFactor = (end.g - start.g) / 255;
	uint8 bFactor = (end.b - start.b) / 255;
	uint8 aFactor = (end.a - start.a) / 255;
	colors[0] = start;
	colors[255] = end;
	for (uint8 c = 1; c < 255; c++) {
		colors[c].r = colors[c - 1].r + rFactor;
		colors[c].g = colors[c - 1].g + gFactor;
		colors[c].b = colors[c - 1].b + bFactor;
		colors[c].a = colors[c - 1].a + aFactor;
	}
}

