#include "campoVettoriale.h"

PuntoDelCampo::PuntoDelCampo(float xpos, float ypos) {
  posizione.x = xpos;
  posizione.y = ypos;
}

void PuntoDelCampo::addVector2(vector2 toBeAdded) {
  vettoriDaSommare.push_back(toBeAdded);
}

vector2 PuntoDelCampo::getPosition() { return posizione; }

void PuntoDelCampo::computeVectors() {
  vector2 normalizzando;
  for (it = vettoriDaSommare.begin(); it != vettoriDaSommare.end(); it++) {
    sommaFinale.x += it->x;
    sommaFinale.y += it->y;
    carica += it->modulo;
    normalizzando = normalize(sommaFinale);
    sommaFinale.xNormalized = normalizzando.xNormalized;
    sommaFinale.yNormalized = normalizzando.yNormalized;
  }
}

void PuntoDelCampo::emptyVectors() {
  vettoriDaSommare.clear();
  sommaFinale.x = 0;
  sommaFinale.y = 0;
  carica = 0;
}

void PuntoDelCampo::render(SDL_Renderer *renderer) {
  if (carica > 0) {
    if (carica > maxCarica) {
      carica = maxCarica;
    }
    if ((abs(carica) / maxCarica) * 0xFF < coloreBase) {
      SDL_SetRenderDrawColor(renderer, COLORE(coloreBaseRGB));
    } else {
      SDL_SetRenderDrawColor(renderer, 0x00, ((carica / maxCarica) * 0xBF), 0x00,
                             0xFF);
    }
  } else {
    if (abs(carica) > maxCarica) {
      carica = maxCarica;
    }
    if ((abs(carica) / maxCarica) * 0xFF < coloreBase) {
      SDL_SetRenderDrawColor(renderer, COLORE(coloreBaseRGB));
    } else {
      SDL_SetRenderDrawColor(renderer, ((abs(carica) / maxCarica) * 0xFF), 0x00,
                             0x00, 0xFF);
    }
  }
  Settings::canvas.drawLine( posizione,
                            {
                               posizione.x + (sommaFinale.xNormalized * lunghezza),
                               posizione.y + (sommaFinale.yNormalized * lunghezza)

                            }
                            );
}

void PuntoDelCampo::setCarica(float charge) { carica = charge; }
PuntoDelCampo::~PuntoDelCampo() {
  vettoriDaSommare.clear();
}
