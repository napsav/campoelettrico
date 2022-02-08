#include <SDL2/SDL.h>

#include <cmath>
#include <iostream>

#include <vector>

#include "ui/imgui_impl_sdl.h"


#include "./draw.h"
#include "./timer.h"
#include "entities/campoVettoriale.h"
#include "entities/charge.h"
#include "entities/lineeDiForza.h"
#include "settings.h"
#include "entities/sorgente.h"
#include "entities/vector.h"
#include "fisica.h"
#include "graph.h"
#include "ui/ui.h"
#include "utils.h"
#include "macros.h"
#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif



SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

bool init() {
  bool success = true;
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
    printf("SDL could not be initialized, error: %s\n", SDL_GetError());
    success = false;
  } else {

    // THIS MAKES SURE COMPOSITOR IS NOT DISABLED AUTOMATICALLY ON LINUX
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    gWindow =
        SDL_CreateWindow("Campo Elettrostatico", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                         window_flags);
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
        SDL_RendererInfo info;
        SDL_GetRendererInfo(gRenderer, &info);
        SDL_Log("Current SDL_Renderer: %s", info.name);

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
      }
    }
  }
  return success;
}

int main(int argc,  char** argv) {
  if (!init()) {
    std::cout << "Init di SDL fallito" << std::endl;
    return 1;
  }
  bool quit = false;
  SDL_Event e;
  Timer stepTimer;
  bool pause = true;
  int x, y;
  Graph grafico;
  vector2 intensitaMouse = {0, 0};
  std::vector<Sorgente> sorgenti;
  std::vector<Sorgente>::iterator itSorgenti;
  std::vector<PuntoDelCampo> punti;
  std::vector<PuntoDelCampo>::iterator it;
  std::vector<Carica>::iterator itCariche;
  std::vector<Carica> cariche;
  std::vector<CaricaLineaDiForza> lineeDiForza;
  std::vector<vector2> mouseVector;
  Sorgente sorgentePrima = Sorgente({300, 500}, 2.5e-9);
  Sorgente sorgenteSeconda = Sorgente({600, 500}, 5.2e-8);
  sorgenti.push_back(sorgentePrima);
  sorgenti.push_back(sorgenteSeconda);

  ImGui::CreateContext();

  /**
   * imgui_sdl (vecchio)
   * ImGuiSDL::Initialize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
   */

  // carica caricaDiProva = {{600, 300}, {}};
  setDensity(punti, densita);
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  // vecchio sdl: ImGui_ImplSDL2_InitTest(gWindow);
  io.WantCaptureKeyboard = true;
  io.Fonts->AddFontFromMemoryCompressedTTF(robotoFont_compressed_data, robotoFont_compressed_size, 16);

  ImGui_ImplSDL2_InitForSDLRenderer(gWindow);
  ImGui_ImplSDLRenderer_Init(gRenderer);

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
          addSorgenteFunc(sorgenti, -1, -1);
          break;
        case SDLK_UP:
          lunghezza += 10;
          break;
        case SDLK_DOWN:
          lunghezza -= 10;
          break;
        case SDLK_r:
          sorgenti.clear();
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
      } else if (e.type == SDL_WINDOWEVENT) {
        if (e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(gWindow))
          quit = true;
        if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
          SDL_SetWindowSize(gWindow, e.window.data1, e.window.data2);
          SCREEN_WIDTH = e.window.data1;
          SCREEN_HEIGHT = e.window.data2;
          setDensity(punti, densita);

          // TODO: resizing window fix for imgui
          // ImGuiSDL::Deinitialize();
          // ImGuiSDL::Initialize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
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

    // Finestra impostazioni

    // --------------------
    // 	IMGUI
    // --------------------

    // ImGUI input handling

    const int buttons = SDL_GetMouseState(&x, &y);
    io.DeltaTime = 1.0f / 1000.0f;
    io.MousePos = ImVec2(static_cast<float>(x), static_cast<float>(y));
    io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
    io.MouseWheel = static_cast<float>(wheel);



    SDL_SetRenderDrawColor(gRenderer, COLORE(coloreSfondo));
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);


    unsigned int sizeCariche = cariche.size();

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
      spawnLinee(lineeDiForza, itSorgenti);
      // lineeDiForza.push_back(CaricaLineaDiForza(it->getPosition().x + raggio,
      // it->getPosition().y + raggio));

      // Simulazione delle cariche di prova, non definitivo
      // (da unire con l'altra funzione o creare classe base)
      simulazioneCampo(itSorgenti, mouseVector, x, y);
      if (sizeCariche > 0) {
        for (itCariche = cariche.begin(); itCariche != cariche.end();
             itCariche++) {
          simulazioneCampo(itSorgenti, itCariche, cariche);
        }
      }
      if (drawSorgenti)
        itSorgenti->render(gRenderer);
    }

    // Simulazione movimento cariche di prova

    float dt = stepTimer.getTicks() / 1000.f;
    if (sizeCariche > 0) {
      for (itCariche = cariche.begin(); itCariche != cariche.end();
           itCariche++) {
        itCariche->computeForces();
        if (!pause)
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
    if (sizeCariche > 0 && drawGraficoCariche) {
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

    for (auto vec : mouseVector) {
      intensitaMouse.x += vec.x;
      intensitaMouse.y += vec.y;
    }

    intensitaMouse.modulo = sqrt((intensitaMouse.x * intensitaMouse.x) +
                                 (intensitaMouse.y * intensitaMouse.y));
    intensitaMouse.x = 0;
    intensitaMouse.y = 0;
    mouseVector.clear();

    // RENDERING GRAFICO

    SDL_SetRenderDrawColor(gRenderer, COLORE(coloreGrCariche));
    grafico.render(gRenderer);

    // LINEE DI CAMPO
    if (drawLineeDiCampo) {
      SDL_SetRenderDrawColor(gRenderer, COLORE(coloreLinee));
      for (auto linea : lineeDiForza) {
        linea.computeVectors(sorgenti);
        linea.render(gRenderer);
        linea.emptyVectors();
      }
      lineeDiForza.clear();
    }

    // Rendering IMGUI
    renderUi(pause, gWindow, darkMode, intensitaMouse, punti, cariche, sorgenti);

    SDL_RenderPresent(gRenderer);
  }

  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);

  SDL_Quit();
  return 0;
}
