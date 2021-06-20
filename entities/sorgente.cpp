#include "sorgente.h"

#include <SDL2/SDL.h>

#include "../draw.h"
#include "../ui/imgui.h"
#include "lineeDiForza.h"
#include "settings.h"
#include "vector.h"
#define COLORE(color) \
  color[0] * 0xFF, color[1] * 0xFF, color[2] * 0xFF, color[3] * 0xFF

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

void Sorgente::spawnLinee() {
}

void Sorgente::render(SDL_Renderer *renderer) {
  if (sorgentiColoreSegno) {
    if (carica >= 0) {
      SDL_SetRenderDrawColor(renderer, 0x00, 0xBF, 0x00, 0xFF);
    } else {
      SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    }
  } else {
    SDL_SetRenderDrawColor(renderer, COLORE(coloreSorgente));
  }
  DrawCircle(renderer, posizione.x, posizione.y, raggio);
}

void Sorgente::handleEnvent(SDL_Event &e, int x, int y) {
  if (e.type == SDL_MOUSEBUTTONDOWN) {
    SDL_GetMouseState(&x, &y);
    if (e.button.button == SDL_BUTTON_LEFT) {
      if (isInside(x, y, posizione.x, posizione.y, raggio)) {
        selected = true;
      }
    } else if (e.button.button == SDL_BUTTON_RIGHT) {
      this->spawnLinee();
      if (isInside(x, y, posizione.x, posizione.y, raggio)) {
        if (windowOpen) {
          windowOpen = false;
        } else {
          windowOpen = true;
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
