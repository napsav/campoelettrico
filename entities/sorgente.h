#pragma once
#include "vector.h"
#include <SDL2/SDL.h>

class Sorgente {
public:
  Sorgente(vector2 pos, float coloumb);
  vector2 getPosition();
  float getCharge();
  void setCharge(float charge);
  void setPosition(vector2 position);
  bool selected = false;
  void handleEnvent(SDL_Event &e, int x, int y);
  bool windowOpen = false;
  void spawnLinee();
  void render(SDL_Renderer *renderer);

private:
  vector2 posizione;
  float carica;
};
