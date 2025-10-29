#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define file_not_found -1

typedef struct Person* Position;
typedef struct Person {
    char firstName[30];
    char lastName[30];
    int birthYear;
    Position NEXT;
} Person;

// Deklaracije funkcija
int InsertAtBeginning(Position, char*, char*, int);
int InsertAtEnd(Position, char*, char*, int);
int PrintList(Position);
Position FindByLastName(char*, Position);
int DeleteByLastName(char*, Position);
int InsertAfter(Position, char*, char*, int);
int InsertBefore(Position, char*, char*, char*, int);
int SortByLastName(Position);
int SaveListToFile(Position);
int LoadListFromFile(Position);
int FreeList(Position);
int Start(Position);

int main() {
    Person Head = { "", "", 0, NULL };
    return Start(&Head);
}

int Start(Position head) {
    char firstName[30], lastName[30], targetLastName[30];
    int birthYear, n;

    printf("Koliko osoba zelite unijeti? ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("\nIme: ");
        scanf("%s", firstName);
        printf("Prezime: ");
        scanf("%s", lastName);
        printf("Godina rodjenja: ");
        scanf("%d", &birthYear);

        InsertAtEnd(head, firstName, lastName, birthYear);
    }

    printf("\nLista osoba:\n");
    PrintList(head->NEXT);

    // Pretraga
    printf("\nUnesite prezime za pronalazak: ");
    scanf("%s", lastName);
    Position found = FindByLastName(lastName, head->NEXT);
    if (found)
        printf("Pronadena osoba: %s %s %d\n", found->firstName, found->lastName, found->birthYear);

    // InsertAfter
    if (found) {
        printf("\nUnesite osobu koju zelite ubaciti nakon %s:\n", lastName);
        printf("Ime: ");
        scanf("%s", firstName);
        printf("Prezime: ");
        scanf("%s", lastName);
        printf("Godina rodjenja: ");
        scanf("%d", &birthYear);
        InsertAfter(found, firstName, lastName, birthYear);
    }

    // InsertBefore
    printf("\nUnesite prezime osobe prije koje zelite ubaciti novu osobu: ");
    scanf("%s", targetLastName);
    printf("Ime: ");
    scanf("%s", firstName);
    printf("Prezime: ");
    scanf("%s", lastName);
    printf("Godina rodjenja: ");
    scanf("%d", &birthYear);

    if (InsertBefore(head, firstName, lastName, targetLastName, birthYear) != 0)
        printf("Prezime nije pronadeno za InsertBefore.\n");

    // Brisanje
    printf("\nUnesite prezime osobe koju zelite obrisati: ");
    scanf("%s", lastName);
    DeleteByLastName(lastName, head);

    // Sortiranje i ispis
    printf("\nSortiranje liste po prezimenima\n");
    SortByLastName(head);
    PrintList(head->NEXT);

    // Spremanje u datoteku
    SaveListToFile(head->NEXT);
    // Učitavanje iz datoteke i ispis
    if (LoadListFromFile(head) == file_not_found)
        printf("Datoteka nije pronadena.\n");

    FreeList(head->NEXT);
    head->NEXT = NULL;

    return 0;
}

// Funkcije

int InsertAtBeginning(Position p, char* firstName, char* lastName, int year) {
    Position q = (Position)malloc(sizeof(Person));
    if (!q) { printf("Greska\n"); return -1; }
    strcpy(q->firstName, firstName);
    strcpy(q->lastName, lastName);
    q->birthYear = year;
    q->NEXT = p->NEXT;
    p->NEXT = q;
    return 0;
}

int InsertAtEnd(Position p, char* firstName, char* lastName, int year) {
    while (p->NEXT != NULL) p = p->NEXT;
    Position q = (Position)malloc(sizeof(Person));
    if (!q) { printf("Greska\n"); return -1; }
    strcpy(q->firstName, firstName);
    strcpy(q->lastName, lastName);
    q->birthYear = year;
    q->NEXT = NULL;
    p->NEXT = q;
    return 0;
}

int PrintList(Position p) {
    while (p != NULL) {
        printf("%s %s %d\n", p->firstName, p->lastName, p->birthYear);
        p = p->NEXT;
    }
    return 0;
}

Position FindByLastName(char* lastName, Position p) {
    while (p != NULL && strcmp(p->lastName, lastName) != 0)
        p = p->NEXT;
    return p;
}

int DeleteByLastName(char* lastName, Position p) {
    Position temp;
    while (p->NEXT != NULL && strcmp(p->NEXT->lastName, lastName) != 0)
        p = p->NEXT;
    if (p->NEXT == NULL) {
        printf("Osoba s prezimenom %s nije pronadena.\n", lastName);
        return -1;
    }
    temp = p->NEXT;
    p->NEXT = temp->NEXT;
    free(temp);
    printf("Osoba s prezimenom %s je obrisana.\n", lastName);
    return 0;
}

int InsertAfter(Position p, char* firstName, char* lastName, int year) {
    if (!p) return -1;
    Position q = (Position)malloc(sizeof(Person));
    if (!q) { printf("Greska\n"); return -1; }
    strcpy(q->firstName, firstName);
    strcpy(q->lastName, lastName);
    q->birthYear = year;
    q->NEXT = p->NEXT;
    p->NEXT = q;
    return 0;
}

int InsertBefore(Position head, char* firstName, char* lastName, char* targetLastName, int year) {
    Position prev = head;
    while (prev->NEXT != NULL && strcmp(prev->NEXT->lastName, targetLastName) != 0)
        prev = prev->NEXT;

    if (prev->NEXT == NULL) return -1;
    return InsertAfter(prev, firstName, lastName, year);
}

// Bubble sort
int SortByLastName(Position head) {
    if (!head->NEXT) return 0;
    Position p, q, temp;
    Position sorted = NULL;
    int swapped;

    do {
        swapped = 0;
        p = head;
        q = p->NEXT;
        while (q->NEXT != sorted) {
            if (strcmp(q->lastName, q->NEXT->lastName) > 0) {
                temp = q->NEXT;
                p->NEXT = temp;
                q->NEXT = temp->NEXT;
                temp->NEXT = q;
                swapped = 1;
            }
            p = p->NEXT;
            q = p->NEXT;
        }
        sorted = q;
    } while (swapped);

    return 0;
}

int SaveListToFile(Position p) {
    FILE* fp = fopen("osobe.txt", "w");
    if (!fp) { printf("Greska\n"); return -1; }
    while (p != NULL) {
        fprintf(fp, "%s %s %d\n", p->firstName, p->lastName, p->birthYear);
        p = p->NEXT;
    }
    fclose(fp);
    printf("\nLista je spremljena u datoteku osobe.txt\n");
    return 0;
}

int LoadListFromFile(Position head) {
    FreeList(head->NEXT);
    head->NEXT = NULL;

    FILE* fp = fopen("osobe.txt", "r");
    if (!fp) {
        return file_not_found;
    }

    char firstName[30], lastName[30];
    int year;

    printf("\nLista ucitana iz datoteke:\n");
    while (fscanf(fp, "%s %s %d", firstName, lastName, &year) == 3) {
        InsertAtEnd(head, firstName, lastName, year);
    }

    fclose(fp);
    PrintList(head->NEXT);

    return 0;
}

int FreeList(Position p) {
    Position temp;
    while (p != NULL) {
        temp = p;
        p = p->NEXT;
        free(temp);
    }
    return 0;
}
