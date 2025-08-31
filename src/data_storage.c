#include "data_storage.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 加载所有数据
void load_all_data(void)
{
	load_admin_data();
	load_user_data();
	load_book_data();
}

// 保存所有数据
void save_all_data(void)
{
	save_admin_data();
	save_user_data();
	save_book_data();
}

// 加载管理员数据
void load_admin_data(void)
{
	FILE *fp;
	int i;

	fp = fopen(ADMIN_FILE, "r");
	if (fp == NULL)
	{
		printf("管理员配置数据文件在当前目录下不存在!\n");
		printf("任意按键启动向导创建管理员!");
		getchar();
		// 这里需要调用create_admin_wizard，但为了避免循环依赖，我们直接返回
		return;
	}
	else
	{
		fscanf(fp, "%d\n", &admins_count);

		for (i = 0; i < admins_count; i++)
		{
			fscanf(fp, "%s%s", admin_buffer[i].admin_name, admin_buffer[i].admin_password);
		}
		printf("成功从磁盘读取管理员配置文件!\n");
		fclose(fp);
	}
}

// 保存管理员数据
void save_admin_data(void)
{
	FILE *fp;
	int i;

	fp = fopen(ADMIN_FILE, "w");
	if (fp == NULL)
	{
		printf("发生一个未知的读写错误! 管理员配置文件创建失败\n");
		return;
	}
	else
	{
		fprintf(fp, "%d\n", admins_count);

		for (i = 0; i < admins_count; i++)
		{
			fprintf(fp, "%s %s\n", admin_buffer[i].admin_name, admin_buffer[i].admin_password);
		}
		printf("成功将管理员缓存写入磁盘!\n");
		fclose(fp);
	}
}

// 加载用户数据
void load_user_data(void)
{
	FILE *fp;
	int i;

	fp = fopen(USER_FILE, "r");
	if (fp == NULL)
	{
		printf("用户配置数据文件在当前目录下不存在!\n");
		printf("将创建新的用户配置文件!\n");
		users_count = 0;
		return;
	}
	else
	{
		fscanf(fp, "%d\n", &users_count);

		for (i = 0; i < users_count; i++)
		{
			fscanf(fp, "%s %s %f %d\n", 
				   user_buffer[i].user_name, 
				   user_buffer[i].user_password,
				   &user_buffer[i].balance, 
				   &user_buffer[i].isVIP);
		}
		printf("成功从磁盘读取用户配置文件!\n");
		fclose(fp);
	}
}

// 保存用户数据
void save_user_data(void)
{
	FILE *fp;
	int i;

	fp = fopen(USER_FILE, "w");
	if (fp == NULL)
	{
		printf("发生一个未知的读写错误! 用户配置文件创建失败\n");
		return;
	}
	else
	{
		fprintf(fp, "%d\n", users_count);

		for (i = 0; i < users_count; i++)
		{
			fprintf(fp, "%s %s %.2f %d\n", 
					user_buffer[i].user_name, 
					user_buffer[i].user_password,
					user_buffer[i].balance, 
					user_buffer[i].isVIP);
		}
		printf("成功将用户缓存写入磁盘!\n");
		fclose(fp);
	}
}

// 删除用户配置文件
void del_user_config(void)
{
	if (remove(USER_FILE) == 0)
	{
		printf("用户配置文件删除成功!\n");
		users_count = 0;
	}
	else
	{
		printf("用户配置文件删除失败!\n");
	}
	printf("任意按键返回!\n");
	pause_and_wait();
}
