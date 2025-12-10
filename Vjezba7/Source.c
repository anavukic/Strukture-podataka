#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define file_not_found -1
#define exit_failure -2


typedef struct direktory* directoryPosition;
typedef struct Stack* stackPosition;

typedef struct direktory {
    char name[MAX];
    directoryPosition Next;           
    directoryPosition SubDirPosition;
} direktory;

typedef struct Stack {
    stackPosition Next;
    directoryPosition DirectoryLevel; // pokazivac na trenutni direktorij
} Stack;


int Push(stackPosition headStack, directoryPosition dir)
{
    stackPosition NewEl = (stackPosition)malloc(sizeof(Stack));
    if (!NewEl) return exit_failure;

    NewEl->DirectoryLevel = dir;
    NewEl->Next = headStack->Next;
    headStack->Next = NewEl;

    return 0;
}

int Pop(stackPosition headStack)
{
    if (headStack->Next == NULL) return file_not_found;

    stackPosition temp = headStack->Next;
    headStack->Next = temp->Next;
    free(temp);

    return 0;
}

int MakeDirectory(stackPosition headStack, char* name)
{
    directoryPosition ParentDir = headStack->Next->DirectoryLevel;
    directoryPosition NewDirectory = (directoryPosition)malloc(sizeof(direktory));
    if (!NewDirectory) return exit_failure;

    strcpy(NewDirectory->name, name);
    NewDirectory->SubDirPosition = NULL;

    
    NewDirectory->Next = ParentDir->SubDirPosition;
    ParentDir->SubDirPosition = NewDirectory;

    return 0;
}

int ChangeDirectory(stackPosition headStack, char* name)
{
    directoryPosition temp = headStack->Next->DirectoryLevel->SubDirPosition;

    while (temp != NULL && strcmp(temp->name, name) != 0)
        temp = temp->Next;

    if (temp == NULL) {
        printf("Direktorij ne postoji!\n");
        return file_not_found;
    }

    // push na stack
    stackPosition NewEl = (stackPosition)malloc(sizeof(Stack));
    if (!NewEl) return exit_failure;

    NewEl->DirectoryLevel = temp;
    NewEl->Next = headStack->Next;
    headStack->Next = NewEl;

    return 0;
}

int GoBack(stackPosition headStack)
{
    if (headStack->Next == NULL || headStack->Next->Next == NULL) {
        printf("Vec ste u root direktoriju!\n");
        return file_not_found;
    }

    Pop(headStack);
    return 0;
}


int PrintDirectory(directoryPosition current, int level)
{
    directoryPosition currentSub = NULL;

    if (current == NULL) return 0;

    // ispis trenutnog direktorija
    for (int i = 0; i < level; i++) printf("  ");
    printf("%s\n", current->name);

    if (current->SubDirPosition != NULL) {
        currentSub = current->SubDirPosition;
        while (currentSub != NULL) {
            PrintDirectory(currentSub, level + 1);
            currentSub = currentSub->Next;
        }
    }

    return 0;
}

int Dir(stackPosition headStack)
{
    directoryPosition curr = headStack->Next->DirectoryLevel;
    if (!curr) return file_not_found;

    printf("\nSadrzaj direktorija '%s':\n", curr->name);
    PrintDirectory(curr->SubDirPosition, 1);
    printf("\n");

    return 0;
}

int DeleteTree(directoryPosition root)
{
    if (!root) return 0;

    DeleteTree(root->SubDirPosition);
    DeleteTree(root->Next);
    free(root);

    return 0;
}

int main()
{
    char command[MAX];
    char name[MAX];

    // HeadDirektoriji
    direktory HeadDirectory;
    HeadDirectory.Next = NULL;
    HeadDirectory.SubDirPosition = NULL;

    // alocirati memoriju za korijen
    directoryPosition korijenaDir = (directoryPosition)malloc(sizeof(direktory));
    strcpy(korijenaDir->name, "C:");
    korijenaDir->Next = NULL;
    korijenaDir->SubDirPosition = NULL;

    // HeadDir->Next = korijen
    HeadDirectory.Next = korijenaDir;

    // HeadStack
    Stack headStack;
    headStack.Next = NULL;

    // push korijen na vrh stoga
    Push(&headStack, korijenaDir);

    while (1)
    {
        printf("\nUnesi naredbu (md, cd, cd.., dir, exit): ");
        scanf("%s", command);

        if (strcmp(command, "md") == 0) {
            printf("Ime novog direktorija: ");
            scanf("%s", name);
            MakeDirectory(&headStack, name);
        }
        else if (strcmp(command, "cd") == 0) {
            printf("Ime direktorija: ");
            scanf("%s", name);
            ChangeDirectory(&headStack, name);
        }
        else if (strcmp(command, "cd..") == 0) {
            GoBack(&headStack);
        }
        else if (strcmp(command, "dir") == 0) {
            Dir(&headStack);
        }
        else if (strcmp(command, "exit") == 0) {
            DeleteTree(korijenaDir->SubDirPosition);
            free(korijenaDir);
            return 0;
        }
        else {
            printf("Nepoznata naredba!\n");
        }
    }
}

