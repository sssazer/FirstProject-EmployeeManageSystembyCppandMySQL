#pragma once
#include<iostream>
#include<vector>
#include<mysql.h>
#include<string>
#include<iomanip>
#include "EmployeeManage.h"
#include "UserManage.h"

using namespace std;

class ManageSystem {
private:
	MYSQL mysql;
	MYSQL_RES* res; // 存储查询结果集 res作为参数传入或者跨函数时就会变为null！
	MYSQL_ROW row; // 读取结果集
	int fieldNums; // 当前查询结果集的字段数

	EmployeeManage em;
	UserManage um;

	string user_name;
	string user_pwd;
	bool isRoot; // 记录当前登录用户是否是root用户

	void loginPage(); // 实现登录功能
	void printLogin(); // 打印登录界面

public:
	void mainPage(); // 用于操作整个系统
	ManageSystem(); // 无参构造
	~ManageSystem();
};