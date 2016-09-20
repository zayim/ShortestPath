#ifndef GRAF_H
#define GRAF_H

#include "vertex_info.h"
#include "heap.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <climits>
#define INF 100000000
using namespace std;
typedef unsigned long ulong;
template <class T=ulong>
class Graf
{
    vector<T> oznakeCvorova;
    unordered_map<T,ulong> indexiCvorova;
    vector<vector<ulong> > grane;
    vector<ulong> stepeniCvorova;
    ulong brojGrana;

    public:
    Graf() : oznakeCvorova(0), grane(0), brojGrana(0) {}
    Graf(ulong n, T* cvorovi);

    T oznakaCvora(ulong index) const { return oznakeCvorova[index]; }
    ulong indexCvora(const T& oznaka) const;
    ulong tezinaGraneI(ulong ind1, ulong ind2) const { return grane[ind1][ind2]; }
    ulong tezinaGrane(const T& oz1, const T& oz2) const;
    bool daLiSuPovezani(ulong ind1, ulong ind2) const { return grane[ind1][ind2]; }
    bool daLiSuPovezani(const T& oz1, const T& oz2) const { return tezinaGrane(oz1,oz2); }
    void dodajCvor(const T& oznaka);
    void dodajGranuI(ulong ind1, ulong ind2, ulong tezina);
    void dodajGranu(const T& oz1, const T& oz2, ulong tezina);
    ulong vratiBrojCvorova() const { return oznakeCvorova.size(); }
    ulong vratiBrojGrana() const { return brojGrana; }
    void izbrisiCvor(ulong ind);
    void izbrisiCvor(const T& oznaka);
    void izbrisiGranu(ulong ind1, ulong ind2);
    void izbrisiGranu(const T& oz1, const T& oz2);
    ulong vratiStepenCvoraI(ulong indeks) const;
    ulong vratiStepenCvora(const T& oznaka) const;

    long long najkraciPut(ulong indeksStart, ulong indeksEnd, const vector<ulong> &indeksiZaPosjetiti, const vector<ulong> &indeksiZaZaobici, stack<T> &najkraciPutOznake);

    void ispisi() const;
    void ispisiUDatoteku(char imeDatoteke[]) const;
    void foo();
};
// Konstruktor koji kreira graf sa cvorovima koji se proslijede kao parametri, i bez grana
template <class T>
Graf<T>::Graf(ulong n, T* cvorovi) : oznakeCvorova(n), grane(n), brojGrana(0), stepeniCvorova(n)
{
    for (ulong i=0; i<n; i++)
    {
        oznakeCvorova[i]=cvorovi[i];
        indexiCvorova[cvorovi[i]]=i;
        stepeniCvorova[i]=0;
    }

    for (ulong i=0; i<n; i++)
        for (ulong j=0; j<n; j++)
            grane[i].push_back(0);

}

template <class T>
ulong Graf<T>::indexCvora(const T& oznaka) const
{
    typename unordered_map<T,ulong>::const_iterator it=indexiCvorova.find(oznaka);
    if (it==indexiCvorova.end())
        throw "Cvor nije u grafu!";

    return it->second;
}

template <class T>
ulong Graf<T>::tezinaGrane(const T& oz1, const T& oz2) const
{
    typename unordered_map<T,ulong>::const_iterator it1=indexiCvorova.find(oz1);
    if (it1==indexiCvorova.end())
        throw "Cvor nije u grafu!";

    typename unordered_map<T,ulong>::const_iterator it2=indexiCvorova.find(oz2);
    if (it2==indexiCvorova.end())
        throw "Cvor nije u grafu!";

    return grane[it1->second][it2->second];
}

template <class T>
void Graf<T>::dodajCvor(const T& oznaka)
{
    typename unordered_map<T,ulong>::iterator it=indexiCvorova.find(oznaka);
    if (it!=indexiCvorova.end())
        return;

    indexiCvorova[oznaka]=oznakeCvorova.size();
    oznakeCvorova.push_back(oznaka);
    stepeniCvorova.push_back(0);

    for (ulong i=0; i<grane.size(); i++)
        grane[i].push_back(0);
    grane.push_back(vector<ulong>(oznakeCvorova.size()));
    for (ulong i=0; i<grane[grane.size()-1].size(); i++)
        grane[grane.size()-1][i]=0;


}

template <class T>
void Graf<T>::dodajGranuI(ulong ind1, ulong ind2, ulong tezina)
{
    grane[ind1][ind2]=tezina;
    grane[ind2][ind1]=tezina;
    brojGrana++;

    stepeniCvorova[ind1]++;
    stepeniCvorova[ind2]++;
}

template <class T>
void Graf<T>::dodajGranu(const T& oz1, const T& oz2, ulong tezina)
{
    typename unordered_map<T,ulong>::iterator it1=indexiCvorova.find(oz1);
    if (it1==indexiCvorova.end())
        throw "Cvor nije u grafu!";

    typename unordered_map<T,ulong>::iterator it2=indexiCvorova.find(oz2);
    if (it2==indexiCvorova.end())
        throw "Cvor nije u grafu!";

    grane[it1->second][it2->second]=tezina;
    grane[it2->second][it1->second]=tezina;
    brojGrana++;

    stepeniCvorova[it1->second]++;
    stepeniCvorova[it2->second]++;
}

template <class T>
void Graf<T>::izbrisiCvor(ulong ind)
{
    for (ulong i=0; i<grane[ind].size(); i++)
        if (grane[ind][i])
            stepeniCvorova[i]--;

    indexiCvorova.erase( oznakeCvorova[ind] );
    for (ulong i=ind+1; i<oznakeCvorova.size(); i++)
        indexiCvorova[ oznakeCvorova[i] ] = i-1;

    auto itg=grane.begin();
    auto itic=oznakeCvorova.begin();
    auto itsc=stepeniCvorova.begin();
    for (ulong i=0; i<ind; i++)
        { itg++; itic++; itsc++; }
    grane.erase(itg);
    oznakeCvorova.erase(itic);
    stepeniCvorova.erase(itsc);

    for (ulong i=0; i<grane.size(); i++)
    {
        auto itgi=grane[i].begin();
        for (ulong j=0; j<ind; j++)
            itgi++;
        grane[i].erase(itgi);
    }
}

template <class T>
void Graf<T>::izbrisiCvor(const T& oznaka)
{
    typename unordered_map<T,ulong>::iterator it=indexiCvorova.find(oznaka);
    if (it==indexiCvorova.end())
        return;

    izbrisiCvor(it->second);
}

template <class T>
void Graf<T>::izbrisiGranu(ulong ind1, ulong ind2)
{
    if (grane[ind1][ind2])
    {
        brojGrana--;
        stepeniCvorova[ind1]--;
        stepeniCvorova[ind2]--;
    }
    grane[ind1][ind2]=0;
    grane[ind2][ind1]=0;
}

template <class T>
void Graf<T>::izbrisiGranu(const T& oz1, const T& oz2)
{
    typename unordered_map<T,ulong>::iterator it1=indexiCvorova.find(oz1);
    if (it1==indexiCvorova.end())
        return;

    typename unordered_map<T,ulong>::iterator it2=indexiCvorova.find(oz2);
    if (it2==indexiCvorova.end())
        return;

    if (grane[it1->second][it2->second])
    {
        brojGrana--;
        stepeniCvorova[it1->second]--;
        stepeniCvorova[it2->second]--;
    }
    grane[it1->second][it2->second]=0;
    grane[it2->second][it1->second]=0;

}

template <class T>
ulong Graf<T>::vratiStepenCvoraI(ulong indeks) const
{
    return stepeniCvorova[indeks];
}

template <class T>
ulong Graf<T>::vratiStepenCvora(const T& oznaka) const
{
    typename unordered_map<T,ulong>::iterator it=indexiCvorova.find(oznaka);
    if (it==indexiCvorova.end())
        throw "Nema cvora u grafu!";

    return stepeniCvorova[it->second];
}

template <class T>
void Graf<T>::ispisi() const
{
    cout << "OZNAKE CVOROVA:" << endl;
    for (ulong i=0; i<oznakeCvorova.size(); i++)
        cout << "Ind: " << i << " ; Oznaka: " << oznakeCvorova[i] << " ; Stepen: " << stepeniCvorova[i] << endl;

    cout << "INDEXI CVOROVA:" << endl;
    for (auto it=indexiCvorova.begin(); it!=indexiCvorova.end(); it++)
        cout << "Oznaka: " << it->first << " ; Ind: " << it->second << endl;

    cout << "BROJ GRANA: " << brojGrana << endl;
    cout << "GRANE:" << endl;
    for (ulong i=0; i<grane.size(); i++)
    {
        for (ulong j=0; j<grane[i].size(); j++)
            cout << setw(4) << grane[i][j];
        cout << endl;
    }
}

template <class T>
void Graf<T>::ispisiUDatoteku(char imeDatoteke[]) const
{
    fstream datoteka(imeDatoteke, ios::out);
    datoteka << "OZNAKE CVOROVA:" << endl;
    for (ulong i=0; i<oznakeCvorova.size(); i++)
        datoteka << "Ind: " << i << " ; Oznaka: " << oznakeCvorova[i] << " ; Stepen: " << stepeniCvorova[i] << endl;

    datoteka << "INDEXI CVOROVA:" << endl;
    for (auto it=indexiCvorova.begin(); it!=indexiCvorova.end(); it++)
        datoteka << "Oznaka: " << it->first << " ; Ind: " << it->second << endl;

    datoteka << "BROJ GRANA: " << brojGrana << endl;
    datoteka << "GRANE:" << endl;
    for (ulong i=0; i<grane.size(); i++)
    {
        for (ulong j=0; j<grane[i].size(); j++)
            datoteka << " " << grane[i][j];
        datoteka << endl;
    }
    datoteka.close();
}
template <class T>
long long Graf<T>::najkraciPut(ulong indeksStart, ulong indeksEnd, const vector<ulong> &indeksiZaPosjetiti, const vector<ulong> &indeksiZaZaobici, stack<T> &najkraciPutOznake)
{
    vector<bool> visited(oznakeCvorova.size(),false);
    vector<long long> prev(oznakeCvorova.size(),-1);
    vertex_info* niz=new vertex_info[oznakeCvorova.size()];

    // popunjavamo niz svim cvorovima sa d[v]=infinity i prioritetom 1 (prioritet: 0-najveci/mora se obici, 1-srednji/normalan, 2-najmanji/ne smije se obici
    for (ulong i=0; i<oznakeCvorova.size(); i++)
        niz[i]=vertex_info(1,INF,0,0);
    niz[indeksStart].value=0;
    niz[indeksStart].priority=0;

    for (auto i=0; i<indeksiZaPosjetiti.size(); i++)
        niz[indeksiZaPosjetiti[i]].priority=0;

    for (auto i=0; i<indeksiZaZaobici.size(); i++)
        niz[indeksiZaZaobici[i]].priority=2;

    gomila Q(oznakeCvorova.size(),niz);

    while (!Q.is_empty())
    {
        pair<ulong,vertex_info> u=Q.najmanji_element();
        Q.ukloni_najmanji();
        visited[u.first]=true;

        if (u.second.value == INF)
            return -1;
        if (u.first == indeksEnd)
        {
            ulong tmp=indeksEnd;
            while(tmp!=indeksStart)
            {
                najkraciPutOznake.push( oznakeCvorova[tmp] );
                tmp=prev[tmp];
            }
            najkraciPutOznake.push(oznakeCvorova[indeksStart]);

            return u.second.value;
        }

        for (auto v=0; v<grane[u.first].size(); v++)
        {
            if (grane[u.first][v] && !visited[v])
            {
                ulong tmp_d=u.second.value+grane[u.first][v];
                bool is_wanted = Q.get_priority(v) == 0;
                bool is_unwanted = Q.get_priority(v) == 2;
                vertex_info tmp_v=vertex_info(  Q.get_priority(v), tmp_d, u.second.wanted_nodes + is_wanted , u.second.unwanted_nodes + is_unwanted );
                if (tmp_v < Q.get_vertex_info(v))
                {
                    prev[v]=u.first;
                    Q.azuriraj_vrijednost(v,tmp_v);
                }
            }
        }
    }
    return -1;
}

template <class T>
void Graf<T>::foo()
{
    for (ulong i=0; i<grane.size(); i++)
        for (ulong j=0; j<grane[i].size(); j++)
            grane[i][j]=i*grane[j].size() + j;
}
#endif // GRAF_H

