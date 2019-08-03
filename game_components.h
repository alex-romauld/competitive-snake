#ifndef GAME_COMPONENTS_H
#define GAME_COMPONENTS_H

#include <SDL2/SDL.h>


enum compass { NORTH, SOUTH, EAST, WEST, NONE };

extern const int SCREEN_WIDTH, SCREEN_HEIGHT;
extern int rows, columns;
extern void DrawTile(SDL_Renderer* renderer, int row, int column, int r, int g, int b);


struct BodyPart {
	int x;
	int y;
	BodyPart* link = NULL;
};

struct Player {
	BodyPart Head;
	compass current_dir, prev_dir;
	int num_body_parts;

	void init(int start_x, int start_y);
	void AddBodyPart();
	int BodyCollision(Player* o_player);

	void HandleEvent(SDL_Event& e);
	void Update();
	void Draw(SDL_Renderer* renderer, int p);
};


struct Food{
	int x,y;

	void SetPos(int x, int y);
	void RandomMove(Player* p1, Player* p2);
};

#endif //GAME_COMPONENTS
