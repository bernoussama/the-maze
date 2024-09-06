#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#define PI 3.14159265358979323846

// Key press surfaces constants

#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

int worldMap[mapWidth][mapHeight] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

/* Screen dimension constants */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

void drawRays(SDL_Renderer *renderer, double px, double py,
              double playerAngle) {
  int ray, dof, mX, mY;
  double rayX, rayY, rayAngle, xoffset, yoffset;
  rayAngle = playerAngle;
  for (ray = 0; ray < 1; ray++) {
    dof = 0;
    double aTan = -1 / tan(rayAngle);
    if (rayAngle > PI) {
      rayY = (((int)py >> 6) << 6) - 0.0001;
      rayX = px + (py - rayY) * aTan;
      yoffset = -64;
      xoffset = -yoffset * aTan;
    }
    if (rayAngle < PI) {
      rayY = (((int)py >> 6) << 6) + 64;
      rayX = px + (py - rayY) * aTan;
      yoffset = 64;
      xoffset = -yoffset * aTan;
    }
    if (rayAngle == 0 || rayAngle == PI) {
      rayX = px;
      rayY = py;
      dof = 8;
    }

    while (dof < 8) {
      mX = (int)rayX >> 6;
      mY = (int)rayY >> 6;
      if (mX >= 0 && mX < mapWidth && mY >= 0 && mY < mapHeight &&
          worldMap[mX][mY] > 0) {
        dof = 8;
        break;
      } else {
        rayX += xoffset;
        rayY += yoffset;
        dof += 1;
      }
    }

    // set green color
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderDrawLine(renderer, px, py, rayX, rayY);
    // SDL_RenderPresent(renderer);
  }
}

int main(void) {
  /* window will be rendering to */
  SDL_Window *window;
  SDL_Renderer *renderer;

  /* The surface contained by the window */
  SDL_Surface *screenSurface = NULL;

  double w = SCREEN_WIDTH, h = SCREEN_HEIGHT;
  double posX = 22, posY = 12;     // player x and y start position
  double pdeltaX = 0, pdeltaY = 0; // player direction vector
  double pangle = 0;               // player angle
  double dirX = -1, dirY = 0;      // initial direction vector
  double planeX = 0,
         planeY = 0.66; // the 2d raycaster version of camera plane

  double time = 0;    // time of current frame
  double oldTime = 0; // time of previous frame

  /* Initialize SDL */
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    return (1);
  }

  /* Create a new window instance */
  window =
      SDL_CreateWindow("SDL2 raycaster", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (window == NULL) {
    fprintf(stderr, "SDL_Createwindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return (1);
  }

  /* create a new renderer instance linked to the Window */
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    SDL_DestroyWindow(window);
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
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
  // SDL_Rect fillRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH /
  // 2,
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
  // Draw the map
  for (int y = 0; y < mapHeight; y++) {
    for (int x = 0; x < mapWidth; x++) {
      if (worldMap[x][y] > 0) {
        SDL_Rect wall = {x * 20, y * 20, 20, 20}; // Adjust size as needed
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00,
                               0xFF); // Black color for walls
        SDL_RenderFillRect(renderer, &wall);
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
  while (quit == false) {
    while (SDL_PollEvent(&e)) {
      printf("Event type: %d\n", e.type);
      if (e.type == SDL_QUIT) {
        printf("Event quit: %d\n", e.type);
        quit = true;
      }
      // User presses a key
      else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
          player.x += pdeltaX;
          player.y += pdeltaY;
          break;
        case SDLK_DOWN:
          player.x -= pdeltaX;
          player.y -= pdeltaY;
          break;
        case SDLK_LEFT:
          pangle -= 0.1; // Rotate the player left
          if (pangle < 0)
            pangle += 2 * PI;
          pdeltaX = cos(pangle) * 5;
          pdeltaY = sin(pangle) * 5;
          break;
        case SDLK_RIGHT:
          pangle += 0.1; // Rotate the player left
          if (pangle > 2 * PI)
            pangle -= 2 * PI;
          pdeltaX = cos(pangle) * 5;
          pdeltaY = sin(pangle) * 5;
          break;
        default:
          continue;
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        // Draw the map
        for (int x = 0; x < mapWidth; x++) {
          for (int y = 0; y < mapHeight; y++) {
            if (worldMap[x][y] > 0) {
              SDL_Rect wall = {x * 20, y * 20, 20, 20}; // Adjust size as needed
              SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00,
                                     0xFF); // Black color for walls
              SDL_RenderFillRect(renderer, &wall);
            }
          }
        }
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(renderer, &player);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_RenderDrawLine(renderer, player.x, player.y, player.x + pdeltaX * 5,
                           player.y + pdeltaY * 5);
        drawRays(renderer, player.x, player.y, pangle);
        SDL_RenderPresent(renderer);
      }
    }
  }
  // Destroy window
  SDL_DestroyWindow(window);

  // Quit SDL subsystems
  SDL_Quit();

  return (0);
}
