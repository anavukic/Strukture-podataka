#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct _city* ListPos;
typedef struct _city {
    char name[MAX];
    int population;
    ListPos next;
} City;

typedef struct _country* TreePos;
typedef struct _country {
    char name[MAX];
    ListPos cityListHead;
    TreePos left;
    TreePos right;
} Country;

TreePos insertCountry(TreePos root, char* name, char* file);
int readCities(Country* country, char* file);
int insertCity(ListPos head, char* name, int pop);
int printAll(TreePos root);
int printList(ListPos p, int limit);
int search(TreePos root);

int main() {
    TreePos root = NULL;
    FILE* fp = fopen("drzave.txt", "r");
    if (!fp) return -1;

    char cName[MAX], cFile[MAX];
    while (fscanf(fp, "%s %s", cName, cFile) == 2) {
        root = insertCountry(root, cName, cFile);
    }
    fclose(fp);

    printf("--- ZADATAK B: STABLO -> LISTA ---\n");
    printAll(root);
    search(root);

    return 0;
}

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
    if (strcmp(name, root->name) < 0) root->left = insertCountry(root->left, name, file);
    else if (strcmp(name, root->name) > 0) root->right = insertCountry(root->right, name, file);
    return root;
}

int readCities(Country* country, char* file) {
    FILE* fp = fopen(file, "r");
    if (!fp) return -1;
    char cName[MAX];
    int pop;
    while (fscanf(fp, " %[^,],%d", cName, &pop) == 2) {
        insertCity(country->cityListHead, cName, pop);
    }
    fclose(fp);
    return 0;
}

int insertCity(ListPos head, char* name, int pop) {
    ListPos p = head;
    while (p->next != NULL && p->next->population > pop) p = p->next;
    while (p->next != NULL && p->next->population == pop && strcmp(p->next->name, name) < 0) p = p->next;

    ListPos newC = (ListPos)malloc(sizeof(City));
    strcpy(newC->name, name);
    newC->population = pop;
    newC->next = p->next;
    p->next = newC;
    return 0;
}

int printAll(TreePos root) {
    if (root == NULL) return 0;
    printAll(root->left);
    printf("\nDrzava: %s\n", root->name);
    printList(root->cityListHead->next, 0);
    printAll(root->right);
    return 0;
}

int printList(ListPos p, int limit) {
    while (p != NULL) {
        if (p->population >= limit) printf("  -> %s (%d)\n", p->name, p->population);
        p = p->next;
    }
    return 0;
}

int search(TreePos root) {
    char target[MAX];
    int limit;
    printf("\nPretraga drzave: ");
    scanf("%s", target);
    printf("Granica populacije: ");
    scanf("%d", &limit);

    TreePos curr = root;
    while (curr) {
        int cmp = strcmp(target, curr->name);
        if (cmp == 0) return printList(curr->cityListHead->next, limit);
        if (cmp < 0) curr = curr->left;
        else curr = curr->right;
    }
    printf("Nije pronadjena.\n");
    return 0;
}