#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define wag_TO_IDX(wag) ((wag) - 1)
#define IDX_TO_wag(idx) ((idx) + 1)

typedef struct cell {
    unsigned wag;
    struct cell* next;
} cell;

unsigned* wag_time; // t(w)
unsigned wag_count; // |W|
unsigned last_time; // D
unsigned max_wag_left; // K
cell** wag_Rest;

unsigned cur_time;
unsigned min_time;
unsigned max_wag_use;
unsigned* wags_rest;
unsigned* wags_use;
unsigned* cur_res;
unsigned* cur_best_res;

bool ListPush(cell** p_list, unsigned wag) {
    cell* newcell = (cell*)malloc(sizeof(cell));
    if (!newcell)
        return false;
    newcell->wag = wag;
    newcell->next = *p_list;
    *p_list = newcell;
    return true;
}

void ListPrint(cell* list) {
    cell* i = list;
    while (i) {
        printf("%d ", IDX_TO_wag(i->wag));
        i = i->next;
    }
}

void ListDestroy(cell* list) {
    cell* i = list;
    while (i) {
        cell* toFree = i;
        i = i->next;
        free(toFree);
    }
}

void RestrictionsDestroy() {
    for (unsigned i = 0; i < wag_count; i++)
        ListDestroy(wag_Rest[i]);
    free(wag_Rest);
}

bool ReadInputData(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file)
        return false;

    fscanf(file, "%d %d %d", &wag_count, &last_time, &max_wag_left);

    wag_time = (unsigned*)malloc(wag_count * sizeof(unsigned));
    if (!wag_time) {
        fclose(file);
        return false;
    }

    wag_Rest = (cell**)calloc(wag_count, sizeof(cell*));
    if (!wag_Rest) {
        free(wag_time);
        fclose(file);
        return false;
    }

    for (unsigned i = 0; i < wag_count; i++)
        fscanf(file, "%d ", &wag_time[i]);

    unsigned pairsCount = 0;
    fscanf(file, "%d", &pairsCount);
    for (unsigned i = 0; i < pairsCount; i++) {
        unsigned before = 0, after = 0;
        fscanf(file, "%d %d", &before, &after);
        if (!ListPush(&wag_Rest[wag_TO_IDX(after)], wag_TO_IDX(before))) {
            free(wag_time);
            fclose(file);
            RestrictionsDestroy();
            return false;
        }
    }
    fclose(file);
    return true;
}

void CompressSearchSpace(unsigned wag) {
    wags_use[wag] = 1;
    cell* i = wag_Rest[wag];
    while (i) {
        wags_rest[i->wag] = 1;
        i = i->next;
    }
}

void DecompressSearchSpace(unsigned wag) {
    wags_use[wag] = 0;
    cell* i = wag_Rest[wag];
    while (i) {
        wags_rest[i->wag] = 0;
        i = i->next;
    }
}

void FindBestSolution(unsigned m) {
    if (m >= wag_count - max_wag_left)
        if (m == max_wag_use && cur_time < min_time) {
            min_time = cur_time;
            memcpy(cur_best_res, cur_res, wag_count * sizeof(unsigned));
        }
        else if (m > max_wag_use) {
            max_wag_use = m;
            min_time = cur_time;
            memcpy(cur_best_res, cur_res, wag_count * sizeof(unsigned));
        }
    if (m < wag_count)
        for (unsigned i = 0; i < wag_count; i++) {
            if (!wags_rest[i] && !wags_use[i] && cur_time + wag_time[i] <= last_time) {
                cur_res[m] = i;
                cur_time += wag_time[i];
                CompressSearchSpace(i);
                FindBestSolution(m + 1);
                DecompressSearchSpace(i);
                cur_time -= wag_time[i];
            }
        }
}

bool FindBestSolutionWrap() {
    cur_time = 0;
    min_time = last_time + 1;
    max_wag_use = 0;
    cur_res = (unsigned*)malloc(wag_count * sizeof(unsigned));
    if (!cur_res)
        return false;
    cur_best_res = (unsigned*)malloc(wag_count * sizeof(unsigned));
    if (!cur_best_res) {
        free(cur_res);
        return false;
    }
    wags_rest = (unsigned*)calloc(wag_count, sizeof(unsigned));
    if (!wags_rest) {
        free(cur_res);
        free(cur_best_res);
        return false;
    }
    wags_use = (unsigned*)calloc(wag_count, sizeof(unsigned));
    if (!wags_use) {
        free(cur_res);
        free(cur_best_res);
        free(wags_rest);
        return false;
    }
    FindBestSolution(0);
    free(cur_res);
    free(wags_rest);
    free(wags_use);
    return true;
}

unsigned Distributewags(const char* filenameIn, const char* filenameOut) {
    if (!ReadInputData(filenameIn)) // allocating memory for times and restrictions
        return 1;

    printf("wag_time: ");
    for (unsigned i = 0; i < wag_count; i++)
        printf("%d ", wag_time[i]);
    printf("\n");
    printf("wag_Rest:\n");
    for (unsigned i = 0; i < wag_count; i++) {
        printf("Cannot be inserted after %d: ", IDX_TO_wag(i));
        ListPrint(wag_Rest[i]);
        printf("\n");
    }

    FILE* fileOut = fopen(filenameOut, "w");
    if (!fileOut)
        return 1;

    printf("wag_time: ");
    for (unsigned i = 0; i < wag_count; i++)
        printf("%d ", wag_time[i]);
    printf("\n");
    printf("wag_Rest:\n");
    for (unsigned i = 0; i < wag_count; i++) {
        printf("Cannot be inserted after %d: ", IDX_TO_wag(i));
        ListPrint(wag_Rest[i]);
        printf("\n");
    }

    if (FindBestSolutionWrap()) {
        if (max_wag_use > 0) {
            for (unsigned i = 0; i < max_wag_use; i++)
                fprintf(fileOut, "%d ", IDX_TO_wag(cur_best_res[i]));
            fprintf(fileOut, "\n");
        }
        else
            fprintf(fileOut, "0\n");
        free(cur_best_res);
        free(wag_time);
        RestrictionsDestroy();
        fclose(fileOut);
        return 0;
    }
    free(wag_time);
    RestrictionsDestroy();
    fclose(fileOut);
    return 1;
}

int main(void) {
    const char* f_in = "input.txt", * f_out = "output.txt";
    Distributewags(f_in, f_out);
    return 0;
}