#pragma once
#include <vector>

#include "entities/campoVettoriale.h"
#include "entities/charge.h"
#include "entities/sorgente.h"

const float costanteColoumb = 8.987551792314e9;
void simulazioneCampo(std::vector<Sorgente>::iterator &itSorgenti,
		      std::vector<Carica>::iterator &it,
		      std::vector<Carica> &cariche, int raggioSorgente);
void simulazioneCampo(std::vector<Sorgente>::iterator &itSorgenti,
		      std::vector<PuntoDelCampo>::iterator &it);
