
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "graphics.h"
#include "globals.h"


/* Graphics class
 * Holds all information dealing with graphics for the game
 */

Graphics::Graphics() {
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);
	SDL_SetWindowTitle(this->_window, "Cavestory");
}

Graphics::~Graphics() {
	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);
}

SDL_Surface* Graphics::loadImage(const std::string &filePath) {
	if (this->_spriteSheets.count(filePath) == 0) {
		this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
	}
	return this->_spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

void Graphics::drawOutLine(int x, int y, int w, int h) {
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
	//std::cout << x << " " << y << " " << w << " " << h << std::endl;
	SDL_RenderDrawLine(_renderer, x, y, x, y + h);
	SDL_RenderDrawLine(_renderer, x, y, x + w, y);
	SDL_RenderDrawLine(_renderer, x + w, y + h, x, y + h);
	SDL_RenderDrawLine(_renderer, x + w, y + h, x + w, y);
}

void Graphics::flip() {
	SDL_RenderPresent(this->_renderer);
}

void Graphics::clear() {
	SDL_RenderClear(this->_renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
	return this->_renderer;
}
