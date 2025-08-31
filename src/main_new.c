#include "common.h"
#include "utils.h"
#include "user_management.h"
#include "book_management.h"
#include "admin_management.h"
#include "system_ui.h"
#include "data_storage.h"

#ifdef _WIN32
#include <conio.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// 全局变量定义
admin admin_buffer[MAX_BUFFER_SIZE];
int admins_count;

user user_buffer[MAX_BUFFER_SIZE];
int users_count;

book book_buffer[MAX_BUFFER_SIZE];
int books_count;

shoplist shop_list[MAX_BUFFER_SIZE];
int bought_times_count;

admins *admin_first_node;
users *user_first_node;
books *book_first_node;

int current_logined_user_in_buffer;
int is_user_logined;
int is_admin_logined;

// 检查并创建数据目录
void ensure_data_directory(void)
{
    DIR *dir = opendir(DATA_DIR);
    if (dir == NULL)
    {
        printf("数据目录不存在，正在创建...\n");
        if (mkdir(DATA_DIR, 0755) == 0)
        {
            printf("数据目录创建成功: %s\n", DATA_DIR);
        }
        else
        {
            printf("错误: 无法创建数据目录 %s\n", DATA_DIR);
            exit(1);
        }
    }
    else
    {
        closedir(dir);
        printf("数据目录检查完成: %s\n", DATA_DIR);
    }
}

// 程序初始化
void initialize_system(void)
{
    printf("正在初始化图书馆管理系统...\n");
    
    // 确保数据目录存在
    ensure_data_directory();
    
    // 初始化全局变量
    admins_count = 0;
    users_count = 0;
    books_count = 0;
    bought_times_count = 0;
    
    admin_first_node = NULL;
    user_first_node = NULL;
    book_first_node = NULL;
    
    current_logined_user_in_buffer = -1;
    is_user_logined = 0;
    is_admin_logined = 0;
    
    // 加载所有数据
    load_all_data();
    
    printf("系统初始化完成!\n");
    printf("任意按键继续...\n");
    pause_and_wait();
}

// 程序清理
void cleanup_system(void)
{
    printf("正在保存数据...\n");
    save_all_data();
    printf("数据保存完成!\n");
    printf("系统即将退出...\n");
}

int main(int argc, char const *argv[])
{
    // 系统初始化
    initialize_system();
    
    // 显示主菜单
    print_system_menu();
    
    // 程序清理（实际上这里不会执行到，因为print_system_menu中有exit(0)）
    cleanup_system();
    
    return 0;
}
