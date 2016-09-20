#ifndef HEAP_H
#define HEAP_H
#include "vertex_info.h"
#include <iostream>
#include <climits>
#include <utility>
#define INF 100000000
using namespace std;
typedef unsigned long ulong;
class gomila
{
    pair<ulong,vertex_info> *elems; // first -> key; second -> value
    ulong *key_mapping;
    ulong max_velicina;
    ulong velicina;
    gomila(const gomila&);
    gomila& operator=(const gomila&);

    public:
    gomila(ulong num_of_keys, vertex_info *values);
    ~gomila();
    void napravi_gomilu();
    pair<ulong,vertex_info> najmanji_element() { return elems[0]; }
    void ukloni_najmanji();
    void dodaj_element(ulong key, vertex_info value);
    void azuriraj_vrijednost(ulong key, vertex_info new_value);
    ulong parent(ulong indeks) { return (indeks-1)/2; }
    ulong left_child(ulong indeks) { return 2*indeks + 1; }
    ulong right_child(ulong indeks) { return 2*indeks + 2; }
    bool has_parent(ulong indeks) { return indeks>0; }
    bool has_left_child(ulong indeks) { return 2*indeks + 1 < velicina; }
    bool has_right_child(ulong indeks) { return 2*indeks + 1 < velicina; }
    bool is_root(ulong indeks) { return indeks==0; }
    bool is_leaf(ulong indeks) { return indeks >= velicina/2; }
    void up_heap(ulong indeks);
    void down_heap(ulong indeks);
    void swap_nodes(ulong indeks1, ulong indeks2);
    pair<ulong,vertex_info>* vrati_pocetak() { return elems; }
    ulong vrati_broj_elemenata() { return velicina; }
    bool is_empty() { return velicina==0; }
    ulong get_value(ulong indeks) { return  elems[key_mapping[indeks]].second.value; }
    ulong get_priority(ulong indeks) { return elems[key_mapping[indeks]].second.priority; }
    ulong get_wanted_nodes(ulong indeks) { return elems[key_mapping[indeks]].second.wanted_nodes; }
    vertex_info get_vertex_info(ulong indeks) { return elems[key_mapping[indeks]].second; }

    void ispisi();
    void foo();
    string toString();
};



#endif // HEAP_H
