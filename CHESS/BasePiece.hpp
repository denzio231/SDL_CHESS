#include<vector>
#include<SDL.h>
#include "PIECE_ENUM.hpp"
#include "GAME_DEFINITIONS.hpp"
#include<SDL_image.h>
#include "SideEnum.hpp"
#include<iostream>
class position {
	public:
		int x;
		int y;
		position add(int xa, int ya) {
			int a_x = x + xa;
			int a_y = y + ya;
			position n;
			n.x = a_x;
			n.y = a_y;
			return n;
		}
};

class BasePiece {
public:
	SDL_Texture* Sprite;
	SDL_Rect dstrect;
	std::vector<position>(*GetValidMovesPt)();
	void init(SDL_Renderer* r) {};
	virtual void move_by_idx(int idx) {
		position to_move = GetValidMoves()[idx].first;
		BasePiece* capture = GetValidMoves()[idx].second;
		move(to_move,capture);
	}
	void move(position p, BasePiece* capture);
	position coordinates;
	PIECE_ENUM PIECE_TYPE;
	SIDE_ENUM SIDE;
	bool override = false;
	void render(SDL_Renderer* renderer);
	void render_nolimit(SDL_Renderer* renderer,int x, int y);
	virtual std::vector<std::pair<position, BasePiece*>> GetValidMoves(){
		std::vector<std::pair<position, BasePiece*>> a;
		return a;
	};
};

extern BasePiece* BOARD[8][8];