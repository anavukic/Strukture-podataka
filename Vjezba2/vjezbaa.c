#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NOT_FOUND "Greska"

typedef struct Person* Position;

typedef struct Person {
    char firstName[30];
    char lastName[30];
    int birthYear;
    Position NEXT;
} Person;

int InsertAtBeginning(Position, char*, char*, int);
int PrintList(Position);
int InsertAtEnd(Position, char*, char*, int);
Position FindByLastName(char*, Position);
int DeleteByLastName(char*, Position);

int main() {
    Person Head = { "", "", 0, NULL };

    InsertAtBeginning(&Head, "Ana", "Anic", 1980);
    InsertAtBeginning(&Head, "Mate", "Matic", 1970);
    InsertAtEnd(&Head, "Goge", "Gogic", 1978);

    printf("Lista osoba:\n");
    PrintList(Head.NEXT);

    Position found = FindByLastName("Anic", Head.NEXT);
    if (found != NULL)
        printf("\nPronadena osoba: %s %s %d\n", found->firstName, found->lastName, found->birthYear);
    else
        printf("\nOsoba nije pronaðena.\n");

    DeleteByLastName("Matic", &Head);
    printf("\nLista nakon brisanja:\n");
    PrintList(Head.NEXT);

    return 0;
}

int InsertAtBeginning(Position p, char* firstName, char* lastName, int year) {
    int i;
    Position q = (Position)malloc(sizeof(Person));
    if (q == NULL) {
        printf(FILE_NOT_FOUND "\n");
        return EXIT_FAILURE;
    }
    for (i = 0; i < 30; i++) {
        q->firstName[i] = firstName[i];
        q->lastName[i] = lastName[i];
        if (firstName[i] == '\0' && lastName[i] == '\0') break;
    }
    q->birthYear = year;
    q->NEXT = p->NEXT;
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

int InsertAtEnd(Position p, char* firstName, char* lastName, int year) {
    Position q;
    int i;

    while (p->NEXT != NULL) {
        p = p->NEXT;
    }
    q = (Position)malloc(sizeof(Person));
    if (q == NULL) {
        printf(FILE_NOT_FOUND "\n");
        return EXIT_FAILURE;
    }
    for (i = 0; i < 30; i++) {
        q->firstName[i] = firstName[i];
        q->lastName[i] = lastName[i];
        if (firstName[i] == '\0' && lastName[i] == '\0') break;
    }
    q->birthYear = year;
    q->NEXT = NULL;
    p->NEXT = q;
    return 0;
}

Position FindByLastName(char* lastName, Position p) {
    while (p != NULL && strcmp(p->lastName, lastName) != 0) {
        p = p->NEXT;
    }
    return p;
}

int DeleteByLastName(char* lastName, Position p) {
    Position temp;

    while (p->NEXT != NULL && strcmp(p->NEXT->lastName, lastName) != 0) {
        p = p->NEXT;
    }

    if (p->NEXT == NULL) {
        printf("Osoba s prezimenom %s nije pronaðena.\n", lastName);
        return EXIT_FAILURE;
    }

    temp = p->NEXT;
    p->NEXT = temp->NEXT;
    temp->NEXT = NULL;
    free(temp);

    printf("Osoba s prezimenom %s je obrisana.\n", lastName);
    return 0;
}

