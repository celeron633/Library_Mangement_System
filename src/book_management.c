#include "book_management.h"
#include "utils.h"
#include "data_storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 全局变量声明（extern）
extern book book_buffer[MAX_BUFFER_SIZE];
extern int books_count;
extern books *book_first_node;

// 书籍数据链表操作
void book_buffer_to_linklist()
{
	book_first_node = (books *)malloc(sizeof(books));
	book_first_node->next = NULL;
	books *new_node;
	books *tail;
	tail = book_first_node;

	int i;

	for (i = 0; i < books_count; i++)
	{
		new_node = (books *)malloc(sizeof(books));
		new_node->data.book_id = book_buffer[i].book_id;
		strcpy(new_node->data.book_name, book_buffer[i].book_name);
		strcpy(new_node->data.book_author, book_buffer[i].book_author);
		strcpy(new_node->data.book_introduce, book_buffer[i].book_introduce);
		new_node->data.book_price = book_buffer[i].book_price;
		new_node->data.book_storage_count = book_buffer[i].book_storage_count;

		tail->next = new_node;
		tail = new_node;
		tail->next = NULL;
	}
	printf("书本数据链表创建成功!\n");
}

void linklist_to_book_buffer(books *u)
{
	int i = 0;

	if (book_first_node == NULL)
	{
		printf("错误! 书本数据链表没有初始化\n");
		return;
	}
	else
	{
		while (u->next != NULL)
		{
			u = u->next;
			book_buffer[i].book_id = u->data.book_id;
			strcpy(book_buffer[i].book_name, u->data.book_name);
			strcpy(book_buffer[i].book_author, u->data.book_author);
			strcpy(book_buffer[i].book_introduce, u->data.book_introduce);
			book_buffer[i].book_price = u->data.book_price;
			book_buffer[i].book_storage_count = u->data.book_storage_count;
			i++;
		}

		books_count = i;
		printf("书本数据链表已经复制到缓存数组\n");
	}
}

// 管理员查看书籍信息
void admin_book_info_look_up(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("书籍信息查询\n");
	printf("#######################################################\n");
	print_book_list();
	printf("任意按键返回管理员菜单!\n");
	pause_and_wait();
}

// 添加新书籍
void add_new_book(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("添加新书籍\n");
	printf("#######################################################\n");

	book new_book;
	int i;

	// 生成新的书籍ID
	new_book.book_id = 1;
	for (i = 0; i < books_count; i++)
	{
		if (book_buffer[i].book_id >= new_book.book_id)
		{
			new_book.book_id = book_buffer[i].book_id + 1;
		}
	}

	printf("请输入书籍名称:");
	scanf("%s", new_book.book_name);
	printf("请输入作者:");
	scanf("%s", new_book.book_author);
	printf("请输入书籍简介:");
	scanf("%s", new_book.book_introduce);
	printf("请输入价格:");
	scanf("%f", &new_book.book_price);
	printf("请输入库存数量:");
	scanf("%d", &new_book.book_storage_count);

	// 添加到缓冲区
	book_buffer[books_count] = new_book;
	books_count++;

	// 保存到文件
	save_book_data();

	printf("书籍添加成功! ID: %d\n", new_book.book_id);
	printf("任意按键返回!\n");
	pause_and_wait();
}

// 根据ID删除书籍
void delete_book_by_id(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("删除书籍\n");
	printf("#######################################################\n");

	int book_id;
	int i, j;
	int found = 0;

	printf("请输入要删除的书籍ID:");
	scanf("%d", &book_id);

	for (i = 0; i < books_count; i++)
	{
		if (book_buffer[i].book_id == book_id)
		{
			found = 1;
			break;
		}
	}

	if (!found)
	{
		printf("书籍ID不存在!\n");
		printf("任意按键返回!\n");
		pause_and_wait();
		return;
	}

	// 删除书籍
	for (j = i; j < books_count - 1; j++)
	{
		book_buffer[j] = book_buffer[j + 1];
	}
	books_count--;

	save_book_data();
	printf("书籍删除成功!\n");
	printf("任意按键返回!\n");
	pause_and_wait();
}

// 修改书籍信息
void modify_book_info(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("修改书籍信息\n");
	printf("#######################################################\n");

	int book_id;
	int i;
	int found = 0;
	int choice;

	printf("请输入要修改的书籍ID:");
	scanf("%d", &book_id);

	for (i = 0; i < books_count; i++)
	{
		if (book_buffer[i].book_id == book_id)
		{
			found = 1;
			break;
		}
	}

	if (!found)
	{
		printf("书籍ID不存在!\n");
		printf("任意按键返回!\n");
		pause_and_wait();
		return;
	}

	while (1)
	{
		clear_screen();
		printf("#######################################################\n");
		printf("修改书籍: %s\n", book_buffer[i].book_name);
		printf("#######################################################\n");
		printf("1.修改名称\n");
		printf("2.修改作者\n");
		printf("3.修改简介\n");
		printf("4.修改价格\n");
		printf("5.修改库存\n");
		printf("6.返回\n");
		printf("#######################################################\n");
		printf("请输入选择:");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			printf("请输入新名称:");
			scanf("%s", book_buffer[i].book_name);
			break;
		case 2:
			printf("请输入新作者:");
			scanf("%s", book_buffer[i].book_author);
			break;
		case 3:
			printf("请输入新简介:");
			scanf("%s", book_buffer[i].book_introduce);
			break;
		case 4:
			printf("请输入新价格:");
			scanf("%f", &book_buffer[i].book_price);
			break;
		case 5:
			printf("请输入新库存:");
			scanf("%d", &book_buffer[i].book_storage_count);
			break;
		case 6:
			save_book_data();
			printf("修改完成!\n");
			printf("任意按键返回!\n");
			pause_and_wait();
			return;
		default:
			printf("输入错误!\n");
		}
	}
}

// 打印书籍列表
void print_book_list(void)
{
	if (books_count == 0)
	{
		printf("暂无书籍信息!\n");
		return;
	}

	printf("ID\t名称\t\t作者\t\t简介\t\t\t价格\t\t库存\n");
	printf("--------------------------------------------------------------------\n");
	
	for (int i = 0; i < books_count; i++)
	{
		printf("%d\t%s\t\t%s\t\t%s\t\t%.2f\t\t%d\n",
			   book_buffer[i].book_id,
			   book_buffer[i].book_name,
			   book_buffer[i].book_author,
			   book_buffer[i].book_introduce,
			   book_buffer[i].book_price,
			   book_buffer[i].book_storage_count);
	}
}

// 加载书籍数据
void load_book_data(void)
{
	FILE *fp;
	int i;

	fp = fopen(BOOK_FILE, "r");
	if (fp == NULL)
	{
		printf("书籍配置文件不存在，将创建新的配置文件!\n");
		books_count = 0;
		return;
	}

	fscanf(fp, "%d\n", &books_count);
	for (i = 0; i < books_count; i++)
	{
		fscanf(fp, "%d %s %s %s %f %d\n",
			   &book_buffer[i].book_id,
			   book_buffer[i].book_name,
			   book_buffer[i].book_author,
			   book_buffer[i].book_introduce,
			   &book_buffer[i].book_price,
			   &book_buffer[i].book_storage_count);
	}
	printf("成功从磁盘读取书籍配置文件!\n");
	fclose(fp);
}

// 保存书籍数据
void save_book_data(void)
{
	FILE *fp;
	int i;

	fp = fopen(BOOK_FILE, "w");
	if (fp == NULL)
	{
		printf("发生错误! 书籍配置文件创建失败\n");
		return;
	}

	fprintf(fp, "%d\n", books_count);
	for (i = 0; i < books_count; i++)
	{
		fprintf(fp, "%d %s %s %s %.2f %d\n",
				book_buffer[i].book_id,
				book_buffer[i].book_name,
				book_buffer[i].book_author,
				book_buffer[i].book_introduce,
				book_buffer[i].book_price,
				book_buffer[i].book_storage_count);
	}
	printf("成功将书籍缓存写入磁盘!\n");
	fclose(fp);
}

// 删除书籍配置文件
void del_book_config(void)
{
	if (remove(BOOK_FILE) == 0)
	{
		printf("书籍配置文件删除成功!\n");
		books_count = 0;
	}
	else
	{
		printf("书籍配置文件删除失败!\n");
	}
	printf("任意按键返回!\n");
	pause_and_wait();
}
