#include "graf.h"
#include "heap.h"
#include "vertex_info.h"
#include "generator.h"
#include <iostream>
#include <string>
#include <fstream>
#include <climits>
#include <algorithm>
#include <utility>
#include <ctime>
#define INF 100000000
using namespace std;
//bool f(pair<ulong,vertex_info> x, pair<ulong,vertex_info> y) { return x.second>y.second; }
//bool h(ulong x, ulong y) { return x>y; }
int main()
{
    srand(time(0));
    cout << "Da li zelite unijeti graf sa tastature ili zelite slucajno generisati primjer? (unesite 0 ili 1)" << endl;
    cout << " [1] - unos grafa sa tastature" << endl;
    cout << " [2] - unos grafa iz datoteke" << endl;
    cout << " [3] - generisanje slucajnog primjera" << endl;
    cout << "? ";
    unsigned short izbor; cin >> izbor;
    long long broj_cvorova, broj_grana;

    if (izbor==3) // u slucaju da se izabere random generisanje primjera
    {
        cout << "Unesite broj cvorova: "; cin >> broj_cvorova;
        cout << "Unesite broj grana: "; cin >> broj_grana;
        while (broj_grana>broj_cvorova*(broj_cvorova-1)/2 || broj_grana<0)
        {
            cout << "Broj grana mora biti nenegativan i ne veci od n*(n-1)/2! Pokusajte ponovo!" << endl;
            cout << "Unesite broj grana: "; cin >> broj_grana;
        }
        // promjenjive koje cemo koristiti
        long long najveci_stepen, broj_cvorova_posjetiti, broj_cvorova_ne_posjetiti;
        unsigned short povezanost;
        fstream rezultati_vrijeme("rezultati_testiranja/vrijeme.txt",ios::out);
        fstream rezultati_optimalan_put("rezultati_testiranja/optimalan_put.txt",ios::out);
        fstream rezultati_cvorovi_za_posjetu("rezultati_testiranja/cvorovi_za_posjetu.txt",ios::out);

        // unos stepena, uz ogranicenja za vrijednost
        cout << "Unesite najveci stepen cvora u grafu: "; cin >> najveci_stepen;
        while (najveci_stepen>=broj_cvorova || najveci_stepen>broj_grana || najveci_stepen<0 || najveci_stepen*broj_cvorova<2*broj_grana)
        {
            cout << "Pogresan unos! dmax ne smije biti vece od min(n,m) i mora biti nenegativno!" << endl;
            cout << "Unesite najveci stepen cvora u grafu: "; cin >> najveci_stepen;
        }

        // unos uslova povezanosti, ukoliko graf sa unesenim parametrima uopste moze biti povezan
        if (broj_grana-najveci_stepen < broj_cvorova - najveci_stepen - 1)
        {
            cout << "Graf sa unesenim parametrima ne moze biti povezan!" << endl;
            povezanost=false;
        }
        else
        {
            cout << "Unesite povezanost grafa (0-Ne mora biti povezan, 1-Mora biti povezan): ";
            cin >> povezanost;
        }

        // slucajno generisanje datog grafa i upis rezultata u datoteku
        Graf<ulong>* G;
        double vrijeme=generisi_primjer(broj_cvorova,broj_grana,najveci_stepen,povezanost,G);
        cout << "Vrijeme potrebno za generisanje grafa: " << vrijeme << "s" << endl;

        bool izbor_upis_u_fajl;
        cout << "Da li zelite upisati graf u datoteku? 0-NE, 1-DA "; cin >> izbor_upis_u_fajl;
        if (izbor_upis_u_fajl)
            G->ispisiUDatoteku("rezultati_testiranja/graf.txt");

        rezultati_vrijeme << "Broj cvorova: " << broj_cvorova << endl;
        rezultati_vrijeme << "Broj grana: " << broj_grana << endl;
        rezultati_vrijeme << "Najveci stepen: " << najveci_stepen << endl;
        rezultati_vrijeme << "Povezanost: " << (povezanost ? "DA" : "NE") << endl;
        rezultati_vrijeme << "Vrijeme za generisanje grafa: " << vrijeme << "s" << endl;

        // unos broja cvorova koje zelimo/ne zelimo posjetiti
        cout << "Unesite broj cvorova za posjetiti: "; cin >> broj_cvorova_posjetiti;
        while (broj_cvorova_posjetiti<0 || broj_cvorova_posjetiti>broj_cvorova)
        {
            cout << "Pogresan unos! Morate unijeti broj iz [0,broj_cvorova]!" << endl;
            cout << "Unesite broj cvorova za posjetiti: "; cin >> broj_cvorova_posjetiti;
        }

        cout << "Unesite broj cvorova za ne posjetiti: "; cin >> broj_cvorova_ne_posjetiti;
        while (broj_cvorova_ne_posjetiti<0 || broj_cvorova_ne_posjetiti>broj_cvorova-broj_cvorova_posjetiti-2)
        {
            cout << "Pogresan unos! Morate unijeti broj iz [0,broj_cvorova-broj_cvorova_posjetiti]!" << endl;
            cout << "Unesite broj cvorova za ne posjetiti: "; cin >> broj_cvorova_ne_posjetiti;
        }

        // generisanje cvorova za posjetiti/ne posjetiti i ispisivanje istih u datoteku
        vector<ulong> indeksi_cvorova_za_posjetiti(broj_cvorova_posjetiti);
        vector<ulong> indeksi_cvorova_za_ne_posjetiti(broj_cvorova_ne_posjetiti);
        generisi_cvorove_za_obilazak(broj_cvorova,
                                     indeksi_cvorova_za_posjetiti,broj_cvorova_posjetiti,
                                     indeksi_cvorova_za_ne_posjetiti,broj_cvorova_ne_posjetiti);
        rezultati_cvorovi_za_posjetu << "Indeksi cvorova za posjetiti: " << endl;
        for (ulong i=0; i<indeksi_cvorova_za_posjetiti.size(); i++)
            rezultati_cvorovi_za_posjetu << indeksi_cvorova_za_posjetiti[i] << "   ";
        rezultati_cvorovi_za_posjetu << endl;
        rezultati_cvorovi_za_posjetu << "Indeksi cvorova za ne posjetiti: " << endl;
        for (ulong i=0; i<indeksi_cvorova_za_ne_posjetiti.size(); i++)
            rezultati_cvorovi_za_posjetu << indeksi_cvorova_za_ne_posjetiti[i] << "   ";
        rezultati_cvorovi_za_posjetu << endl;

        // rjesavanje problema
        stack<ulong> najkraci_put_oznake;
        ulong t=clock();
        long long duzina_najkraceg_puta=G->najkraciPut(0,broj_cvorova-1,indeksi_cvorova_za_posjetiti,indeksi_cvorova_za_ne_posjetiti,najkraci_put_oznake);
        t=clock()-t;
        rezultati_vrijeme << "Vrijeme izvrsavanja algoritma: " << double(t)/CLOCKS_PER_SEC << "s" << endl;

        rezultati_optimalan_put << "Duzina najkraceg puta od prvog do zadnjeg cvora: " << duzina_najkraceg_puta << endl;
        rezultati_optimalan_put << "Najkraci put (oznake): " << endl;

        while(!najkraci_put_oznake.empty())
        {
            ulong oz=najkraci_put_oznake.top();
            najkraci_put_oznake.pop();

            rezultati_optimalan_put << oz << "  ";
        }

        // zatvaranje koristenih datoteka
        rezultati_vrijeme.close();
        rezultati_optimalan_put.close();
        rezultati_cvorovi_za_posjetu.close();
    }
    else if (izbor==1)
    {
        cout << "Unesite broj cvorova: "; cin >> broj_cvorova;
        cout << "Unesite broj grana: "; cin >> broj_grana;
        while (broj_grana>broj_cvorova*(broj_cvorova-1)/2 || broj_grana<0)
        {
            cout << "Broj grana mora biti nenegativan i ne veci od n*(n-1)/2! Pokusajte ponovo!" << endl;
            cout << "Unesite broj grana: "; cin >> broj_grana;
        }


        Graf<string> *G;
        string *cvorovi= new string[broj_cvorova];

        // unos cvorova grafa
        cout << "Unesite imena cvorova (kao stringove): " << endl;
        for (unsigned long i=0; i<broj_cvorova; i++)
        {
            cout << "[" << i << "]: ";
            cin >> cvorovi[i];
        }

        G = new Graf<string> (broj_cvorova,cvorovi);

        // unos grana grafa
        string cvor1, cvor2;
        unsigned long tezina;
        cout << "Unesite grane u formatu: ime_cvora1 ime_cvora2 tezina:" << endl;
        for (unsigned long i=0; i<broj_grana; i++)
        {
            cout << "[" << i << "]: ";
            cin >> cvor1 >> cvor2 >> tezina;

            try
            {
                G->dodajGranu(cvor1,cvor2,tezina);
            }
            catch(const char poruka[])
            {
                cout << poruka << " Ponovo!" << endl;
                i--;
            }
        }

        bool izbor_upis_u_fajl;
        cout << "Da li zelite upisati graf u datoteku? 0-NE, 1-DA "; cin >> izbor_upis_u_fajl;
        if (izbor_upis_u_fajl)
            G->ispisiUDatoteku("rezultati_testiranja_tastatura/graf.txt");

        vector<ulong> indeksi_cvorova_za_posjetiti;
        vector<ulong> indeksi_cvorova_za_ne_posjetiti;
        stack<string> najkraci_put_oznake;
        ulong indeks_pocetnog_cvora;
        ulong indeks_zavrsnog_cvora;
        string oznaka_pocetnog_cvora, oznaka_zavrsnog_cvora, oznaka_tmp="";

        // unos pocetnog cvora
        bool unosOK=false;
        do
        {
            cout << "Unesi oznaku pocetnog cvora: "; cin >> oznaka_pocetnog_cvora;
            try
            {
                indeks_pocetnog_cvora=G->indexCvora(oznaka_pocetnog_cvora);
                unosOK=true;
            }
            catch(const char poruka[])
            {
                cout << poruka << " Ponovo!" << endl;
                unosOK=false;
            }
        } while(!unosOK);

        // unos zavrsnog cvora
        unosOK=false;
        do
        {
            cout << "Unesi oznaku zavrsnog cvora: "; cin >> oznaka_zavrsnog_cvora;
            try
            {
                indeks_zavrsnog_cvora=G->indexCvora(oznaka_zavrsnog_cvora);
                unosOK=true;
            }
            catch(const char poruka[])
            {
                cout << poruka << " Ponovo!" << endl;
                unosOK=false;
            }
        } while(!unosOK);

        // unos cvorova za posjetiti
        cout << "Unesi cvorove koje zelite posjetiti (\"kraj\" za kraj): " << endl;
        while (true)
        {
            cout << "? "; cin >> oznaka_tmp;
            if (oznaka_tmp=="kraj") break;

            try
            {
                ulong indeks_tmp=G->indexCvora(oznaka_tmp);
                indeksi_cvorova_za_posjetiti.push_back(indeks_tmp);
            }
            catch(const char poruka[])
            {
                cout << poruka << " Ponovo!" << endl;
            }
        }

        // unos cvorova za zaobici
        cout << "Unesi cvorove koje ne zelite posjetiti (\"kraj\" za kraj): " << endl;
        while (true)
        {
            cout << "? "; cin >> oznaka_tmp;
            if (oznaka_tmp=="kraj") break;

            try
            {
                ulong indeks_tmp=G->indexCvora(oznaka_tmp);
                indeksi_cvorova_za_ne_posjetiti.push_back(indeks_tmp);
            }
            catch(const char poruka[])
            {
                cout << poruka << " Ponovo!" << endl;
            }
        }

        ulong t=clock();
        long long duzina_najkraceg_puta=G->najkraciPut(indeks_pocetnog_cvora,indeks_zavrsnog_cvora,indeksi_cvorova_za_posjetiti,indeksi_cvorova_za_ne_posjetiti,
                                                       najkraci_put_oznake);
        t=clock()-t;

        cout << "Optimalan put: ";
        while(!najkraci_put_oznake.empty())
        {
            string oz=najkraci_put_oznake.top();
            najkraci_put_oznake.pop();

            cout << oz << " ";
        }
        cout << endl << "Duzina: " << duzina_najkraceg_puta << endl;
        cout << "Vrijeme izvrsavanja: " << double(t)/CLOCKS_PER_SEC << "s" << endl;
    }

    else if (izbor==2)
    {
        fstream ulaz("input.txt",ios::in);
        fstream izlaz("rezultati_testiranja/optimalan_put.txt",ios::out);

        ulong n,m,bc_posjetiti,bc_ne_posjetiti,w,c_pocetak,c_kraj;
        vector<ulong> c_posjetiti, c_ne_posjetiti;
        string oz1,oz2;

        Graf<string> *G;
        string *cvorovi= new string[n];

        ulaz >> n;
        for (ulong i=0; i<n; i++)
            ulaz >> cvorovi[i];
        G=new Graf<string> (n,cvorovi);

        ulaz >> m;
        for (ulong i=0; i<m; i++)
        {
            ulaz >> oz1 >> oz2 >> w;
            G->dodajGranu(oz1,oz2,w);
        }

        G->ispisiUDatoteku("rezultati_testiranja/graf.txt");

        ulaz >> oz1 >> oz2;
        c_pocetak=G->indexCvora(oz1);
        c_kraj=G->indexCvora(oz2);

        ulaz >> bc_posjetiti;
        for (ulong i=0; i<bc_posjetiti; i++)
        {
            ulaz >> oz1;
            c_posjetiti.push_back(G->indexCvora(oz1));
        }

        ulaz >> bc_ne_posjetiti;
        for (ulong i=0; i<bc_ne_posjetiti; i++)
        {
            ulaz >> oz1;
            c_ne_posjetiti.push_back(G->indexCvora(oz1));
        }

        stack<string> najkraci_put;
        ulong t=clock();
        long long duzina=G->najkraciPut(c_pocetak,c_kraj,c_posjetiti,c_ne_posjetiti,najkraci_put);
        t=clock()-t;

        izlaz<< "Optimalan put: ";
        while(!najkraci_put.empty())
        {
            string oz=najkraci_put.top();
            najkraci_put.pop();

            izlaz << oz << " ";
        }
        izlaz << endl << "Duzina: " << duzina << endl;
        izlaz << "Vrijeme izvrsavanja: " << double(t)/CLOCKS_PER_SEC << "s" << endl;

        ulaz.close();
        izlaz.close();

        cout << "Rezultati su upisani u datoteku!" << endl;
    }

    return 0;
}

