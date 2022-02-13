#pragma once
#include <vector>
#include <sstream>
#include "../settings.h"
#include "font.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "../utils.h"
#include "imgui_internal.h"
#include "../graph.h"


void renderUi(bool &pause, SDL_Window *gWindow, bool &darkMode, vector2 &intensitaMouse, std::vector<PuntoDelCampo> &punti, std::vector<Carica> &cariche, std::vector<Sorgente> &sorgenti, Graph &graficoCariche);
