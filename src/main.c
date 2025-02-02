#include <SDL2/SDL.h>
#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define PI 3.14159265358979323846
#define DEG 0.01745329 /* one degree in radians */

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
const int WALL_HEIGHT = UNIT;
const int PLAYER_HEIGHT = WALL_HEIGHT / 2;
const int FOV = 60;
// const int INIT_ANGLE = 45;
//
// const int projection_width = 320;
// const int projection_height = 200;
// const double column = FOV / projection_width;
// const double columns = projection_width;
// const double distance_to_projection = projection_width / tan(FOV / 2);
//
double correctDistance(double distance, double rayAngle, double playerAngle);
double correctDistance(double distance, double rayAngle, double playerAngle)
{
	double angle = playerAngle - rayAngle;
	if (angle < 0)
		angle += 2 * PI;
	if (angle > 2 * PI)
		angle -= 2 * PI;
	return distance * cos(angle);
}

void drawRays(SDL_Renderer *renderer, double x, double y, double playerAngle);

void drawRays(SDL_Renderer *renderer, double px, double py, double playerAngle)
{

	double rayAngle, rayX, rayY, Xoffset, Yoffset, hRayX, hRayY, vRayX,
	    vRayY, hDistance, vDistance, distance, factor, lineHeight;
	rayAngle = playerAngle + DEG * 30;
	if (rayAngle < 0)
		rayAngle += 2 * PI;
	if (rayAngle > 2 * PI)
		rayAngle -= 2 * PI;

	int rays, x, y;
	factor = 64 >> 8; /* multiply by 256 */
	rays = FOV;
	for (int ray = 0; ray < rays; ray++)
	{
		/* --- horizontal lines --- */
		if (rayAngle < PI && rayAngle > 0)
		{
			rayY = (((int)py >> 6) << 6) -
			       1; // bit shift shananigans to round down
			rayX = px + (py - rayY) / tan(rayAngle);
			Yoffset = -64;
			Xoffset = 64 / tan(rayAngle);
		}
		else if (rayAngle > PI && rayAngle < (2 * PI))
		{
			rayY = (((int)py >> 6) << 6) +
			       64; // bit shift shananigans to round down
			rayX = px + (py - rayY) / tan(rayAngle);
			Yoffset = 64;

			Xoffset = -64 / tan(rayAngle);
		}
		else if (rayAngle == 0 || rayAngle == PI)
		{
			rayY = py;
			rayX = px;
			// Yoffset = 0;
			Xoffset = 64 / tan(rayAngle);
		}
		x = (int)rayX >> 6;
		y = (int)rayY >> 6;
		while (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight &&
		       worldMap[y][x] <= 0)
		{
			rayX += Xoffset;
			rayY += Yoffset;
			x = (int)rayX >> 6;
			y = (int)rayY >> 6;
		}
		hRayX = rayX;
		hRayY = rayY;
		hDistance = sqrt(pow(px - rayX, 2) + pow(py - rayY, 2));

		/* --- vertical lines --- */
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
			Yoffset = 64 * tan(rayAngle);
			Xoffset = -64;
		}
		else if (rayAngle == PI / 2 || rayAngle == (3 * PI) / 2)
		{
			rayY = py;
			rayX = px;
			Xoffset = 0;
			Yoffset = 64 / tan(rayAngle);
		}
		x = (int)rayX >> 6;
		y = (int)rayY >> 6;
		while (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight &&
		       worldMap[y][x] <= 0)
		{
			rayX += Xoffset;
			rayY += Yoffset;
			x = (int)rayX >> 6;
			y = (int)rayY >> 6;
		}

		vRayX = rayX;
		vRayY = rayY;

		vDistance = sqrt(pow(px - rayX, 2) + pow(py - rayY, 2));
		if (vDistance > hDistance)
		{
			rayX = hRayX;
			rayY = hRayY;
			distance = hDistance;
		}
		else
		{
			distance = vDistance;
		}

		SDL_SetRenderDrawColor(renderer, 0x00, 0xFE, 0x00, 0xFF);
		SDL_RenderDrawLine(renderer, px, py, rayX, rayY);

		/* --- draw 3d walls --- */
		int height = SCREEN_HEIGHT;
		lineHeight = (WALL_HEIGHT * height) /
			     correctDistance(distance, rayAngle, playerAngle);
		if (lineHeight > height)
			lineHeight = height;

		distance == hDistance
		    ? SDL_SetRenderDrawColor(renderer, 0xAA, 0x00, 0x00, 0xFF)
		    : SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

		SDL_Rect wallSlice;
		wallSlice.x =
		    ((ray) * (SCREEN_WIDTH / 2 / rays)) + (SCREEN_WIDTH / 2);
		wallSlice.y = (int)(SCREEN_HEIGHT / 2) - (lineHeight / 2);
		wallSlice.w = (SCREEN_WIDTH / 2) / rays;
		wallSlice.h = lineHeight;

		SDL_RenderFillRect(renderer, &wallSlice);

		/* increment ray angle */
		rayAngle -= DEG;
		if (rayAngle < 0)
			rayAngle += 2 * PI;
		if (rayAngle > 2 * PI)
			rayAngle -= 2 * PI;
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

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	SDL_Rect player = {posX - 4, posY - 4, 8, 8};

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

	// draw floor
	SDL_SetRenderDrawColor(renderer, 0x00, 0x80, 0x00, 0xFE);
	SDL_Rect floorRect = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
			      SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
	SDL_RenderFillRect(renderer, &floorRect);

	// draw ceiling
	SDL_Rect ceilingRect = {SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2,
				SCREEN_HEIGHT / 2};
	SDL_SetRenderDrawColor(renderer, 0x00, 0x50, 0xFF, 0x60);
	SDL_RenderFillRect(renderer, &ceilingRect);

	// Draw the player
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_Rect playerRect = {posX - 4, posY - 4, 8, 8};
	SDL_RenderFillRect(renderer, &playerRect);

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

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		while (SDL_PollEvent(&e))
		{

			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			/* collison detection */
			/* offset for player collision detection */
			int xo, yo;
			int ipx, ipy, ipxAddxo, ipyAddyo, ipxSubxo, ipySubyo;
			xo = 0;
			if (pdeltaX < 0)
				xo = -10;
			else
				xo = 10;

			yo = 0;
			if (pdeltaY < 0)
				yo = -10;
			else
				yo = 10;

			ipx = (int)(player.x) >> 6;
			ipy = (int)(player.y) >> 6;
			ipxAddxo = (int)(player.x + xo) >> 6;
			ipyAddyo = (int)(player.y + yo) >> 6;
			ipxSubxo = (int)(player.x - xo) >> 6;
			ipySubyo = (int)(player.y - yo) >> 6;

			if (state[SDL_SCANCODE_UP])
			{
				if (worldMap[ipyAddyo][ipxAddxo] == 0)
				{
					player.x += pdeltaX;
					player.y += pdeltaY;
				}
			}
			if (state[SDL_SCANCODE_DOWN])
			{
				if (worldMap[ipySubyo][ipxSubxo] == 0)
				{
					player.x -= pdeltaX;
					player.y -= pdeltaY;
				}
			}
			if (state[SDL_SCANCODE_LEFT])
			{
				pangle += 0.1; // Rotate the player left
				if (pangle > 2 * PI)
					pangle -= 2 * PI;
				pdeltaX = cos(pangle) * 5;
				pdeltaY = -sin(pangle) * 5;
				/* minus because sdl have y flipped */
			}
			if (state[SDL_SCANCODE_RIGHT])
			{
				pangle -= 0.1; // Rotate the player right
				if (pangle < 0)
					pangle += 2 * PI;
				pdeltaX = cos(pangle) * 5;
				pdeltaY =
				    -sin(pangle) *
				    5; // minus because fckn sdl have y flipped
			}

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF,
					       0xFF);

			SDL_RenderClear(renderer);

			// Draw the map
			for (int x = 0; x < mapWidth; x++)
			{
				for (int y = 0; y < mapHeight; y++)
				{
					if (worldMap[y][x] > 0)
					{
						SDL_Rect wall = {x * UNIT,
								 y * UNIT, UNIT,
								 UNIT};
						SDL_SetRenderDrawColor(
						    renderer, 0x00, 0x00, 0x00,
						    0xFF);
						SDL_RenderFillRect(renderer,
								   &wall);

						// render horizontal and
						// vertical lines
						SDL_Rect line = {
						    x * UNIT, 0, UNIT,
						    mapHeight * UNIT};
						SDL_Rect line2 = {
						    0, y * UNIT,
						    mapWidth * UNIT, UNIT};
						SDL_SetRenderDrawColor(
						    renderer, 0x80, 0x80, 0x80,
						    0xFF);
						SDL_RenderDrawRect(renderer,
								   &line2);
						SDL_SetRenderDrawColor(
						    renderer, 0x80, 0x80, 0x80,
						    0xFF);
						SDL_RenderDrawRect(renderer,
								   &line);
						line.y = mapHeight * UNIT;
						SDL_RenderDrawRect(renderer,
								   &line);
					}
				}
			}
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00,
					       0xFF);

			/* player rect */
			playerRect.x = player.x - 4;
			playerRect.y = player.y - 4;
			SDL_RenderFillRect(renderer, &playerRect);

			// SDL_Rect offsetRect = {player.x + xo, player.y + yo,
			// 2, 		       2}; SDL_RenderFillRect(renderer,
			// &offsetRect);

			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF,
					       0xFF);
			// SDL_RenderDrawLine(renderer, player.x,
			// player.y, 		   player.x + pdeltaX *
			// 5, 		   player.y + pdeltaY * 5);
			//

			SDL_SetRenderDrawColor(renderer, 0x00, 0x50, 0xFF,
					       0x80);
			SDL_RenderFillRect(renderer, &ceilingRect);
			SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80,
					       0xFE);
			SDL_RenderFillRect(renderer, &floorRect);
			drawRays(renderer, player.x, player.y, pangle);
			SDL_RenderPresent(renderer);
		}
	}
	// Destroy window
	SDL_DestroyWindow(window);

	// Quit SDL subsystems
	SDL_Quit();

	return (0);
}
