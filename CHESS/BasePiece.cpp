#include "BasePiece.hpp"
#include<iostream>


void BasePiece::render(SDL_Renderer* renderer) {
	if (!override) {
		dstrect.x = coordinates.x * TILESIZE;
		dstrect.y = coordinates.y * TILESIZE;

		dstrect.w = TILESIZE;
		dstrect.h = TILESIZE;
	}
	SDL_RenderCopy(renderer, Sprite,NULL, &dstrect);
	override = false;
}
void BasePiece::render_nolimit(SDL_Renderer* renderer,int x,int y) {
	dstrect.x = x;
	dstrect.y = y;
	dstrect.w = TILESIZE;
	dstrect.h = TILESIZE;
	
	override = true;
}
void BasePiece::move(position p, BasePiece* capture) {
	BOARD[coordinates.x][coordinates.y] = NULL;
	coordinates.x = p.x;
	coordinates.y = p.y;
	if (capture != NULL) {
		BOARD[capture->coordinates.x][capture->coordinates.y] = NULL;
		delete capture;
		capture = NULL;
	}
	BOARD[coordinates.x][coordinates.y] = this;
}
BasePiece* BOARD[8][8];