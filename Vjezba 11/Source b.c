#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define HASH_SIZE 11

typedef struct _city* CityPos;
typedef struct _city {
    char name[MAX];
    int population;
    CityPos next;
} City;

struct _country;
typedef struct _country* ListPos;
typedef struct _country {
    char name[MAX];
    CityPos cityListHead; 
    ListPos next;
} Country;

typedef struct {
    ListPos buckets[HASH_SIZE];
} HashTable;

int calculateHash(char* name);
HashTable* initTable();
int readCountries(HashTable* table, char* filename);
int insertCountrySorted(ListPos head, char* name, char* file);
int readCitiesToList(ListPos countryNode, char* filename);
int insertCitySortedList(CityPos head, char* name, int pop);
int printTable(HashTable* table);
int printList(CityPos p, int limit);
int searchHash(HashTable* table);
int freeTable(HashTable* table);


int main() {
    HashTable* table = initTable();
    if (!table) return -1;

    if (readCountries(table, "drzave.txt") != 0) {
        return -1;
    }

    printf("--- ZADATAK 11.b: HASH TABLICA -> LISTA GRADOVA ---\n");
    printTable(table);
    searchHash(table);

    freeTable(table);
    return 0;
}


int calculateHash(char* name) {
    int sum = 0;
    for (int i = 0; i < 5 && name[i] != '\0'; i++) sum += (int)name[i];
    return sum % HASH_SIZE;
}

HashTable* initTable() {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;
    for (int i = 0; i < HASH_SIZE; i++) {
        table->buckets[i] = (ListPos)malloc(sizeof(Country));
        table->buckets[i]->next = NULL;
        table->buckets[i]->cityListHead = NULL;
    }
    return table;
}

int readCountries(HashTable* table, char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;
    char cName[MAX], cFile[MAX];
    while (fscanf(fp, "%s %s", cName, cFile) == 2) {
        int index = calculateHash(cName);
        insertCountrySorted(table->buckets[index], cName, cFile);
    }
    fclose(fp);
    return 0;
}

int insertCountrySorted(ListPos head, char* name, char* file) {
    ListPos p = head;
    while (p->next != NULL && strcmp(p->next->name, name) < 0) p = p->next;

    ListPos newC = (ListPos)malloc(sizeof(Country));
    strcpy(newC->name, name);
    newC->cityListHead = (CityPos)malloc(sizeof(City)); 
    newC->cityListHead->next = NULL;

    readCitiesToList(newC, file);

    newC->next = p->next;
    p->next = newC;
    return 0;
}

int readCitiesToList(ListPos countryNode, char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;
    char cName[MAX];
    int pop;
    while (fscanf(fp, " %[^,],%d", cName, &pop) == 2) {
        insertCitySortedList(countryNode->cityListHead, cName, pop);
    }
    fclose(fp);
    return 0;
}

int insertCitySortedList(CityPos head, char* name, int pop) {
    CityPos p = head;
    while (p->next != NULL && p->next->population < pop) p = p->next;
    while (p->next != NULL && p->next->population == pop && strcmp(p->next->name, name) < 0) p = p->next;

    CityPos newCity = (CityPos)malloc(sizeof(City));
    strcpy(newCity->name, name);
    newCity->population = pop;
    newCity->next = p->next;
    p->next = newCity;
    return 0;
}

int printTable(HashTable* table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        ListPos p = table->buckets[i]->next;
        if (p) {
            printf("\n--- Bucket [%d] ---\n", i);
            while (p) {
                printf("Drzava: %s\n", p->name);
                printList(p->cityListHead->next, 0);
                p = p->next;
            }
        }
    }
    return 0;
}

int printList(CityPos p, int limit) {
    while (p != NULL) {
        if (p->population >= limit) printf("  -> %s (%d)\n", p->name, p->population);
        p = p->next;
    }
    return 0;
}

int searchHash(HashTable* table) {
    char target[MAX];
    int limit;
    printf("\nPretraga drzave: ");
    scanf("%s", target);
    printf("Granica populacije: ");
    scanf("%d", &limit);

    int index = calculateHash(target);
    ListPos p = table->buckets[index]->next;
    while (p && strcmp(p->name, target) != 0) p = p->next;

    if (p) printList(p->cityListHead->next, limit);
    else printf("Nije pronadjena.\n");
    return 0;
}

int freeTable(HashTable* table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        ListPos p = table->buckets[i];
        while (p) {
            ListPos tempCountry = p;
            p = p->next;
            CityPos c = tempCountry->cityListHead;
            while (c) {
                CityPos tempCity = c;
                c = c->next;
                free(tempCity);
            }
            free(tempCountry);
        }
    }
    free(table);
    return 0;
}