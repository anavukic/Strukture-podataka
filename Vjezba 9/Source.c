#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct tree {
    int value;
    struct tree* left, * right;
} Tree, * treePos;

treePos insert(treePos root, int val);
int replace(treePos root);
int inorderToFile(treePos root, FILE* fp);
treePos freeTree(treePos root);


int main() {
    treePos root = NULL;
    int niz[] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };
    FILE* fp = fopen("stablo.txt", "w");
    srand((unsigned)time(NULL));

    if (!fp) return -1;

    for (int i = 0; i < 10; i++) root = insert(root, niz[i]);

    printf("Prvo stablo: ");
    inorderToFile(root, fp); fprintf(fp, "\n");

    replace(root);

    printf("\nNakon replace-a: ");
    inorderToFile(root, fp); fprintf(fp, "\n");

    root = freeTree(root);
    printf("\nRandom stablo: ");
    for (int i = 0; i < 10; i++) root = insert(root, (rand() % 81) + 10);
    inorderToFile(root, fp);

    fclose(fp);
    return 0;
}

treePos insert(treePos root, int value) {
    if (!root) {
        root = (treePos)malloc(sizeof(Tree));
        if (root) { root->value = value; root->left = root->right = NULL; }
        return root;
    }
    if (value >= root->value) root->left = insert(root->left, value);
    else root->right = insert(root->right, value);
    return root;
}

int replace(treePos root) {
    if (!root) return 0;
    int oldVal = root->value;
    root->value = replace(root->left) + replace(root->right);
    return root->value + oldVal;
}
int inorderToFile(treePos root, FILE* fp) {
    if (root) {
        inorderToFile(root->left, fp);
        printf("%d ", root->value);
        fprintf(fp, "%d ", root->value);
        inorderToFile(root->right, fp);
    }
    return 0;
}

treePos freeTree(treePos root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
    return NULL;
}