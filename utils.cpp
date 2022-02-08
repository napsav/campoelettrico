#include "utils.h"

void setDensity(std::vector<PuntoDelCampo> &punti, float densita) {
  punti.clear();
  for (unsigned int i = 0; i < SCREEN_WIDTH; i = i + densita) {
    for (unsigned int j = 0; j < SCREEN_HEIGHT; j = j + densita) {
      punti.push_back(
          *new PuntoDelCampo(static_cast<float>(i), static_cast<float>(j)));
    }
  }
}

void addSorgenteFunc(std::vector<Sorgente> &array, float x, float y) {
    if(x == -1 || y == -1) {
        array.push_back(*new Sorgente({static_cast<float>(array.size() * 50), 200}, 1.5e-9));
    } else {
        array.push_back(*new Sorgente({x, y}, 1.5e-9));
    }
}

void addCaricaFunc(std::vector<Carica> &array, float x, float y) {
  array.push_back(*new Carica(x, y, caricaDiProva, massa));
  if (abilitaLog)
    LOGXY(x, y);
}
