#include "draw.h"
#include "entities/campoVettoriale.h"
#include "entities/settings.h"
#include "entities/sorgente.h"
#include "entities/vector.h"
#include "timer.h"
#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <vector>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

const float costanteColoumb = 8.987551792314e9;
int densita = 32;
int lunghezza = 10;
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

// void simulazione(float &v, float &a, float dt) {
//   float acc;
//   //acc = accelerazionePendolo(v, a);

//   a += v * dt;
//   v += acc * dt;

//   //std::cout << "Acc: " << acc << " Angolo: " << a << " Velocità: " << v << std::endl;
// }

// void velocitaMouse(float x, float y, float posx, float posy, float &vel) {
//   vel += (x - posx) / 30;
//   std::cout << "X: " << posx << " Y: " << y << " Velocità applicata: " << vel << std::endl;
// }

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

void setDensity(std::vector<PuntoDelCampo> &punti, float densita) {
  punti.clear();
  for (unsigned int i = 0; i < SCREEN_WIDTH; i = i + densita) {
    for (unsigned int j = 0; j < SCREEN_HEIGHT; j = j + densita) {
      punti.push_back(*new PuntoDelCampo((float)i, (float)j));
    }
  }
}

void addSorgenteFunc(std::vector<Sorgente> &array) {
  array.push_back(*new Sorgente({(float)array.size() * 50, 200}, 1.5e-9));
}

int main() {
  if (!init()) {
    std::cout << "Init di SDL fallito" << std::endl;
    return 1;
  }
  bool quit = false;
  SDL_Event e;
  Timer stepTimer;

  int x, y;
  std::vector<Sorgente> sorgenti;
  std::vector<Sorgente>::iterator itSorgenti;
  std::vector<PuntoDelCampo> punti;
  std::vector<PuntoDelCampo>::iterator it;
  Sorgente sorgentePrima = Sorgente({200, 300}, -0.5e-9);
  Sorgente sorgenteSeconda = Sorgente({400, 300}, -3.5e-9);
  Sorgente sorgenteTerza = Sorgente({200, 600}, 0.1e-9);
  sorgenti.push_back(sorgentePrima);
  sorgenti.push_back(sorgenteSeconda);
  sorgenti.push_back(sorgenteTerza);
  //carica caricaDiProva = {{600, 300}, {}};
  setDensity(punti, densita);
  while (!quit) {

    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_n:
          addSorgenteFunc(sorgenti);
        case SDLK_UP:
          lunghezza += 10;
          break;
        case SDLK_DOWN:
          lunghezza -= 10;
          break;
        case SDLK_RIGHT:
          densita += 2;
          setDensity(punti, densita);
          break;
        case SDLK_LEFT:
          densita -= 2;
          setDensity(punti, densita);
          break;
        }
      }
      SDL_GetMouseState(&x, &y);
      for (itSorgenti = sorgenti.begin(); itSorgenti != sorgenti.end(); itSorgenti++) {
        itSorgenti->handleEnvent(e, x, y);
      }
    }

    // SIMULAZIONE

    //float dt = stepTimer.getTicks() / 1000.f;

    //stepTimer.start();

    // RENDERING

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
    //SDL_RenderDrawLine(gRenderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, (SCREEN_WIDTH / 2) + itSorgenti->x, (SCREEN_HEIGHT / 4) + itSorgenti->y);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    vector2 intensita;
    vector2 distanzaVettore;
    // Punti di prova
    for (itSorgenti = sorgenti.begin(); itSorgenti != sorgenti.end(); itSorgenti++) {

      if (itSorgenti->selected) {
        itSorgenti->setPosition(vector2{(float)x, (float)y});
      }
      for (it = punti.begin(); it != punti.end(); it++) {
        distanzaVettore = distanza(itSorgenti->getPosition(), it->getPosition());

        float valoreCampo = costanteColoumb * (itSorgenti->getCharge() / (distanzaVettore.modulo * distanzaVettore.modulo));
        intensita.x = valoreCampo * distanzaVettore.xNormalized;
        intensita.y = valoreCampo * distanzaVettore.yNormalized;
        intensita.intensita = valoreCampo;
        it->addVector2(intensita);

        // float percentuale;
        // if (intensita * 100000 > 0xFF)
        //   percentuale = 0xFF;
        // else if (intensita * 100000 < 0x20)
        //   percentuale = 0x20;
        // else
        //   percentuale = abs(intensita * 100000);

        //DrawCircle(gRenderer, it->x, it->y, 5);
      }
      //SDL_RenderDrawLine(gRenderer, it->x, it->y, it->x + distanzaVettore.x * lunghezza, it->y + distanzaVettore.y * lunghezza);

      SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      DrawCircle(gRenderer, itSorgenti->getPosition().x, itSorgenti->getPosition().y, 10);
    }

    for (it = punti.begin(); it != punti.end(); it++) {
      it->computeVectors();
      it->render(gRenderer);
      it->emptyVectors();
    }
    SDL_RenderPresent(gRenderer);
  }

  SDL_Quit();
  return 0;
}
