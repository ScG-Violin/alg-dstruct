#include "str_sor.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	char* more = "more", *less = "less", *same = "same", *empty= "";
	list_* list = ListCreate(), *listUn = ListCreate();
	AddCellSort(CellCreate(more), list);
	AddCellSort(CellCreate(less), list);
	AddCellSort(CellCreate(same), list);
	AddCellSort(CellCreate(same), list);
	AddCellSort(CellCreate(empty), list);
	ListPrint(list);
	ListDestroy(list);
	AddCell(CellCreate(same), listUn, 0);
	AddCell(CellCreate(more), listUn, 1);
	AddCell(CellCreate(same), listUn, 43);
	AddCell(CellCreate(less), listUn, 2);
	AddCell(CellCreate(empty), listUn,2);
	ListBublSort(listUn);
	ListPrint(listUn);
	ListDestroy(listUn);
	return 0;
}