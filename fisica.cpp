#include "fisica.h"

#include <SDL2/SDL.h>

#include <iostream>

#include "entities/settings.h"

bool outOfBounds(std::vector<Carica>::iterator it) {
  bool outside = false;
  if (it->getPosition().y > SCREEN_HEIGHT ||
      it->getPosition().y < 0) {
    outside = true;
  } else if (it->getPosition().x > SCREEN_WIDTH ||
             it->getPosition().x < 0) {
    outside = true;
  }
  return outside;
}

void simulazioneCampo(std::vector<Sorgente>::iterator &itSorgenti,
                      std::vector<Carica>::iterator &it,
                      std::vector<Carica> &cariche, int raggioSorgente) {
  vector2 intensita;
  float valoreForzaCampo;
  vector2 distanzaVettore =
      distanza(itSorgenti->getPosition(), it->getPosition());

  // LEGGE DI COLOUMB:
  //
  //
  // F(1->2) = k*(Q1*Q2)
  //             ------
  //               r^2
  //
  //
  // F = E*q
  if (distanzaVettore.modulo > raggioSorgente && !outOfBounds(it)) {
    valoreForzaCampo =
        costanteColoumb * (itSorgenti->getCharge() /
                           ((distanzaVettore.modulo * distanzaVettore.modulo) *
                            (1.0f / (scala * scala))));

    std::cout << "Valore: \t" << valoreForzaCampo << std::endl;
    intensita.x = valoreForzaCampo * distanzaVettore.xNormalized;
    intensita.y = valoreForzaCampo * distanzaVettore.yNormalized;
    intensita.modulo = valoreForzaCampo;
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

  intensita.x = valoreCampo * distanzaVettore.xNormalized * (1.0f / scala);
  intensita.y = valoreCampo * distanzaVettore.yNormalized * (1.0f / scala);
  intensita.modulo = valoreCampo;
  it->addVector2(intensita);
}
