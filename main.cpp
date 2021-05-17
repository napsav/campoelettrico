#include <SDL2/SDL.h>

#include <cmath>
#include <iostream>
#include <vector>

#include "./draw.h"
#include "./timer.h"
#include "entities/campoVettoriale.h"
#include "entities/charge.h"
#include "entities/settings.h"
#include "entities/sorgente.h"
#include "entities/vector.h"
#include "ui/imgui.h"
#include "ui/imgui_sdl.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
double scala = 0.000001;
const float costanteColoumb = 8.987551792314e9;
int densita = 16;
int lunghezza = 10;
int raggioSorgente = 10;
float maxCarica = 0.05;
int coloreBase = 0x14;
const float caricaDiProva = 1.602176634e-19;
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
void simulazioneCampo(std::vector<Sorgente>::iterator &itSorgenti,
                      std::vector<Carica>::iterator &it,
                      std::vector<Carica> &cariche) {
  vector2 intensita;
  float valoreCampo;
  vector2 distanzaVettore =
      distanza(itSorgenti->getPosition(), it->getPosition());

  if (distanzaVettore.modulo > raggioSorgente) {
    valoreCampo =
        costanteColoumb * (itSorgenti->getCharge() /
                           ((distanzaVettore.modulo * distanzaVettore.modulo)*scala)) * it->getCharge();
    std::cout << "Valore: " << valoreCampo << std::endl;
    intensita.x = valoreCampo * distanzaVettore.xNormalized;
    intensita.y = valoreCampo * distanzaVettore.yNormalized;
    intensita.intensita = valoreCampo;
    it->addForce(intensita);
  } else {
    cariche.erase(it);
    std::cout << "Cancellazione carica" << std::endl;
    it--;
  }
}
void simulazioneCampo(std::vector<Sorgente>::iterator &itSorgenti,
                      std::vector<PuntoDelCampo>::iterator &it) {
  vector2 intensita;
  float valoreCampo;
  vector2 distanzaVettore =
      distanza(itSorgenti->getPosition(), it->getPosition());

  valoreCampo =
      costanteColoumb * (itSorgenti->getCharge() /
                         (distanzaVettore.modulo * distanzaVettore.modulo));
  intensita.x = valoreCampo * distanzaVettore.xNormalized * scala;
  intensita.y = valoreCampo * distanzaVettore.yNormalized * scala;
  intensita.intensita = valoreCampo;
  it->addVector2(intensita);
}

int main() {
  if (!init()) {
    std::cout << "Init di SDL fallito" << std::endl;
    return 1;
  }
  bool quit = false;
  SDL_Event e;
  Timer stepTimer;
  bool open = true;
  bool pause = true;
  int x, y;
  std::vector<Sorgente> sorgenti;
  std::vector<Sorgente>::iterator itSorgenti;
  std::vector<PuntoDelCampo> punti;
  std::vector<PuntoDelCampo>::iterator it;
  std::vector<Carica>::iterator itCariche;
  std::vector<Carica> cariche;
  Sorgente sorgentePrima = Sorgente({200, 300}, -0.5e-9);
  Sorgente sorgenteSeconda = Sorgente({600, 300}, -3.5e-9);
  Sorgente sorgenteTerza = Sorgente({200, 600}, 0.1e-9);
  sorgenti.push_back(sorgentePrima);
  sorgenti.push_back(sorgenteSeconda);
  sorgenti.push_back(sorgenteTerza);
  ImGui::CreateContext();
  ImGuiSDL::Initialize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  // carica caricaDiProva = {{600, 300}, {}};
  setDensity(punti, densita);
  while (!quit) {
    ImGuiIO &io = ImGui::GetIO();
    int wheel = 0;
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
          case SDLK_i:
            open = true;
            break;
          case SDLK_SPACE:
            addCaricaFunc(cariche, x, y);
            break;
	  case SDLK_p:
	    pause = !pause;
	    break;
        }
      } else if (e.type == SDL_MOUSEBUTTONDOWN) {
      } else if (e.type == SDL_MOUSEWHEEL) {
        wheel = e.wheel.y;
      }
      for (itSorgenti = sorgenti.begin(); itSorgenti != sorgenti.end();
           itSorgenti++) {
        itSorgenti->handleEnvent(e, x, y);
      }
    }

    // ImGUI input handling
    const int buttons = SDL_GetMouseState(&x, &y);

    io.DeltaTime = 1.0f / 60.0f;
    io.MousePos = ImVec2(static_cast<float>(x), static_cast<float>(y));
    io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
    io.MouseWheel = static_cast<float>(wheel);

    // Finestra impostazioni

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Punti del campo vettoriale, ogni vettore rappresenta la forza totale
    // del/dei campi elettrico/i

    for (itSorgenti = sorgenti.begin(); itSorgenti != sorgenti.end();
         itSorgenti++) {
      if (itSorgenti->selected) {
        itSorgenti->setPosition(
            vector2{static_cast<float>(x), static_cast<float>(y)});
      }
      for (it = punti.begin(); it != punti.end(); it++) {
        simulazioneCampo(itSorgenti, it);
      }

      // Simulazione delle cariche di prova, non definitivo (da unire con
      // l'altra funzione o creare classe base)

      if (cariche.size() > 0) {
        for (itCariche = cariche.begin(); itCariche != cariche.end();
             itCariche++) {
          simulazioneCampo(itSorgenti, itCariche, cariche);
        }
      }
      SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      DrawCircle(gRenderer, itSorgenti->getPosition().x,
                 itSorgenti->getPosition().y, raggioSorgente);

      //          ImGui::Begin("Sorgente", &itSorgenti->windowOpen);
      //          ImGui::SliderFloat("Carica", &itSorgenti->carica,
      //          -10.0e-9, 10.0e-9); ImGui::End();
    }

    // --------------------
    // 	IMGUI
    // --------------------

    ImGui::NewFrame();
    ImGui::Begin("Impostazioni", &open);
    ImGui::Text("Densità campo vettoriale");
    if (ImGui::SliderInt("densità", &densita, 4, 64))
      setDensity(punti, densita);
    ImGui::Text("Lunghezza vettori normalizzati");
    ImGui::SliderInt("lunghezza", &lunghezza, 1, 30);
    ImGui::Text("Raggio rappresentazione sorgenti");
    ImGui::SliderInt("raggio", &raggioSorgente, 1, 30);
    ImGui::Text("Max Carica (Rendering)");
    ImGui::SliderFloat("maxCarica", &maxCarica, 0.001, 0.1);
    ImGui::Text("Colore base (Rendering)");
    ImGui::SliderInt("coloreBase",&coloreBase, 0, 255);
    ImGui::End();

    // Simulazione movimento cariche di prova

    float dt = stepTimer.getTicks() / 1000.f;
    if (!pause && cariche.size() > 0) {
      for (itCariche = cariche.begin(); itCariche != cariche.end();
           itCariche++) {
        itCariche->computeForces();
        itCariche->updatePosition(dt);
      }
    }
    stepTimer.start();

    // Rendering cariche di prova

    if (cariche.size() > 0) {
      for (itCariche = cariche.begin(); itCariche != cariche.end();
           itCariche++) {
        itCariche->render(gRenderer);
        itCariche->emptyVectors();
      }
    }

    // Rendering campo vettoriale

    for (it = punti.begin(); it != punti.end(); it++) {
      it->computeVectors();
      it->render(gRenderer);
      it->emptyVectors();
    }

    // Rendering ImGUI

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
