#ifndef __SYSTEM_UI_H__
#define __SYSTEM_UI_H__

#include "common.h"

// 系统主菜单
void print_system_menu(void);
void print_system_config_menu(void);

// 购物相关
void user_buy_book_process(void);
void print_shop_list(void);

// 全局变量声明
extern shoplist shop_list[MAX_BUFFER_SIZE];
extern int bought_times_count;

#endif
