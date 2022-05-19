# define _CRT_SECURE_NO_WARNINGS
#include<inttypes.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include <time.h>

#define BYTE 256
uint16_t  CRC_TABLE[BYTE];
#define WIDTH  (8 * sizeof(uint16_t))
#define TOPBIT (1 << (WIDTH - 1))

#define POLYNOMIAL (0x1021)

#define TABLE_SIZE 256
int COLISSION_TABLE[TABLE_SIZE] = { 0 };



typedef struct List_t {
    struct List_t* next;
    char* data;
}List_t;
List_t* HASH_TABLE[TABLE_SIZE];

static void AddList(List_t* parent, char* message) {
    int length = strlen(message) + 1;

    List_t* current = parent;
    for (; current->next != NULL; current = current->next) {
        if (strcmp(current->next->data, message) == 0) {
            return;
        }
    }

    List_t* new_list = (List_t*)malloc(sizeof(List_t));
    if (new_list == NULL) {
        return;
    }

    new_list->next = NULL;
    new_list->data = (char*)calloc(length, sizeof(char));
    /*
    if (new_list->data == NULL) {
        printf("The problem with the pointer to the allocated memory!");
        return;
    }
    */
    strcpy(new_list->data, message);
    current->next = new_list;
}

static int FindList(List_t* parent, char* message) {
    for (List_t* current = parent->next; current != NULL; current = current->next) {
        if (strcmp(current->data, message) == 0) {
            return 1;
        }
    }
    return 0;
}

static void DeleteList(List_t* parent, char* message) {
    List_t* previous = parent;
    for (List_t* current = parent->next; current != NULL; current = current->next) {
        if (strcmp(current->data, message) == 0) {
            previous->next = current->next;

            free(current->data);
            free(current);
            return;
        }
        previous = current;
    }
}

void Create_CRC_Table() {
    uint16_t  remainder;
    for (int dividend = 0; dividend < BYTE; dividend++) {
        remainder = dividend << (WIDTH - 8);

        for (uint8_t bit = 8; bit > 0; bit--) {
            if (remainder & TOPBIT) {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else {
                remainder = (remainder << 1);
            }
        }
        CRC_TABLE[dividend] = remainder;
    }
}

uint16_t CRC(char* message) {
    int length = strlen(message) + 1;

    uint8_t data;
    uint16_t remainder = 0;

    for (int i = 0; i < length; i++) {
        data = message[i] ^ (remainder >> (WIDTH - 8));
        remainder = CRC_TABLE[data] ^ (remainder << 8);
    }
    return remainder;
}

void AddHashTable(char* message) {
    unsigned index = CRC(message) % TABLE_SIZE;

    if (HASH_TABLE[index] == NULL) {
        HASH_TABLE[index] = (List_t*)malloc(sizeof(List_t));
        if (HASH_TABLE[index] == NULL) {
            return;
        }
        HASH_TABLE[index]->next = NULL;
    }
    COLISSION_TABLE[index]++;

    AddList(HASH_TABLE[index], message);
}

int FindHashTable(char* message) {
    unsigned index = CRC(message) % TABLE_SIZE;

    if (HASH_TABLE[index] == NULL) {
        return 0;
    }
    return FindList(HASH_TABLE[index], message);
}

void DeleteHashTable(char* message) {
    unsigned index = CRC(message) % TABLE_SIZE;

    if (HASH_TABLE[index] == NULL) {
        return;
    }
    DeleteList(HASH_TABLE[index], message);
}

void Collision(char* message) {
    unsigned a = CRC(message);
    int i = 2;
    char c[10000];

    while (1) {
        _itoa(i, c, 10);
        if (a == CRC(c)) {
            puts(c);
            return;
        }
        i++;
    }
}

void CollisionTest(FILE* f) {
    srand(time(NULL));

    char* str = (char*)calloc(101, sizeof(char));
    if (str == NULL) {
        printf("The problem with the pointer to the allocated memory!");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        for (int i = 0; i < 101; i++) {
            str[i] = '\0';
        }
        int words_num = rand() % 100;
        for (int k = 0; k < words_num; k++) {
            str[k] = (char)(rand() % 256);
        }
        AddHashTable(str);
    }

    for (int i = 1; i < TABLE_SIZE + 1; i++) {
        fprintf(f, "%i  ", COLISSION_TABLE[i - 1]);
        if (i % 25 == 0) {
            fprintf(f, "\n");
        }
    }

    int collision_number[15] = { 0 };
    for (int i = 0; i < TABLE_SIZE; i++) {
        collision_number[COLISSION_TABLE[i]]++;
    }

    fprintf(f, "\n");
    int flag = 0;

    for (int i = 14; i >= 0; i--) {
        if (collision_number[i] != 0) {
            flag = 1;
        }
        if (flag) {
            if (collision_number[i] != -1) {
                fprintf(f, "Number of cells with %i elements: %i \n", i, collision_number[i]);
            }
        }
    }
}

int main() {
    Create_CRC_Table();
    FILE* f = fopen("test.txt","w");
    CollisionTest(f);
    fclose(f);
    char command;
    char message[1000];

    while (scanf("%c", &command) >= 1) {

        switch (command) {
        case 'a':
            scanf("%s", message);
            AddHashTable(message);
            break;
        case 'r':
            scanf("%s", message);
            DeleteHashTable(message);
            break;
        case 'f':
            scanf("%s", message);
            if (FindHashTable(message))
                puts("yes");
            else
                puts("no");
            break;
        case 'q':
            return 0;
            break;
        }
    }
    return 0;
}