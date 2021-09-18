#include "str_sor.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	char* more = "more", *less = "less", *same = "same", *empty= "";
	list_* list = ListCreate();
	AddCell(CellCreate(more), list);
	AddCell(CellCreate(less), list);
	AddCell(CellCreate(same), list);
	AddCell(CellCreate(same), list);
	AddCell(CellCreate(empty), list);
	ListPrint(list);
	ListDestroy(list);
	return 0;
}