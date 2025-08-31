#ifndef __BOOK_MANAGEMENT_H__
#define __BOOK_MANAGEMENT_H__

#include "common.h"

// 书籍信息管理
void admin_book_info_look_up(void);
void add_new_book(void);
void delete_book_by_id(void);
void modify_book_info(void);
void print_book_list(void);

// 书籍数据操作
void book_buffer_to_linklist(void);
void linklist_to_book_buffer(books *b);
void load_book_data(void);
void save_book_data(void);
void del_book_config(void);

// 全局变量声明
extern book book_buffer[MAX_BUFFER_SIZE];
extern int books_count;
extern books *book_first_node;

#endif
