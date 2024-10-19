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
	printf("�û������������ɹ�!\n");
}

void linklist_to_user_buffer(users* u)
{
	int count;
	int i=0;


	if (user_first_node==NULL)
	{
		printf("�û���������û�г�ʼ��!\n");
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
		printf("�û���������ɹ����Ƶ���������\n");
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
	printf("�鱾�����������ɹ�!\n");
}

void linklist_to_book_buffer(books* u)
{
	int count;
	int i=0;


	if (book_first_node==NULL)
	{
		printf("����! �鱾��������û�г�ʼ��\n");
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
		printf("�鱾���������Ѿ����Ƶ���������\n");
	}
}


void read_admins_file()
{
	FILE *fp;
	int i;

	fp=fopen("./admin.dat","r");
	if (fp==NULL)
	{
		printf("����Ա���������ļ��ڵ�ǰĿ¼�²�����!\n");
		printf("���ⰴ�������򵼴�������Ա!");
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
		printf("�ɹ��Ӵ��̶�ȡ����Ա�����ļ�!\n");
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
		printf("����һ��δ֪�Ķ�д����! ����Ա�����ļ�����ʧ��\n");
		return;
	}
	else
	{
		fprintf(fp,"%d\n",admins_count);

		for(i=0;i<admins_count;i++)
		{
			fprintf(fp,"%s %s\n",admin_buffer[i].admin_name,admin_buffer[i].admin_password);
		}
		printf("�ɹ�������Ա����д�����!\n");
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
		printf("�û������ļ��ڵ�ǰĿ¼��û���ҵ�!\n");
		printf("����û�м����κ��û�����\n");
		printf("���ⰴ������ϵͳ��¼�˵�");
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
		printf("�ɹ��Ӵ��̶�ȡ�û������ļ�\n");
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
		printf("����һ��δ֪�Ķ�д���� �û������ļ�����ʧ��!");
	}
	else
	{
		fprintf(fp,"%d\n",users_count);

		for(i=0;i<users_count;i++)
		{
			fprintf(fp,"%s %s %f %d\n",user_buffer[i].user_name,user_buffer[i].user_password,user_buffer[i].balance,user_buffer[i].isVIP);
		}
		printf("�ɹ����û����û���д�����!\n");
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
		printf("�ڵ�ǰĿ¼��û���ҵ�ͼ����Ϣ�����ļ�\n");
		printf("����û�м����κ�ͼ����Ϣ\n");
		printf("���ⰴ������ϵͳ��¼�˵�\n");
		getchar();
	}
	else
	{
		fscanf(fp,"%d\n",&books_count);

		for(i=0;i<books_count;i++)
		{
			fscanf(fp,"%d%s%s%s%f%d",&book_buffer[i].book_id,book_buffer[i].book_name,book_buffer[i].book_author,book_buffer[i].book_introduce,&book_buffer[i].book_price,&book_buffer[i].book_storage_count);
		}
		printf("�ɹ��Ӵ��̶�ȡ�鼮�����ļ�!\n");
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
		printf("����һ��δ֪�Ķ�д���� �鼮�����ļ�����ʧ��!");
	}
	else
	{
		fprintf(fp,"%d\n",books_count);

		for(i=0;i<books_count;i++)
		{
			fprintf(fp,"%d %s %s %s %f %d\n",book_buffer[i].book_id,book_buffer[i].book_name,book_buffer[i].book_author,book_buffer[i].book_introduce,book_buffer[i].book_price,book_buffer[i].book_storage_count);
		}
		printf("�ɹ����鼮���ϻ���д�����!\n");
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
		printf("��ӭ��������Ա������\n");
		printf("#######################################################\n");

		while(1)
		{
			printf("�����������:");
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
				printf("����! ����ͬ������Ա! \n");
				printf("���ⰴ����������!\n");
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
			printf("����������(��ʾΪ�Ǻ�):");
			input_secret(password);
			printf("��ȷ������:");
			input_secret(password_confirm);
			if (strcmp(password,password_confirm)==0)
			{
				printf("�ɹ�! �µĹ���Ա�Ѿ����\n");
				strcpy(admin_buffer[admins_count].admin_name,name);
				strcpy(admin_buffer[admins_count].admin_password,password);
				admins_count++;
				break;
			}
			else
			{
				printf("������������벻ͬ ������!\n");
			}
		}
		printf("������������Ա?(y/n):");
		fflush(stdin);
		scanf("%s",choice);
		if (strcmp(choice,"n")==0 || strcmp(choice,"no")==0 || strcmp(choice,"0")==0)
		{
			break;
		}
	}
	printf("����д�����.............\n");
	write_admins_file(admin_buffer);
	printf("д����̳ɹ�........���ⰴ������\n");
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
		printf("��ӭ��������Ա�����\n\n");
		printf("#######################################################\n");

		while(1)
		{
			printf("�������:");
			
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
				printf("���� ����ͬ������Ա\n");
				printf("������ʹ�ñ������!\n");
			}
			else
			{
				break;
			}
		}

		while(1)
		{
			printf("����������(��ʾΪ�Ǻ�):");
			input_secret(password);
			printf("��ȷ������:");
			input_secret(password_confirm);
			if (strcmp(password,password_confirm)==0)
			{
				printf("�õ�! �ɹ�����µĹ���Ա\n");
				strcpy(admin_buffer[admins_count].admin_name,name);
				strcpy(admin_buffer[admins_count].admin_password,password);
				admins_count++;
				break;
			}
			else
			{
				printf("������������벻ͬ ������\n");
			}
		}
		printf("������ӹ���Ա?(y/n):");
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
		printf("û���κ�ͼ��,���ⰴ�����ع���Ա�˵�!");
		system("pause");
		print_admin_menu();
	}
	else
	{
		printf("ID\t\t ����\t\t ����\t\t ���\t\t �۸�\t\t �����\t\t \n");
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
		printf("û���κ�ͼ�� ���ⰴ�������û��˵�!");
		system("pause");
		print_user_menu();
	}
	else
	{
		printf("ID\t\t ����\t\t ����\t\t ���\t\t �۸�\t\t �����\t\t \n");
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
	printf("***********���������*****************\n");
	printf("******************************************\n");

	printf("�������Ȿ���ID:");
	scanf("%d",&book_buffer[books_count].book_id);
	
	while(1)
	{
		printf("����������:");
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
			printf("������ͬ����\n");
			printf("���ⰴ��������������\n");
			override_flag=0;
			system("pause");
		}
		else
		{
			break;
		}

	}

	printf("�����������������:");
	scanf("%s",book_buffer[books_count].book_author);
	printf("��������ļ��(��Ҫ�пո�):");
	scanf("%s",book_buffer[books_count].book_introduce);
	printf("��������ļ۸�:");
	scanf("%f",&book_buffer[books_count].book_price);
	printf("��������Ŀ��:");
	scanf("%d",&book_buffer[books_count].book_storage_count);
	books_count++;
	printf("����д�����...........\n");
	write_books_file();
	book_buffer_to_linklist();
	printf("��ӳɹ�! ���ⰴ�����ع���Ա�˵�\n");
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
		printf("������Ҫɾ�������ID:");
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
			printf("ID��Ӧ���鲻���� ���ⰴ������!");
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
	printf("ɾ���ɹ�!\n");
	printf("����д�����...............\n");
	linklist_to_book_buffer(book_first_node);
	write_books_file();
	printf("д��ɹ� ���ⰴ�����ع���Ա�˵�");
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
	printf("*************�����û�ɾ��*****************\n");
	printf("******************************************\n");
	
	while(1)
	{
		printf("������Ҫɾ�����û�������:");
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
			printf("������û������� ���ⰴ������!");
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
	
	printf("ɾ���ɹ�!\n");
	printf("����д�����...............\n");
	linklist_to_user_buffer(user_first_node);
	write_users_file();
	printf("д��ɹ� ���ⰴ������ϵͳ���ò˵�");
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
		printf("���������޸Ŀ���ͼ���ID:");
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
			printf("ID��Ӧ���鲻����!");
			found_flag=0;
			printf("���ⰴ����������");
			system("pause");
		}

	}

	printf("�����Ȿ�鵱ǰ�Ŀ����:");
	scanf("%d",&count);
	book_buffer[i].book_storage_count=count;
	book_buffer_to_linklist(book_first_node);
	printf("д�����...........");
	write_books_file();
	printf("���! ���ⰴ�����ع���Ա�˵�\n");
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
		printf("���������޸���Ϣ��ͼ���ID:");
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
			printf("ID��Ӧ���鲻����! ���ⰴ������! ");
			found_flag=0;
			system("pause");
		}

	}

	printf("�����Ȿ�����ID:");
	scanf("%d",&book_buffer[i].book_id);
	printf("�����Ȿ���������:");
	scanf("%s",book_buffer[i].book_name);
	printf("�����������������:");
	scanf("%s",book_buffer[i].book_author);
	printf("��������ļ��:");
	scanf("%s",book_buffer[i].book_introduce);
	printf("��������ļ۸�:");
	scanf("%f",&book_buffer[i].book_price);
	printf("��������Ŀ����:");
	scanf("%d",&book_buffer[i].book_storage_count);

	book_buffer_to_linklist(book_first_node);
	write_books_file();
	printf("д�����...........");
	write_books_file();
	printf("���! ���ⰴ�����ع���Ա�˵�\n");
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
		printf("�����ѯ����ͼ���ID:");
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
			printf("ID��Ӧ���鲻����! ���ⰴ������! ");
			found_flag=0;
			system("pause");
		}
	}
	printf("##################ͼ����#####################\n");
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

	printf("ID\t\t ����\t\t ����\t\t ���\t\t �۸�\t\t �����\t\t \n");
	
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
	
	printf("ID\t\t ����\t\t ����\t\t ���\t\t �۸�\t\t �����\t\t \n");

	for(i=0;i<books_count;i++)
	{
		printf("%-15d %-15s %-15s %-15s %-15.2f %-15d\n",book_buffer[i].book_id,book_buffer[i].book_name,book_buffer[i].book_author,book_buffer[i].book_introduce,book_buffer[i].book_price,book_buffer[i].book_storage_count);
	}

	for(i=0;i<books_count;i++)
	{
		sum=sum+(book_buffer[i].book_price*book_buffer[i].book_storage_count);
	}

	printf("��ǰͼ���ܼ�Ϊ:%.2f\n",sum);
	system("pause");
	admin_book_info_look_up();
}


void admin_login(void)
{
	printf("\t\t\t���Դ�Ӳ�̶�ȡ����Ա����,���Ժ�......\n");
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
	printf("��ӭ��������Ա��¼��\n\n");
	printf("#######################################################\n");

	while(1)
	{
		printf("�������Ա����:");
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
			printf("���� �ù���Ա������\n");
			printf("���ⰴ������!\n");
			system("pause");
			
			name_error_time++;
			if (name_error_time>=3)
			{
				printf("���ش���!!! ���Ѿ� %d ������˻���\n",name_error_time);
				printf("�ܾ���¼!\n");
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
		printf("�������������(��ʾΪ�Ǻ�):");
		input_secret(password);
		printf("��ȷ������:");
		input_secret(password_confirm);

		if (strcmp(password,password_confirm)==0)
		{

			while(1)
			{
				if(strcmp(password_confirm,admin_buffer[j].admin_password)==0)
				{
					printf("��½�ɹ�, ���ⰴ������\n");
					system("pause");
					print_admin_menu();
					break;
				}
				else
				{
					password_error_time++;
					printf("�˻� %s ��Ӧ�����벻��ȷ\n",admin_buffer[j].admin_name);
					printf("�����ⰴ����������\n");
					system("pause");
					if (password_error_time>=3)
					{
						printf("���ش���!!! ���Ѿ� %d ���������!\n",password_error_time);
						printf("�ܾ���¼!\n");
						exit(0);
					}
					break;
				}
			}
			break;
		}
		else
		{
			printf("������������벻ͬ! ���ⰴ������\n");
			system("pause");
		}
	}
}


void user_login(void)
{
	printf("���ڴӴ��̶�ȡ�û������ļ�, ���Ժ�.........!\n");
	read_users_file();
	printf("��ȡ�û��ļ��ɹ� ���ⰴ������\n"); 
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
	printf("��ӭ�����û���¼��\n");
	printf("#######################################################\n");

	while(1)
	{
		printf("����������û���:");
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
			printf("����! ��������û���������\n");
			printf("���ⰴ������!\n");
			system("pause");
			name_error_time++;
			if (name_error_time>=3)
			{
				printf("���ش���!!! ���Ѿ� %d ������˻���\n",name_error_time);
				printf("�ܾ���¼!\n");
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
		printf("����������(��ʾΪ�Ǻ�):");
		input_secret(password);
		printf("��ȷ������:");
		input_secret(password_confirm);

		if (strcmp(password,password_confirm)==0)
		{

			while(1)
			{
				if(strcmp(password_confirm,user_buffer[j].user_password)==0)
				{
					printf("��¼�ɹ����ⰴ������\n");
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
					printf("�˺� %s ��Ӧ���������\n",user_buffer[j].user_name);
					printf("������\n");
					if (password_error_time>=3)
					{
						printf("���ش��� ���Ѿ�������� %d ��!\n",password_error_time);
						printf("�ܾ�����!\n");
						exit(0);
					}
					break; 
				}
			}
		}
		else
		{
			printf("�������! ���ⰴ����������!!\n");
		}
	}
}


void getVIP()
{

	char password[20];
	char password_confirm[20];

	system("cls");
	printf("#######################################################\n");
	printf("��ӭ����ͼ���VIP\n");
	printf("#######################################################\n");
	printf("Ϊ�˰�ȫ,������Ҫ����ȷ��������\n");



	while(1)
	{
		printf("����������(����ʾΪ��):");
		input_secret(password);
		printf("��ȷ������:");
		input_secret(password_confirm);

		if (strcmp(password,password_confirm)==0)
		{

			while(1)
			{
				if(strcmp(password_confirm,user_buffer[current_logined_user_in_buffer].user_password)==0)
				{
					user_buffer[current_logined_user_in_buffer].isVIP=1;
					write_users_file();
					printf("�ɹ�����VIP\n");
					system("pause");
					print_user_menu();
					break;
				}
				else
				{
					printf("�˻���%s ��Ӧ���������\n",user_buffer[current_logined_user_in_buffer].user_name);
					printf("����VIPʧ��\n");
					print_user_menu();
					break;
				}
			}
			break;
		}
		else
		{
			printf("�������! ���ⰴ����������!\n");
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
	printf("��ӭ����ͼ����û��˻���ֵ\n");
	printf("#######################################################\n");
	printf("Ϊ�˰�ȫ,������Ҫ����ȷ��������\n");


	while(1)
	{
		printf("����������(����ʾΪ��):");
		input_secret(password);
		printf("��ȷ������:");
		input_secret(password_confirm);

		if (strcmp(password,password_confirm)==0)
		{

			while(1)
			{
				if(strcmp(password_confirm,user_buffer[current_logined_user_in_buffer].user_password)==0)
				{
					while(1)
					{	
						printf("����������Ҫ��ֵ�Ľ��:");
						scanf("%f",&charge);
						if(charge<0)
						{
							printf("����С��0 ����������\n");
						}
						else
						{
							user_buffer[current_logined_user_in_buffer].balance=user_buffer[current_logined_user_in_buffer].balance+charge;
							break;
						}
					}	
					write_users_file();
					printf("��ֵ�ɹ� ���ⰴ�������û��˵�\n");
					system("pause");
					print_user_menu();
					break;
				}
				else
				{
					printf("�˻���%s ��Ӧ���������\n",user_buffer[current_logined_user_in_buffer].user_name);
					printf("����VIPʧ��\n");
					print_user_menu();
					break;
				}
			}
			break;
		}
		else
		{
			printf("�������! ���ⰴ����������!\n");
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
	printf("*****************����ͼ��*****************\n");
	printf("******************************************\n");
	
	while(1)
	{
		printf("���������빺������ID:");
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
			printf("������ID��Ӧ����\n");
			printf("���ⰴ����������ID\n");
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
		printf("���������빺�������:");
		scanf("%d",&count);
		
		if(book_buffer[i].book_storage_count < count)
		{
			printf("�Բ���! ����û���������������ô��! ȥ�������\n");
			system("pause");
			print_user_menu();
			break;
		}
		else if(book_buffer[i].book_price*count > user_buffer[current_logined_user_in_buffer].balance)
		{
			printf("������˼ �������Ѿ������������������ ȥ������İ�\n");
			printf("�����ȥ�û��˵���ֵ!\n");
			system("pause");
			print_user_menu();
			break;
		}
		else
		{	
			book_buffer[i].book_storage_count=book_buffer[i].book_storage_count-count;
			if(user_buffer[current_logined_user_in_buffer].isVIP==1)
			{
				printf("����VIP ��ӵ��9���Ż�!\n");
				user_buffer[current_logined_user_in_buffer].balance=user_buffer[current_logined_user_in_buffer].balance-book_buffer[i].book_price*count*0.9;
			}
			else
			{
				printf("�㲻��VIP ��û��9���Ż�!\n");
				printf("����԰���VIP��Ա!\n");
				user_buffer[current_logined_user_in_buffer].balance=user_buffer[current_logined_user_in_buffer].balance-book_buffer[i].book_price*count;
			}
			shop_list[bought_times_count].book_id=id;
			shop_list[bought_times_count].buy_count=count;
			strcpy(shop_list[bought_times_count].book_author,book_buffer[i].book_author);
			shop_list[bought_times_count].book_price=book_buffer[i].book_price;
			bought_times_count++;
			printf("����ɹ�!");
			break;
		}
	}
	
	printf("����д�����...........\n");
	write_books_file();
	write_users_file();
	printf("�Ѿ��ɹ�����! ���ⰴ�����ع���Ա�˵�\n");
	system("pause");
	print_user_menu();
}

void show_shop_list(void)
{
	printf("******************************************\n");
	printf("***************�����嵥*******************\n");
	printf("******************************************\n");
	printf("\n");
	
	int i;
	float total;
	
	printf("ID\t\t ����\t\t ����\t\t ��������\t\t �ϼ�\t\n");
	
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
	printf("\t\t\t*************ͼ����Ϣ��ʾ*****************\n");
	printf("\t\t\t******************************************\n");
	printf("\n");
	
	admin_show_all_books(book_first_node);
}

void admin_book_operate()
{
	int choice;

	system("cls");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t**********����Աͼ�����Ա�˵�************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t\t1.ͼ�����\n");
	printf("\t\t\t\t2.ͼ�����(��ID)\n");
	printf("\t\t\t\t3.�޸Ķ�ӦIDͼ����\n");
	printf("\t\t\t\t4.�޸Ķ�ӦIDͼ����Ϣ\n");
	printf("\t\t\t\t5.���ع���Ա�˵�\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t���������ѡ��:");
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
		printf("�������! ���ⰴ����������!!!");
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
	printf("\t\t\t**********����Ա��ѯԱ�˵�****************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t\t1.����ͼ�鵥��������ʾ\n");
	printf("\t\t\t\t2.��������ͼ���ܼ�\n");
	printf("\t\t\t\t3.����ID����ͼ����\n");
	printf("\t\t\t\t4.���ع���Ա�˵�\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t���������ѡ��:");
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
		printf("�������! ���ⰴ����������!!!");
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
	printf("\t\t\t**********ͼ��ݹ���Ա�˵�****************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t\t1.��ʾ�鼮��Ϣ\n");
	printf("\t\t\t\t2.ͼ�����\n");
	printf("\t\t\t\t3.ͼ���ѯ\n");
	printf("\t\t\t\t4.����ϵͳ��¼\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t���������ѡ��:");
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
		printf("�������! ���ⰴ����������!!!");
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
	printf("\t\t\t************ͼ����û��˵�****************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t\t1.��ʾ�����鼮��Ϣ\n");
	printf("\t\t\t\t2.����VIP\n");
	printf("\t\t\t\t3.�˻���ֵ\n");
	printf("\t\t\t\t4.����ָ��ID���鼮\n");
	printf("\t\t\t\t5.�鿴���ι����嵥\n");
	printf("\t\t\t\t6.�˳�ϵͳ\n");
	printf("\t\t\t\t7.���ص�¼\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t��ǰ�û�:%s ���:%.2f ��Ա: %d\n",user_buffer[current_logined_user_in_buffer].user_name,user_buffer[current_logined_user_in_buffer].balance,user_buffer[current_logined_user_in_buffer].isVIP);
	printf("\t\t\t���������ѡ��:");
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
		printf("лл���Ĺ��� �ټ�\n");
		exit(0);
	}
	else if(choice==7)
	{
		print_system_menu();
	}
	else
	{
		printf("�������! ���ⰴ����������!");
		fflush(stdin);
		getchar();
		print_user_menu();
	}
}

void system_config_login()
{
	printf("\t\t\t���Դ�Ӳ�̶�ȡ����Ա����,���Ժ�......\n");
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
	printf("��ӭ��������Ա��¼��\n\n");
	printf("#######################################################\n");

	while(1)
	{
		printf("�������Ա����:");
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
			printf("���� �ù���Ա������\n");
			printf("���ⰴ������!\n");
			system("pause");
			
			name_error_time++;
			if (name_error_time>=3)
			{
				printf("���ش���!!! ���Ѿ� %d ������˻���\n",name_error_time);
				printf("�ܾ���¼!\n");
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
		printf("�������������(��ʾΪ�Ǻ�):");
		input_secret(password);
		printf("��ȷ������:");
		input_secret(password_confirm);

		if (strcmp(password,password_confirm)==0)
		{

			while(1)
			{
				if(strcmp(password_confirm,admin_buffer[j].admin_password)==0)
				{
					printf("��½�ɹ�, ���ⰴ����������ϵͳ����\n");
					system("pause");
					print_system_config_menu();
					break;
				}
				else
				{
					password_error_time++;
					printf("�˻� %s ��Ӧ�����벻��ȷ\n",admin_buffer[j].admin_name);
					printf("�����ⰴ����������\n");
					system("pause");
					if (password_error_time>=3)
					{
						printf("���ش���!!! ���Ѿ� %d ���������!\n",password_error_time);
						printf("�ܾ���¼!\n");
						exit(0);
					}
					break;
				}
			}
			break;
		}
		else
		{
			printf("������������벻ͬ! ���ⰴ������\n");
			system("pause");
		}
	}
}

void print_admin_list()
{
	read_admins_file();
	int i;
	
	printf("����\t\t����\t\n");
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
	
	printf("�û���\t\t����\t\t���\t\t�Ƿ�VIP\t\n");
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
	printf("\t\t\t*************�鼮�������*****************\n");
	printf("\t\t\t******************************************\n");
	
	printf("����1ȷ������,���ⰴ�����ز˵�\n");
	
	printf("�ҵ�ѡ��:");
	scanf("%d",&choice);
	
	if(choice==1)
	{
		system("del book.dat");
		printf("ɾ��ͼ���������!!!!!!!!\n");
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
	printf("\t\t\t*************�û��������*****************\n");
	printf("\t\t\t******************************************\n");
	
	printf("����1ȷ������,���ⰴ�����ز˵�\n");
	
	printf("�ҵ�ѡ��:");
	scanf("%d",&choice);
	
	if(choice==1)
	{
		system("del user.dat");
		printf("ɾ���û��������!!!!!!!!\n");
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
		printf("��ӭ����ͼ����û�������\n");
		printf("#######################################################\n");
		
		printf("���򵼽������㴴��һ����ͨ�û��˺� ���ⰴ������\n");
		system("pause");

		while(1)
		{
			printf("�����������:");
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
				printf("����! ����ͬ���û�! \n");
				printf("���ⰴ�����������û���!\n");
				system("pause");
			}
			else
			{
				break;
			}
		}

		while(1)
		{
			printf("����������(��ʾΪ�Ǻ�):");
			input_secret(password);
			printf("��ȷ������:");
			input_secret(password_confirm);
			if (strcmp(password,password_confirm)==0)
			{
				printf("�ɹ�! �µ��û��Ѿ����\n");
				strcpy(user_buffer[users_count].user_name,name);
				strcpy(user_buffer[users_count].user_password,password);
				users_count++;
				break;
			}
			else
			{
				printf("������������벻ͬ ������!\n");
			}
		}
		break;
	}
	printf("����д�����.............\n");
	write_users_file();
	printf("�û�����д����̳ɹ�........���ⰴ������ϵͳ��¼�˵�\n");
	system("pause");
	
	
	print_system_menu();
}

void print_system_config_menu()
{
	int choice;

	system("cls");

	printf("\t\t\t******************************************\n");
	printf("\t\t\t*************ϵͳ����*********************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t\t1.��ʾ��ǰ�Ĺ���Ա�б�\n");
	printf("\t\t\t\t2.�½�����Ա\n");
	printf("\t\t\t\t3.����鼮��Ϣ�ļ�\n");
	printf("\t\t\t\t4.����û���Ϣ�ļ�\n");
	printf("\t\t\t\t5.��ʾ��ǰ�����û�\n");
	printf("\t\t\t\t6.ɾ��ָ�������û�\n");
	printf("\t\t\t\t7.���ص�¼\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t���������ѡ��:");
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
		printf("����! �⽫��ʧ���е��鼮��Ϣ�����ļ������Ѿ�������鼮!");
		system("pause");
		del_book_config();
	}
	else if (choice==4)
	{
		system("cls");
		printf("����! �⽫��ʧ���е��û���Ϣ�����ļ������Ѿ�������鼮!");
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
		printf("�������! ���ⰴ����������!");
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
	printf("\t\t\t**************ͼ���ϵͳ��¼**************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t\t1.����Ա��¼\n");
	printf("\t\t\t\t2.�û���¼\n");
	printf("\t\t\t\t3.ͼ���ϵͳ����\n");
	printf("\t\t\t\t4.���û�ע��\n");
	printf("\t\t\t\t5.�˳�ϵͳ\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t******************************************\n");
	printf("\t\t\t���������ѡ��:");
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
		printf("�㽫����ϵͳ���ò˵�! Ϊ�˰�ȫ ��Ҫ��֤��Ĺ���Ա���!\n");
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
		printf("�ݰ�!\n");
		exit(0);
	}
	else
	{
		printf("�������! ���ⰴ����������!");
		fflush(stdin);
		getchar();
		print_system_menu();
	}
}

int main(int argc, char const *argv[])
{
	print_system_menu();
}
