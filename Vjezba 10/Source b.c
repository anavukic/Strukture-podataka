#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// Struktura za grad (vezana lista)
struct _city;
typedef struct _city* ListPos;
typedef struct _city {
    char name[MAX];
    int population;
    ListPos next;
} City;

// Struktura za drzavu (stablo)
struct _country;
typedef struct _country* TreePos;
typedef struct _country {
    char name[MAX];
    ListPos cityListHead;
    TreePos left;
    TreePos right;
} Country;

// Prototipi
TreePos insertCountry(TreePos root, char* name, char* file);
int readCities(Country* country, char* file);
int insertCity(ListPos head, char* name, int pop);
void printAll(TreePos root);
void printList(ListPos p, int limit);
TreePos search(TreePos root, char* target);

int main() {
    TreePos root = NULL;
    FILE* fp = fopen("drzave.txt", "r");

    if (!fp) {
        printf("Greska: Datoteka drzave.txt nije pronadjena.\n");
        return -1;
    }

    char cName[MAX], cFile[MAX];
    while (fscanf(fp, "%s %s", cName, cFile) == 2) {
        root = insertCountry(root, cName, cFile);
    }
    fclose(fp);

    printf("--- ZADATAK B: STABLO DRZAVA -> LISTA GRADOVA ---\n");
    printAll(root);

    char target[MAX];
    int limit;
    printf("\nUnesite ime drzave: ");
    scanf("%s", target);
    printf("Unesite broj stanovnika: ");
    scanf("%d", &limit);

    TreePos found = search(root, target);
    if (found) {
        printf("Gradovi u %s s vise od %d stanovnika:\n", found->name, limit);
        printList(found->cityListHead->next, limit);
    }
    else {
        printf("Drzava nije pronadjena.\n");
    }

    return 0;
}

// Rekurzivno umetanje drzave u stablo
TreePos insertCountry(TreePos root, char* name, char* file) {
    if (root == NULL) {
        TreePos newN = (TreePos)malloc(sizeof(Country));
        strcpy(newN->name, name);
        newN->cityListHead = (ListPos)malloc(sizeof(City));
        newN->cityListHead->next = NULL;
        readCities(newN, file);
        newN->left = newN->right = NULL;
        return newN;
    }
    if (strcmp(name, root->name) < 0)
        root->left = insertCountry(root->left, name, file);
    else if (strcmp(name, root->name) > 0)
        root->right = insertCountry(root->right, name, file);
    return root;
}

int readCities(Country* country, char* file) {
    FILE* fp = fopen(file, "r");
    if (!fp) return -1;
    char cityName[MAX];
    int pop;
    while (fscanf(fp, " %[^,],%d", cityName, &pop) == 2) {
        insertCity(country->cityListHead, cityName, pop);
    }
    fclose(fp);
    return 0;
}

// Umetanje u listu (sortirano po broju stanovnika)
int insertCity(ListPos head, char* name, int pop) {
    ListPos p = head;
    // Sortiranje od najveceg prema najmanjem
    while (p->next != NULL && p->next->population > pop) {
        p = p->next;
    }
    // Ako je ista populacija, sortiraj po imenu
    while (p->next != NULL && p->next->population == pop && strcmp(p->next->name, name) < 0) {
        p = p->next;
    }

    ListPos newC = (ListPos)malloc(sizeof(City));
    strcpy(newC->name, name);
    newC->population = pop;
    newC->next = p->next;
    p->next = newC;
    return 0;
}

void printAll(TreePos root) {
    if (root == NULL) return;
    printAll(root->left);
    printf("\nDrzava: %s\n", root->name);
    printList(root->cityListHead->next, 0);
    printAll(root->right);
}

void printList(ListPos p, int limit) {
    while (p != NULL) {
        if (p->population >= limit) {
            printf("  -> %s (%d)\n", p->name, p->population);
        }
        p = p->next;
    }
}

TreePos search(TreePos root, char* target) {
    if (root == NULL) return NULL;
    int cmp = strcmp(target, root->name);
    if (cmp == 0) return root;
    if (cmp < 0) return search(root->left, target);
    return search(root->right, target);
}