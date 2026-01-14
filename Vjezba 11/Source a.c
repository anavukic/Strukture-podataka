#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define HASH_SIZE 11

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

typedef struct {
    ListPos buckets[HASH_SIZE];
} HashTable;

int calculateHash(char* name);
HashTable* initTable();
int readCountriesToHash(HashTable* table, char* filename);
int insertCountrySorted(ListPos head, char* name, char* file);
int readCities(ListPos countryNode, char* filename);
TreePos insertCity(TreePos root, char* name, int pop);
int printTable(HashTable* table);
int printTree(TreePos root, int limit);
int searchHash(HashTable* table);
int freeTree(TreePos root);
int freeList(ListPos head);
int freeTable(HashTable* table);


int main() {
    HashTable* table = initTable();
    if (!table) {
        return -1;
    }

    if (readCountriesToHash(table, "drzave.txt") != 0) {
        return -1;
    }

    printf("--- ZADATAK 11: HASH TABLICA DRZAVA ---\n");
    printTable(table);

    searchHash(table);
    freeTable(table);

    return 0;
}

int calculateHash(char* name) {
    int sum = 0;
    for (int i = 0; i < 5 && name[i] != '\0'; i++) {
        sum += (int)name[i];
    }
    return sum % HASH_SIZE;
}

HashTable* initTable() {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;
    for (int i = 0; i < HASH_SIZE; i++) {
        table->buckets[i] = (ListPos)malloc(sizeof(Country));
        if (table->buckets[i]) {
            table->buckets[i]->next = NULL;
            table->buckets[i]->cityRoot = NULL;
            strcpy(table->buckets[i]->name, "");
        }
    }
    return table;
}

int readCountriesToHash(HashTable* table, char* filename) {
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
        if (!newN) return NULL;
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

int printTable(HashTable* table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        ListPos p = table->buckets[i]->next;
        if (p) {
            printf("\n--- Bucket [%d] ---\n", i);
            while (p) {
                printf("Drzava: %s\n", p->name);
                printTree(p->cityRoot, 0);
                p = p->next;
            }
        }
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

int searchHash(HashTable* table) {
    char target[MAX];
    int limit;
    printf("\nPretraga - Unesite drzavu: ");
    scanf("%s", target);
    printf("Minimalni broj stanovnika: ");
    scanf("%d", &limit);

    int index = calculateHash(target);
    ListPos p = table->buckets[index]->next;

    while (p != NULL && strcmp(p->name, target) != 0)
        p = p->next;

    if (p) {
        printf("Gradovi u %s s populacijom > %d:\n", p->name, limit);
        printTree(p->cityRoot, limit);
    }
    else {
        printf("Drzava nije pronadjena.\n");
    }
    return 0;
}


int freeTree(TreePos root) {
    if (root == NULL) return 0;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
    return 0;
}

int freeList(ListPos head) {
    while (head != NULL) {
        ListPos temp = head;
        head = head->next;
        freeTree(temp->cityRoot);
        free(temp);
    }
    return 0;
}

int freeTable(HashTable* table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        freeList(table->buckets[i]);
    }
    free(table);
    return 0;
}

int printTable(HashTable* table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        ListPos p = table->buckets[i]->next;
        if (p) {
            printf("\n--- Bucket [%d] ---\n", i);
            while (p) {
                printf("Drzava: %s\n", p->name);
                printTree(p->cityRoot, 0);
                p = p->next;
            }
        }
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

int searchHash(HashTable* table) {
    char target[MAX];
    int limit;
    printf("\nPretraga - Unesite drzavu: ");
    scanf("%s", target);
    printf("Minimalni broj stanovnika: ");
    scanf("%d", &limit);

    int index = calculateHash(target); 
    ListPos p = table->buckets[index]->next;

    while (p != NULL && strcmp(p->name, target) != 0)
        p = p->next;

    if (p) {
        printf("Gradovi u %s s populacijom > %d:\n", p->name, limit);
        printTree(p->cityRoot, limit);
    }
    else {
        printf("Drzava nije pronadjena.\n");
    }
    return 0;
}


int freeTree(TreePos root) {
    if (root == NULL) return 0;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
    return 0;
}

int freeList(ListPos head) {
    while (head != NULL) {
        ListPos temp = head;
        head = head->next;
        freeTree(temp->cityRoot);
        free(temp); 
    }
    return 0;
}

int freeTable(HashTable* table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        freeList(table->buckets[i]);
    }
    free(table);
    return 0;
}