#include "draw.h"
#include "timer.h"
#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <vector>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

const float costanteColoumb = 8.987551792314e9;
const float densita = 12;
const float angoloIniziale = 3.1415 / 3;
const float velocitaIniziale = 0;

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

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

void simulazione(float &v, float &a, float dt) {
  float acc;
  //acc = accelerazionePendolo(v, a);

  a += v * dt;
  v += acc * dt;

  //std::cout << "Acc: " << acc << " Angolo: " << a << " Velocità: " << v << std::endl;
}

void velocitaMouse(float x, float y, float posx, float posy, float &vel) {
  vel += (x - posx) / 30;
  std::cout << "X: " << posx << " Y: " << y << " Velocità applicata: " << vel << std::endl;
}

bool init() {
  bool success = true;
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
    printf("SDL could not be initialized, error: %s", SDL_GetError());
    success = false;
  } else {
    gWindow = SDL_CreateWindow("Campo Elettrostatico", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
      printf("Window could not be initialized, error: %s\n", SDL_GetError());
      success = false;
    } else {
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      if (gRenderer == NULL) {
        printf("Renderer could not be initialized, error: %s\n", SDL_GetError());
      } else {
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      }
    }
  }
  return success;
}

struct vector2 {
  float x = 0;
  float y = 0;
  float modulo = 0;
};

struct carica {
  vector2 posizione;
  float modulo = 0;
};

vector2 normalize(vector2 daNormalizzare) {
  vector2 normalizzato;
  float modulo = sqrt((daNormalizzare.x * daNormalizzare.x) + (daNormalizzare.y * daNormalizzare.y));
  normalizzato.x = daNormalizzare.x / modulo;
  normalizzato.y = daNormalizzare.y / modulo;
  normalizzato.modulo = modulo;
  return normalizzato;
}

vector2 distanza(vector2 a, vector2 b) {
  vector2 distanzaNormalizzata;
  distanzaNormalizzata.x = -a.x + b.x;
  distanzaNormalizzata.y = -a.y + b.y;
  distanzaNormalizzata.modulo;
  return normalize(distanzaNormalizzata);
}

int main() {
  if (!init()) {
    std::cout << "Init di SDL fallito" << std::endl;
    return 1;
  }
  bool quit = false;
  SDL_Event e;
  Timer stepTimer;
  float lunghezza = 30;
  bool lpress = false;
  int x, y;
  std::vector<vector2> punti;
  std::vector<vector2>::iterator it;
  carica sorgentePrima = {{200, 300}, 2.5e-9};
  //carica caricaDiProva = {{600, 300}, {}};
  for (unsigned int i = 0; i < SCREEN_WIDTH; i = i + densita) {
    for (unsigned int j = 0; j < SCREEN_HEIGHT; j = j + densita) {

      punti.push_back(*new vector2{(float)i, (float)j});
    }
  }
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {

      if (e.type == SDL_QUIT) {
        quit = true;
      } else if (e.type == SDL_MOUSEBUTTONDOWN) {
        SDL_GetMouseState(&x, &y);
        if (e.button.button == SDL_BUTTON_LEFT && isInside(x, y, sorgentePrima.posizione.x, sorgentePrima.posizione.y, 20)) {
          lpress = true;
          printf("Inside left\n");
        }
      } else if (e.type == SDL_MOUSEBUTTONUP) {
        if (e.button.button == SDL_BUTTON_LEFT && lpress) {
          SDL_GetMouseState(&x, &y);
          //velocitaMouse(x, y, (SCREEN_WIDTH / 2) + sorgentePrima.x, (SCREEN_HEIGHT / 4) + sorgentePrima.y, velocita.x);
          lpress = false;
        }
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
          lunghezza += 10;
          break;
        case SDLK_DOWN:
          lunghezza -= 10;
          break;
        case SDLK_z:
          sorgentePrima.modulo -= 0.1e-9;
          break;
        case SDLK_x:
          sorgentePrima.modulo += 0.1e-9;
          break;
        }
      }
    }
    SDL_GetMouseState(&x, &y);

    // SIMULAZIONE

    float dt = stepTimer.getTicks() / 1000.f;

    stepTimer.start();

    // RENDERING

    if (lpress) {
      sorgentePrima.posizione.x = x;
      sorgentePrima.posizione.y = y;
    }

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
    //SDL_RenderDrawLine(gRenderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, (SCREEN_WIDTH / 2) + sorgentePrima.x, (SCREEN_HEIGHT / 4) + sorgentePrima.y);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Punti di prova

    for (it = punti.begin(); it != punti.end(); it++) {
      vector2 distanzaPunto = distanza(sorgentePrima.posizione, *it);
      float intensita;
      if (distanzaPunto.modulo != 0) {
        intensita = abs(costanteColoumb * (sorgentePrima.modulo / (distanzaPunto.modulo * distanzaPunto.modulo)));
        it->modulo = intensita;
      } else {
        intensita = 0;
        it->modulo = 0xFF;
      }
      float percentuale;
      if (intensita * 100000 > 0xFF)
        percentuale = 0xFF;
      else if (intensita * 100000 < 0x20)
        percentuale = 0x20;
      else
        percentuale = abs(intensita * 100000);
      if (sorgentePrima.modulo > 0) {
        SDL_SetRenderDrawColor(gRenderer, 0x00, percentuale, 0x00, 0xFF);
        SDL_RenderDrawLine(gRenderer, it->x, it->y, it->x + distanzaPunto.x * lunghezza, it->y + distanzaPunto.y * lunghezza);
      } else {
        SDL_SetRenderDrawColor(gRenderer, percentuale, 0x00, 0x00, 0xFF);
        SDL_RenderDrawLine(gRenderer, it->x, it->y, it->x - distanzaPunto.x * lunghezza, it->y - distanzaPunto.y * lunghezza);
      }

      SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
      //DrawCircle(gRenderer, it->x, it->y, 5);
    }
    DrawCircle(gRenderer, sorgentePrima.posizione.x, sorgentePrima.posizione.y, 10);
    SDL_RenderPresent(gRenderer);
  }

  SDL_Quit();
  return 0;
}
