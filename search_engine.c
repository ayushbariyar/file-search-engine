#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void toLowerCase(char* word);
#define TABLE_SIZE 100
struct FileNode {
    char filename[100];
    int count;
    struct FileNode* next;
};
struct WordNode {
    char word[50];
    struct FileNode* files;
    struct WordNode* next;
};
struct WordNode* hashTable[TABLE_SIZE];
void initTable() {
    for(int i=0; i< TABLE_SIZE; i++){
        hashTable[i] = NULL;
    }
}
int hash(char* word) {
    int sum = 0;
    for(int i = 0; word[i] !='\0'; i++) {
        sum += word[i];
    }
    return sum % TABLE_SIZE;
}
struct FileNode* addFile(struct FileNode* head, char* filename) {
    struct FileNode* temp = head;
    while(temp !=NULL) 
    {
        if(strcmp(temp->filename, filename) == 0) {
            temp->count++;
            return head;
        }
        temp = temp->next;
    }
    struct FileNode* newFile = malloc(sizeof(struct FileNode));
    strcpy(newFile->filename, filename);
    newFile->count = 1; 
    newFile->next = head;
    return newFile;
}
void insertWord(char* word, char* filename) {
    int index = hash(word);
    struct WordNode* temp = hashTable[index];
    while(temp != NULL) {
        if(strcmp(temp->word, word) == 0) {
            temp->files = addFile(temp->files, filename);
            return;
        }
        temp = temp->next;
    }
    struct WordNode* newWord = malloc(sizeof(struct WordNode));
    strcpy(newWord->word, word);
    newWord->files = NULL;
    newWord->files = addFile(newWord->files, filename);
    newWord->next = hashTable[index];
    hashTable[index] = newWord;
}
void searchWord(char* word) {
    toLowerCase(word);
    int index = hash(word);
    struct WordNode* temp = hashTable[index];
    while(temp != NULL) {
        if(strcmp(temp->word, word) == 0) {
            printf("\nWord found in:\n");
            struct FileNode* fileTemp = temp->files;
            while(fileTemp != NULL) {
                printf("- %s (%d times)\n", fileTemp->filename, fileTemp->count);
                fileTemp = fileTemp->next;
            }
            return;
        }
        temp = temp->next;
    }
    printf("\nWord not found in any file.\n");
}
void indexFile(char* filename) {
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }
    char word[50];
    while(fscanf(file, "%49s", word) == 1) {
        toLowerCase(word);
        insertWord(word, filename);
    }
    fclose(file);
}
void toLowerCase(char* word) {
    for(int i = 0; word[i]; i++) {
        if(word[i] >= 'A' && word[i] <= 'Z')
            word[i] = word[i] + 32;
    }
}
int main() {
    initTable();
    indexFile("notes.txt");
    indexFile("dsa.txt");
    indexFile("algo.txt");
    int choice;
    char search[50];
    while(1) {
        printf("\n1. Search Word\n");
        printf("2. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        if(choice == 1) {
            printf("Enter word: ");
            scanf("%s", search);
            searchWord(search);
        } else {
            break;
        }
    }
    return 0;
}

