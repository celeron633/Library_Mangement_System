#ifndef __COMMON_H__
#define __COMMON_H__

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
} admin;

typedef struct user
{
    char user_name[20];
    char user_password[20];
    float balance;
    int isVIP;
} user;

typedef struct book
{
    int book_id;
    char book_name[48];
    char book_author[48];
    char book_introduce[100];
    float book_price;
    int book_storage_count;
} book;

typedef struct shoplist
{
    int book_id;
    char book_author[48];
    float book_price;
    int buy_count;
} shoplist;

typedef struct books
{
    book data;
    struct books *next;
} books;

typedef struct admins
{
    admin data;
    struct admins *next;
} admins;

typedef struct users
{
    user data;
    struct users *next;
} users;

#endif