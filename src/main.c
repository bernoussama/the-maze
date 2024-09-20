#include <SDL2/SDL.h>
#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#define PI 3.14159265358979323846

// Key press surfaces constants

#define screenWidth 640
#define screenHeight 480

#define mapWidth 8
#define mapHeight 8

// clang-format off
int worldMap[mapWidth][mapHeight] = 
  {
    {0, 0, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 1, 1, 1},
    {1, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};
// clang-format on

/* Screen dimension constants */
const int SCREEN_HEIGHT = 512;
const int SCREEN_WIDTH = SCREEN_HEIGHT * 2;

const int UNIT = 64;
// const int WALL_HEIGHT = 64 * UNIT;
// const int PLAYER_HEIGHT = WALL_HEIGHT / 2;
// const int FOV = 60;
// const int INIT_ANGLE = 45;
//
// const int projection_width = 320;
// const int projection_height = 200;
// const double column = FOV / projection_width;
// const double columns = projection_width;
// const double distance_to_projection = projection_width / tan(FOV / 2);
//

void drawRays(SDL_Renderer *renderer, double x, double y, double playerAngle);

void drawRays(SDL_Renderer *renderer, double px, double py, double playerAngle)
{

	double rayAngle, rayX, rayY, Xoffset, Yoffset, hRayX, hRayY, vRayX,
	    vRayY, hDistance, vDistance;
	rayAngle = playerAngle;
	printf("rayAngle: %f\n", rayAngle);

	int rays, x, y;
	rays = 1;
	for (int ray = 0; ray < rays; ray++)
	{
		/* horizontal lines */
		// if (rayAngle < PI && rayAngle > 0)
		// {
		// 	rayY = (((int)py >> 6) << 6) -
		// 	       1; // bit shift shananigans to round down
		// 	rayX = px + (py - rayY) / tan(rayAngle);
		// 	Yoffset = -64;
		// 	Xoffset = 64 / tan(rayAngle);
		// }
		// else if (rayAngle > PI && rayAngle < (2 * PI))
		// {
		// 	rayY = (((int)py >> 6) << 6) +
		// 	       64; // bit shift shananigans to round down
		// 	rayX = px + (py - rayY) / tan(rayAngle);
		// 	Yoffset = 64;
		//
		// 	Xoffset = -64 / tan(rayAngle);
		// }
		// else if (rayAngle == 0 || rayAngle == PI)
		// {
		// 	rayY = py;
		// 	rayX = px;
		// 	// Yoffset = 0;
		// 	Xoffset = 64 / tan(rayAngle);
		// }
		// printf("rayX: %f, rayY: %f\n", rayX, rayY);
		// x = (int)rayX >> 6;
		// y = (int)rayY >> 6;
		// printf("x: %d, y: %d\n", x, y);
		// while (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight &&
		//        worldMap[y][x] <= 0)
		// {
		// 	printf("x: %d, y: %d\n", x, y);
		// 	rayX += Xoffset;
		// 	rayY += Yoffset;
		// 	x = (int)rayX >> 6;
		// 	y = (int)rayY >> 6;
		// 	printf("x: %d, y: %d\n", x, y);
		// }
		// hDistance = sqrt(pow(px - rayX, 2) + pow(py - rayY, 2));
		// SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
		// SDL_RenderDrawLine(renderer, px, py, rayX, rayY);
		//
		// SDL_RenderPresent(renderer);

		/* vertical lines */
		if (rayAngle < PI / 2 || rayAngle > (3 * PI) / 2)
		{
			rayX = (((int)px >> 6) << 6) +
			       64; // bit shift shananigans to round down
			rayY = py + (px - rayX) * tan(rayAngle);
			Yoffset = -64 * tan(rayAngle);
			Xoffset = 64;
		}
		else if (rayAngle > PI / 2 && rayAngle < (3 * PI) / 2)
		{
			rayX = (((int)px >> 6) << 6) -
			       1; // bit shift shananigans to round down
			rayY = py + (px - rayX) * tan(rayAngle);
			Yoffset = -64 * tan(rayAngle);
			Xoffset = 64;
		}
		else if (rayAngle == PI / 2 || rayAngle == (3 * PI) / 2)
		{
			rayY = py;
			rayX = px;
			Xoffset = 0;
			Yoffset = 64 / tan(rayAngle);
		}
		printf("rayX: %f, rayY: %f\n", rayX, rayY);
		x = (int)rayX >> 6;
		y = (int)rayY >> 6;
		printf("x: %d, y: %d\n", x, y);
		while (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight &&
		       worldMap[y][x] <= 0)
		{
			printf("x: %d, y: %d\n", x, y);
			rayX += Xoffset;
			rayY += Yoffset;
			x = (int)rayX >> 6;
			y = (int)rayY >> 6;
			printf("x: %d, y: %d\n", x, y);
		}
		vDistance = sqrt(pow(px - rayX, 2) + pow(py - rayY, 2));
		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawLine(renderer, px, py, rayX, rayY);

		SDL_RenderPresent(renderer);
	}
}

int main(void)
{
	/* window will be rendering to */
	SDL_Window *window;
	SDL_Renderer *renderer;

	/* The surface contained by the window */
	SDL_Surface *screenSurface = NULL;

	double w = SCREEN_WIDTH, h = SCREEN_HEIGHT;
	double posX = 22, posY = 12;	 // player x and y start position
	double pdeltaX = 0, pdeltaY = 0; // player direction vector
	double pangle = 0;		 // player angle
	double dirX = -1, dirY = 0;	 // initial direction vector
	double planeX = 0,
	       planeY = 0.66; // the 2d raycaster version of camera plane

	double time = 0;    // time of current frame
	double oldTime = 0; // time of previous frame

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n",
			SDL_GetError());
		return (1);
	}

	/* Create a new window instance */
	window = SDL_CreateWindow("SDL2 raycaster", SDL_WINDOWPOS_CENTERED,
				  SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
				  SCREEN_HEIGHT, 0);
	if (window == NULL)
	{
		fprintf(stderr, "SDL_Createwindow Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}

	/* create a new renderer instance linked to the Window */
	renderer = SDL_CreateRenderer(
	    window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n",
			SDL_GetError());
		SDL_Quit();
		return (1);
	}
	// Get window surface
	screenSurface = SDL_GetWindowSurface(window);

	// Fill the surface white
	// SDL_FillRect(screenSurface, NULL,
	//              SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
	//
	// // Update the surface
	// SDL_UpdateWindowSurface(window);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	// Render red filled quad
	// SDL_Rect fillRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4,
	// SCREEN_WIDTH / 2,
	//                      SCREEN_HEIGHT / 2};
	//
	// SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	// SDL_RenderFillRect(renderer, &fillRect);
	//
	// // Render green outlined quad
	// SDL_Rect outlineRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6,
	//                         SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
	// SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
	// SDL_RenderDrawRect(renderer, &outlineRect);
	//
	// // Render blue line
	// SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	// SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH,
	//                    SCREEN_HEIGHT / 2);
	// SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
	// for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
	//   SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2, i);
	// }

	SDL_Rect player = {posX, posY, 8, 8};

	// Fill the surface white
	SDL_FillRect(screenSurface, NULL,
		     SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

	/* draw map */
	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			if (worldMap[y][x] > 0)
			{
				SDL_Rect wall = {x * UNIT, y * UNIT, UNIT,
						 UNIT};
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00,
						       0x00, 0xFF);
				SDL_RenderFillRect(renderer, &wall);
				// render horizontal and vertical lines
				SDL_Rect line = {x * UNIT, 0, UNIT,
						 mapHeight * UNIT};
				SDL_Rect line2 = {0, y * UNIT, mapWidth * UNIT,
						  UNIT};
				SDL_SetRenderDrawColor(renderer, 0x80, 0x80,
						       0x80, 0xFF);
				SDL_RenderDrawRect(renderer, &line2);
				SDL_SetRenderDrawColor(renderer, 0x80, 0x80,
						       0x80, 0xFF);
				SDL_RenderDrawRect(renderer, &line);
				line.y = mapHeight * UNIT;
				SDL_RenderDrawRect(renderer, &line);
			}
		}
	}

	// Update the surface
	SDL_UpdateWindowSurface(window);

	// Draw the player
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &player);

	pdeltaX = cos(pangle) * 5;
	pdeltaY = sin(pangle) * 5;
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderDrawLine(renderer, player.x, player.y, player.x + pdeltaX * 5,
			   player.y + pdeltaY * 5);
	SDL_RenderPresent(renderer);

	// Hack to get window to stay up
	SDL_Event e;
	bool quit = false;
	while (quit == false)
	{
		while (SDL_PollEvent(&e))
		{
			printf("Event type: %d\n", e.type);
			if (e.type == SDL_QUIT)
			{
				printf("Event quit: %d\n", e.type);
				quit = true;
			}
			// User presses a key
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					player.x += pdeltaX;
					player.y += pdeltaY;
					break;
				case SDLK_DOWN:
					player.x -= pdeltaX;
					player.y -= pdeltaY;
					break;
				case SDLK_LEFT:
					pangle += 0.1; // Rotate the player left
					if (pangle > 2 * PI)
						pangle -= 2 * PI;
					pdeltaX = cos(pangle) * 5;
					pdeltaY = -sin(pangle) *
						  5; // minus because fckn sdl
						     // have y flipped
					break;
				case SDLK_RIGHT:
					pangle -= 0.1; // Rotate the player left
					if (pangle < 0)
						pangle += 2 * PI;
					pdeltaX = cos(pangle) * 5;
					pdeltaY = -sin(pangle) *
						  5; // minus because fckn sdl
						     // have y flipped
					break;
				default:
					continue;
				}

				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF,
						       0xFF, 0xFF);
				SDL_RenderClear(renderer);
				// Draw the map
				for (int x = 0; x < mapWidth; x++)
				{
					for (int y = 0; y < mapHeight; y++)
					{
						if (worldMap[y][x] > 0)
						{
							SDL_Rect wall = {
							    x * UNIT, y * UNIT,
							    UNIT, UNIT};
							SDL_SetRenderDrawColor(
							    renderer, 0x00,
							    0x00, 0x00, 0xFF);
							SDL_RenderFillRect(
							    renderer, &wall);

							// render horizontal and
							// vertical lines
							SDL_Rect line = {
							    x * UNIT, 0, UNIT,
							    mapHeight * UNIT};
							SDL_Rect line2 = {
							    0, y * UNIT,
							    mapWidth * UNIT,
							    UNIT};
							SDL_SetRenderDrawColor(
							    renderer, 0x80,
							    0x80, 0x80, 0xFF);
							SDL_RenderDrawRect(
							    renderer, &line2);
							SDL_SetRenderDrawColor(
							    renderer, 0x80,
							    0x80, 0x80, 0xFF);
							SDL_RenderDrawRect(
							    renderer, &line);
							line.y =
							    mapHeight * UNIT;
							SDL_RenderDrawRect(
							    renderer, &line);
						}
					}
				}
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x00,
						       0x00, 0xFF);
				SDL_RenderFillRect(renderer, &player);

				SDL_SetRenderDrawColor(renderer, 0x00, 0x00,
						       0xFF, 0xFF);
				SDL_RenderDrawLine(renderer, player.x, player.y,
						   player.x + pdeltaX * 5,
						   player.y + pdeltaY * 5);
				drawRays(renderer, player.x, player.y, pangle);
				SDL_RenderPresent(renderer);
				printf("px: %d, py: %d\n", player.x, player.y);
				printf("pdeltaX: %f, pdeltaY: %f\n", pdeltaX,
				       pdeltaY);
				printf("pangle: %f\n", pangle);
			}
		}
	}
	// Destroy window
	SDL_DestroyWindow(window);

	// Quit SDL subsystems
	SDL_Quit();

	return (0);
}
