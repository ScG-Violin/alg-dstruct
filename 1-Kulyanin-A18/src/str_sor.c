#include "str_sor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#pragma warning(disable: 4996)

cell_* CellCreate(char *data)
{
	cell_* body = malloc(sizeof(cell_));
	char* copy = malloc(strlen(data) + 1);

	if (body && copy)
	{
		strcpy(copy, data);
		body->next = NULL;
		body->data = copy;
	}
	else { perror("Malloc failed"); }
	return body;
};

void CellDestroy(cell_* cell)
{
	if (cell) {
		free(cell->data);
		free(cell);
	}
}

list_* ListCreate(void) {
	list_* body = malloc(sizeof(list_));
	if (body)
	{
		body->head = NULL;
		body->size = 0;
	}
	else { perror("Malloc failed"); }
	return body;
}

void ListDestroy(list_* list)
{
	if (list) {
		int i = list->size;
		cell_ *cell = list->head, *cellNext = cell;
		while (i--)
		{
			cell = cellNext;
			cellNext = cell->next;
			CellDestroy(cell);
		}
		free(list);
	}
}

void ListPrint(list_* list)
{
	if (list) {
		int i = list->size;
		cell_* cell = list->head, * cellNext = cell;
		while (i--)
		{
			cell = cellNext;
			cellNext = cell->next;
			printf("[ %s ]  ", cell->data);
		}
		printf("\n");
	}
}

void CellSwitch(cell_* left, cell_* rigth)
{
	char* buff = left->data;
	left->data = rigth->data;
	rigth->data = buff;
}

int CellComperison(cell_* left, cell_* right)
{
	int key = 0, i = 0;
	if (right)
		while (left->data[i] && (key == 0))
		{
			key = left->data[i] - right->data[i];
			i++;
		}
		if (key > 0) { return 1; }
	return 0;
}

void AddCell(cell_* cell, list_* list)
{
	if (cell->next) { perror("Intersection of list"); }
	else
	{
		cell_* buff = list->head, * prev = NULL;
		while (list->size && CellComperison(cell, buff))
		{
			prev = buff;
			buff = buff->next;
		}
		if (prev)
		{
			prev->next = cell;
			cell->next = buff;
		}
		else
		{
			list->head = cell;
			cell->next = buff;
		}
		(list->size)++;
	}
}

//void ListBublSort(list_* list)
//{
//	if (list->size)
//	{
//		int i = list->size - 1, j = i;
//		while (i)
//		{
//			cell_* right = list->head,* left;
//			while (j--)
//			{
//				left = right;
//				right = left->next;
//				CellComperison(left, right);
//			}
//			j = --i;
//		}
//	}
//}