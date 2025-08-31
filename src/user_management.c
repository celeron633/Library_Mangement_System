#include "user_management.h"
#include "utils.h"
#include "data_storage.h"
#include "book_management.h"
#include "system_ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 全局变量声明（extern）
extern user user_buffer[MAX_BUFFER_SIZE];
extern int users_count;
extern users *user_first_node;
extern int current_logined_user_in_buffer;
extern int is_user_logined;

// 用户数据链表操作
void user_buffer_to_linklist()
{
	user_first_node = (users *)malloc(sizeof(users));
	user_first_node->next = NULL;
	users *new_node;
	users *tail;
	tail = user_first_node;

	int i;

	for (i = 0; i < users_count; i++)
	{
		new_node = (users *)malloc(sizeof(users));
		strcpy(new_node->data.user_name, user_buffer[i].user_name);
		strcpy(new_node->data.user_password, user_buffer[i].user_password);
		new_node->data.balance = user_buffer[i].balance;
		new_node->data.isVIP = user_buffer[i].isVIP;

		tail->next = new_node;
		tail = new_node;
		tail->next = NULL;
	}
	printf("用户数据链表创建成功!\n");
}

void linklist_to_user_buffer(users *u)
{
	int i = 0;

	if (user_first_node == NULL)
	{
		printf("用户数据链表没有初始化!\n");
		return;
	}
	else
	{
		while (u->next != NULL)
		{
			u = u->next;
			strcpy(user_buffer[i].user_name, u->data.user_name);
			strcpy(user_buffer[i].user_password, u->data.user_password);
			user_buffer[i].balance = u->data.balance;
			user_buffer[i].isVIP = u->data.isVIP;
			i++;
		}
		users_count = i;
		printf("用户数据链表成功复制到缓存数组\n");
	}
}

// 用户登录
void user_login(void)
{
	printf("正在从磁盘读取用户配置文件, 请稍后.........!\n");
	load_user_data();
	printf("读取用户文件成功 任意按键继续\n");
	pause_and_wait();
	fflush(stdin);
	char name[20];
	char password[20];
	char password_confirm[20];
	int j;
	int found_flag = 0;
	int name_error_time = 0;
	int password_error_time = 0;

	clear_screen();
	printf("#######################################################\n");
	printf("欢迎来到用户登录向导\n");
	printf("#######################################################\n");

	while (1)
	{
		printf("请输入你的用户名:");
		scanf("%s", name);

		for (j = 0; j < users_count; j++)
		{
			if (strcmp(name, user_buffer[j].user_name) == 0)
			{
				found_flag = 1;
				break;
			}
		}

		if (found_flag == 0)
		{
			printf("错误! 你输入的用户名不存在\n");
			printf("任意按键继续!\n");
			pause_and_wait();
			name_error_time++;
			if (name_error_time >= 3)
			{
				printf("严重错误!!! 你已经 %d 次输错账户名\n", name_error_time);
				printf("拒绝登录!\n");
				exit(0);
			}
		}
		else
		{
			break;
		}
	}

	while (1)
	{
		printf("请输入密码(显示为星号):");
		// 这里需要实现input_secret函数或使用其他方式
		scanf("%s", password);
		printf("请确认密码:");
		scanf("%s", password_confirm);

		if (strcmp(password, password_confirm) == 0)
		{
			while (1)
			{
				if (strcmp(password_confirm, user_buffer[j].user_password) == 0)
				{
					printf("登录成功任意按键继续\n");
					current_logined_user_in_buffer = j;
					is_user_logined = 1;
					fflush(stdin);
					getchar();
					print_user_menu();
					break;
				}
				else
				{
					password_error_time++;
					printf("账号 %s 对应的密码错误\n", user_buffer[j].user_name);
					printf("请重试\n");
					if (password_error_time >= 3)
					{
						printf("严重错误 你已经输错密码 %d 次!\n", password_error_time);
						printf("拒绝访问!\n");
						exit(0);
					}
					break;
				}
			}
		}
		else
		{
			printf("输入错误! 任意按键重新输入!!\n");
		}
	}
}

// 新用户注册
void new_user_regist(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("欢迎来到新用户注册向导\n");
	printf("#######################################################\n");

	char name[20];
	char password[20];
	char password_confirm[20];
	int i;
	int name_conflict = 0;

	printf("请输入你的用户名:");
	scanf("%s", name);

	// 检查用户名是否已存在
	for (i = 0; i < users_count; i++)
	{
		if (strcmp(name, user_buffer[i].user_name) == 0)
		{
			name_conflict = 1;
			break;
		}
	}

	if (name_conflict)
	{
		printf("错误! 用户名 %s 已经存在!\n", name);
		printf("任意按键返回主菜单!\n");
		pause_and_wait();
		return;
	}

	printf("请输入密码:");
	scanf("%s", password);
	printf("请确认密码:");
	scanf("%s", password_confirm);

	if (strcmp(password, password_confirm) == 0)
	{
		// 创建新用户
		strcpy(user_buffer[users_count].user_name, name);
		strcpy(user_buffer[users_count].user_password, password);
		user_buffer[users_count].balance = 100.0; // 默认余额
		user_buffer[users_count].isVIP = 0;       // 默认非VIP

		users_count++;
		save_user_data();

		printf("注册成功! 你的账户余额为: %.2f\n", user_buffer[users_count - 1].balance);
		printf("任意按键返回主菜单!\n");
		pause_and_wait();
	}
	else
	{
		printf("两次密码输入不一致! 注册失败!\n");
		printf("任意按键返回主菜单!\n");
		pause_and_wait();
	}
}

// 用户菜单
void print_user_menu(void)
{
	int choice;

	while (1)
	{
		clear_screen();
		printf("#######################################################\n");
		printf("欢迎用户 %s 使用图书馆系统\n", user_buffer[current_logined_user_in_buffer].user_name);
		printf("#######################################################\n");
		printf("1.查看书籍信息\n");
		printf("2.购买书籍\n");
		printf("3.查看账户余额\n");
		printf("4.查看购买历史\n");
		printf("5.退出登录\n");
		printf("#######################################################\n");
		printf("请输入你的选择:");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			user_book_info_look_up();
			break;
		case 2:
			user_buy_book();
			break;
		case 3:
			user_view_balance();
			break;
		case 4:
			user_view_purchase_history();
			break;
		case 5:
			user_logout();
			return;
		default:
			printf("输入错误! 任意按键重新输入!\n");
			pause_and_wait();
		}
	}
}

// 用户查看书籍信息
void user_book_info_look_up(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("书籍信息查询\n");
	printf("#######################################################\n");
	print_book_list();
	printf("任意按键返回用户菜单!\n");
	pause_and_wait();
}

// 用户购买书籍
void user_buy_book(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("购买书籍\n");
	printf("#######################################################\n");
	
	int book_id;
	int quantity;
	int i;
	int found = 0;

	printf("请输入要购买的书籍ID:");
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

	printf("请输入购买数量:");
	scanf("%d", &quantity);

	if (quantity <= 0)
	{
		printf("购买数量必须大于0!\n");
		printf("任意按键返回!\n");
		pause_and_wait();
		return;
	}

	if (quantity > book_buffer[i].book_storage_count)
	{
		printf("库存不足! 当前库存: %d\n", book_buffer[i].book_storage_count);
		printf("任意按键返回!\n");
		pause_and_wait();
		return;
	}

	float total_cost = book_buffer[i].book_price * quantity;
	if (total_cost > user_buffer[current_logined_user_in_buffer].balance)
	{
		printf("余额不足! 需要: %.2f, 当前余额: %.2f\n", 
			   total_cost, user_buffer[current_logined_user_in_buffer].balance);
		printf("任意按键返回!\n");
		pause_and_wait();
		return;
	}

	// 执行购买
	user_buffer[current_logined_user_in_buffer].balance -= total_cost;
	book_buffer[i].book_storage_count -= quantity;

	// 记录购买历史
	shop_list[bought_times_count].book_id = book_id;
	strcpy(shop_list[bought_times_count].book_author, book_buffer[i].book_author);
	shop_list[bought_times_count].book_price = book_buffer[i].book_price;
	shop_list[bought_times_count].buy_count = quantity;
	bought_times_count++;

	// 保存数据
	save_user_data();
	save_book_data();

	printf("购买成功! 总花费: %.2f, 剩余余额: %.2f\n", 
		   total_cost, user_buffer[current_logined_user_in_buffer].balance);
	printf("任意按键返回!\n");
	pause_and_wait();
}

// 用户查看余额
void user_view_balance(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("账户余额查询\n");
	printf("#######################################################\n");
	printf("当前用户: %s\n", user_buffer[current_logined_user_in_buffer].user_name);
	printf("账户余额: %.2f\n", user_buffer[current_logined_user_in_buffer].balance);
	printf("VIP状态: %s\n", user_buffer[current_logined_user_in_buffer].isVIP ? "是" : "否");
	printf("#######################################################\n");
	printf("任意按键返回!\n");
	pause_and_wait();
}

// 用户查看购买历史
void user_view_purchase_history(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("购买历史\n");
	printf("#######################################################\n");
	
	if (bought_times_count == 0)
	{
		printf("暂无购买记录!\n");
	}
	else
	{
		printf("ID\t作者\t\t价格\t\t数量\n");
		for (int i = 0; i < bought_times_count; i++)
		{
			printf("%d\t%s\t\t%.2f\t\t%d\n", 
				   shop_list[i].book_id,
				   shop_list[i].book_author,
				   shop_list[i].book_price,
				   shop_list[i].buy_count);
		}
	}
	printf("#######################################################\n");
	printf("任意按键返回!\n");
	pause_and_wait();
}

// 用户退出登录
void user_logout(void)
{
	is_user_logined = 0;
	current_logined_user_in_buffer = -1;
	printf("用户已退出登录!\n");
}

// 打印用户列表
void print_user_list(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("用户列表\n");
	printf("#######################################################\n");
	printf("用户名\t\t密码\t\t余额\t\tVIP状态\n");
	
	for (int i = 0; i < users_count; i++)
	{
		printf("%s\t\t%s\t\t%.2f\t\t%s\n",
			   user_buffer[i].user_name,
			   user_buffer[i].user_password,
			   user_buffer[i].balance,
			   user_buffer[i].isVIP ? "是" : "否");
	}
	printf("#######################################################\n");
	printf("任意按键返回!\n");
	pause_and_wait();
}

// 删除指定用户
void delete_user_by_name(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("删除用户\n");
	printf("#######################################################\n");
	
	char name[20];
	int i, j;
	int found = 0;

	printf("请输入要删除的用户名:");
	scanf("%s", name);

	for (i = 0; i < users_count; i++)
	{
		if (strcmp(name, user_buffer[i].user_name) == 0)
		{
			found = 1;
			break;
		}
	}

	if (!found)
	{
		printf("用户不存在!\n");
		printf("任意按键返回!\n");
		pause_and_wait();
		return;
	}

	// 删除用户
	for (j = i; j < users_count - 1; j++)
	{
		user_buffer[j] = user_buffer[j + 1];
	}
	users_count--;

	save_user_data();
	printf("用户 %s 删除成功!\n", name);
	printf("任意按键返回!\n");
	pause_and_wait();
}
