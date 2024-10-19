#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>


#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 1024
#endif

#ifndef MAX_RECORD_SIZE
#define MAX_RECORD_SIZE 1024
#endif


typedef struct admin
{
	char admin_name[20];
	char admin_password[20];
}admin;

typedef struct user
{
	char user_name[20];
	char user_password[20];
	float balance;
	int isVIP;
}user;

typedef struct book
{
	int book_id;
	char book_name[48];
	char book_author[48];
	char book_introduce[100];
	float book_price;
	int book_storage_count;
}book;

typedef struct shoplist
{
	int book_id;
	char book_author[48];
	float book_price;
	int buy_count;
}shoplist;

typedef struct books
{
	book data;
	struct books *next;
}books;

typedef struct admins
{
	admin data;
	struct admins* next;
}admins;

typedef struct users
{
	user data;
	struct users* next;
}users;

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

void create_admin_wizard(void);
void print_admin_menu(void);
void print_system_menu(void);
void print_user_menu(void);
void admin_book_info_look_up();
void system_config_login();
void print_system_config_menu();



void user_buffer_to_linklist()
{
	user_first_node=(users*)malloc(sizeof(users));
	user_first_node->next=NULL;
	users* new_node;
	users* tail;
	tail=user_first_node;

	int i;

	for(i=0;i<users_count;i++)
	{
		new_node=(users*)malloc(sizeof(users));
		strcpy(new_node->data.user_name,user_buffer[i].user_name);
		strcpy(new_node->data.user_password,user_buffer[i].user_password);
		new_node->data.balance=user_buffer[i].balance;
		new_node->data.isVIP=user_buffer[i].isVIP;

		tail->next=new_node;
		tail=new_node;
		tail->next=NULL;
	}
	printf("用户数据链表创建成功!\n");
}

void linklist_to_user_buffer(users* u)
{
	int count;
	int i=0;


	if (user_first_node==NULL)
	{
		printf("用户数据链表没有初始化!\n");
		return;
	}
	else
	{
		while(u->next!=NULL)
		{
			u=u->next;
			strcpy(user_buffer[i].user_name,u->data.user_name);
			strcpy(user_buffer[i].user_password,u->data.user_password);
			user_buffer[i].balance=u->data.balance;
			user_buffer[i].isVIP=u->data.isVIP;
			i++;
		}
		users_count=i;
		printf("用户数据链表成功复制到缓存数组\n");
	}
}

void book_buffer_to_linklist()
{
	book_first_node=(books*)malloc(sizeof(books));
	book_first_node->next=NULL;
	books* new_node;
	books* tail;
	tail=book_first_node;

	int i;

	for(i=0;i<books_count;i++)
	{
		new_node=(books*)malloc(sizeof(books));
		new_node->data.book_id=book_buffer[i].book_id;
		strcpy(new_node->data.book_name,book_buffer[i].book_name);
		strcpy(new_node->data.book_author,book_buffer[i].book_author);
		strcpy(new_node->data.book_introduce,book_buffer[i].book_introduce);
		new_node->data.book_price=book_buffer[i].book_price;
		new_node->data.book_storage_count=book_buffer[i].book_storage_count;

		tail->next=new_node;
		tail=new_node;
		tail->next=NULL;
	}
	printf("书本数据链表创建成功!\n");
}

void linklist_to_book_buffer(books* u)
{
	int count;
	int i=0;


	if (book_first_node==NULL)
	{
		printf("错误! 书本数据链表没有初始化\n");
		return;
	}
	else
	{
		while(u->next!=NULL)
		{
			u=u->next;
			book_buffer[i].book_id=u->data.book_id;
			strcpy(book_buffer[i].book_name,u->data.book_name);
			strcpy(book_buffer[i].book_author,u->data.book_author);
			strcpy(book_buffer[i].book_introduce,u->data.book_introduce);

			book_buffer[i].book_price=u->data.book_price;
			book_buffer[i].book_storage_count=u->data.book_storage_count;

			i++;
		}

		books_count=i;
		printf("书本数据链表已经复制到缓存数组\n");
	}
}


void read_admins_file()
{
	FILE *fp;
	int i;

	fp=fopen("./admin.dat","r");
	if (fp==NULL)
	{
		printf("管理员配置数据文件在当前目录下不存在!\n");
		printf("任意按键启动向导创建管理员!");
		getchar();
		create_admin_wizard();
	}
	else
	{
		fscanf(fp,"%d\n",&admins_count);

		for(i=0;i<admins_count;i++)
		{
			fscanf(fp,"%s%s",admin_buffer[i].admin_name,admin_buffer[i].admin_password);
		}
		printf("成功从磁盘读取管理员配置文件!\n");
		fclose(fp);
	}
}



void write_admins_file()
{
	FILE* fp;
	int i;

	fp=fopen("./admin.dat","w");
	if (fp==NULL)
	{
		printf("发生一个未知的读写错误! 管理员配置文件创建失败\n");
		return;
	}
	else
	{
		fprintf(fp,"%d\n",admins_count);

		for(i=0;i<admins_count;i++)
		{
			fprintf(fp,"%s %s\n",admin_buffer[i].admin_name,admin_buffer[i].admin_password);
		}
		printf("成功将管理员缓存写入磁盘!\n");
		fclose(fp);
	}
}

void read_users_file()
{
	FILE *fp;
	int i;

	fp=fopen("./user.dat","r");
	if (fp==NULL)
	{
		printf("用户配置文件在当前目录下没有找到!\n");
		printf("所以没有加载任何用户数据\n");
		printf("任意按键返回系统登录菜单");
		fflush(stdin);
		getchar();
		print_system_menu();
	}
	else
	{
		fscanf(fp,"%d\n",&users_count);

		for(i=0;i<users_count;i++)
		{
			fscanf(fp,"%s%s%f%d",user_buffer[i].user_name,user_buffer[i].user_password,&user_buffer[i].balance,&user_buffer[i].isVIP);
		}
		printf("成功从磁盘读取用户配置文件\n");
		fclose(fp);
	}
}


void write_users_file()
{
	FILE* fp;
	int i;

	fp=fopen("./user.dat","w");
	if (fp==NULL)
	{
		printf("发生一个未知的读写错误 用户配置文件创建失败!");
	}
	else
	{
		fprintf(fp,"%d\n",users_count);

		for(i=0;i<users_count;i++)
		{
			fprintf(fp,"%s %s %f %d\n",user_buffer[i].user_name,user_buffer[i].user_password,user_buffer[i].balance,user_buffer[i].isVIP);
		}
		printf("成功将用户配置缓存写入磁盘!\n");
		fclose(fp);
	}
}

void read_books_file()
{
	FILE *fp;
	int i;

	fp=fopen("./book.dat","r");
	if (fp==NULL)
	{
		printf("在当前目录下没有找到图书信息配置文件\n");
		printf("所以没有加载任何图书信息\n");
		printf("任意按键返回系统登录菜单\n");
		getchar();
	}
	else
	{
		fscanf(fp,"%d\n",&books_count);

		for(i=0;i<books_count;i++)
		{
			fscanf(fp,"%d%s%s%s%f%d",&book_buffer[i].book_id,book_buffer[i].book_name,book_buffer[i].book_author,book_buffer[i].book_introduce,&book_buffer[i].book_price,&book_buffer[i].book_storage_count);
		}
		printf("成功从磁盘读取书籍配置文件!\n");
		fclose(fp);
	}
}


void write_books_file()
{
	FILE* fp;
	int i;

	fp=fopen("./book.dat","w");
	if (fp==NULL)
	{
		printf("发生一个未知的读写错误 书籍配置文件创建失败!");
	}
	else
	{
		fprintf(fp,"%d\n",books_count);

		for(i=0;i<books_count;i++)
		{
			fprintf(fp,"%d %s %s %s %f %d\n",book_buffer[i].book_id,book_buffer[i].book_name,book_buffer[i].book_author,book_buffer[i].book_introduce,book_buffer[i].book_price,book_buffer[i].book_storage_count);
		}
		printf("成功将书籍资料缓存写入磁盘!\n");
		fclose(fp);
	}
}

void input_secret(char *secret)
{
	char temp;
	int i=0;

	while(1)
	{
		temp=getch();
		if (temp!='\r')
		{
			printf("*");
			secret[i]=temp;
			i++;
		}
		else
		{
			printf("\n");
			break;
		}
		secret[i]='\0';
	}
}

void create_admin_wizard(void)
{
	char name[20];
	char password[20];
	char password_confirm[20];
	int i,j;
	char choice[10];
	int override_flag=0;
	
	read_admins_file();
	
	while(1)
	{
		system("cls");
		printf("#######################################################\n");
		printf("欢迎来到管理员创建向导\n");
		printf("#######################################################\n");

		while(1)
		{
			printf("输入你的名字:");
			scanf("%s",name);

			for(j=0;j<admins_count;j++)
			{
				if (strcmp(name,admin_buffer[j].admin_name)==0)
				{
					override_flag=1;
				}
			}

			if (override_flag==1)
			{
				printf("错误! 存在同名管理员! \n");
				printf("任意按键重新输入!\n");
				override_flag=0;
				system("pause");
			}
			else
			{
				break;
			}
		}

		while(1)
		{
			printf("请输入密码(显示为星号):");
			input_secret(password);
			printf("请确认密码:");
			input_secret(password_confirm);
			if (strcmp(password,password_confirm)==0)
			{
				printf("成功! 新的管理员已经添加\n");
				strcpy(admin_buffer[admins_count].admin_name,name);
				strcpy(admin_buffer[admins_count].admin_password,password);
				admins_count++;
				break;
			}
			else
			{
				printf("两次输入的密码不同 请重试!\n");
			}
		}
		printf("继续创建管理员?(y/n):");
		fflush(stdin);
		scanf("%s",choice);
		if (strcmp(choice,"n")==0 || strcmp(choice,"no")==0 || strcmp(choice,"0")==0)
		{
			break;
		}
	}
	printf("正在写入磁盘.............\n");
	write_admins_file(admin_buffer);
	printf("写入磁盘成功........任意按键返回\n");
	system("pause");
	print_system_config_menu();
}

void add_admin_wizard(void)
{
	char name[20];
	char password[20];
	char password_confirm[20];
	int i,j;
	char choice[10];
	int override_flag=0;

	while(1)
	{
		system("cls");
		printf("#######################################################\n");
		printf("欢迎来到管理员添加向导\n\n");
		printf("#######################################################\n");

		while(1)
		{
			printf("你的名字:");
			
			scanf("%s",name);

			for(j=0;j<admins_count;j++)
			{
				if (strcmp(name,admin_buffer[j].admin_name)==0)
				{
					override_flag=1;
				}
			}

			if (override_flag==1)
			{
				printf("错误 存在同名管理员\n");
				printf("请另外使用别的名字!\n");
			}
			else
			{
				break;
			}
		}

		while(1)
		{
			printf("请输入密码(显示为星号):");
			input_secret(password);
			printf("请确认密码:");
			input_secret(password_confirm);
			if (strcmp(password,password_confirm)==0)
			{
				printf("好的! 成功添加新的管理员\n");
				strcpy(admin_buffer[admins_count].admin_name,name);
				strcpy(admin_buffer[admins_count].admin_password,password);
				admins_count++;
				break;
			}
			else
			{
				printf("两次输入的密码不同 请重试\n");
			}
		}
		printf("继续添加管理员?(y/n):");
		fflush(stdin);
		scanf("%s",choice);
		if (strcmp(choice,"n")==0 || strcmp(choice,"no")==0 || strcmp(choice,"0")==0)
		{
			break;
		}
	}
}


void admin_show_all_books(books* b)
{
	read_books_file();
	book_buffer_to_linklist();

	if (b==NULL)
	{
		printf("没有任何图书,任意按键返回管理员菜单!");
		system("pause");
		print_admin_menu();
	}
	else
	{
		printf("ID\t\t 名字\t\t 作者\t\t 简介\t\t 价格\t\t 库存量\t\t \n");
		while(b->next!=NULL)
		{
			b=b->next;
			printf("%-15d %-15s %-15s %-15s %-15.2f %-15d\n",b->data.book_id,b->data.book_name,b->data.book_author,b->data.book_introduce,b->data.book_price,b->data.book_storage_count);
		}
		system("pause");
		print_admin_menu();
	}
}

void user_show_all_books(books* b)
{
	read_books_file();
	book_buffer_to_linklist();

	if (b==NULL)
	{
		printf("没有任何图书 任意按键返回用户菜单!");
		system("pause");
		print_user_menu();
	}
	else
	{
		printf("ID\t\t 名字\t\t 作者\t\t 简介\t\t 价格\t\t 库存量\t\t \n");
		while(b->next!=NULL)
		{
			b=b->next;
			printf("%-15d %-15s %-15s %-15s %-15.2f %-15d\n",b->data.book_id,b->data.book_name,b->data.book_author,b->data.book_introduce,b->data.book_price,b->data.book_storage_count);
		}
		system("pause");
		print_user_menu();
	}
}

void append_book()
{
	read_books_file();
	
	int i;
	int override_flag=0;

	printf("******************************************\n");
	printf("***********添加新书向导*****************\n");
	printf("******************************************\n");

	printf("请输入这本书的ID:");
	scanf("%d",&book_buffer[books_count].book_id);
	
	while(1)
	{
		printf("请输入书名:");
		scanf("%s",book_buffer[books_count].book_name);

		for (i = 0; i < books_count; ++i)
		{
			if(strcmp(book_buffer[books_count].book_name,book_buffer[i].book_name)==0)
			{
				override_flag=1;
				break;
			}
		}

		if (override_flag==1)
		{
			printf("存在相同书名\n");
			printf("任意按键重新输入书名\n");
			override_flag=0;
			system("pause");
		}
		else
		{
			break;
		}

	}

	printf("请输入书的作者名字:");
	scanf("%s",book_buffer[books_count].book_author);
	printf("请输入书的简介(不要有空格):");
	scanf("%s",book_buffer[books_count].book_introduce);
	printf("请输入输的价格:");
	scanf("%f",&book_buffer[books_count].book_price);
	printf("请输入书的库存:");
	scanf("%d",&book_buffer[books_count].book_storage_count);
	books_count++;
	printf("正在写入磁盘...........\n");
	write_books_file();
	book_buffer_to_linklist();
	printf("添加成功! 任意按键返回管理员菜单\n");
	system("pause");
	print_admin_menu();
}

void delete_book_by_id()
{
	read_books_file();
	book_buffer_to_linklist();

	books *p,*q;
	p=book_first_node;
	q=p->next;

	int id;
	int i;
	int found_flag;

	while(1)
	{
		printf("请输入要删除的书的ID:");
		scanf("%d",&id);

		for (i = 0; i < books_count; ++i)
		{
			if (book_buffer[i].book_id==id)
			{
				found_flag=1;
				break;
			}
		}

		if(found_flag==1)
		{
			break;
		}
		else
		{
			printf("ID对应的书不存在 任意按键重试!");
			found_flag=0;
			system("pause");
		}
	}

	while(q!=NULL)
	{
		if(q->data.book_id!=id)
		{
			p=q;
			q=q->next;
		}
		else
		{
			p->next=q->next;
			free(q);
			q=p->next;
		}
	}
	printf("删除成功!\n");
	printf("正在写入磁盘...............\n");
	linklist_to_book_buffer(book_first_node);
	write_books_file();
	printf("写入成功 任意按键返回管理员菜单");
	system("pause");
	print_admin_menu();
}

void delete_user_by_name()
{
	read_users_file();
	user_buffer_to_linklist();

	users *p,*q;
	p=user_first_node;
	q=p->next;

	char name[20];
	int i;
	int found_flag;
	
	printf("******************************************\n");
	printf("*************单个用户删除*****************\n");
	printf("******************************************\n");
	
	while(1)
	{
		printf("请输入要删除的用户的名字:");
		scanf("%s",name);

		for (i = 0; i < users_count; ++i)
		{
			if (strcmp(name,user_buffer[i].user_name)==0)
			{
				found_flag=1;
				break;
			}
		}

		if(found_flag==1)
		{
			break;
		}
		
		else
		{
			printf("输入的用户不存在 任意按键重试!");
			found_flag=0;
			system("pause");
		}
	}

	while(q!=NULL)
	{
		if(strcmp(name,q->data.user_name)!=0)
		{
			p=q;
			q=q->next;
		}
		else
		{
			p->next=q->next;
			free(q);
			q=p->next;
		}
	}
	
	printf("删除成功!\n");
	printf("正在写入磁盘...............\n");
	linklist_to_user_buffer(user_first_node);
	write_users_file();
	printf("写入成功 任意按键返回系统设置菜单");
	system("pause");
	print_system_config_menu();
}

void edit_book_storage_by_id(void)
{
	read_books_file();
	int i;
	int id;
	int count;
	int found_flag=0;

	while(1)
	{
		printf("输入你想修改库存的图书的ID:");
		scanf("%d",&id);

		for (i = 0; i < books_count; ++i)
		{
			if (book_buffer[i].book_id==id)
			{
				found_flag=1;
				break;
			}
		}

		if(found_flag==1)
		{
			break;
		}
		else
		{
			printf("ID对应的书不存在!");
			found_flag=0;
			printf("任意按键重新输入");
			system("pause");
		}

	}

	printf("输入这本书当前的库存量:");
	scanf("%d",&count);
	book_buffer[i].book_storage_count=count;
	book_buffer_to_linklist(book_first_node);
	printf("写入磁盘...........");
	write_books_file();
	printf("完成! 任意按键返回管理员菜单\n");
	system("pause");
	print_admin_menu();
}

void edit_book_info_by_id(void)
{
	read_books_file();
	int i;
	int id;
	int count;
	int found_flag=0;

	while(1)
	{
		printf("输入你想修改信息的图书的ID:");
		scanf("%d",&id);

		for (i = 0; i < books_count; ++i)
		{
			if (book_buffer[i].book_id==id)
			{
				found_flag=1;
				break;
			}
		}

		if(found_flag==1)
		{
			break;
		}
		else
		{
			printf("ID对应的书不存在! 任意按键重试! ");
			found_flag=0;
			system("pause");
		}

	}

	printf("输入这本书的新ID:");
	scanf("%d",&book_buffer[i].book_id);
	printf("输入这本书的新名字:");
	scanf("%s",book_buffer[i].book_name);
	printf("请输入书的作者名字:");
	scanf("%s",book_buffer[i].book_author);
	printf("请输入书的简介:");
	scanf("%s",book_buffer[i].book_introduce);
	printf("请输入书的价格:");
	scanf("%f",&book_buffer[i].book_price);
	printf("请输入书的库存量:");
	scanf("%d",&book_buffer[i].book_storage_count);

	book_buffer_to_linklist(book_first_node);
	write_books_file();
	printf("写入磁盘...........");
	write_books_file();
	printf("完成! 任意按键返回管理员菜单\n");
	system("pause");
	print_admin_menu();
}

void admin_lookup_introduce_by_id(void)
{
	read_books_file();
	int i;
	int id;
	int count;
	int found_flag=0;

	while(1)
	{
		printf("输入查询简介的图书的ID:");
		scanf("%d",&id);

		for (i = 0; i < books_count; ++i)
		{
			if (book_buffer[i].book_id==id)
			{
				found_flag=1;
				break;
			}
		}

		if(found_flag==1)
		{
			break;
		}
		else
		{
			printf("ID对应的书不存在! 任意按键重试! ");
			found_flag=0;
			system("pause");
		}
	}
	printf("##################图书简介#####################\n");
	printf("%s\n",book_buffer[i].book_introduce);
	system("pause");
	admin_book_info_look_up();
}

void sub_sort_books_by_price()
{
	read_books_file();
	int i,j;

	book temp;

	for(i=0;i<books_count-1;i++)
	{
		for(j=i;j>=0;j--)
		{
			if (book_buffer[j+1].book_price < book_buffer[j].book_price)
			{
				temp=book_buffer[j];
				book_buffer[j]=book_buffer[j+1];
				book_buffer[j+1]=temp;
			}
		}
	}

	printf("ID\t\t 名字\t\t 作者\t\t 简介\t\t 价格\t\t 库存量\t\t \n");
	
	for(i=0;i<books_count;i++)
	{
		printf("%-15d %-15s %-15s %-15s %-15.2f %-15d\n",book_buffer[i].book_id,book_buffer[i].book_name,book_buffer[i].book_author,book_buffer[i].book_introduce,book_buffer[i].book_price,book_buffer[i].book_storage_count);
	}
	system("pause");
	admin_book_info_look_up();
}

void get_price_sum()
{
	read_books_file();
	int i;
	float sum=0;
	
	printf("ID\t\t 名字\t\t 作者\t\t 简介\t\t 价格\t\t 库存量\t\t \n");

	for(i=0;i<books_count;i++)
	{
		printf("%-15d %-15s %-15s %-15s %-15.2f %-15d\n",book_buffer[i].book_id,book_buffer[i].book_name,book_buffer[i].book_author,book_buffer[i].book_introduce,book_buffer[i].book_price,book_buffer[i].book_storage_count);
	}

	for(i=0;i<books_count;i++)
	{
		sum=sum+(book_buffer[i].book_price*book_buffer[i].book_storage_count);
	}

	printf("当前图书总价为:%.2f\n",sum);
	system("pause");
	admin_book_info_look_up();
}


void admin_login(void)
{
	printf("\t\t\t尝试从硬盘读取管理员配置,请稍后......\n");
	printf("\t\t\t");
	read_admins_file();
	system("pause");
	fflush(stdin);
	
	char name[20];
	char password[20];
	char password_confirm[20];
	int i,j;
	int found_flag=0;
	int name_error_time=0;
	int password_error_time=0;


	system("cls");
	printf("#######################################################\n");
	printf("欢迎来到管理员登录向导\n\n");
	printf("#######################################################\n");

	while(1)
	{
		printf("输入管理员名字:");
		scanf("%s",name);

		for(j=0;j<admins_count;j++)
		{
			if (strcmp(name,admin_buffer[j].admin_name)==0)
			{
				found_flag=1;
				break;
			}
		}

		if (found_flag==0)
		{
			printf("错误 该管理员不存在\n");
			printf("任意按键重试!\n");
			system("pause");
			
			name_error_time++;
			if (name_error_time>=3)
			{
				printf("严重错误!!! 你已经 %d 次输错账户名\n",name_error_time);
				printf("拒绝登录!\n");
				exit(0);
			}
		}
		else
		{
			break;
		}
	}

	while(1)
	{
		printf("请输入你的密码(显示为星号):");
		input_secret(password);
		printf("请确认密码:");
		input_secret(password_confirm);

		if (strcmp(password,password_confirm)==0)
		{

			while(1)
			{
				if(strcmp(password_confirm,admin_buffer[j].admin_password)==0)
				{
					printf("登陆成功, 任意按键继续\n");
					system("pause");
					print_admin_menu();
					break;
				}
				else
				{
					password_error_time++;
					printf("账户 %s 对应的密码不正确\n",admin_buffer[j].admin_name);
					printf("请任意按键重新输入\n");
					system("pause");
					if (password_error_time>=3)
					{
						printf("严重错误!!! 你已经 %d 次输错密码!\n",password_error_time);
						printf("拒绝登录!\n");
						exit(0);
					}
					break;
				}
			}
			break;
		}
		else
		{
			printf("两次输入的密码不同! 任意按键重试\n");
			system("pause");
		}
	}
}


void user_login(void)
{
	printf("正在从磁盘读取用户配置文件, 请稍后.........!\n");
	read_users_file();
	printf("读取用户文件成功 任意按键继续\n"); 
	system("pause");
	fflush(stdin);
	char name[20];
	char password[20];
	char password_confirm[20];
	int i,j;
	int found_flag=0;
	int name_error_time=0;
	int password_error_time=0;
	bought_times_count=0;


	system("cls");
	printf("#######################################################\n");
	printf("欢迎来到用户登录向导\n");
	printf("#######################################################\n");

	while(1)
	{
		printf("请输入你的用户名:");
		scanf("%s",name);

		for(j=0;j<users_count;j++)
		{
			if (strcmp(name,user_buffer[j].user_name)==0)
			{
				found_flag=1;
				break;
			}
		}

		if (found_flag==0)
		{
			printf("错误! 你输入的用户名不存在\n");
			printf("任意按键继续!\n");
			system("pause");
			name_error_time++;
			if (name_error_time>=3)
			{
				printf("严重错误!!! 你已经 %d 次输错账户名\n",name_error_time);
				printf("拒绝登录!\n");
				exit(0);
			}
		}
		else
		{
			break;
		}
	}

	while(1)
	{
		printf("请输入密码(显示为星号):");
		input_secret(password);
		printf("请确认密码:");
		input_secret(password_confirm);

		if (strcmp(password,password_confirm)==0)
		{

			while(1)
			{
				if(strcmp(password_confirm,user_buffer[j].user_password)==0)
				{
					printf("登录成功任意按键继续\n");
					current_logined_user_in_buffer=j;
					is_user_logined=1;
					fflush(stdin);
					getchar();
					print_user_menu();
					break;
				}
				else
				{
					password_error_time++;
					printf("账号 %s 对应的密码错误\n",user_buffer[j].user_name);
					printf("请重试\n");
					if (password_error_time>=3)
					{
						printf("严重错误 你已经输错密码 %d 次!\n",password_error_time);
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


void getVIP()
{

	char password[20];
	char password_confirm[20];

	system("cls");
	printf("#######################################################\n");
	printf("欢迎申请图书馆VIP\n");
	printf("#######################################################\n");
	printf("为了安全,我们需要重新确认你的身份\n");



	while(1)
	{
		printf("请输入密码(将显示为星):");
		input_secret(password);
		printf("请确认密码:");
		input_secret(password_confirm);

		if (strcmp(password,password_confirm)==0)
		{

			while(1)
			{
				if(strcmp(password_confirm,user_buffer[current_logined_user_in_buffer].user_password)==0)
				{
					user_buffer[current_logined_user_in_buffer].isVIP=1;
					write_users_file();
					printf("成功申请VIP\n");
					system("pause");
					print_user_menu();
					break;
				}
				else
				{
					printf("账户名%s 对应的密码错误\n",user_buffer[current_logined_user_in_buffer].user_name);
					printf("申请VIP失败\n");
					print_user_menu();
					break;
				}
			}
			break;
		}
		else
		{
			printf("输入错误! 任意按键重新输入!\n");
		}
	}
}

void charge_for_balance(void)
{
	char password[20];
	char password_confirm[20];
	float charge;

	system("cls");
	printf("#######################################################\n");
	printf("欢迎进行图书馆用户账户充值\n");
	printf("#######################################################\n");
	printf("为了安全,我们需要重新确认你的身份\n");


	while(1)
	{
		printf("请输入密码(将显示为星):");
		input_secret(password);
		printf("请确认密码:");
		input_secret(password_confirm);

		if (strcmp(password,password_confirm)==0)
		{

			while(1)
			{
				if(strcmp(password_confirm,user_buffer[current_logined_user_in_buffer].user_password)==0)
				{
					while(1)
					{	
						printf("请输入你想要充值的金额:");
						scanf("%f",&charge);
						if(charge<0)
						{
							printf("金额不能小于0 请重新输入\n");
						}
						else
						{
							user_buffer[current_logined_user_in_buffer].balance=user_buffer[current_logined_user_in_buffer].balance+charge;
							break;
						}
					}	
					write_users_file();
					printf("充值成功 任意按键返回用户菜单\n");
					system("pause");
					print_user_menu();
					break;
				}
				else
				{
					printf("账户名%s 对应的密码错误\n",user_buffer[current_logined_user_in_buffer].user_name);
					printf("申请VIP失败\n");
					print_user_menu();
					break;
				}
			}
			break;
		}
		else
		{
			printf("输入错误! 任意按键重新输入!\n");
		}
	}
}

void buy_book_via_id(void)
{
	read_books_file();
	
	int i;
	int found_flag=0;
	int id;
	int count;

	printf("******************************************\n");
	printf("*****************购买图书*****************\n");
	printf("******************************************\n");
	
	while(1)
	{
		printf("请输入你想购买的书的ID:");
		scanf("%d",&id);

		for (i = 0; i < books_count; ++i)
		{
			if(book_buffer[i].book_id==id)
			{
				found_flag=1;
				break;
			}
		}

		if (found_flag==0)
		{
			printf("不存在ID对应的书\n");
			printf("任意按键重新输入ID\n");
			found_flag=0;
			system("pause");
		}
		
		else
		{
			break;
		}

	}
	
	while(1)
	{	
		printf("请输入你想购买的数量:");
		scanf("%d",&count);
		
		if(book_buffer[i].book_storage_count < count)
		{
			printf("对不起! 该书没有你想买的数量那么多! 去看看别的\n");
			system("pause");
			print_user_menu();
			break;
		}
		else if(book_buffer[i].book_price*count > user_buffer[current_logined_user_in_buffer].balance)
		{
			printf("不好意思 你的余额已经不够你买想买的书了 去看看别的吧\n");
			printf("你可以去用户菜单充值!\n");
			system("pause");
			print_user_menu();
			break;
		}
		else
		{	
			book_buffer[i].book_storage_count=book_buffer[i].book_storage_count-count;
			if(user_buffer[current_logined_user_in_buffer].isVIP==1)
			{
				printf("你是VIP 你拥有9折优惠!\n");
				user_buffer[current_logined_user_in_buffer].balance=user_buffer[current_logined_user_in_buffer].balance-book_buffer[i].book_price*count*0.9;
			}
			else
			{
				printf("你不是VIP 你没有9折优惠!\n");
				printf("你可以办理VIP会员!\n");
				user_buffer[current_logined_user_in_buffer].balance=user_buffer[current_logined_user_in_buffer].balance-book_buffer[i].book_price*count;
			}
			shop_list[bought_times_count].book_id=id;
			shop_list[bought_times_count].buy_count=count;
			strcpy(shop_list[bought_times_count].book_author,book_buffer[i].book_author);
			shop_list[bought_times_count].book_price=book_buffer[i].book_price;
			bought_times_count++;
			printf("购买成功!");
			break;
		}
	}
	
	printf("正在写入磁盘...........\n");
	write_books_file();
	write_users_file();
	printf("已经成功购买! 任意按键返回管理员菜单\n");
	system("pause");
	print_user_menu();
}

void show_shop_list(void)
{
	printf("******************************************\n");
	printf("***************购买清单*******************\n");
	printf("******************************************\n");
	printf("\n");
	
	int i;
	float total;
	
	printf("ID\t\t 作者\t\t 单价\t\t 购买数量\t\t 合计\t\n");
	
	for(i=0;i<bought_times_count;i++)
	{
	printf("%-15d %-15s %-15.2f %-15d %-15.2f\n",shop_list[i].book_id,shop_list[i].book_author,shop_list[i].book_price,shop_list[i].buy_count,shop_list[i].book_price*shop_list[i].buy_count);
	}
	
	system("pause");
	
	print_user_menu();
	
}

void admin_show_book_info()
{
	system("cls");

	printf("\t\t\t******************************************\n");
	printf("\t\t\t*************图书信息显示*****************\n");
	printf("\t\t\t******************************************\n");
	printf("\n");
	
	admin_show_all_books(book_first_node);
}

void admin_book_operate()
{
	int choice;

	system("cls");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t**********管理员图书操作员菜单************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t\t1.图书入库\n");
	printf("\t\t\t\t2.图书出库(按ID)\n");
	printf("\t\t\t\t3.修改对应ID图书库存\n");
	printf("\t\t\t\t4.修改对应ID图书信息\n");
	printf("\t\t\t\t5.返回管理员菜单\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t请输入你的选择:");
	fflush(stdin);
	scanf("%d",&choice);
	
	
	if (choice==1)
	{
		append_book();
	}
	else if (choice==2)
	{
		delete_book_by_id();
	}
	else if (choice==3)
	{
		edit_book_storage_by_id();
	}
	else if (choice==4)
	{
		edit_book_info_by_id();
	}
	else if (choice==5)
	{
		print_admin_menu();
	}
	else
	{
		printf("输入错误! 任意按键重新输入!!!");
		fflush(stdin);
		system("admin");
		admin_book_operate();
	}	
}

void admin_book_info_look_up()
{
	int choice;

	system("cls");

	printf("\t\t\t******************************************\n");
	printf("\t\t\t**********管理员查询员菜单****************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t\t1.按照图书单价排序显示\n");
	printf("\t\t\t\t2.计算所有图书总价\n");
	printf("\t\t\t\t3.按照ID查找图书简介\n");
	printf("\t\t\t\t4.返回管理员菜单\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t请输入你的选择:");
	fflush(stdin);
	scanf("%d",&choice);

	if (choice==1)
	{
		sub_sort_books_by_price();
	}
	else if (choice==2)
	{
		get_price_sum();
	}
	else if (choice==3)
	{
		admin_lookup_introduce_by_id();
	}
	else if (choice==4)
	{
		print_admin_menu();
	}
	else
	{
		printf("输入错误! 任意按键重新输入!!!");
		fflush(stdin);
		getchar();
		admin_book_info_look_up();
	}
}

void print_admin_menu(void)
{
	read_books_file();
	book_buffer_to_linklist();
	int choice;

	system("cls");

	printf("\t\t\t******************************************\n");
	printf("\t\t\t**********图书馆管理员菜单****************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t\t1.显示书籍信息\n");
	printf("\t\t\t\t2.图书操作\n");
	printf("\t\t\t\t3.图书查询\n");
	printf("\t\t\t\t4.返回系统登录\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t请输入你的选择:");
	fflush(stdin);
	scanf("%d",&choice);

	if (choice==1)
	{
		admin_show_book_info();
	}
	else if (choice==2)
	{
		admin_book_operate();
	}
	else if (choice==3)
	{
		admin_book_info_look_up();
	}
	else if (choice==4)
	{
		print_system_menu();
	}
	else
	{
		printf("输入错误! 任意按键重新输入!!!");
		fflush(stdin);
		getchar();
		print_admin_menu();
	}
}

void print_user_menu()
{
	read_users_file();
	read_books_file();
	book_buffer_to_linklist();
	int choice;

	system("cls");

	printf("\t\t\t******************************************\n");
	printf("\t\t\t************图书馆用户菜单****************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t\t1.显示所有书籍信息\n");
	printf("\t\t\t\t2.申请VIP\n");
	printf("\t\t\t\t3.账户充值\n");
	printf("\t\t\t\t4.购买指定ID的书籍\n");
	printf("\t\t\t\t5.查看本次购物清单\n");
	printf("\t\t\t\t6.退出系统\n");
	printf("\t\t\t\t7.返回登录\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t当前用户:%s 余额:%.2f 会员: %d\n",user_buffer[current_logined_user_in_buffer].user_name,user_buffer[current_logined_user_in_buffer].balance,user_buffer[current_logined_user_in_buffer].isVIP);
	printf("\t\t\t请输入你的选择:");
	fflush(stdin);
	scanf("%d",&choice);

	if (choice==1)
	{
		user_show_all_books(book_first_node);
	}
	else if (choice==2)
	{
		getVIP();
	}
	else if (choice==3)
	{
		charge_for_balance();
	}
	else if (choice==4)
	{
		buy_book_via_id();
	}
	else if (choice==5)
	{
		show_shop_list();
	}
	else if(choice==6)
	{
		system("cls");
		printf("谢谢您的光临 再见\n");
		exit(0);
	}
	else if(choice==7)
	{
		print_system_menu();
	}
	else
	{
		printf("输入错误! 任意按键重新输入!");
		fflush(stdin);
		getchar();
		print_user_menu();
	}
}

void system_config_login()
{
	printf("\t\t\t尝试从硬盘读取管理员配置,请稍后......\n");
	printf("\t\t\t");
	read_admins_file();
	system("pause");
	fflush(stdin);
	
	char name[20];
	char password[20];
	char password_confirm[20];
	int i,j;
	int found_flag=0;
	int name_error_time=0;
	int password_error_time=0;


	system("cls");
	printf("#######################################################\n");
	printf("欢迎来到管理员登录向导\n\n");
	printf("#######################################################\n");

	while(1)
	{
		printf("输入管理员名字:");
		scanf("%s",name);

		for(j=0;j<admins_count;j++)
		{
			if (strcmp(name,admin_buffer[j].admin_name)==0)
			{
				found_flag=1;
				break;
			}
		}

		if (found_flag==0)
		{
			printf("错误 该管理员不存在\n");
			printf("任意按键重试!\n");
			system("pause");
			
			name_error_time++;
			if (name_error_time>=3)
			{
				printf("严重错误!!! 你已经 %d 次输错账户名\n",name_error_time);
				printf("拒绝登录!\n");
				exit(0);
			}
		}
		else
		{
			break;
		}
	}

	while(1)
	{
		printf("请输入你的密码(显示为星号):");
		input_secret(password);
		printf("请确认密码:");
		input_secret(password_confirm);

		if (strcmp(password,password_confirm)==0)
		{

			while(1)
			{
				if(strcmp(password_confirm,admin_buffer[j].admin_password)==0)
				{
					printf("登陆成功, 任意按键继续进入系统设置\n");
					system("pause");
					print_system_config_menu();
					break;
				}
				else
				{
					password_error_time++;
					printf("账户 %s 对应的密码不正确\n",admin_buffer[j].admin_name);
					printf("请任意按键重新输入\n");
					system("pause");
					if (password_error_time>=3)
					{
						printf("严重错误!!! 你已经 %d 次输错密码!\n",password_error_time);
						printf("拒绝登录!\n");
						exit(0);
					}
					break;
				}
			}
			break;
		}
		else
		{
			printf("两次输入的密码不同! 任意按键重试\n");
			system("pause");
		}
	}
}

void print_admin_list()
{
	read_admins_file();
	int i;
	
	printf("名字\t\t密码\t\n");
	for(i=0;i<admins_count;i++)
	{
		printf("%-15s %-15s\n",admin_buffer[i].admin_name,admin_buffer[i].admin_password);
	}
	system("pause");
	print_system_config_menu();
}

void print_user_list()
{
	read_users_file();
	int i;
	
	printf("用户名\t\t密码\t\t余额\t\t是否VIP\t\n");
	for(i=0;i<users_count;i++)
	{
		printf("%-15s\t%-15s\t%-15.2f\t%-15d\n",user_buffer[i].user_name,user_buffer[i].user_password,user_buffer[i].balance,user_buffer[i].isVIP);
	}
	system("pause");
	print_system_config_menu();
}

void del_book_config()
{
	int choice;
	
	printf("\t\t\t******************************************\n");
	printf("\t\t\t*************书籍配置清除*****************\n");
	printf("\t\t\t******************************************\n");
	
	printf("输入1确定操作,任意按键返回菜单\n");
	
	printf("我的选择:");
	scanf("%d",&choice);
	
	if(choice==1)
	{
		system("del book.dat");
		printf("删除图书数据完成!!!!!!!!\n");
		system("pause");
		print_system_config_menu();
	}
	else
	{
		print_system_config_menu();
	}
}

void del_user_config()
{
	int choice;
	
	printf("\t\t\t******************************************\n");
	printf("\t\t\t*************用户配置清除*****************\n");
	printf("\t\t\t******************************************\n");
	
	printf("输入1确定操作,任意按键返回菜单\n");
	
	printf("我的选择:");
	scanf("%d",&choice);
	
	if(choice==1)
	{
		system("del user.dat");
		printf("删除用户数据完成!!!!!!!!\n");
		system("pause");
		print_system_config_menu();
	}
	else
	{
		print_system_config_menu();
	}
}

void new_user_regist()
{
	char name[20];
	char password[20];
	char password_confirm[20];
	
	int i,j;
	char choice[10];
	
	int override_flag=0;
	
	read_users_file();
	
	while(1)
	{
		system("cls");
		printf("#######################################################\n");
		printf("欢迎来到图书馆用户创建向导\n");
		printf("#######################################################\n");
		
		printf("该向导将帮助你创建一个普通用户账号 任意按键继续\n");
		system("pause");

		while(1)
		{
			printf("输入你的名字:");
			scanf("%s",name);

			for(j=0;j<users_count;j++)
			{
				if (strcmp(name,user_buffer[j].user_name)==0)
				{
					override_flag=1;
				}
			}

			if (override_flag==1)
			{
				printf("错误! 存在同名用户! \n");
				printf("任意按键重新输入用户名!\n");
				system("pause");
			}
			else
			{
				break;
			}
		}

		while(1)
		{
			printf("请输入密码(显示为星号):");
			input_secret(password);
			printf("请确认密码:");
			input_secret(password_confirm);
			if (strcmp(password,password_confirm)==0)
			{
				printf("成功! 新的用户已经添加\n");
				strcpy(user_buffer[users_count].user_name,name);
				strcpy(user_buffer[users_count].user_password,password);
				users_count++;
				break;
			}
			else
			{
				printf("两次输入的密码不同 请重试!\n");
			}
		}
		break;
	}
	printf("正在写入磁盘.............\n");
	write_users_file();
	printf("用户数据写入磁盘成功........任意按键返回系统登录菜单\n");
	system("pause");
	
	
	print_system_menu();
}

void print_system_config_menu()
{
	int choice;

	system("cls");

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
	fflush(stdin);
	scanf("%d",&choice);

	if (choice==1)
	{
		print_admin_list();
	}
	else if (choice==2)
	{
		create_admin_wizard();
	}
	else if (choice==3)
	{
		system("cls");
		printf("警告! 这将丢失所有的书籍信息配置文件包括已经保存的书籍!");
		system("pause");
		del_book_config();
	}
	else if (choice==4)
	{
		system("cls");
		printf("警告! 这将丢失所有的用户信息配置文件包括已经保存的书籍!");
		system("pause");
		del_user_config();
	}
	else if (choice==5)
	{
		print_user_list();
	}
	else if(choice==6)
	{
		delete_user_by_name();
	}
	else if(choice==7)
	{
		print_system_menu();
	}
	else
	{
		printf("输入错误! 任意按键重新输入!");
		fflush(stdin);
		getchar();
		print_system_config_menu();
	}
}

void print_system_menu(void)
{
	int choice;

	system("cls");

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
	scanf("%d",&choice);
	if (choice==1)
	{
		admin_login();
	}
	else if (choice==2)
	{
		user_login();
	}
	else if (choice==3)
	{
		system("cls");
		printf("你将访问系统设置菜单! 为了安全 需要验证你的管理员身份!\n");
		system("pause");
		system_config_login();
	}
	else if (choice==4)
	{
		new_user_regist();
	}
	else if (choice==5)
	{
		system("cls");
		printf("拜拜!\n");
		exit(0);
	}
	else
	{
		printf("输入错误! 任意按键重新输入!");
		fflush(stdin);
		getchar();
		print_system_menu();
	}
}

int main(int argc, char const *argv[])
{
	print_system_menu();
}
