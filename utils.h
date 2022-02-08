#include <vector>
#include <iostream>
#include "entities/campoVettoriale.h"
#include "entities/sorgente.h"
#include "entities/charge.h"
#include "entities/vector.h"
#include "macros.h"

void setDensity(std::vector<PuntoDelCampo> &punti, float densita);

void addSorgenteFunc(std::vector<Sorgente> &array, float x, float y);

void addCaricaFunc(std::vector<Carica> &array, float x, float y);
