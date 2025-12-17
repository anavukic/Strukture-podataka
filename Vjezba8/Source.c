#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define MALLOC_ERROR -1
#define FILE_NOT_FOUND -2

// Struktura èvora
typedef struct tree {
    int value;
    struct tree* left;
    struct tree* right;
} Tree;

typedef Tree* treePos;

// PROTOTIPI
treePos insertTree(treePos root, int value);
treePos deleteTree(treePos root, int value);
treePos findMin(treePos root);
treePos searchTree(treePos root, int value);

treePos inorder(treePos root);
treePos preorder(treePos root);
treePos postorder(treePos root);
int printLevelorder(treePos root);

treePos freeTree(treePos root);


treePos insertTree(treePos root, int value) {
    if (root == NULL) {
        root = (treePos)malloc(sizeof(Tree));
        if (!root) {
            printf("Greska pri alokaciji!\n");
            return NULL;
        }
        root->value = value;
        root->left = NULL;
        root->right = NULL;
        return root;
    }
    if (value < root->value)
        root->left = insertTree(root->left, value);
    else if (value > root->value)
        root->right = insertTree(root->right, value);
    return root;
}

treePos findMin(treePos root) {
    while (root->left != NULL)
        root = root->left;
    return root;
}

treePos deleteTree(treePos root, int value) {
    if (root == NULL)
        return NULL;

    if (value < root->value)
        root->left = deleteTree(root->left, value);
    else if (value > root->value)
        root->right = deleteTree(root->right, value);
    else {
        if (root->left != NULL && root->right != NULL) {
            treePos temp = findMin(root->right);
            root->value = temp->value;
            root->right = deleteTree(root->right, temp->value);
        }
        else {
            treePos temp = root;
            if (root->left == NULL)
                root = root->right;
            else
                root = root->left;
            free(temp);
        }
    }
    return root;
}

treePos searchTree(treePos root, int value) {
    if (root == NULL) return NULL;
    if (value == root->value) return root;
    if (value < root->value) return searchTree(root->left, value);
    return searchTree(root->right, value);
}

treePos inorder(treePos root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->value);
        inorder(root->right);
    }
    return root;
}

treePos preorder(treePos root) {
    if (root != NULL) {
        printf("%d ", root->value);
        preorder(root->left);
        preorder(root->right);
    }
    return root;
}

treePos postorder(treePos root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->value);
    }
    return root;
}

// ================= LEVEL ORDER (FIFO) =================

int printLevelorder(treePos root) {
    if (root == NULL) {
        printf("Stablo je prazno.\n");
        return 0;
    }

    treePos queue[MAX];
    int front = 0;
    int rear = 0;

    queue[rear++] = root; // enqueue korijena

    while (front < rear) {
        treePos node = queue[front++]; // dequeue FIFO
        printf("%d ", node->value);

        // prvo lijevo, pa desno
        if (node->left != NULL)  queue[rear++] = node->left;
        if (node->right != NULL) queue[rear++] = node->right;
    }

    printf("\n");
    return 0;
}

// ================= FREE TREE =================

treePos freeTree(treePos root) {
    if (root == NULL) return NULL;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
    return NULL;
}

// ================= MAIN =================

int main() {
    treePos root = NULL;
    int izbor, value;

    do {
        printf("\n===== MENU =====\n");
        printf("1. Unos\n2. Inorder\n3. Preorder\n4. Postorder\n5. Level order\n6. Brisanje elementa\n7. Pronadji element\n8. Obrisi cijelo stablo\n9. Izlaz\n");

        while (1) {
            printf("Odabir: ");
            if (scanf("%d", &izbor) == 1) {
                while (getchar() != '\n'); // oèisti buffer
                break;
            }
            else {
                printf("Neispravan unos! Pokusajte ponovo.\n");
                while (getchar() != '\n');
            }
        }

        switch (izbor) {
        case 1:
            printf("Unesi vrijednost: ");
            if (scanf("%d", &value) != 1) {
                printf("Neispravan unos!\n");
                while (getchar() != '\n');
                break;
            }
            root = insertTree(root, value);
            break;
        case 2:
            inorder(root);
            printf("\n");
            break;
        case 3:
            preorder(root);
            printf("\n");
            break;
        case 4:
            postorder(root);
            printf("\n");
            break;
        case 5:
            printLevelorder(root);
            break;
        case 6:
            printf("Unesi vrijednost: ");
            if (scanf("%d", &value) != 1) {
                printf("Neispravan unos!\n");
                while (getchar() != '\n');
                break;
            }
            root = deleteTree(root, value);
            break;
        case 7:
            printf("Unesi vrijednost: ");
            if (scanf("%d", &value) != 1) {
                printf("Neispravan unos!\n");
                while (getchar() != '\n');
                break;
            }
            if (searchTree(root, value))
                printf("Pronadjen.\n");
            else
                printf("Nije pronadjen.\n");
            break;
        case 8:
            root = freeTree(root);
            printf("Stablo obrisano.\n");
            break;
        case 9:
            printf("Izlaz.\n");
            break;
        default:
            printf("Nepostojeca opcija!\n");
            break;
        }

    } while (izbor != 9);

    return 0;
}



