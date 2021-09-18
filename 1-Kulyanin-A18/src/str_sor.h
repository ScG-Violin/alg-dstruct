#pragma once

typedef struct str_cell {
	struct str_cell *next;
	char* data;
}cell_;

typedef struct str_list
{
	cell_ *head;
	int size;
}list_;

cell_* CellCreate(char* data);

void CellDestroy(cell_* cell);

list_* ListCreate(void);

void ListDestroy(list_* list);

void ListPrint(list_* list);

void AddCell(cell_* cell, list_* list);

void CellSwitch(cell_* left, cell_* rigth);

int CellComperison(cell_* left, cell_* right);

//void ListBublSort(list_* list);