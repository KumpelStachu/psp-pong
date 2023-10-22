#pragma once
#include <SDL.h>
#include "defines.h"

//  _    6
// |_| 5 4 3
// |_| 2 1 0
const unsigned short digits[10] = {
    0b01101111, // 0
    0b00001001, // 1
    0b01011110, // 2
    0b01011011, // 3
    0b00111001, // 4
    0b01110011, // 5
    0b01110111, // 6
    0b01001001, // 7
    0b01111111, // 8
    0b01111011, // 9
};

void RenderDigit(SDL_Renderer *renderer, int x, int y, int digit)
{
  if (digit < 0 || digit > 9)
    return;

  unsigned short digit_mask = digits[digit];

  if (digit_mask & (1 << 6))
    SDL_RenderFillRect(renderer, &(SDL_Rect){x, y, DIGIT_WIDTH, DIGIT_STROKE});
  if (digit_mask & (1 << 4))
    SDL_RenderFillRect(renderer, &(SDL_Rect){x, y + (DIGIT_HEIGHT - DIGIT_STROKE) / 2, DIGIT_WIDTH, DIGIT_STROKE});
  if (digit_mask & (1 << 1))
    SDL_RenderFillRect(renderer, &(SDL_Rect){x, y + DIGIT_HEIGHT - DIGIT_STROKE, DIGIT_WIDTH, DIGIT_STROKE});

  if (digit_mask & (1 << 5))
    SDL_RenderFillRect(renderer, &(SDL_Rect){x, y + DIGIT_STROKE, DIGIT_STROKE, DIGIT_HEIGHT / 2});
  if (digit_mask & (1 << 2))
    SDL_RenderFillRect(renderer, &(SDL_Rect){x, y + (DIGIT_HEIGHT - DIGIT_STROKE) / 2, DIGIT_STROKE, (DIGIT_HEIGHT - DIGIT_STROKE) / 2 + 1});

  if (digit_mask & (1 << 3))
    SDL_RenderFillRect(renderer, &(SDL_Rect){x + DIGIT_WIDTH - DIGIT_STROKE, y, DIGIT_STROKE, DIGIT_HEIGHT / 2});
  if (digit_mask & (1 << 0))
    SDL_RenderFillRect(renderer, &(SDL_Rect){x + DIGIT_WIDTH - DIGIT_STROKE, y + (DIGIT_HEIGHT - DIGIT_STROKE) / 2, DIGIT_STROKE, (DIGIT_HEIGHT - DIGIT_STROKE) / 2 + 1});
}

void RenderScore(SDL_Renderer *renderer, int score_player, int score_computer)
{
  for (int i = 0; !i || score_player; i++, score_player /= 10)
    RenderDigit(renderer, SCREEN_WIDTH / 2 - DIGIT_OFFSET - (DIGIT_WIDTH + DIGIT_OFFSET) * (i + 1), DIGIT_OFFSET, score_player % 10);

  int size = (score_computer == 0) - 1;
  for (int i = score_computer; i; i /= 10)
    size++;

  for (int i = 0; !i || score_computer; i++, score_computer /= 10)
    RenderDigit(renderer, SCREEN_WIDTH / 2 + DIGIT_OFFSET * 2 + (DIGIT_WIDTH + DIGIT_OFFSET) * (size - i), DIGIT_OFFSET, score_computer % 10);
}
