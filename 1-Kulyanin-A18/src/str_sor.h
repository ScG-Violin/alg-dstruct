#pragma once
#ifdef __cplusplus
extern "C" {
#endif

//Struct of string cell
typedef struct str_cell {
	struct str_cell *next;
	char* data;
}cell_;

//Struct of string list
typedef struct str_list
{
	cell_ *head;
	int size;
}list_;

//Create new cell with data and NULL point
cell_* CellCreate(char* data);

//Deleted cell
void CellDestroy(cell_* cell);

//Create new empty list (size = 0, head = NULL)
list_* ListCreate(void);

//Deleted list and cell data
void ListDestroy(list_* list);

//Print in console data of the list cells
void ListPrint(list_* list);

//Add cell in sort list
void AddCellSort(cell_* cell, list_* list);

//Switch data in 2 cell
void CellSwitch(cell_* left, cell_* rigth);

//Comperison data in 2 cell 
//if Left > Right return 1, else 0
int CellComperison(cell_* left, cell_* right);

//Add cell to list at specified position
//if position > list->size cell added to end of list
void AddCell(cell_* cell, list_* list, int position);

//Bubl list sort
void ListBublSort(list_* list);

#ifdef __cplusplus
}
#endif