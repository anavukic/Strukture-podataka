#ifndef RACUNI_H
#define RACUNI_H

typedef struct artikl {
    char naziv[50];
    int kolicina;
    float cijena;
    struct artikl* sljedeci;
} Artikl;

typedef struct racun {
    char datum[20];
    Artikl* artikli;
    struct racun* sljedeci;
} Racun;

Artikl* noviArtikl(const char* naziv, int kolicina, float cijena);
Racun* noviRacun(const char* datum);

Artikl* ubaciArtiklSortirano(Artikl* glava, Artikl* novi);
Racun* ubaciRacunSortirano(Racun* glava, Racun* novi);

Racun* ucitajJedanRacun(const char* imeDatoteke, Racun* lista);
Racun* ucitajSveRacune(const char* popis);

float izracunaj(Racun* lista, const char* artikl, const char* od, const char* do_, int* ukKolicina);

#endif

