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
int readCities(ListPos countryNode, char* filename);
TreePos insertCity(TreePos root, char* name, int pop);
int insertCountry(ListPos head, char* name, char* file);
void printAll(ListPos p);
void printTree(TreePos root, int limit);
void search(ListPos head);

int main() {
    Country head;
    head.next = NULL;
    head.cityRoot = NULL;
    strcpy(head.name, "");

    if (readCountries(&head, "drzave.txt") != 0) {
        printf("Greska: Nije moguce procitati drzave.txt\n");
        return -1;
    }

    printf("--- ZADATAK A: LISTA DRZAVA -> STABLO GRADOVA ---\n");
    printAll(head.next);

    search(head.next);

    return 0;
}

int readCountries(ListPos head, char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    char countryName[MAX], countryFile[MAX];
    while (fscanf(fp, "%s %s", countryName, countryFile) == 2) {
        insertCountry(head, countryName, countryFile);
    }

    fclose(fp);
    return 0;
}

int insertCountry(ListPos head, char* name, char* file) {
    ListPos p = head;
    while (p->next != NULL && strcmp(p->next->name, name) < 0) {
        p = p->next;
    }

    ListPos newCountry = (ListPos)malloc(sizeof(Country));
    if (!newCountry) return -1;

    strcpy(newCountry->name, name);
    newCountry->cityRoot = NULL;

    readCities(newCountry, file);

    newCountry->next = p->next;
    p->next = newCountry;

    return 0;
}

int readCities(ListPos countryNode, char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    char cityName[MAX];
    int pop;
    while (fscanf(fp, " %[^,],%d", cityName, &pop) == 2) {
        countryNode->cityRoot = insertCity(countryNode->cityRoot, cityName, pop);
    }

    fclose(fp);
    return 0;
}

TreePos insertCity(TreePos root, char* name, int pop) {
    if (root == NULL) {
        TreePos newNode = (TreePos)malloc(sizeof(City));
        if (!newNode) return NULL;
        strcpy(newNode->name, name);
        newNode->population = pop;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    if (pop < root->population) {
        root->left = insertCity(root->left, name, pop);
    }
    else if (pop > root->population) {
        root->right = insertCity(root->right, name, pop);
    }
    else {
        if (strcmp(name, root->name) < 0)
            root->left = insertCity(root->left, name, pop);
        else
            root->right = insertCity(root->right, name, pop);
    }
    return root;
}

void printAll(ListPos p) {
    while (p != NULL) {
        printf("\nDRZAVA: %s\n", p->name);
        printTree(p->cityRoot, 0);
        p = p->next;
    }
}

void printTree(TreePos root, int limit) {
    if (root == NULL) return;
    printTree(root->left, limit);
    if (root->population >= limit) {
        printf("  - %s: %d\n", root->name, root->population);
    }
    printTree(root->right, limit);
}

void search(ListPos head) {
    char target[MAX];
    int limit;
    printf("\nUnesite ime drzave: ");
    scanf("%s", target);
    printf("Minimalan broj stanovnika: ");
    scanf("%d", &limit);

    ListPos p = head;
    while (p != NULL && strcmp(p->name, target) != 0) {
        p = p->next;
    }

    if (p) {
        printf("Gradovi u %s s vise od %d stanovnika:\n", p->name, limit);
        printTree(p->cityRoot, limit);
    }
    else {
        printf("Drzava nije pronadjena.\n");
    }

}

