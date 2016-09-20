#include "generator.h"
#include <algorithm>
#include <ctime>
#include <climits>
#define INF 100000000
using namespace std;
ulong kodiraj_granu(ulong n, ulong prvi, ulong drugi) { return prvi*n + drugi; }
void dekodiraj_granu(ulong n, ulong kod, ulong &prvi, ulong &drugi) { prvi=kod/n; drugi=kod%n; }
double generisi_primjer(ulong n,ulong m,ulong dmax,bool povezanost, Graf<ulong>*& G)
{
    ulong t=clock();
    const ulong MAX_TEZINA(100);

    // stvaranje grafa sa n cvorova oznacenih sa 0..n-1
    ulong *niz=new ulong[n];
    for (ulong i=0; i<n; i++)
        niz[i]=i;
    G = new Graf<ulong>(n,niz);

    // tvaranje niza grane_kod koji ce cuvati kod grane
    ulong *grane_kod = new ulong[n*n];
    for (ulong i=0; i<n*n; i++)
        grane_kod[i]=i;

    ulong broj_ubacenih=0;

    // u slucaju da moramo forsirati povezanost, prvo napravimo pokrivajuce stablo
    if (povezanost)
    {
        //prvo primjenimo algoritam za konstruisanje stabla
        vector<ulong> Q(grane_kod,grane_kod+n);
        random_shuffle(Q.begin(),Q.end());
        broj_ubacenih=1;

        while (broj_ubacenih<n)
        {
            ulong i=rand()%broj_ubacenih;
            ulong j=broj_ubacenih + rand()%(n-broj_ubacenih);

            G->dodajGranuI(Q[i],Q[j],1+rand()%MAX_TEZINA);
            swap(grane_kod[kodiraj_granu(n,Q[i],Q[j])],grane_kod[n*n-broj_ubacenih]);

            swap(Q[broj_ubacenih],Q[j]);
            broj_ubacenih++;
        }

        broj_ubacenih--;
    }

    ulong jos_treba_ubaciti=m-broj_ubacenih;
    ulong zaustavljanje=n*n-broj_ubacenih;
    broj_ubacenih=0;
    ulong i=0,v1,v2;

    random_shuffle(grane_kod,grane_kod+zaustavljanje);

    // sada napravimo niz od n^2 mogucih "grana" (zapravo brojeva iz kojih cemo dekodirati grane) i onda ga ispremjestamo, te za svaku
    // granu provjeravamo mozemo li je ubaciti i to radimo dok ne ubacimo dovoljan broj grana

    while (broj_ubacenih<jos_treba_ubaciti && i<zaustavljanje)
    {
        dekodiraj_granu(n,grane_kod[i],v1,v2);

        if (G->vratiStepenCvoraI(v1) < dmax && G->vratiStepenCvoraI(v2) < dmax && v1!=v2 && !G->tezinaGraneI(v1,v2))
        {
            G->dodajGranuI(v1,v2,1+rand()%MAX_TEZINA);

            broj_ubacenih++;
        }
        i++;
    }
    return double(clock()-t)/CLOCKS_PER_SEC;
}
void generisi_cvorove_za_obilazak(ulong n, vector<ulong> &posjete, ulong broj_za_posjetiti, vector<ulong> &zaobilasci, ulong broj_za_zaobici)
{
    vector<ulong> svi_cvorovi(n-2);
    for (ulong i=1; i<n-1; i++)
        svi_cvorovi[i-1]=i;

    random_shuffle(svi_cvorovi.begin(),svi_cvorovi.end());
    copy(svi_cvorovi.begin(),svi_cvorovi.begin()+broj_za_posjetiti,posjete.begin());
    copy(svi_cvorovi.begin()+broj_za_posjetiti,svi_cvorovi.begin()+broj_za_posjetiti+broj_za_zaobici,zaobilasci.begin());

}
