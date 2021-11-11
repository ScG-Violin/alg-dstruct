#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable: 4996)

typedef struct{
	int own;
	int* add;
	int count;
}node;


node* ListCreate(int n)
{
	int i = 0;
	node* list = malloc(sizeof(node) * n);
	for (; i < n; i++)
	{
		list[i].add = NULL;
		list[i].own = 0;
		list[i].count = 0;
	}
	return list;
}

int searchI(int* list, int n, int count) {
	int i = 0;
	for (;i < count;i++)
	{
		if (list[i] == n) return 0;
	}
	return n;
}

void AddI(node* list, int i, int n){
	if (list[i].add == NULL) {
		list[i].add = malloc(sizeof(int));
		list[i].add[0] = n;
		list[i].count++;
	}
	int isInadd = searchI(list[i].add, n, list[i].count);
	if (isInadd) {
		int count = list[i].count + 1;
		list[i].add = realloc(list[i].add, sizeof(int) * count);
		if (list[i].add) {
			list[i].add[count - 1] = n;
			list[i].count++;
		}
		else perror("Realloc error!\n");
	}
}

void ReadList(node* list, int n) {
	int i = 0;
	for (; i < n; i++) {
		int j, k = 0;
		char c;
		scanf("%d%c", &j, &c);
		list[i].own = j;
		while (c == ' ')
		{
			scanf("%d%c", &j, &c);
			AddI(list, i, j);
			AddI(list, j, i);
		}
	}
}

void PrintList(node* list, int n){

	int i = 0;
	for (; i < n; i++) {
		int j = 0;
		printf("\n%d", list[i].own);
		for (;j < list[i].count; j++)
		{
			printf(" %d", list[i].add[j]);
		}
	}
	printf("\n");
}

void FreeList(node* list, int n) {
	int i = 0;
	for (; i < n; i++) {
		free(list[i].add);
	}
	free(list);
}

void DFS(node* list, int n, int point, int* res, int* res_count) {
	int i = 0;
	for (; i < list[point].count; i++) {
		int new_point = searchI(res, list[point].add[i], *res_count);
		if (new_point) {
			res[*res_count] = new_point;
			(*res_count)++;
			DFS(list, n, new_point, res, res_count);
		}
	}
}

void PrintIarr(int* res, int count) {
	int i = 0;
	for (; i < count; i++) {
		printf("%d ", res[i]);
	}
}

int main(void) {
	int n = 0, i = 0, res_count = 1;
	int* res = NULL;
	if (scanf("%d", &n) < 0) {
		perror("Scan failed!");
		return 1;
	}
	node* list = NULL;
	list = ListCreate(n);
	if (!list) return 1;
	ReadList(list, n);
	//PrintList(list, n);
	res = malloc(sizeof(int) * n);
	if (res) {
		res[0] = 0;
		DFS(list, n, 0, res, &res_count);
		PrintIarr(res, res_count);
	}
	//free(res);
	FreeList(list, n);
	return 0;
}