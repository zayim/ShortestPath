#include "heap.h"
#include <string>
#include <sstream>
using namespace std;
gomila::gomila(ulong num_of_keys, vertex_info *values)
{
    max_velicina=2*num_of_keys;
    velicina=num_of_keys;

    elems = new pair<ulong,vertex_info> [max_velicina];
    key_mapping = new ulong[max_velicina];

    for (ulong i=0; i<velicina; i++)
    {
        elems[i]=make_pair(i,values[i]);
        key_mapping[i]=i;
    }

    napravi_gomilu();
}
gomila::~gomila()
{
    delete elems; elems=0;
    delete key_mapping; key_mapping=0;
}

void gomila::swap_nodes(ulong indeks1, ulong indeks2)
{
    swap(elems[indeks1],elems[indeks2]);
    key_mapping[elems[indeks1].first]=indeks1;
    key_mapping[elems[indeks2].first]=indeks2;
}

void gomila::up_heap(ulong indeks)
{
    while (elems[parent(indeks)].second > elems[indeks].second && !is_root(indeks))
    {
        swap_nodes(indeks,parent(indeks));
        indeks=parent(indeks);
    }
}

void gomila::down_heap(ulong indeks)
{
    while (!is_leaf(indeks))
    {
        vertex_info left_child_value=elems[left_child(indeks)].second;
        vertex_info right_child_value=elems[right_child(indeks)].second;

        if (left_child_value < right_child_value || right_child(indeks)>=velicina)
        {
            if (left_child_value < elems[indeks].second)
                swap_nodes(indeks,left_child(indeks));
            else return;
            indeks=left_child(indeks);
        }
        else
        {
            if (right_child_value < elems[indeks].second)
                swap_nodes(indeks,right_child(indeks));
            else return;
            indeks=right_child(indeks);
        }
    }
}

void gomila::napravi_gomilu()
{
    for (long i=velicina/2; i>=0; i--)
        { down_heap(i); }
}

void gomila::ukloni_najmanji()
{
    swap_nodes(0,--velicina);
    down_heap(0);
}

void gomila::azuriraj_vrijednost(ulong key, vertex_info new_value)
{
    if (key_mapping[key] >= velicina) return;

    if (new_value > elems[key_mapping[key]].second )
    {
        elems[key_mapping[key]].second = new_value;
        down_heap(key_mapping[key]);
    }
    else
    {
        elems[key_mapping[key]].second = new_value;
        up_heap(key_mapping[key]);
    }
}

void gomila::dodaj_element(ulong key, vertex_info value)
{
    if (velicina==max_velicina)
    {
        pair<ulong,vertex_info>* tmp=elems;
        ulong* tmp2=key_mapping;

        max_velicina*=2;
        elems=new pair<ulong,vertex_info> [max_velicina];

        for (ulong i=0; i<velicina; i++)
            elems[i]=*(tmp+i);
        delete tmp;

        key_mapping=new ulong[max_velicina];
        for (ulong i=0; i<velicina; i++)
            key_mapping[i]=*(tmp2+i);
        delete tmp2;
    }

    elems[velicina]=make_pair(key,value);
    key_mapping[key]=velicina++;
    up_heap(velicina-1);
}

void gomila::ispisi()
{
    cout << "KEY MAPPING:" << endl;
    for (ulong i=0; i<velicina; i++)
        cout << "[" << i << "]=" << key_mapping[i] << " ";

    cout << endl << "ELEMS (key,value): " << endl;
    for (ulong i=0; i<velicina; i++)
        cout << "[" << i << "]=(" << elems[i].first << "," << elems[i].second.priority << "-" << elems[i].second.value << ")" << endl;
}

void gomila::foo()
{
    swap_nodes(velicina/4, velicina/2);
}

string itos(int number)
{
   stringstream ss;
   ss << number;
   return ss.str();
}

string gomila::toString()
{
    string rez="";
    for (auto i=0; i<velicina; i++)
        rez+= string("[") + itos(elems[i].first) + string("]=(") + itos(elems[i].second.priority) + string(",") + itos(elems[i].second.value) + string(",") + itos(elems[i].second.wanted_nodes) + string(") ");
    rez+="\n";

    return rez;
}
