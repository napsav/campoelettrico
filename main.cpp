#include <SDL2/SDL.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

#include "ui/imgui_impl_sdl.h"
#define LOGVECTOR(vector)                            \
  std::cout << "Vettore\tX: " vector.getPosition().x \
            << "\tY: " << vector.getPosition().y << std::endl
#define LOGXY(x, y) std::cout << "Coppia\tX: " << x << "\tY: " << y << std::endl
#define COLORE(color) \
  color[0] * 0xFF, color[1] * 0xFF, color[2] * 0xFF, color[3] * 0xFF
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
#include "ui/font.h"
#include "ui/imgui.h"
#include "ui/imgui_impl_sdl.h"
#include "ui/imgui_impl_sdlrenderer.h"
#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif
float costanteColoumb = 8.987551792314e9;
unsigned int SCREEN_HEIGHT = 720;
unsigned int SCREEN_WIDTH = 1280;
vector2 caricaNuova = {226.97501, 300};
float massa = 1e4;
float densitaLinee = 1e8;
int scala = 1000;
int densita = 16;
int lunghezza = 10;
int raggio = 10;
float maxCarica = 0.05;
bool drawGrid = true;
bool drawCampoVettoriale = true;
bool sorgentiColoreSegno = false;
bool drawLineeDiCampo = true;
bool drawSorgenti = true;
bool drawGraficoCariche = true;
bool darkMode = true;
bool abilitaLog = false;
float caricaDiProva = 1.602176634e-19;

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

bool init() {
  bool success = true;
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
    printf("SDL could not be initialized, error: %s\n", SDL_GetError());
    success = false;
  } else {
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
  array.push_back(*new Carica(static_cast<float>(x), static_cast<float>(y),
                              caricaDiProva, massa));
  if (abilitaLog)
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
  bool open = false;
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
          addSorgenteFunc(sorgenti);
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

    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame(gWindow);
    ImGui::NewFrame();

    if (darkMode) {
      ImGui::StyleColorsDark();
    } else {
      ImGui::StyleColorsLight();
    }

    ImGui::Begin("Impostazioni", &open);
    if (pause) {
      ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Stato della simulazione: IN PAUSA");
    } else {
      ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Stato della simulazione: IN ESECUZIONE");
    }
    ImGui::Separator();
    ImGui::Text("Intensità campo elettrico nel cursore totale: %f N/C",
                intensitaMouse.modulo);
    ImGui::Separator();

    ImGui::Checkbox("Log di debug", &abilitaLog);

    if (ImGui::CollapsingHeader("Opzioni principali",
                                ImGuiTreeNodeFlags_None)) {
      ImGui::Text("Massa cariche di prova");
      ImGui::InputFloat("massa", &massa, 1e-5f, 1000.0f, "%e");
      ImGui::Text("Coloumb carica di prova");
      ImGui::InputFloat("caricaDiProva", &caricaDiProva, 1e-20f, 1.0f, "%e");
      ImGui::Text("Linee di forza step max");
      ImGui::InputInt("maxStep", &maxStep, 1, 10000);

      ImGui::Text("Densità linee");
      ImGui::InputFloat("densitaLinee", &densitaLinee, 1e2, 1e24, "%e");

      ImGui::Text("Dettaglio linee di forza");
      ImGui::InputFloat("salto", &salto, 0.1, 100);

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
      ImGui::Text("Raggio cariche di prova");
      ImGui::SliderInt("raggioCarica", &raggioCarica, 1, 100);
      ImGui::Checkbox("Griglia", &drawGrid);
      ImGui::Checkbox("Linee di campo", &drawLineeDiCampo);
      ImGui::Checkbox("Traiettoria cariche di prova", &drawGraficoCariche);
      ImGui::Checkbox("Campo vettoriale", &drawCampoVettoriale);
      ImGui::Checkbox("Sorgenti visibili", &drawSorgenti);
      ImGui::Checkbox("Sorgenti colorate", &sorgentiColoreSegno);
      ImGui::Checkbox("Modalità scura", &darkMode);
      ImGui::Separator();
      ImGui::ColorEdit4("Colore sfondo", &coloreSfondo[0]);
      ImGui::ColorEdit4("Colore cariche", &coloreCarica[0]);
      if (!sorgentiColoreSegno)
        ImGui::ColorEdit4("Colore sorgente", &coloreSorgente[0]);
      ImGui::ColorEdit4("Colore grafico cariche", &coloreGrCariche[0]);
      ImGui::ColorEdit4("Colore base vettori", &coloreBaseRGB[0]);
      ImGui::ColorEdit4("Colore griglia primario", &coloreGrigliaPrimario[0]);
      ImGui::ColorEdit4("Colore griglia secondario",
                        &coloreGrigliaSecondario[0]);
      ImGui::ColorEdit4("Colore linee di campo", &coloreLinee[0]);
    }

    ImGui::Separator();
    ImGui::Text("Aggiungi carica in una posizione precisa");
    ImGui::InputFloat("x", &caricaNuova.x, 0, SCREEN_WIDTH, "%e");
    ImGui::InputFloat("y", &caricaNuova.y, 0, SCREEN_HEIGHT, "%e");
    if (ImGui::Button("Aggiungi")) {
      cariche.push_back(
          *new Carica(caricaNuova.x, caricaNuova.y, caricaDiProva, massa));
    }
    ImGui::End();
    // TODO: resolve crash when collapsing source charges window
    // TODO: better handle placing of windows
    ImGui::Begin("Sorgenti");
    ImGui::BeginTable("sorgenti", 4, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn("N");
    ImGui::TableSetupColumn("Coloumb");
    ImGui::TableSetupColumn("X");
    ImGui::TableSetupColumn("Y");
    ImGui::TableHeadersRow();
    for (unsigned int i = 0; i < sorgenti.size(); i++) {

      std::ostringstream streamObj;
      streamObj << sorgenti[i].getCharge();
      // ImGui::Text("Sorgente %d: %s C", i, streamObj.str().c_str());

      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("%d", i);
      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%s", streamObj.str().c_str());
      ImGui::TableSetColumnIndex(2);
      ImGui::Text("%f", sorgenti[i].getPosition().x);
      ImGui::TableSetColumnIndex(3);
      ImGui::Text("%f", sorgenti[i].getPosition().y);
    }
    ImGui::EndTable();
    ImGui::End();

    if (cariche.size() > 0) {
      ImGui::Begin("Cariche di prova");
      ImGui::BeginTable("cariche", 5, ImGuiTableFlags_Borders);
      ImGui::TableSetupColumn("Carica");
      ImGui::TableSetupColumn("Accelerazione x");
      ImGui::TableSetupColumn("Accelerazione y");
      ImGui::TableSetupColumn("Posizione x");
      ImGui::TableSetupColumn("Posizione y");
      ImGui::TableHeadersRow();
      for (unsigned int i = 0; i < cariche.size(); i++) {
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%d", i);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%f", cariche[i].getAcceleration().x);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%f", cariche[i].getAcceleration().y);
        ImGui::TableSetColumnIndex(3);
        ImGui::Text("%f", cariche[i].getPosition().x);
        ImGui::TableSetColumnIndex(4);
        ImGui::Text("%f", cariche[i].getPosition().y);

        // ImGui::Text("Carica %d: ACC\tX=%f\tY=%f\tPOS\tX=%f\tY=%f", i,
        // cariche[i].getAcceleration().x, cariche[i].getAcceleration().y,
        // cariche[i].getPosition().x, cariche[i].getPosition().y);
      }
      ImGui::EndTable();
      ImGui::End();
    }
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
      spawnLinee(lineeDiForza, itSorgenti);
      // lineeDiForza.push_back(CaricaLineaDiForza(it->getPosition().x + raggio,
      // it->getPosition().y + raggio));

      // Simulazione delle cariche di prova, non definitivo
      // (da unire con l'altra funzione o creare classe base)
      simulazioneCampo(itSorgenti, mouseVector, x, y);
      if (cariche.size() > 0) {
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
    if (cariche.size() > 0) {
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
    if (cariche.size() > 0 && drawGraficoCariche) {
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

    // Rendering ImGUI
    ImGui::ShowDemoWindow();

    ImGui::Render();

    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
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
