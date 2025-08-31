#ifndef __ADMIN_MANAGEMENT_H__
#define __ADMIN_MANAGEMENT_H__

#include "common.h"

// 管理员登录和认证
void admin_login(void);
void admin_logout(void);
void system_config_login(void);
void create_admin_wizard(void);

// 管理员菜单和功能
void print_admin_menu(void);
void print_admin_list(void);

// 全局变量声明
extern admin admin_buffer[MAX_BUFFER_SIZE];
extern int admins_count;
extern admins *admin_first_node;
extern int is_admin_logined;

#endif
