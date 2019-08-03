/*
  Alex Romauld

  Compiled Using MinGW

  Run Instructions (2 Players):
  -(host):    comp_snake.exe <PORT>
  -(client):  comp_snake.exe <ADDR> <PORT>

  Competitive Snake (comp_snake)
  -Arrow Keys - Move
  -You Lose if you run into yourself or the other player
  -You Win by not being the losing player
  -Eat the Purple Dots to grow and speed up the game
*/


#include <iostream>
#include <winsock2.h>
#include <SDL2/SDL.h>

#include "game_net.h"
#include "game_components.h"


SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* renderer;

// Init SDL
int init()
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) return -1;

	window = SDL_CreateWindow( "Competitive Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(window == NULL) return -1;

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if(renderer == NULL) return -1;

	screenSurface = SDL_GetWindowSurface( window );
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF) );

	SDL_UpdateWindowSurface( window);

	return 0;
}

// Quit SDL
void close()
{
	SDL_DestroyWindow( window );
	window = NULL;

	SDL_DestroyRenderer( renderer );
	renderer = NULL;

	SDL_Quit();
}

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
int rows = 40;
int columns = 40;

// Delta Time
double now = 0;
double last = 0;
float dt = 0;

// Game Tick
float game_speed = 65;
float game_tick = 0;

int main(int argc, char** argv)
{
	//Init Players / Food
	Player p1,p2;
	Food food;
	p1.init(columns/2, rows*0.25);
	p2.init(columns/2, rows*0.75);
	food.SetPos(columns/2, rows/2);
	Player* my_player, *o_player;
	my_player = &p1;
	o_player = &p2;

	game_net con;
	if(argc == 2)      // Host
	{
		con.init_host(argv[1]);
		my_player = &p1;
		o_player = &p2;
	}
	else if(argc == 3) // Client
	{
		if(con.connect_to(argv[1], argv[2]) == 0)	return EXIT_FAILURE;
		my_player = &p2;
		o_player = &p1;
	}
	else
	{
		std::cout << "HOST: " << argv[0] << " PORT" << std::endl;
		std::cout << "CLIENT: " << argv[0] << " ADDR PORT" << std::endl;
		return EXIT_FAILURE;
	}


	init();
	SDL_ShowCursor( SDL_DISABLE );
	bool quit = false;
	SDL_Event e;
	while(!quit){
		// Delta Time
		last = now;
		now = SDL_GetTicks();
		dt = (now - last) / 1000.f;
		if(dt * 1000.f > 50) dt = 50/1000.f; // Cap dt


		// INPUT
		while(SDL_PollEvent(&e)){
			if( e.type == SDL_QUIT)		quit = true;
			else						my_player->HandleEvent(e);
		}


		// UPDATE
		if(game_tick >= game_speed){
			// Move Players (Player Directions are Sent and Received)
			con.send_dir(my_player->current_dir);
			o_player->current_dir = con.recv_dir();
			p1.Update();
			p2.Update();


			// Handle Players Colliding ( WIN / LOSE)
			if(my_player->BodyCollision(o_player)){
				std::cout << "You Lose!" << std::endl;
				quit = true;
			}
			else if(o_player->BodyCollision(my_player)){
				std::cout << "You Win!" << std::endl;
				quit = true;
			}

			// Eat Food
			if((my_player->Head.x == food.x && my_player->Head.y == food.y)	|| (o_player->Head.x == food.x && o_player->Head.y == food.y)){
				if( game_speed > 45 )	game_speed -= 1;	// Speed-Up game

				if(my_player->Head.x == food.x && my_player->Head.y == food.y)		my_player->AddBodyPart();
				else if (o_player->Head.x == food.x && o_player->Head.y == food.y)	o_player->AddBodyPart();

				food.RandomMove(&p1, &p2);
			}

			game_tick = 0;
		}
		else{
			game_tick += 1000 * dt;
		}

		// DRAW
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		DrawTile(renderer, food.x, food.y, 255, 0, 255 ); // Food

		p1.Draw(renderer, 1);      // Player 1
		p2.Draw(renderer, 2);      // Player 2

		SDL_RenderPresent(renderer);
	}

	close();

	return EXIT_SUCCESS;
}
