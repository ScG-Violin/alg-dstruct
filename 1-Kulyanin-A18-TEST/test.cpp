#include "pch.h"
#include "str_sor.h"

TEST(CELLCREATE_TEST, CellCreate) {
	char *data = "1";
	cell_* cell;
	cell = CellCreate(data);
	ASSERT_NE(cell, nullptr);
	EXPECT_EQ(*(cell->data), *data);
	EXPECT_EQ(cell->next, nullptr);
	CellDestroy(cell);
}

TEST(LISTCREATE_TEST, ListCreate)
{
	list_* list = ListCreate();
	ASSERT_NE(list, nullptr);
	ASSERT_EQ(list->head, nullptr);
	ASSERT_EQ(list->size, 0);
	ListDestroy(list);
}

TEST(CELLSWITCH_TEST, CellSwitch)
{
	cell_ cell_1 = { nullptr, nullptr };
	cell_ cell_2 = { nullptr, nullptr };

	char* data1 = "1";
	char* data2 = "2";

	char* copy1 = (char*)malloc(strlen(data1) + 1);
	char* copy2 = (char*)malloc(strlen(data2) + 1);
	if (copy1 && copy2)
	{
		strcpy(copy1, data1);
		strcpy(copy2, data2);

		(&cell_1)->data = copy1;
		(&cell_2)->data = copy2;

		CellSwitch(&cell_1, &cell_2);

		ASSERT_EQ((&cell_1)->data, copy2);
		ASSERT_EQ((&cell_2)->data, copy1);

		(&cell_1)->data = nullptr;
		(&cell_2)->data = nullptr;
		free(copy1);
		free(copy2);
	}
	else { perror("Malloc failed"); }
}

TEST(CELLCOMPERISON_TEST, CellComperison)
{
	cell_ cell_1 = { nullptr, nullptr };
	cell_ cell_2 = { nullptr, nullptr };

	char* data1 = "more";
	char* data2 = "less";

	char* copy1 = (char*)malloc(strlen(data1) + 1);
	char* copy2 = (char*)malloc(strlen(data2) + 1);
	if (copy1 && copy2)
	{
		strcpy(copy1, data1);
		strcpy(copy2, data2);

		(&cell_1)->data = copy1;
		(&cell_2)->data = copy2;

		int more, less, same, zero, incorrect_input;//more = 1 other 0

		more = CellComperison(&cell_1, &cell_2);
		less = CellComperison(&cell_2, &cell_1);
		same = CellComperison(&cell_1, &cell_1);
		zero = CellComperison(&cell_1, nullptr);
		incorrect_input = CellComperison(nullptr, &cell_1);

		ASSERT_EQ(more, 1);
		ASSERT_EQ(less, 0);
		ASSERT_EQ(same, 0);
		ASSERT_EQ(zero, 0);
		ASSERT_EQ(incorrect_input, 0);

		free(copy1);
		free(copy2);
	}
	else { perror("Malloc failed"); }
}

TEST(ADDCELL_TEST, AddCell)
{
	list_ list = { nullptr, 0 };

	cell_ cell_1 = { nullptr, nullptr };
	cell_ cell_2 = { nullptr, nullptr };
	cell_ cell_3 = { nullptr, nullptr };
	cell_ cell_4 = { nullptr, nullptr };

	AddCell(&cell_2, &list, 1);//add to empty list
	ASSERT_EQ((&list)->size, 1);

	AddCell(&cell_1, &list, 0);//add on first pos
	ASSERT_EQ((&list)->size, 2);

	AddCell(&cell_4, &list, 4);//add to end list
	ASSERT_EQ((&list)->size, 3);

	AddCell(&cell_3, &list, 2);//add to mid
	ASSERT_EQ((&list)->size, 4);

	cell_* buff = (&list)->head;
	ASSERT_EQ(buff, &cell_1);

	buff = (&cell_1)->next;
	ASSERT_EQ(buff, &cell_2);

	buff = (&cell_2)->next;
	ASSERT_EQ(buff, &cell_3);

	buff = (&cell_3)->next;
	ASSERT_EQ(buff, &cell_4);

	AddCell(&cell_2, &list, 1);//try to add linked cell
	ASSERT_EQ((&list)->size, 4);
}

TEST(ADDCELLSOR_TEST, AddCellSort)
{
	list_ list = { nullptr, 0 };

	cell_ cell_1 = { nullptr, nullptr };
	cell_ cell_2 = { nullptr, nullptr };
	cell_ cell_3 = { nullptr, nullptr };
	cell_ cell_4 = { nullptr, nullptr };

	char* data1 = "less";
	char* data2 = "more";

	char* copy1 = (char*)malloc(strlen(data1) + 1);
	char* copy2 = (char*)malloc(strlen(data2) + 1);
	if (copy1 && copy2)
	{
		strcpy(copy1, data1);
		strcpy(copy2, data2);

		(&cell_1)->data = copy1;
		(&cell_2)->data = copy1;
		(&cell_3)->data = copy2;
		(&cell_4)->data = copy2;

		AddCellSort(&cell_2, &list);
		ASSERT_EQ((&list)->size, 1);

		AddCellSort(&cell_1, &list);
		ASSERT_EQ((&list)->size, 2);

		AddCellSort(&cell_4, &list);
		ASSERT_EQ((&list)->size, 3);

		AddCellSort(&cell_3, &list);
		ASSERT_EQ((&list)->size, 4);

		cell_* buff = (&list)->head;
		ASSERT_EQ(buff, &cell_1);

		buff = (&cell_1)->next;
		ASSERT_EQ(buff, &cell_2);

		buff = (&cell_2)->next;
		ASSERT_EQ(buff, &cell_3);

		buff = (&cell_3)->next;
		ASSERT_EQ(buff, &cell_4);

		AddCellSort(&cell_2, &list);//try to add linked cell
		ASSERT_EQ((&list)->size, 4);

		free(copy1);
		free(copy2);
	}
	else { perror("Malloc failed"); }
}

TEST(LISTBUBLSORT_TEST, ListBublSort)
{
	list_ list = { nullptr, 0 };

	cell_ cell_1 = { nullptr, nullptr };
	cell_ cell_2 = { nullptr, nullptr };
	cell_ cell_3 = { nullptr, nullptr };

	char* data1 = "more";
	char* data2 = "less";

	char* copy1 = (char*)malloc(strlen(data1) + 1);
	char* copy2 = (char*)malloc(strlen(data2) + 1);
	if (copy1 && copy2)
	{
		strcpy(copy1, data1);
		strcpy(copy2, data2);

		(&cell_1)->data = copy1;
		(&cell_2)->data = copy1;
		(&cell_3)->data = copy2;

		(&list)->head = &cell_1;
		(&cell_1)->next = &cell_2;
		(&cell_2)->next = &cell_3;
		(&list)->size = 3;

		ListBublSort(&list);

		ASSERT_EQ((&cell_1)->data, copy2);
		ASSERT_EQ((&cell_2)->data, copy1);
		ASSERT_EQ((&cell_3)->data, copy1);

		free(copy1);
		free(copy2);
	}
	else { perror("Malloc failed"); }
}