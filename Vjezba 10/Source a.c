#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct _city;
typedef struct _city* TreePos;
typedef struct _city {
    char name[MAX];
    int population;
    TreePos left;
    TreePos right;
} City;

struct _country;
typedef struct _country* ListPos;
typedef struct _country {
    char name[MAX];
    TreePos cityRoot;
    ListPos next;
} Country;

int readCountries(ListPos head, char* filename);
int insertCountry(ListPos head, char* name, char* file);
int readCities(ListPos countryNode, char* filename);
TreePos insertCity(TreePos root, char* name, int pop);
int printAll(ListPos p);
int printTree(TreePos root, int limit);
int search(ListPos head);

int main() {
    Country head;
    head.next = NULL;
    head.cityRoot = NULL;
    strcpy(head.name, "");

    if (readCountries(&head, "drzave.txt") != 0) {
        printf("Greska: Datoteka nije pronadjena!\n");
        return -1;
    }

    printf("--- ZADATAK A: LISTA -> STABLO ---\n");
    printAll(head.next);
    search(head.next);

    return 0;
}

int readCountries(ListPos head, char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;
    char cName[MAX], cFile[MAX];
    while (fscanf(fp, "%s %s", cName, cFile) == 2) {
        insertCountry(head, cName, cFile);
    }
    fclose(fp);
    return 0;
}

int insertCountry(ListPos head, char* name, char* file) {
    ListPos p = head;
    while (p->next != NULL && strcmp(p->next->name, name) < 0)
        p = p->next;

    ListPos newC = (ListPos)malloc(sizeof(Country));
    if (!newC) return -1;
    strcpy(newC->name, name);
    newC->cityRoot = NULL;
    readCities(newC, file);

    newC->next = p->next;
    p->next = newC;
    return 0;
}

int readCities(ListPos countryNode, char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;
    char cName[MAX];
    int pop;
    while (fscanf(fp, " %[^,],%d", cName, &pop) == 2) {
        countryNode->cityRoot = insertCity(countryNode->cityRoot, cName, pop);
    }
    fclose(fp);
    return 0;
}

TreePos insertCity(TreePos root, char* name, int pop) {
    if (root == NULL) {
        TreePos newN = (TreePos)malloc(sizeof(City));
        strcpy(newN->name, name);
        newN->population = pop;
        newN->left = newN->right = NULL;
        return newN;
    }
    if (pop < root->population)
        root->left = insertCity(root->left, name, pop);
    else if (pop > root->population)
        root->right = insertCity(root->right, name, pop);
    else {
        if (strcmp(name, root->name) < 0) root->left = insertCity(root->left, name, pop);
        else root->right = insertCity(root->right, name, pop);
    }
    return root;
}

int printAll(ListPos p) {
    while (p != NULL) {
        printf("\nDrzava: %s\n", p->name);
        printTree(p->cityRoot, 0);
        p = p->next;
    }
    return 0;
}

int printTree(TreePos root, int limit) {
    if (root == NULL) return 0;
    printTree(root->left, limit);
    if (root->population >= limit)
        printf("  - %s (%d)\n", root->name, root->population);
    printTree(root->right, limit);
    return 0;
}

int search(ListPos head) {
    char target[MAX];
    int limit;
    printf("\nUnesite ime drzave: ");
    scanf("%s", target);
    printf("Minimalna populacija: ");
    scanf("%d", &limit);

    ListPos p = head;
    while (p != NULL && strcmp(p->name, target) != 0) p = p->next;

    if (p) printTree(p->cityRoot, limit);
    else printf("Nema te drzave.\n");
    return 0;
}