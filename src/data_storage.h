#ifndef __DATA_STORAGE_H__
#define __DATA_STORAGE_H__

#include "common.h"

// 前向声明
extern admin admin_buffer[];
extern int admins_count;
extern user user_buffer[];
extern int users_count;

// 数据加载和保存
void load_all_data(void);
void save_all_data(void);
void load_admin_data(void);
void save_admin_data(void);
void load_user_data(void);
void save_user_data(void);

// 书籍数据函数声明
void load_book_data(void);
void save_book_data(void);

// 链表操作
void admin_buffer_to_linklist(void);
void linklist_to_admin_buffer(admins *a);

// 数据清理
void del_user_config(void);

#endif
