#include<iostream>
#include"Dervived_Pieces.hpp"
#include<SDL.h>
#include<SDL_image.h>
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* sprite = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* character_appearance = NULL;
int SCREEN_WIDTH = TILESIZE * 8;
int SCREEN_HEIGHT = TILESIZE * 8;
SDL_Rect TILES[8][8];
void render_board() {
    bool white = true;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            SDL_Rect pos;
            pos.x = x * TILESIZE;
            pos.y = y * TILESIZE;
            pos.w = TILESIZE;
            pos.h = TILESIZE;
            TILES[x][y] = pos;
            if (white) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255);
            }
            SDL_RenderFillRect(renderer, &TILES[x][y]);

            white = not(white);
        }
        white = not(white);
    }
}
bool init()
{
    bool success = true;
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("CHESS",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
            if (renderer == NULL) {
                std::cout << SDL_GetError() << std::endl;
                exit(0);
            }

        }
    }
    return success;
}
void init_pieces() {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            BOARD[x][y] = NULL;
        }
    }
    for (int x = 0; x < 8; x++) {
        Pawn* a = new Pawn;
        a->coordinates.x = x;
        a->coordinates.y = 1;
        a->init(renderer,SIDE_ENUM::BLACK);
        BOARD[x][1] = a;
    }
    for (int x = 0; x < 8; x++) {
        Pawn* a = new Pawn;
        a->coordinates.x = x;
        a->coordinates.y = 6;
        a->init(renderer, SIDE_ENUM::WHITE);
        BOARD[x][6] = a;
    }
}
void render_pieces() {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (BOARD[x][y] != NULL) {
                BOARD[x][y]->render(renderer);
            }
        }
    }
}
BasePiece* check_for_collisions(int c_x,int c_y) {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (BOARD[x][y] != NULL) {
                SDL_Rect res = BOARD[x][y]->dstrect;
                if (res.x <= c_x and res.x + res.w >= c_x) {
                    if (res.y <= c_y and res.y + res.h >= c_y) {
                        return BOARD[x][y];
                    }
                }
              
            }
        }
    }
    BasePiece fail;
    fail.coordinates.x = -1;
    return &fail;
}
std::vector<std::pair<position, BasePiece*>> moves;
int check_for_tile(int c_x, int c_y) {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            SDL_Rect a = TILES[x][y];
            if (a.x <= c_x and a.x + a.w >= c_x) {
                if (a.y <= c_y and a.y + a.h >= c_y) {
                    for (int i = 0; i < moves.size();i++) {
                        if (moves[i].first.x == x and moves[i].first.y == y) {
                            return i;
                        }
                    }
                    return -1;
                }
            }
        }
    }
    return -1;

}
void clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}
void render_general() {
    clear();
    render_board();
    render_pieces();
    SDL_RenderPresent(renderer);
}


int main(int argc,char* argv[]) {
    if (!init()) {
        return 1;
    }
    bool running = true;
    SDL_Event e;
    BasePiece* res = NULL;
    bool follow_mouse = false;
    init_pieces();
    render_general();
    while (running) {
        if (follow_mouse) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            res->render_nolimit(renderer, x - TILESIZE / 2, y - TILESIZE / 2);
            render_general();
        }
        while (SDL_PollEvent(&e)) {
            int x, y;
            switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                  
                    SDL_GetMouseState(&x,&y);
                    res = check_for_collisions(x, y);
                    if (res->coordinates.x == -1) {
                        break;
                    }
                    moves = res->GetValidMoves();
                    for (auto i : moves) {
                        std::cout << i.first.x << " " << i.first.y;
                    }
                    if (res->coordinates.x != -1) {
                        follow_mouse = true;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (follow_mouse) {
                        follow_mouse = false;
                        SDL_GetMouseState(&x, &y);
                        int idx = check_for_tile(x, y);
                        if (idx >= 0) {
                            res->move_by_idx(idx);
                        }
                        res->render(renderer);
                        render_general();
                    }
                    
            }
                    
                    break;
                    

            }
        }
	return 0;
}