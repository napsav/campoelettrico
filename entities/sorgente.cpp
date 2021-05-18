#include "sorgente.h"

#include <SDL2/SDL.h>

#include "../ui/imgui.h"
#include "vector.h"

bool isInside(float x, float y, float posx, float posy, float radius) {
  bool inside = true;
  if (x < (posx - radius)) {
    inside = false;
  } else if (x > (posx + radius)) {
    inside = false;
  } else if (y < (posy - radius)) {
    inside = false;
  } else if (y > (posy + radius)) {
    inside = false;
  }
  return inside;
}

Sorgente::Sorgente(vector2 pos, float coloumb) {
  posizione = pos;
  carica = coloumb;
}
vector2 Sorgente::getPosition() { return posizione; }
float Sorgente::getCharge() { return carica; }

void Sorgente::setCharge(float charge) { carica = charge; }

void Sorgente::setPosition(vector2 position) {
  posizione.x = position.x;
  posizione.y = position.y;
}

void Sorgente::handleEnvent(SDL_Event &e, int x, int y) {
  if (e.type == SDL_MOUSEBUTTONDOWN) {
    SDL_GetMouseState(&x, &y);
    if (e.button.button == SDL_BUTTON_LEFT) {
      if (isInside(x, y, posizione.x, posizione.y, 10)) {
        selected = true;
      }
    } else if (e.button.button == SDL_BUTTON_RIGHT) {
      if (isInside(x, y, posizione.x, posizione.y, 10)) {
        if (!windowOpen) {
          windowOpen = true;
        } else {
          windowOpen = false;
        }
      } else {
        windowOpen = false;
      }
    }
  } else if (e.type == SDL_MOUSEBUTTONUP) {
    if (e.button.button == SDL_BUTTON_LEFT) {
      if (selected) {
        selected = false;
      }
    }
  } else if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_z:
        if (isInside(x, y, posizione.x, posizione.y, 10)) {
          carica -= 0.1e-9;
          break;
        }
      case SDLK_x:
        if (isInside(x, y, posizione.x, posizione.y, 10)) {
          carica += 0.1e-9;
          break;
        }
    }
  }
}
