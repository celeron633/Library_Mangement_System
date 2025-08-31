#include "system_ui.h"
#include "utils.h"
#include "admin_management.h"
#include "user_management.h"
#include "data_storage.h"
#include <stdio.h>
#include <stdlib.h>

// 全局变量声明（extern）
extern shoplist shop_list[MAX_BUFFER_SIZE];
extern int bought_times_count;

// 系统主菜单
void print_system_menu(void)
{
	int choice;

	while (1)
	{
		clear_screen();
		printf("\t\t\t******************************************\n");
		printf("\t\t\t**************图书馆系统登录**************\n");
		printf("\t\t\t******************************************\n");
		printf("\t\t\t\t1.管理员登录\n");
		printf("\t\t\t\t2.用户登录\n");
		printf("\t\t\t\t3.图书馆系统设置\n");
		printf("\t\t\t\t4.新用户注册\n");
		printf("\t\t\t\t5.退出系统\n");
		printf("\t\t\t******************************************\n");
		printf("\t\t\t******************************************\n");
		printf("\t\t\t请输入你的选择:");
		scanf("%d", &choice);
		
		switch (choice)
		{
		case 1:
			admin_login();
			break;
		case 2:
			user_login();
			break;
		case 3:
			clear_screen();
			printf("你将访问系统设置菜单! 为了安全 需要验证你的管理员身份!\n");
			pause_and_wait();
			system_config_login();
			print_system_config_menu();
			break;
		case 4:
			new_user_regist();
			break;
		case 5:
			clear_screen();
			printf("拜拜!\n");
			exit(0);
		default:
			printf("输入错误! 任意按键重新输入!");
			fflush(stdin);
			getchar();
		}
	}
}

// 系统配置菜单
void print_system_config_menu(void)
{
	int choice;

	while (1)
	{
		clear_screen();
		printf("\t\t\t******************************************\n");
		printf("\t\t\t*************系统设置*********************\n");
		printf("\t\t\t******************************************\n");
		printf("\t\t\t\t1.显示当前的管理员列表\n");
		printf("\t\t\t\t2.新建管理员\n");
		printf("\t\t\t\t3.清除书籍信息文件\n");
		printf("\t\t\t\t4.清除用户信息文件\n");
		printf("\t\t\t\t5.显示当前所有用户\n");
		printf("\t\t\t\t6.删除指定名字用户\n");
		printf("\t\t\t\t7.返回登录\n");
		printf("\t\t\t******************************************\n");
		printf("\t\t\t******************************************\n");
		printf("\t\t\t请输入你的选择:");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			print_admin_list();
			break;
		case 2:
			create_admin_wizard();
			break;
		case 3:
			clear_screen();
			printf("警告! 这将丢失所有的书籍信息配置文件包括已经保存的书籍!");
			pause_and_wait();
			// 这里需要调用del_book_config函数，但为了避免循环依赖，我们直接实现
			{
				if (remove(BOOK_FILE) == 0)
				{
					printf("书籍配置文件删除成功!\n");
				}
				else
				{
					printf("书籍配置文件删除失败!\n");
				}
			}
			break;
		case 4:
			clear_screen();
			printf("警告! 这将丢失所有的用户信息配置文件包括已经保存的书籍!");
			pause_and_wait();
			del_user_config();
			break;
		case 5:
			print_user_list();
			break;
		case 6:
			delete_user_by_name();
			break;
		case 7:
			return;
		default:
			printf("输入错误! 任意按键重新输入!");
			fflush(stdin);
			getchar();
		}
	}
}

// 用户购买书籍处理
void user_buy_book_process(void)
{
	// 这个函数已经在user_management.c中实现了
	// 这里只是为了保持接口一致性
}

// 打印购物列表
void print_shop_list(void)
{
	clear_screen();
	printf("#######################################################\n");
	printf("购物记录\n");
	printf("#######################################################\n");
	
	if (bought_times_count == 0)
	{
		printf("暂无购物记录!\n");
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
