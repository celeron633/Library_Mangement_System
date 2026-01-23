#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include <fstream>

#include "utils.h"

using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::ifstream;

struct AdminInfo
{
	string _adminName;
	string _adminPassword;
};
vector<AdminInfo> g_adminVec;

struct UserInfo
{
	string 	_userName;
	string 	_userPassword;
	double 	_balance;
	bool 	_isVIP;

	UserInfo()
	{
		_isVIP = false;
		_balance = 0;
	}
};
vector<UserInfo> g_userVec;
UserInfo *g_pCurrentLoginedUser = nullptr;

struct BookInfo
{
	string 	_bookId;
	string 	_bookName;
	string 	_bookAuthor;
	string 	_bookIntroduce;
	double 	_bookPrice;
	int 	_bookCount;

	BookInfo()
	{
		_bookPrice = 0.0;
		_bookCount = 0;	
	}
};
vector<BookInfo> g_bookVec;

struct ShopList
{
	BookInfo 	_bookInfo;
	int 		_buyCount;

	ShopList()
	{
		_buyCount = 0;
	}
};
vector<ShopList> g_shopListVec;

bool g_isUserLogined = false;
bool g_isAdminLogined = false;

void createAdminMenu(void);
void printAdminMenu(void);
void printSystemMenu(void);
void printSystemConfigMenu();
void printUserMenu(void);

void adminBookInfoLookup();

void systemAdminLogin();
void printSystemConfigMenu();


void readAdminDataFile()
{
	FILE *fp = NULL;
	int i = 0;

	ifstream adminFileStream;
	adminFileStream.open("admin.dat", std::ios::in);
	if (!adminFileStream.is_open())
	{
		printf("管理员配置数据文件在当前目录下不存在! 任意按键启动向导创建管理员!\n");
		getchar();
		createAdminMenu();
	}
	else
	{
		string line;
		while (getline(adminFileStream, line))
		{
			AdminInfo admin;
			size_t pos = line.find(' ');
			admin._adminName = line.substr(0, pos);
			admin._adminPassword = line.substr(pos + 1);
			g_adminVec.push_back(admin);
		}
		printf("成功从磁盘读取管理员配置文件!\n");
		adminFileStream.close();
	}
}

void writeAdminDataFile()
{
	std::ofstream adminFileStream;
	adminFileStream.open("admin.dat", std::ios::out | std::ios::trunc);
	if (!adminFileStream.is_open())
	{
		printf("发生一个未知的读写错误! 管理员配置文件[%s]创建失败\n", "admin.dat");
		exit(1);
	}
	else
	{
		for (const auto& admin : g_adminVec)
		{
			adminFileStream << admin._adminName << " " << admin._adminPassword << "\n";
		}
		printf("成功将管理员缓存写入磁盘!\n");
		adminFileStream.close();
	}
}

void readUserDataFile()
{
	ifstream userFileStream;
	userFileStream.open("user.dat", std::ios::in);
	if (!userFileStream.is_open())
	{
		printf("用户配置数据文件在当前目录下不存在! 没有加载任何用户数据! 任意按键返回系统登录菜单!\n");
		getchar();
		printSystemMenu();
	}
	else
	{
		string line;
		while (getline(userFileStream, line))
		{
			UserInfo user;
			size_t pos1 = line.find(' ');
			size_t pos2 = line.find(' ', pos1 + 1);
			size_t pos3 = line.find(' ', pos2 + 1);
			user._userName = line.substr(0, pos1);
			user._userPassword = line.substr(pos1 + 1, pos2 - pos1 - 1);
			user._balance = std::stod(line.substr(pos2 + 1, pos3 - pos2 - 1));
			user._isVIP = (line.substr(pos3 + 1) == "1");
			g_userVec.push_back(user);
		}
		printf("成功从磁盘读取用户配置文件!\n");
		userFileStream.close();
	}
}


void writeUserDataFile()
{
	std::ofstream userFileStream;
	userFileStream.open("user.dat", std::ios::out | std::ios::trunc);
	if (!userFileStream.is_open())
	{
		printf("发生一个未知的读写错误! 用户配置文件[%s]创建失败\n", "user.dat");
		exit(1);
	}
	else
	{
		for (const auto& user : g_userVec)
		{
			userFileStream << user._userName << " " << user._userPassword << " " << user._balance << " " << (user._isVIP ? "1" : "0") << "\n";
		}
		printf("成功将用户缓存写入磁盘!\n");
		userFileStream.close();
	}
}

void readBookDataFile()
{
	g_bookVec.clear();

	ifstream bookFileStream;
	bookFileStream.open("book.dat", std::ios::in);
	if (!bookFileStream.is_open())
	{
		printf("图书配置数据文件在当前目录下不存在! 没有加载任何图书数据! 任意按键返回系统登录菜单!\n");
		getchar();
		printSystemMenu();
	}
	else
	{
		string line;
		while (getline(bookFileStream, line))
		{
			BookInfo book;
			size_t pos1 = line.find(' ');
			size_t pos2 = line.find(' ', pos1 + 1);
			size_t pos3 = line.find(' ', pos2 + 1);
			size_t pos4 = line.find(' ', pos3 + 1);
			size_t pos5 = line.find(' ', pos4 + 1);
			book._bookId = line.substr(0, pos1);
			book._bookName = line.substr(pos1 + 1, pos2 - pos1 - 1);
			book._bookAuthor = line.substr(pos2 + 1, pos3 - pos2 - 1);
			book._bookIntroduce = line.substr(pos3 + 1, pos4 - pos3 - 1);
			book._bookPrice = std::stod(line.substr(pos4 + 1, pos5 - pos4 - 1));
			book._bookCount = std::stoi(line.substr(pos5 + 1));
			g_bookVec.push_back(book);
		}
		printf("成功从磁盘读取图书配置文件!\n");
		bookFileStream.close();
	}
}

void writeBookDataFile()
{
	std::ofstream bookFileStream;
	bookFileStream.open("book.dat", std::ios::out | std::ios::trunc);
	if (!bookFileStream.is_open())
	{
		printf("发生一个未知的读写错误! 图书配置文件[%s]创建失败\n", "book.dat");
		exit(1);
	}
	else
	{
		for (const auto& book : g_bookVec)
		{
			bookFileStream << book._bookId << " " << book._bookName << " " << book._bookAuthor << " " << book._bookIntroduce << " " << book._bookPrice << " " << book._bookCount << "\n";
		}
		printf("成功将图书缓存写入磁盘!\n");
		bookFileStream.close();
	}
}

void createAdminMenu(void)
{
	readAdminDataFile();
	
	string name;
	bool dupFlag = false;

	while(1)
	{
		clearScreen();
		printf("#######################################################\n");
		printf("欢迎来到管理员创建向导\n");
		printf("#######################################################\n");

		while(1)
		{
			printf("输入你的名字:");
			std::cin >> name;

			std::find_if(g_adminVec.begin(), g_adminVec.end(), [&name, &dupFlag](const AdminInfo& admin) {
				if (admin._adminName == name)
				{
					dupFlag = true;
					return true;
				}
				return false;
			});

			if (dupFlag)
			{
				printf("错误! 存在同名管理员! \n");
				printf("任意按键重新输入!\n");
				dupFlag=0;
			}
			else
			{
				break;
			}
		}

		std::string password;
		std::string confirmPassword;
		while(1)
		{
			printf("请输入密码:");
			std::cin >> password;
			printf("请确认密码:");
			std::cin >> confirmPassword;
			if (password == confirmPassword)
			{
				printf("成功! 新的管理员已经添加\n");
				AdminInfo newAdmin;
				newAdmin._adminName = name;
				newAdmin._adminPassword = password;
				g_adminVec.push_back(newAdmin);

				break;
			}
			else
			{
				printf("两次输入的密码不同 请重试!\n");
			}
		}
		std::string choice;
		printf("继续创建管理员?(y/n):");
		fflush(stdin);
		cin >> choice;
		if (choice == "n" || choice == "no" || choice == "0")
		{
			break;
		}
	}
	printf("正在写入磁盘.............\n");
	writeAdminDataFile();
	printf("写入磁盘成功........任意按键返回\n");
	getchar();

	return;
}

void showAllBooks()
{
	readBookDataFile();

	if (g_bookVec.empty())
	{
		printf("没有任何图书,任意按键返回!");
		getchar();
		return;
	}
	else
	{
		printf("ID\t\t 名字\t\t 作者\t\t 简介\t\t 价格\t\t 库存量\t\t \n");
		for (const auto& book : g_bookVec)
		{
			printf("%-15d %-15s %-15s %-15s %-15.2f %-15d\n", book._bookId, book._bookName, book._bookAuthor, book._bookIntroduce, book._bookPrice, book._bookCount);
		}
		getchar();
		return;
	}
}

void addBook()
{
	readBookDataFile();
	
	int i = 0;
	bool dupFlag = false;

	printf("******************************************\n");
	printf("***********添加新书向导*****************\n");
	printf("******************************************\n");

	BookInfo bookInfo;
	printf("请输入这本书的ID:");
	cin >> bookInfo._bookId;
	bool dupFlag = false;
	
	while(1)
	{
		printf("请输入书名:");
		cin >> bookInfo._bookName;

		std::find_if(g_bookVec.begin(), g_bookVec.end(), [&bookInfo, &dupFlag](const BookInfo& book) {
			if (book._bookName == bookInfo._bookName)
			{
				dupFlag = true;
				return true;
			}
			return false;
		});

		if (dupFlag)
		{
			printf("存在相同书名\n");
			printf("任意按键重新输入书名\n");
			dupFlag = false;
			getchar();
		}
		else
		{
			break;
		}

	}

	printf("请输入书的作者名字:");
	cin >> bookInfo._bookAuthor;
	printf("请输入书的简介(不要有空格):");
	cin >> bookInfo._bookIntroduce;
	printf("请输入输的价格:");
	cin >> bookInfo._bookPrice;
	printf("请输入书的库存:");
	cin >> bookInfo._bookCount;
	g_bookVec.push_back(bookInfo);
	printf("正在写入磁盘...........\n");
	writeBookDataFile();
	printf("添加成功! 任意按键返回管理员菜单\n");
	getchar();
}

void deleteBookById()
{
	readBookDataFile();
	
	std::string tmpBookId;
	while(1)
	{
		printf("请输入要删除的书的ID:");
		cin >> tmpBookId;
		for (auto it = g_bookVec.begin(); it != g_bookVec.end(); ++it)
		{
			if (it->_bookId == tmpBookId)
			{
				g_bookVec.erase(it);
				printf("删除成功!\n");
				printf("正在写入磁盘...............\n");
				writeBookDataFile();
				printf("写入成功 任意按键返回");
				getchar();
				return;
			}
		}


		printf("ID对应的书不存在 任意按键重试! 或者按q退出");
		string tmpString;
		cin >> tmpString;
		if (tmpString == "q" || tmpString == "Q")
		{
			return;
		}			
	}
}

void deleteUserByName()
{
	readUserDataFile();
	
	printf("******************************************\n");
	printf("*************单个用户删除*****************\n");
	printf("******************************************\n");
	
	UserInfo userInfo;
	while(1)
	{
		printf("请输入要删除的用户的名字:");
		cin >> userInfo._userName;

		for (auto it = g_userVec.begin(); it != g_userVec.end(); ++it)
		{
			if (it->_userName == userInfo._userName)
			{
				g_userVec.erase(it);
				printf("删除成功!\n");
				printf("正在写入磁盘...............\n");
				writeUserDataFile();
				printf("写入成功 任意按键返回");
				getchar();
				return;
			}
		}
		printf("输入的用户不存在 任意按键重试! 或者按q退出");
		string tmpString;
		cin >> tmpString;
		if (tmpString == "q" || tmpString == "Q")
		{
			return;
		}
	}
}

void editBookStorageById()
{
	readBookDataFile();
	
	BookInfo *pBookInfo;
	string tmpString;
	while(1)
	{
		printf("输入你想修改库存的图书的ID:");
		cin >> tmpString;

		for (auto it = g_bookVec.begin(); it != g_bookVec.end(); ++it)
		{
			if (it->_bookId == tmpString)
			{
				pBookInfo = &(*it);
				break;
			}
		}

		printf("ID对应的书不存在!");
		printf("任意按键重新输入或者按q退出! ");
		cin >> tmpString;
		if (tmpString == "q" || tmpString == "Q")
		{
			return;
		}
	}

	int count = 0;
	printf("输入这本书当前的库存量:");
	cin >> count;
	pBookInfo->_bookCount = count;

	printf("正在写入磁盘...........");	
	writeBookDataFile();
	printf("完成! 任意按键返回\n");
	getchar();
}

void editBookInfoById(void)
{
	readBookDataFile();
	
	string tmpString;
	BookInfo *pBookInfo;
	while(1)
	{
		printf("输入你想修改信息的图书的ID:");
		cin >> tmpString;

		for (auto it = g_bookVec.begin(); it != g_bookVec.end(); ++it)
		{
			if (it->_bookId == tmpString)
			{
				pBookInfo = &(*it);
				break;
			}
		}
		printf("ID对应的书不存在! 任意按键重试! 或者按q退出! ");
			
		string tmpString;
		cin >> tmpString;
		if (tmpString == "q" || tmpString == "Q")
		{
			return;
		}
	}

	printf("输入这本书的新ID:");
	cin >> pBookInfo->_bookId;
	printf("输入这本书的新名字:");
	cin >> pBookInfo->_bookName;
	printf("请输入书的作者名字:");
	cin >> pBookInfo->_bookAuthor;
	printf("请输入书的简介:");
	cin >> pBookInfo->_bookIntroduce;
	printf("请输入书的价格:");
	cin >> pBookInfo->_bookPrice;
	printf("请输入书的库存量:");
	cin >> pBookInfo->_bookCount;

	printf("写入磁盘...........");
	writeBookDataFile();
	printf("完成! 任意按键返回\n");
	getchar();
}

void queryBookIntroduceById(void)
{
	readBookDataFile();
	string tmpString;

	while(1)
	{
		printf("输入查询简介的图书的ID:");
		cin >> tmpString;

		for (auto it = g_bookVec.begin(); it != g_bookVec.end(); ++it) {
			if (it->_bookId == tmpString) {
				printf("##################图书简介#####################\n");
				printf("%s\n", it->_bookIntroduce);
				printf("##################图书简介#####################\n");
				cout << "任意按键继续";
				getchar();
			}
		}

		
		printf("ID对应的书不存在! 任意按键重试! 或者按q退出");
		cin >> tmpString;
		if (tmpString == "q" || tmpString == "Q") {
			return;
		}
	}
}

void sortBookByPrice()
{
	readBookDataFile();
	
	std::sort(g_bookVec.begin(), g_bookVec.end(), [](const BookInfo& a, const BookInfo& b) {
		return a._bookPrice < b._bookPrice;
	});

	printf("ID\t\t 名字\t\t 作者\t\t 简介\t\t 价格\t\t 库存量\t\t \n");
	for (const auto& book : g_bookVec)
	{
		printf("%-15d %-15s %-15s %-15s %-15.2f %-15d\n", book._bookId, book._bookName, book._bookAuthor, book._bookIntroduce, book._bookPrice, book._bookCount);
	}
	getchar();
	
}

void getPriceSum()
{
	readBookDataFile();
	double sum=0;
	
	printf("ID\t\t 名字\t\t 作者\t\t 简介\t\t 价格\t\t 库存量\t\t \n");

	for(auto& book : g_bookVec)
	{
		printf("%-15d %-15s %-15s %-15s %-15.2f %-15d\n",book._bookId,book._bookName,book._bookAuthor,book._bookIntroduce,book._bookPrice,book._bookCount);
	}

	for(auto& book : g_bookVec)
	{
		sum=sum+(book._bookPrice*book._bookCount);
	}

	printf("当前图书总价为:%.2f\n",sum);
	getchar();
}


bool adminLogin(void)
{
	printf("\t\t\t尝试从硬盘读取管理员配置,请稍后......\n");
	readAdminDataFile();
	printf("\t\t\t读取管理员配置文件成功! 任意按键继续\n");
	string name;
	string password;
	string passwordConfirm;

	AdminInfo *pAdminInfo;

	bool tmpFlag = false;
	int errCount = 0;

	clearScreen();
	printf("#######################################################\n");
	printf("欢迎来到管理员登录向导\n\n");
	printf("#######################################################\n");

	while(1)
	{
		printf("输入管理员名字:");
		cin >> name;

		for(auto &admin : g_adminVec)
		{
			if (admin._adminName == name)
			{
				pAdminInfo = &admin;
				tmpFlag = true;
				break;
			}
		}

		if (!tmpFlag)
		{
			printf("错误 该管理员不存在\n");
			printf("任意按键重试!\n");
			getchar();
			
			errCount++;
			if (errCount >= 3)
			{
				printf("严重错误!!! 你已经 %d 次输错账户名\n", errCount);
				printf("拒绝登录!\n");
				exit(0);
			}
		}
		else
		{
			break;
		}
	}

	errCount = 0;
	while(1)
	{
		printf("请输入你的密码:");
		cin >> password;
		printf("请确认密码:");
		cin >> passwordConfirm;

		if (password == passwordConfirm)
		{

			while(1)
			{
				if(password == pAdminInfo->_adminPassword)
				{
					printf("登陆成功, 任意按键继续\n");
					getchar();
					return true;
				}
				else
				{
					errCount++;
					printf("账户 %s 对应的密码不正确\n", pAdminInfo->_adminName);
					printf("请任意按键重新输入\n");
					getchar();
					if (errCount>=3)
					{
						printf("严重错误!!! 你已经 %d 次输错密码!\n",errCount);
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
			getchar();
		}
	}
	return false;
}

bool userLogin(void)
{
	printf("正在从磁盘读取用户配置文件, 请稍后.........!\n");
	readUserDataFile();
	printf("读取用户文件成功\n");

	string name;
	string password;
	string passwordConfirm;
	bool tmpFlag = false;
	UserInfo *pUserInfo = nullptr;
	int errCount = 0;

	clearScreen();
	printf("#######################################################\n");
	printf("欢迎来到用户登录向导\n");
	printf("#######################################################\n");

	while(1)
	{
		printf("请输入你的用户名:");
		cin >> name;

		for(auto &userInfo : g_userVec)
		{
			if (userInfo._userName == name)
			{
				pUserInfo = &userInfo;
				tmpFlag = true;
				break;
			}
		}

		if (!tmpFlag)
		{
			printf("错误! 你输入的用户名不存在\n");
			printf("任意按键继续!\n");
			getchar();
			if (errCount++ >= 3)
			{
				printf("严重错误!!! 你已经 %d 次输错账户名\n", errCount);
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
		printf("请输入密码:");
		cin >> password;
		printf("请确认密码:");
		cin >> passwordConfirm;

		if (password == passwordConfirm)
		{

			while(1)
			{
				if(password == pUserInfo->_userPassword)
				{
					printf("登录成功任意按键继续\n");
					g_pCurrentLoginedUser = pUserInfo;
					fflush(stdin);
					getchar();
					return true;

					break;
				}
				else
				{
					errCount++;
					printf("账号 %s 对应的密码错误\n", pUserInfo->_userName.c_str());
					printf("请重试\n");
					if (errCount>=3)
					{
						printf("严重错误 你已经输错密码 %d 次!\n",errCount);
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


bool getVIP()
{

	string password;

	clearScreen();
	printf("#######################################################\n");
	printf("欢迎申请图书馆VIP\n");
	printf("#######################################################\n");
	printf("我们需要重新确认你的身份\n");

	while(1)
	{
		printf("请输入密码:");
		cin >> password;

		if (password == g_pCurrentLoginedUser->_userPassword)
		{
			g_pCurrentLoginedUser->_isVIP = true;
			writeUserDataFile();
			printf("成功申请VIP, 任意按键继续\n");
			getchar();
			return true;
		} 
		else
		{
			printf("密码错误! 申请失败!\n");
			return false;
		}
	}
}

void userBalanceCharge(void)
{
	char password[20];
	char password_confirm[20];
	float charge;

	clearScreen();
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
					getchar();
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
			getchar();
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
			getchar();
			print_user_menu();
			break;
		}
		else if(book_buffer[i].book_price*count > user_buffer[current_logined_user_in_buffer].balance)
		{
			printf("不好意思 你的余额已经不够你买想买的书了 去看看别的吧\n");
			printf("你可以去用户菜单充值!\n");
			getchar();
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
	getchar();
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
	
	getchar();
	
	print_user_menu();
	
}

void admin_show_book_info()
{
	clearScreen();

	printf("\t\t\t******************************************\n");
	printf("\t\t\t*************图书信息显示*****************\n");
	printf("\t\t\t******************************************\n");
	printf("\n");
	
	admin_show_all_books(book_first_node);
}

void admin_book_operate()
{
	int choice;

	clearScreen();
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

	clearScreen();

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

	clearScreen();

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

	clearScreen();

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
		clearScreen();
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
	getchar();
	fflush(stdin);
	
	char name[20];
	char password[20];
	char password_confirm[20];
	int i,j;
	int found_flag=0;
	int name_error_time=0;
	int password_error_time=0;


	clearScreen();
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
			getchar();
			
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
					getchar();
					print_system_config_menu();
					break;
				}
				else
				{
					password_error_time++;
					printf("账户 %s 对应的密码不正确\n",admin_buffer[j].admin_name);
					printf("请任意按键重新输入\n");
					getchar();
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
			getchar();
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
	getchar();
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
	getchar();
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
		getchar();
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
		getchar();
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
		clearScreen();
		printf("#######################################################\n");
		printf("欢迎来到图书馆用户创建向导\n");
		printf("#######################################################\n");
		
		printf("该向导将帮助你创建一个普通用户账号 任意按键继续\n");
		getchar();

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
				getchar();
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
	getchar();
	
	
	print_system_menu();
}

void print_system_config_menu()
{
	int choice;

	clearScreen();

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
		clearScreen();
		printf("警告! 这将丢失所有的书籍信息配置文件包括已经保存的书籍!");
		getchar();
		del_book_config();
	}
	else if (choice==4)
	{
		clearScreen();
		printf("警告! 这将丢失所有的用户信息配置文件包括已经保存的书籍!");
		getchar();
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

	clearScreen();

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
		clearScreen();
		printf("你将访问系统设置菜单! 为了安全 需要验证你的管理员身份!\n");
		getchar();
		system_config_login();
	}
	else if (choice==4)
	{
		new_user_regist();
	}
	else if (choice==5)
	{
		clearScreen();
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
