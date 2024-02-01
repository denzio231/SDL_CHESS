#include "BasePiece.hpp"
class Pawn : public BasePiece {
public:
	bool first = true;
	bool ep = false;
	void init(SDL_Renderer* r,SIDE_ENUM side) {
		SIDE = side;
		SDL_Surface* buff;
		if (SIDE == SIDE_ENUM::WHITE) {
			buff = IMG_Load("./Assets/WPawn.png");
		}
		else {
			buff = IMG_Load("./Assets/BPawn.png");
		}
		Sprite = SDL_CreateTextureFromSurface(r, buff);
		SDL_FreeSurface(buff);
		PIECE_TYPE = PIECE_ENUM::PAWN;
	}
	std::vector<std::pair<position,BasePiece *>> GetValidMoves() {
		std::vector<std::pair<position, BasePiece*>> a;
		int add = -1;
		if (SIDE == SIDE_ENUM::BLACK) {
			add = 1;
		}
		if (BOARD[coordinates.x][coordinates.y+add] == NULL) {
			position n = coordinates.add(0, add);
			std::pair<position, BasePiece*> p;
			p.first = n;
			p.second = NULL;
			a.push_back(p);
		}
		if (first) {
			position n = coordinates.add(0, add);
			std::pair<position, BasePiece*> p;
			p.first = coordinates.add(0, add * 2);
			p.second = NULL;
			a.push_back(p);
		}
		for (int i = -1; i < 2; i += 2) {
			if (BOARD[coordinates.x + i][coordinates.y + add] != NULL) {
				position n = coordinates.add(i, add);
				std::pair<position, BasePiece*> p;
				p.first = n;
				p.second = BOARD[coordinates.x + i][coordinates.y + add];
				a.push_back(p);
			}
		}
		for (int i = -1; i < 2; i += 2) {
			if (BOARD[coordinates.x + i][coordinates.y] != NULL and BOARD[coordinates.x + i][coordinates.y]->PIECE_TYPE == PIECE_ENUM::PAWN) {
				if (static_cast<Pawn*>(BOARD[coordinates.x + i][coordinates.y])->ep) {
					position n = coordinates.add(i, add);
					std::pair<position, BasePiece*> p;
					p.first = n;
					p.second = BOARD[coordinates.x + i][coordinates.y];
					a.push_back(p);
				}
			}
		}

		return a;
	};
	void move_by_idx(int idx) {
		BasePiece::move_by_idx(idx);
		if (first) {
			ep = true;
		}
		else {
			ep = false;
		}
		first = false;


	}
};

