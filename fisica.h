#pragma once
#include <vector>

#include "entities/campoVettoriale.h"
#include "entities/charge.h"
#include "entities/lineeDiForza.h"
#include "entities/sorgente.h"

//void simulazioneCampo(std::vector<Sorgente>::iterator &itSorgenti,
//                      std::vector<CaricaLineaDiForza>::iterator &it);
void simulazioneCampo(std::vector<Sorgente>::iterator &itSorgenti, std::vector<vector2> &mouseVector, float x, float y);
void simulazioneCampo(std::vector<Sorgente>::iterator &itSorgenti,
                      std::vector<Carica>::iterator &it,
                      std::vector<Carica> &cariche);
void simulazioneCampo(std::vector<Sorgente>::iterator &itSorgenti,
                      std::vector<PuntoDelCampo>::iterator &it);
