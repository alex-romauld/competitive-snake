#include <iostream>
#include "game_components.h"

void DrawTile(SDL_Renderer* renderer, int row, int column, int r, int g, int b)
{
	int w = SCREEN_WIDTH / columns;
	int h = SCREEN_HEIGHT / rows;
	SDL_Rect a = { row * w, column * h, w, h};

	SDL_SetRenderDrawColor( renderer, r, g, b, 255 );
	SDL_RenderDrawRect( renderer, &a );
	SDL_RenderFillRect( renderer, &a );
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
}

void Food::SetPos(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Food::RandomMove(Player* p1, Player* p2)
{
	x = rand() % (columns - 1);
	y = rand() % (rows - 1);

	// Ensure that food doesn't spawn on top of a player
	BodyPart* i = &p1->Head;
	while(i){
		if(x == i->x && y == i->y)
			RandomMove(p1, p2);
		i = i->link;
	}
	i = &p2->Head;
	while(i){
		if(x == i->x && y == i->y)
			RandomMove(p1, p2);
		i = i->link;
	}
}


void Player::AddBodyPart()
{
	BodyPart* cur = &Head;
	while(cur->link != NULL)
		cur = cur->link;
	BodyPart* temp = new BodyPart();
	temp->x = cur->x;
	temp->y = cur->y;
	cur->link = temp;
	num_body_parts++;
}

void Player::init(int start_x, int start_y)
{
	num_body_parts = 0;
	current_dir = NONE;
	prev_dir = NONE;

	Head.link = NULL;
	Head.x = start_x;
	Head.y = start_y;
	for(int i = 0; i < 12; i++){
		AddBodyPart();
	}
}

int Player::BodyCollision(Player* o_player)
{
	BodyPart* i = Head.link;
	while(i){
		if(Head.x == i->x && Head.y == i->y && current_dir != NONE)
			return 1;
		i = i->link;
	}

	i = &o_player->Head;
	while(i){
		if(Head.x == i->x && Head.y == i->y && current_dir != NONE)
			return 1;
		i = i->link;
	}

	return 0;
}

void Player::HandleEvent(SDL_Event& e)
{
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
		switch(e.key.keysym.sym){
			case SDLK_UP:
				if( prev_dir != SOUTH) current_dir = NORTH;
				break;
			case SDLK_DOWN:
				if( prev_dir != NORTH) current_dir = SOUTH;
				break;
			case SDLK_LEFT:
				if( prev_dir != WEST) current_dir = EAST;
				break;
			case SDLK_RIGHT:
				if( prev_dir != EAST) current_dir = WEST;
				break;
		}
	}
}

void Player::Update()
{
	BodyPart* i = &Head;
	int par_x = i->x, par_y = i->y;

	while(i)
	{
		int temp_x = i->x, temp_y = i->y;
		i->x = par_x;
		i->y = par_y;

		par_x = temp_x;
		par_y = temp_y;

		i = i->link;
	}

	switch( current_dir )
	{
		case NORTH:
			Head.y--;
			break;
		case SOUTH:
			Head.y++;
			break;
		case EAST:
			Head.x--;
			break;
		case WEST:
			Head.x++;
			break;
		case NONE:
			break;
	}

	prev_dir = current_dir;


	// Loop on screen
	if(Head.x >= columns)		Head.x = 0;
	if(Head.x < 0)				Head.x = columns;

	if(Head.y >= rows)			Head.y = 0;
	if(Head.y < 0)				Head.y = rows;
  }

void Player::Draw(SDL_Renderer* renderer, int p)
{
	// Draw body with fading color
	BodyPart* i = Head.link;
	int part = 0;
	while(i){
		int color = (part/(float)num_body_parts) * 255;
		if(p == 1)			DrawTile(renderer, i->x, i->y, color, 255, 255);
		else if(p==2)		DrawTile(renderer, i->x, i->y, 255, 255, color);
		part++;
		i = i->link;
	}

	// Draw Head
	DrawTile(renderer,  Head.x, Head.y, 0, 255, 0 );
}
