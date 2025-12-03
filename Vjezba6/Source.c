#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "racuni.h"


Artikl* noviArtikl(const char* naziv, int kolicina, float cijena) {
    Artikl* a = malloc(sizeof(Artikl));
    strcpy(a->naziv, naziv);
    a->kolicina = kolicina;
    a->cijena = cijena;
    a->sljedeci = NULL;
    return a;
}

Racun* noviRacun(const char* datum) {
    Racun* r = malloc(sizeof(Racun));
    strcpy(r->datum, datum);
    r->artikli = NULL;
    r->sljedeci = NULL;
    return r;
}

Artikl* ubaciArtiklSortirano(Artikl* glava, Artikl* novi) {

    Artikl* p = glava;

    if (p == NULL || strcmp(novi->naziv, p->naziv) < 0) {
        novi->sljedeci = p;
        return novi;
    }

    Artikl* q = p->sljedeci;

    while (q != NULL && strcmp(q->naziv, novi->naziv) < 0) {
        p = q;
        q = q->sljedeci;
    }

    p->sljedeci = novi;
    novi->sljedeci = q;

    return glava;
}


Racun* ubaciRacunSortirano(Racun* glava, Racun* novi) {

    Racun* p = glava;

    if (p == NULL || strcmp(novi->datum, p->datum) < 0) {
        novi->sljedeci = p;
        return novi;
    }

    Racun* q = p->sljedeci;

    while (q != NULL && strcmp(q->datum, novi->datum) < 0) {
        p = q;
        q = q->sljedeci;
    }

    p->sljedeci = novi;
    novi->sljedeci = q;

    return glava;
}


Racun* ucitajJedanRacun(const char* imeDatoteke, Racun* lista) {

    FILE* f = fopen(imeDatoteke, "r");
    if (!f) return lista;

    char datum[20];
    fgets(datum, sizeof(datum), f);
    datum[strcspn(datum, "\n")] = '\0';

    Racun* r = noviRacun(datum);

    char linija[150];
    while (fgets(linija, sizeof(linija), f)) {
        char naziv[50];
        int kolicina;
        float cijena;

        sscanf(linija, "%[^,], %d, %f", naziv, &kolicina, &cijena);

        Artikl* a = noviArtikl(naziv, kolicina, cijena);
        r->artikli = ubaciArtiklSortirano(r->artikli, a);
    }

    fclose(f);

    return ubaciRacunSortirano(lista, r);
}


Racun* ucitajSveRacune(const char* popis) {

    FILE* f = fopen(popis, "r");
    if (!f) return NULL;

    Racun* lista = NULL;
    char datoteka[100];

    while (fgets(datoteka, sizeof(datoteka), f)) {
        datoteka[strcspn(datoteka, "\n")] = '\0';
        lista = ucitajJedanRacun(datoteka, lista);
    }

    fclose(f);
    return lista;
}


float izracunaj(Racun* lista, const char* artikl, const char* od, const char* do_, int* ukKolicina) {

    *ukKolicina = 0;
    float ukupno = 0;

    Racun* p = lista;

    while (p != NULL) {

        if (strcmp(p->datum, od) >= 0 && strcmp(p->datum, do_) <= 0) {

            Artikl* q = p->artikli;

            while (q != NULL) {
                if (strcmp(q->naziv, artikl) == 0) {
                    *ukKolicina += q->kolicina;
                    ukupno += q->kolicina * q->cijena;
               }
                q = q->sljedeci;
            }
        }

        p = p->sljedeci;
    }

    return ukupno;
}

int main() {
    Racun* lista = ucitajSveRacune("racuni.txt");
    printf("\n--- SVI RACUNI I ARTIKLI SORTIRANO ---\n");

    Racun* r = lista;
    while (r != NULL) {
        printf("\nRacun (%s):\n", r->datum);

        Artikl* a = r->artikli;
        while (a != NULL) {
            printf(" - %s (%d kom, %.2f EUR)\n", a->naziv, a->kolicina, a->cijena);
            a = a->sljedeci;
        }

        r = r->sljedeci;
    }

    printf("\n---------------------------------\n");

    char artikl[50];
    char od[20];
    char do_[20];

    printf("Upisati trazeni artikl: ");
    scanf(" %49s", artikl);

    printf("Datum od (YYYY-MM-DD): ");
    scanf(" %19s", od);

    printf("Datum do (YYYY-MM-DD): ");
    scanf(" %19s", do_);

    int ukupnaKolicina = 0;
    float ukupno = izracunaj(lista, artikl, od, do_, &ukupnaKolicina);

    printf("\nUkupno kupljeno: %d kom\n", ukupnaKolicina);
    printf("Ukupno potroseno: %.2f EUR\n", ukupno);

    return 0;
}