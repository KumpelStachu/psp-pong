#include <SDL.h>
#include "defines.h"
#include "score.h"

int main(int argc, char *argv[])
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

  SDL_Window *window = SDL_CreateWindow("pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  int running = 1;
  SDL_Event event;

  SDL_FRect paddle_player = {PADDLE_OFFSET, (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2, PADDLE_WIDTH, PADDLE_HEIGHT};
  SDL_FRect paddle_computer = {SCREEN_WIDTH - PADDLE_WIDTH - PADDLE_OFFSET, (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2, PADDLE_WIDTH, PADDLE_HEIGHT};
  SDL_FRect ball = {(SCREEN_WIDTH - BALL_SIZE) / 2, (SCREEN_HEIGHT - BALL_SIZE) / 2 + 100, BALL_SIZE, BALL_SIZE};

  int score_player = 0;
  int score_computer = 0;
  int player_velocity = 0;
  double ball_velocity_x = BALL_VELOCITY;
  double ball_velocity_y = BALL_VELOCITY;

  while (running)
  {
    if (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        running = 0;
        break;
      case SDL_CONTROLLERDEVICEADDED:
        SDL_GameControllerOpen(event.cdevice.which);
        break;
      case SDL_CONTROLLERBUTTONDOWN:
        switch (event.cbutton.button)
        {
        case SDL_CONTROLLER_BUTTON_START:
          running = 0;
          break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
          player_velocity += PADDLE_SPEED;
          break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
          player_velocity -= PADDLE_SPEED;
          break;
        }
        break;
      case SDL_CONTROLLERBUTTONUP:
        switch (event.cbutton.button)
        {
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
          player_velocity -= PADDLE_SPEED;
          break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
          player_velocity += PADDLE_SPEED;
          break;
        }
        break;
      }
    }

    // calculate computer paddle movement
    if (ball_velocity_x > 0 && ball.x > SCREEN_WIDTH / 2)
    {
      int diff = paddle_computer.y - (ball.y + BALL_SIZE / 2 - PADDLE_HEIGHT / 2);
      if (diff < 0)
        paddle_computer.y += PADDLE_SPEED;
      else if (diff > 0)
        paddle_computer.y -= PADDLE_SPEED;
    }

    // move player paddle
    paddle_player.y += player_velocity;

    // clamp paddles to screen
    paddle_player.y = SDL_clamp(paddle_player.y, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);
    paddle_computer.y = SDL_clamp(paddle_computer.y, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);

    // move ball
    ball.x += ball_velocity_x;
    ball.y += ball_velocity_y;

    // bounce ball off top and bottom
    if (ball.y < 0 || ball.y + BALL_SIZE > SCREEN_HEIGHT)
      ball_velocity_y = -ball_velocity_y;

    // bounce ball off paddles
    if (SDL_HasIntersectionF(&ball, &paddle_player) || SDL_HasIntersectionF(&ball, &paddle_computer))
    {
      ball_velocity_x = -ball_velocity_x;
      ball_velocity_x *= BALL_ACCELERATION;
      ball_velocity_y *= BALL_ACCELERATION;
    }

    // check if ball is out of bounds
    if (ball.x < 0 || ball.x + BALL_SIZE > SCREEN_WIDTH)
    {
      if (ball.x < 0)
        score_computer++;
      else
        score_player++;

      paddle_computer.y = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
      paddle_player.y = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
      ball.x = (SCREEN_WIDTH - BALL_SIZE) / 2;
      ball.y = (SCREEN_HEIGHT - BALL_SIZE) / 2;
      ball_velocity_x = BALL_VELOCITY;
      ball_velocity_y = BALL_VELOCITY;
    }

    // clear screen
    SDL_RenderClear(renderer);

    // draw objects
    SDL_SetRenderDrawColor(renderer, DARK_GRAY);
    SDL_RenderFillRect(renderer, &(SDL_Rect){SCREEN_WIDTH / 2, 0, 1, SCREEN_HEIGHT});

    SDL_SetRenderDrawColor(renderer, LIGHT_GRAY);
    RenderScore(renderer, score_player, score_computer);

    SDL_SetRenderDrawColor(renderer, WHITE);
    SDL_RenderFillRectF(renderer, &paddle_player);
    SDL_RenderFillRectF(renderer, &paddle_computer);
    SDL_RenderFillRectF(renderer, &ball);

    // update screen
    SDL_SetRenderDrawColor(renderer, BLACK);
    SDL_RenderPresent(renderer);
  }

  return 0;
}
