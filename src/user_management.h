#ifndef __USER_MANAGEMENT_H__
#define __USER_MANAGEMENT_H__

#include "common.h"

// 用户登录相关
void user_login(void);
void new_user_regist(void);
void user_logout(void);

// 用户菜单和功能
void print_user_menu(void);
void user_book_info_look_up(void);
void user_buy_book(void);
void user_view_balance(void);
void user_view_purchase_history(void);

// 用户数据操作
void user_buffer_to_linklist(void);
void linklist_to_user_buffer(users *u);
void print_user_list(void);
void delete_user_by_name(void);

// 全局变量声明
extern user user_buffer[MAX_BUFFER_SIZE];
extern int users_count;
extern users *user_first_node;
extern int current_logined_user_in_buffer;
extern int is_user_logined;

#endif
