#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define FILE_NOT_FOUND -1

// 1) Definicija strukture
typedef struct Polinom* Position;
typedef struct Polinom {
    int coeficient;
    int power;
    Position Next;
} Polinom;

// Deklaracije
Position CreateElement(int coef, int power);
int InsertSorted(Position Head, Position newEl);
int ReadFromFile(Position firstPoly, Position secondPoly, char* filename);
int PrintPoly(Position first);
int AddPoly(Position result, Position p1, Position p2);
int MultiplyPoly(Position result, Position p1, Position p2);
int DeleteZero(Position Head);

int main() {

    // 2) Inicijalizacija head elemenata (glava lista)
    Polinom Head = { .coeficient = 0, .power = 0, .Next = NULL };
    Polinom firstPoly = { .coeficient = 0, .power = 0, .Next = NULL };
    Polinom secondPoly = { .coeficient = 0, .power = 0, .Next = NULL };
    Polinom sum = { .coeficient = 0, .power = 0, .Next = NULL };
    Polinom product = { .coeficient = 0, .power = 0, .Next = NULL };

    if (ReadFromFile(&firstPoly, &secondPoly, "polinomi.txt") == FILE_NOT_FOUND)
        return EXIT_FAILURE;

    printf("Prvi polinom: ");
    PrintPoly(firstPoly.Next);

    printf("Drugi polinom: ");
    PrintPoly(secondPoly.Next);

    AddPoly(&sum, firstPoly.Next, secondPoly.Next);
    printf("\nZbroj: ");
    PrintPoly(sum.Next);

    MultiplyPoly(&product, firstPoly.Next, secondPoly.Next);
    printf("\nUmnozak: ");
    PrintPoly(product.Next);

    return 0;
}

// 3) Funkcija za čitanje iz datoteke
int ReadFromFile(Position firstPoly, Position secondPoly, char* filename) {

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Greška: datoteka '%s' nije pronađena!\n", filename);
        return FILE_NOT_FOUND;
    }

    int coef, power;

    // Prvi polinom
    while (fscanf(fp, "%d %d", &coef, &power) == 2) {
        Position newEl = CreateElement(coef, power);
        InsertSorted(firstPoly, newEl);

        int c = fgetc(fp);
        if (c == '\n' || c == EOF)
            break;
        else
            ungetc(c, fp);
    }

    // Drugi polinom
    while (fscanf(fp, "%d %d", &coef, &power) == 2) {
        Position newEl = CreateElement(coef, power);
        InsertSorted(secondPoly, newEl);
    }

    fclose(fp);
    return 0;
}

// 4) Alokacija memorije
Position CreateElement(int coef, int power) {
    Position newEl = (Position)malloc(sizeof(Polinom));
    if (!newEl) {
        printf("Greška u alokaciji memorije!\n");
        return NULL;
    }

    newEl->coeficient = coef;
    newEl->power = power;
    newEl->Next = NULL;
    return newEl;
}

// Sortirani unos 
int InsertSorted(Position Head, Position newEl) {

    Position Prev = Head;
    Position q = Head->Next;

    // a) Ako je lista prazna
    if (q == NULL) {
        newEl->Next = Prev->Next;
        Prev->Next = newEl;
        return 0;
    }

    // b) Ako postoji element s većom potencijom – idemo dalje
    while (q != NULL && q->power > newEl->power) {
        Prev = q;
        q = q->Next;
    }

    // c) Ako je ista potencija – zbroji
    if (q != NULL && q->power == newEl->power) {
        q->coeficient += newEl->coeficient;
        if (q->coeficient == 0) {
            DeleteZero(Head);
        }
        free(newEl);
        return 0;
    }

    // d) Ako je manja potencija – umetni novi
    newEl->Next = q;
    Prev->Next = newEl;

    return 0;
}

// 5) Zbrajanje
int AddPoly(Position result, Position p1, Position p2) {
    Position q = p1;

    // Kopiraj prvi polinom
    while (q != NULL) {
        Position newEl = CreateElement(q->coeficient, q->power);
        InsertSorted(result, newEl);
        q = q->Next;
    }

    // Kopiraj drugi polinom
    q = p2;
    while (q != NULL) {
        Position newEl = CreateElement(q->coeficient, q->power);
        InsertSorted(result, newEl);
        q = q->Next;
    }

    DeleteZero(result);
    return 0;
}

// 6) Množenje
int MultiplyPoly(Position result, Position p1, Position p2) {
    Position temp2 = p2;

    while (p1 != NULL) {
        p2 = temp2;
        while (p2 != NULL) {
            Position newEl = CreateElement(
                p1->coeficient * p2->coeficient,
                p1->power + p2->power
            );
            InsertSorted(result, newEl);
            p2 = p2->Next;
        }
        p1 = p1->Next;
    }

    DeleteZero(result);
    return 0;
}

// 7) Brisanje elemenata sa koeficijentom 0
int DeleteZero(Position Head) {
    Position Prev = Head;
    Position q = Head->Next;

    while (q != NULL) {
        if (q->coeficient == 0) {
            Prev->Next = q->Next;
            free(q);
            q = Prev->Next;
        }
        else {
            Prev = q;
            q = q->Next;
        }
    }

    return 0;
}

// Ispis polinoma
int PrintPoly(Position first) {
    if (first == NULL) {
        printf("0\n");
        return 0;
    }

    while (first != NULL) {
        if (first->coeficient > 0)
            printf("+");
        printf("%d", first->coeficient);
        if (first->power > 0)
            printf("x^%d ", first->power);
        else
            printf(" ");
        first = first->Next;
    }

    printf("\n");
    return 0;
}

