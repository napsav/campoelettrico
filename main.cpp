#include <SDL2/SDL.h>

#include <cmath>
#include <iostream>
#include <vector>
#include "ui/imgui_sdl.h"
#include "ui/imgui.h"

#include "./draw.h"
#include "./timer.h"
#include "entities/campoVettoriale.h"
#include "entities/charge.h"
#include "entities/settings.h"
#include "entities/sorgente.h"
#include "entities/vector.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
double scala = 0.000001;
const float costanteColoumb = 8.987551792314e9;
int densita = 32;
int lunghezza = 10;
const float caricaDiProva = 1.602176634e-5;
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

bool init() {
  bool success = true;
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
    printf("SDL could not be initialized, error: %s", SDL_GetError());
    success = false;
  } else {
    gWindow = SDL_CreateWindow("Campo Elettrostatico", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
      printf("Window could not be initialized, error: %s\n", SDL_GetError());
      success = false;
    } else {
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      if (gRenderer == NULL) {
        printf("Renderer could not be initialized, error: %s\n",
               SDL_GetError());
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
      punti.push_back(
          *new PuntoDelCampo(static_cast<float>(i), static_cast<float>(j)));
    }
  }
}

void addSorgenteFunc(std::vector<Sorgente> &array) {
  array.push_back(
      *new Sorgente({static_cast<float>(array.size() * 50), 200}, 1.5e-9));
}

void addCaricaFunc(std::vector<Carica> &array, int x, int y) {
  array.push_back(
      *new Carica(static_cast<float>(x), static_cast<float>(y), caricaDiProva));
  std::cout << "aggiungo carica con posizione x " << x << " e posizione y " << y
            << std::endl;
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
  std::vector<Carica>::iterator itCariche;
  std::vector<Carica> cariche;
  Sorgente sorgentePrima = Sorgente({200, 300}, -0.5e-9);
  Sorgente sorgenteSeconda = Sorgente({400, 300}, -3.5e-9);
  Sorgente sorgenteTerza = Sorgente({200, 600}, 0.1e-9);
  sorgenti.push_back(sorgentePrima);
  sorgenti.push_back(sorgenteSeconda);
  sorgenti.push_back(sorgenteTerza);
  	ImGui::CreateContext();
	ImGuiSDL::Initialize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  // carica caricaDiProva = {{600, 300}, {}};
  setDensity(punti, densita);
  while (!quit) {
    SDL_GetMouseState(&x, &y);
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_n:
            addSorgenteFunc(sorgenti);
            break;
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
          case SDLK_r:
            sorgenti.clear();
            break;
        }
      } else if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_RIGHT) {
          addCaricaFunc(cariche, x, y);
        }
      }
      for (itSorgenti = sorgenti.begin(); itSorgenti != sorgenti.end();
           itSorgenti++) {
        itSorgenti->handleEnvent(e, x, y);
      }
    }

    // SIMULAZIONE

    // RENDERING

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
    // SDL_RenderDrawLine(gRenderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4,
    // (SCREEN_WIDTH / 2) + itSorgenti->x, (SCREEN_HEIGHT / 4) + itSorgenti->y);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    vector2 distanzaVettore;
    // Punti di prova
    for (itSorgenti = sorgenti.begin(); itSorgenti != sorgenti.end();
         itSorgenti++) {
      if (itSorgenti->selected) {
        itSorgenti->setPosition(
            vector2{static_cast<float>(x), static_cast<float>(y)});
      }
      for (it = punti.begin(); it != punti.end(); it++) {
        vector2 intensita;
        float valoreCampo;
        distanzaVettore =
            distanza(itSorgenti->getPosition(), it->getPosition());

        valoreCampo = costanteColoumb *
                      (itSorgenti->getCharge() /
                       (distanzaVettore.modulo * distanzaVettore.modulo));
        intensita.x = valoreCampo * distanzaVettore.xNormalized * scala;
        intensita.y = valoreCampo * distanzaVettore.yNormalized * scala;
        intensita.intensita = valoreCampo;
        it->addVector2(intensita);
      }

      SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      DrawCircle(gRenderer, itSorgenti->getPosition().x,
                 itSorgenti->getPosition().y, 10);
      if (cariche.size() > 0) {
        for (itCariche = cariche.begin(); itCariche != cariche.end();
             itCariche++) {
          std::cout << "Inizio calcolo forze" << std::endl;
          vector2 intensita;
          float valoreCampo;
          std::cout << "Posizione precalcolo x: " << itCariche->getPosition().x
                    << std::endl;

          std::cout << "Posizione precalcolo y: " << itCariche->getPosition().y
                    << std::endl;
          distanzaVettore =
              distanza(itSorgenti->getPosition(), itCariche->getPosition());
          std::cout << "Distanza: " << distanzaVettore.modulo << std::endl;
          if (distanzaVettore.modulo > 4) {
            valoreCampo = costanteColoumb *
                          (itSorgenti->getCharge() /
                           (distanzaVettore.modulo * distanzaVettore.modulo));
            std::cout << "Valore: " << valoreCampo << std::endl;
            intensita.x = valoreCampo * distanzaVettore.xNormalized * scala;
            intensita.y = valoreCampo * distanzaVettore.yNormalized * scala;
            intensita.intensita = valoreCampo;
            itCariche->addForce(intensita);
          } else {
            cariche.erase(itCariche);
            std::cout << "Cancellazione carica" << std::endl;
            // cariche.clear();
            itCariche--;
          }
        }
      }
    }

    float dt = stepTimer.getTicks() / 1000.f;
    if (cariche.size() > 0) {
      for (itCariche = cariche.begin(); itCariche != cariche.end();
           itCariche++) {
        itCariche->computeForces();
        itCariche->updatePosition(dt);
      }
    }
    stepTimer.start();

    if (cariche.size() > 0) {
      for (itCariche = cariche.begin(); itCariche != cariche.end();
           itCariche++) {
        itCariche->render(gRenderer);
        itCariche->emptyVectors();
      }
    }
    for (it = punti.begin(); it != punti.end(); it++) {
      it->computeVectors();
      it->render(gRenderer);
      it->emptyVectors();
    }
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
    SDL_RenderPresent(gRenderer);
  }
	ImGuiSDL::Deinitialize();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	ImGui::DestroyContext();
  SDL_Quit();
  return 0;
}
