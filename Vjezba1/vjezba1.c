#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Studenti {
    char ime[30];
    char prez[30];
    int bodovi;
} Studenti;

int brojRedaka() {
    FILE* fp = fopen("primjer.txt", "r");
    if (fp == NULL) {
        printf("Greška pri otvaranju datoteke u brojRedaka\n");
        return 0;
    }

    char ime[30], prez[30];
    int bodovi;
    int brojac = 0;

    while (fscanf(fp, "%s %s %d", ime, prez, &bodovi) == 3) {
        brojac++; 
    }

    fclose(fp);
    return brojac;
}

Studenti* ucitajStudente(FILE* fp, int n) {
    Studenti* x = (Studenti*)malloc(n * sizeof(Studenti));
    if (x == NULL) {
        printf("Greška pri alokaciji memorije!\n");
        return NULL;
    }

    int brojac = 0; 
    while (brojac < n) {
        fscanf(fp, "%s %s %d", x[brojac].ime, x[brojac].prez, &x[brojac].bodovi);
        brojac++;
    }

    return x;
}

int ispisiStudente(Studenti* x, int n) {
    int max_bodovi = x[0].bodovi;

    for (int i = 1; i < n; i++) {
        if (x[i].bodovi > max_bodovi)
            max_bodovi = x[i].bodovi;
    }

    printf("\nPopis studenata:\n");
    for (int i = 0; i < n; i++) {
        float rel = (float)x[i].bodovi / max_bodovi * 100;
        printf("%s %s ima %d bodova (%.2f%%)\n",
            x[i].ime, x[i].prez, x[i].bodovi, rel);
    }

    return max_bodovi;
}

int main() {
    FILE* fp;
    int n;

    fp = fopen("primjer.txt", "r");
    if (fp == NULL) {
        printf("Greška pri otvaranju datoteke!\n");
        return 1;
    }

    n = brojRedaka();

    Studenti* x = ucitajStudente(fp, n); 
    if (x == NULL) {
        fclose(fp);
        return 1;
    }

    ispisiStudente(x, n); 

    free(x);     
    fclose(fp);  
    return 0;    
}