#include "admin_management.h"
#include "utils.h"
#include "data_storage.h"
#include "book_management.h"
#include "user_management.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 全局变量声明（extern）
extern admin admin_buffer[MAX_BUFFER_SIZE];
extern int admins_count;
extern admins *admin_first_node;
extern int is_admin_logined;

// 管理员数据链表操作
void admin_buffer_to_linklist()
{
	admin_first_node = (admins *)malloc(sizeof(admins));
	admin_first_node->next = NULL;
	admins *new_node;
	admins *tail;
	tail = admin_first_node;

	int i;

	for (i = 0; i < admins_count; i++)
	{
		new_node = (admins *)malloc(sizeof(admins));
		strcpy(new_node->data.admin_name, admin_buffer[i].admin_name);
		strcpy(new_node->data.admin_password, admin_buffer[i].admin_password);

		tail->next = new_node;
		tail = new_node;
		tail->next = NULL;
	}
	printf("管理员数据链表创建成功!\n");
}

void linklist_to_admin_buffer(admins *a)
{
	int i = 0;

	if (admin_first_node == NULL)
	{
		printf("管理员数据链表没有初始化!\n");
		return;
	}
	else
	{
		while (a->next != NULL)
		{
			a = a->next;
			strcpy(admin_buffer[i].admin_name, a->data.admin_name);
			strcpy(admin_buffer[i].admin_password, a->data.admin_password);
			i++;
		}
		admins_count = i;
		printf("管理员数据链表成功复制到缓存数组\n");
	}
}

// 管理员登录
void admin_login(void)
{
	printf("正在从磁盘读取管理员配置文件, 请稍后.........!\n");
	load_admin_data();
	printf("读取管理员文件成功 任意按键继续\n");
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
	printf("欢迎来到管理员登录向导\n");
	printf("#######################################################\n");

	while (1)
	{
		printf("请输入你的管理员用户名:");
		scanf("%s", name);

		for (j = 0; j < admins_count; j++)
		{
			if (strcmp(name, admin_buffer[j].admin_name) == 0)
			{
				found_flag = 1;
				break;
			}
		}

		if (found_flag == 0)
		{
			printf("错误! 你输入的管理员用户名不存在\n");
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
		printf("请输入密码:");
		scanf("%s", password);
		printf("请确认密码:");
		scanf("%s", password_confirm);

		if (strcmp(password, password_confirm) == 0)
		{
			while (1)
			{
				if (strcmp(password_confirm, admin_buffer[j].admin_password) == 0)
				{
					printf("登录成功任意按键继续\n");
					is_admin_logined = 1;
					fflush(stdin);
					getchar();
					print_admin_menu();
					break;
				}
				else
				{
					password_error_time++;
					printf("账号 %s 对应的密码错误\n", admin_buffer[j].admin_name);
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

// 管理员退出登录
void admin_logout(void)
{
	is_admin_logined = 0;
	printf("管理员已退出登录!\n");
}

// 系统配置登录
void system_config_login(void)
{
	printf("正在从磁盘读取管理员配置文件, 请稍后.........!\n");
	load_admin_data();
	printf("读取管理员文件成功 任意按键继续\n");
	pause_and_wait();
	fflush(stdin);
	
	char name[20];
	char password[20];
	int j;
	int found_flag = 0;
	int name_error_time = 0;
	int password_error_time = 0;

	clear_screen();
	printf("#######################################################\n");
	printf("系统配置身份验证\n");
	printf("#######################################################\n");

	while (1)
	{
		printf("请输入你的管理员用户名:");
		scanf("%s", name);

		for (j = 0; j < admins_count; j++)
		{
			if (strcmp(name, admin_buffer[j].admin_name) == 0)
			{
				found_flag = 1;
				break;
			}
		}

		if (found_flag == 0)
		{
			printf("错误! 你输入的管理员用户名不存在\n");
			printf("任意按键继续!\n");
			pause_and_wait();
			name_error_time++;
			if (name_error_time >= 3)
			{
				printf("严重错误!!! 你已经 %d 次输错账户名\n", name_error_time);
				printf("拒绝访问!\n");
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
		printf("请输入密码:");
		scanf("%s", password);

		if (strcmp(password, admin_buffer[j].admin_password) == 0)
		{
			printf("身份验证成功! 任意按键进入系统配置!\n");
			pause_and_wait();
			// 这里应该调用系统配置菜单，但为了避免循环依赖，我们直接返回
			return;
		}
		else
		{
			password_error_time++;
			printf("账号 %s 对应的密码错误\n", admin_buffer[j].admin_name);
			printf("请重试\n");
			if (password_error_time >= 3)
			{
				printf("严重错误 你已经输错密码 %d 次!\n", password_error_time);
				printf("拒绝访问!\n");
				exit(0);
			}
		}
	}
}

// 创建管理员向导
void create_admin_wizard(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("欢迎来到管理员创建向导\n");
	printf("#######################################################\n");

	char name[20];
	char password[20];
	char password_confirm[20];
	int i;
	int name_conflict = 0;

	printf("请输入管理员用户名:");
	scanf("%s", name);

	// 检查用户名是否已存在
	for (i = 0; i < admins_count; i++)
	{
		if (strcmp(name, admin_buffer[i].admin_name) == 0)
		{
			name_conflict = 1;
			break;
		}
	}

	if (name_conflict)
	{
		printf("错误! 管理员用户名 %s 已经存在!\n", name);
		printf("任意按键返回!\n");
		pause_and_wait();
		return;
	}

	printf("请输入密码:");
	scanf("%s", password);
	printf("请确认密码:");
	scanf("%s", password_confirm);

	if (strcmp(password, password_confirm) == 0)
	{
		// 创建新管理员
		strcpy(admin_buffer[admins_count].admin_name, name);
		strcpy(admin_buffer[admins_count].admin_password, password);

		admins_count++;
		save_admin_data();

		printf("管理员创建成功!\n");
		printf("任意按键返回!\n");
		pause_and_wait();
	}
	else
	{
		printf("两次密码输入不一致! 创建失败!\n");
		printf("任意按键返回!\n");
		pause_and_wait();
	}
}

// 管理员菜单
void print_admin_menu(void)
{
	int choice;

	while (1)
	{
		clear_screen();
		printf("#######################################################\n");
		printf("管理员菜单\n");
		printf("#######################################################\n");
		printf("1.查看书籍信息\n");
		printf("2.添加新书籍\n");
		printf("3.删除书籍\n");
		printf("4.修改书籍信息\n");
		printf("5.退出登录\n");
		printf("#######################################################\n");
		printf("请输入你的选择:");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			admin_book_info_look_up();
			break;
		case 2:
			add_new_book();
			break;
		case 3:
			delete_book_by_id();
			break;
		case 4:
			modify_book_info();
			break;
		case 5:
			admin_logout();
			return;
		default:
			printf("输入错误! 任意按键重新输入!\n");
			pause_and_wait();
		}
	}
}

// 打印管理员列表
void print_admin_list(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("管理员列表\n");
	printf("#######################################################\n");
	printf("用户名\t\t密码\n");
	
	for (int i = 0; i < admins_count; i++)
	{
		printf("%s\t\t%s\n",
			   admin_buffer[i].admin_name,
			   admin_buffer[i].admin_password);
	}
	printf("#######################################################\n");
	printf("任意按键返回!\n");
	pause_and_wait();
}
