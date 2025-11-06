#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define FILE_NOT_FOUND "Greska: Datoteka nije pronadena\n"

typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Kreiranje novog èvora
Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Greska pri alokaciji memorije\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

// Dodavanje elemenata na vrh stoga
Node* push(Node* top, int value) {
    Node* newNode = createNode(value);
    newNode->next = top;
    top = newNode;
    return top;
}

// Uklananje i vracanje elemenata s vrha stoga
int pop(Node** top) {
    if (*top == NULL) {
        printf("Greska: Stog je prazan\n");
        exit(EXIT_FAILURE);
    }
    Node* temp = *top;
    int value = temp->data;
    *top = temp->next;
    free(temp);
    return value;
}

// Provjera operatora
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// funkcija koja računa rezultat postfiks izraza
int evaluatePostfix(char* expr) {
    Node* stack = NULL; 
    int i = 0;
    while (expr[i] != '\0') {
        if (isspace(expr[i])) {
            i++;
            continue;
        }
        if (isdigit(expr[i]) || (expr[i] == '-' && isdigit(expr[i + 1]))) {
            int sign = 1;
            if (expr[i] == '-') {
                sign = -1;
                i++;
            }
            int num = 0;
            while (isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            stack = push(stack, sign * num);
        }
        else if (isOperator(expr[i])) {
            int b = pop(&stack);
            int a = pop(&stack);
            switch (expr[i]) {
            case '+': stack = push(stack, a + b); break;
            case '-': stack = push(stack, a - b); break;
            case '*': stack = push(stack, a * b); break;
            case '/': stack = push(stack, a / b); break;
            }
            i++;
        }
        else {
            printf("Greska: nepoznati znak '%c'\n", expr[i]);
            exit(EXIT_FAILURE);
        }
    }
    return pop(&stack);
}

int main() {
    FILE* file = fopen("postfix.txt", "r");
    if (file == NULL) {
        printf(FILE_NOT_FOUND);
        exit(EXIT_FAILURE);
    }
  
        char expr[256];
    if (fgets(expr, sizeof(expr), file) == NULL) {
        printf("Greska pri citanju datoteke\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);

    int result = evaluatePostfix(expr);
    printf("Rezultat postfiks izraza je: %d\n", result);

    return 0;
   
}

