#ifndef GENERATOR_H
#define GENERATOR_H

#include "graf.h"
ulong kodiraj_granu(ulong n, ulong prvi, ulong drugi);
void dekodiraj_granu(ulong n, ulong kod, ulong &prvi, ulong &drugi);
double generisi_primjer(ulong n,ulong m,ulong dmax,bool povezanost, Graf<ulong>*& G);
void generisi_cvorove_za_obilazak(ulong n, vector<ulong> &posjete, ulong broj_za_posjetiti, vector<ulong> &zaobilasci, ulong broj_za_zaobici);
#endif // GENERATOR_H
