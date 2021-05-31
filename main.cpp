#include <SDL2/SDL.h>

#include "ui/imgui_impl_sdl.h"
#include <cmath>
#include <iostream>
#include <vector>
#define LOGVECTOR(vector) std::cout << "Vettore\tX: " vector.getPosition().x << "\tY: " << vector.getPosition().y << std::endl
#define LOGXY(x, y) std::cout << "Coppia\tX: " << x << "\tY: " << y << std::endl
#define COLORE(color) color[0] * 0xFF, color[1] * 0xFF, color[2] * 0xFF, color[3] * 0xFF
#include "./draw.h"
#include "./timer.h"
#include "entities/campoVettoriale.h"
#include "entities/charge.h"
#include "entities/lineeDiForza.h"
#include "entities/settings.h"
#include "entities/sorgente.h"
#include "entities/vector.h"
#include "fisica.h"
#include "graph.h"
#include "ui/imgui.h"
#include "ui/imgui_sdl.h"
float costanteColoumb = 8.987551792314e9;
unsigned int SCREEN_HEIGHT = 720;
unsigned int SCREEN_WIDTH = 1280;
float massa = 1e2;
float coloreSfondo[4];
float coloreCarica[4] = {0, 0, 1, 1};
float coloreSorgente[4] = {1, 1, 1, 1};
float coloreGrCariche[4] = {1, 1, 1, 1};
int scala = 500;
int densita = 16;
int lunghezza = 10;
int raggio = 10;
float maxCarica = 0.05;
int coloreBase = 0x14;
bool drawGrid = true;
bool drawCampoVettoriale = true;
const float caricaDiProva = 1.602176634e-19;
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

bool init() {
  bool success = true;
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
    printf("SDL could not be initialized, error: %s\n", SDL_GetError());
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
        printf(
            "Renderer could not be initialized, error: "
            "%s\n",
            SDL_GetError());
      } else {
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
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
      *new Carica(static_cast<float>(x), static_cast<float>(y), caricaDiProva, massa));
  LOGXY(x, y);
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
  Graph grafico;
  std::vector<Sorgente> sorgenti;
  std::vector<Sorgente>::iterator itSorgenti;
  std::vector<PuntoDelCampo> punti;
  std::vector<PuntoDelCampo>::iterator it;
  std::vector<Carica>::iterator itCariche;
  std::vector<Carica> cariche;
  Sorgente sorgentePrima = Sorgente({200, 300}, 2.5e-9);
  Sorgente sorgenteSeconda = Sorgente({350, 300}, 5.2e-8);
  sorgenti.push_back(sorgentePrima);
  sorgenti.push_back(sorgenteSeconda);
  ImGui::CreateContext();
  ImGuiSDL::Initialize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  // carica caricaDiProva = {{600, 300}, {}};
  setDensity(punti, densita);
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui_ImplSDL2_InitTest(gWindow);
  io.WantCaptureKeyboard = true;
  CaricaLineaDiForza linea(280, 300);
  while (!quit) {

    int wheel = 0;
    SDL_GetMouseState(&x, &y);
    while (SDL_PollEvent(&e) != 0) {
      ImGui_ImplSDL2_ProcessEvent(&e);
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
          open = !open;
          break;
        case SDLK_k:
          grafico.puntiDelGrafico.clear();
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
    io.DeltaTime = 1.0f / 1000.0f;
    io.MousePos = ImVec2(static_cast<float>(x), static_cast<float>(y));
    io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
    io.MouseWheel = static_cast<float>(wheel);

    // Finestra impostazioni

    // --------------------
    // 	IMGUI
    // --------------------

    ImGui::NewFrame();
    ImGui::Begin("Impostazioni", &open);
    if (ImGui::CollapsingHeader("Opzioni principali", ImGuiTreeNodeFlags_None)) {

      ImGui::Text("Massa cariche di prova");
      ImGui::InputFloat("massa", &massa, 1e-5f, 1000.0f, "%e");
      ImGui::Text("Densità campo vettoriale");
      if (ImGui::SliderInt("densità", &densita, 4, 64))
        setDensity(punti, densita);
      ImGui::Text("Lunghezza vettori normalizzati");
      ImGui::SliderInt("lunghezza", &lunghezza, 1, 30);
      ImGui::Text("Raggio rappresentazione sorgenti");
      ImGui::SliderInt("raggio", &raggio, 1, 30);
    }
    if (ImGui::CollapsingHeader("Opzioni visuali", ImGuiTreeNodeFlags_None)) {
      ImGui::Text("Max Carica (Rendering)");
      ImGui::InputFloat("maxCarica", &maxCarica, 0.001, 0.1, "%e");
      ImGui::Text("Colore base (Rendering)");
      ImGui::SliderInt("coloreBase", &coloreBase, 0, 255);
      ImGui::Text("Scala pixel (Rendering)");
      ImGui::SliderInt("scala", &scala, 1, 1000);
      ImGui::Checkbox("Griglia", &drawGrid);
      ImGui::Checkbox("Campo vettoriale", &drawCampoVettoriale);
      ImGui::ColorEdit4("Colore sfondo", &coloreSfondo[0]);
      ImGui::ColorEdit4("Colore cariche", &coloreCarica[0]);
      ImGui::ColorEdit4("Colore sorgente", &coloreSorgente[0]);
      ImGui::ColorEdit4("Colore grafico cariche", &coloreGrCariche[0]);
    }

    ImGui::End();

    SDL_SetRenderDrawColor(gRenderer, COLORE(coloreSfondo));
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Punti del campo vettoriale, ogni vettore rappresenta la forza
    // totale del/dei campi elettrico/i
    unsigned int index;
    for (itSorgenti = sorgenti.begin(); itSorgenti != sorgenti.end();
         itSorgenti++) {
      index = itSorgenti - sorgenti.begin();
      if (itSorgenti->selected) {
        itSorgenti->setPosition(
            vector2{static_cast<float>(x), static_cast<float>(y)});
      }
      if (drawCampoVettoriale) {
        for (it = punti.begin(); it != punti.end(); it++) {
          simulazioneCampo(itSorgenti, it);
        }
      }

      // Simulazione delle cariche di prova, non definitivo
      // (da unire con l'altra funzione o creare classe base)

      if (cariche.size() > 0) {
        for (itCariche = cariche.begin(); itCariche != cariche.end();
             itCariche++) {
          simulazioneCampo(itSorgenti, itCariche, cariche);
        }
      }
      SDL_SetRenderDrawColor(gRenderer, COLORE(coloreSorgente));
      itSorgenti->render(gRenderer);

      // --------------------------------------------
      // TODO: Finestra per la gestione delle cariche
      // --------------------------------------------
      /*
      std::string titolo = "Carica " + std::to_string(index);
      ImGui::Begin(titolo.c_str(), &itSorgenti->windowOpen);
      // ImGui::SliderFloat("Carica", &itSorgenti->carica,
      // -10.0e-9, 10.0e-9);
      ImGui::End();
      */
    }

    // Simulazione movimento cariche di prova

    float dt = stepTimer.getTicks() / 1000.f;
    if (!pause && cariche.size() > 0) {
      for (itCariche = cariche.begin(); itCariche != cariche.end();
           itCariche++) {
        itCariche->computeForces();
        itCariche->updatePosition(dt);

        grafico.puntiDelGrafico.push_back(
            Point(itCariche->getPosition().x, itCariche->getPosition().y));
      }
    }

    stepTimer.start();

    // RENDERING GRIGLIA
    if (drawGrid) {
      RenderGriglia(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT, scala);
    }

    // Rendering cariche di prova
    SDL_SetRenderDrawColor(gRenderer, COLORE(coloreCarica));
    if (cariche.size() > 0) {
      for (itCariche = cariche.begin(); itCariche != cariche.end();
           itCariche++) {
        itCariche->render(gRenderer);
        itCariche->emptyVectors();
      }
    }

    // Rendering campo vettoriale
    if (drawCampoVettoriale) {

      for (it = punti.begin(); it != punti.end(); it++) {
        it->computeVectors();
        it->render(gRenderer);
        it->emptyVectors();
      }
    }

    // RENDERING GRAFICO

    SDL_SetRenderDrawColor(gRenderer, COLORE(coloreGrCariche));
    grafico.render(gRenderer);
    linea.computeVectors(sorgenti);
    linea.render(gRenderer);
    linea.emptyVectors();
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
