#pragma once
#include<iostream>
#include<vector>
#include<mysql.h>
#include<string>
#include<iomanip>

using namespace std;

class UserManage {
private:
	MYSQL mysql;
	MYSQL_RES* res; // 存储查询结果集 res作为参数传入或者跨函数时就会变为null！
	MYSQL_ROW row; // 读取结果集
	int fieldNums; // 当前查询结果集的字段数
	MYSQL_FIELD* fields; // 当前查询结果集的表头信息
	vector<string> usrFields; // 保存usr_info表的表头信息

	enum Optcode { // 操作码
		QUIT = 0,
		SHOW = 1,
		FIND = 2,
		SORT = 3,
		ADD = 4,
		DEL = 5,
		ALTER = 6,
		CLEAR = 7
	};

	void printMain();
	void printTableHead(); // 打印表头信息
	void showUsrInfo(); // 显示所有用户账号
	void deleteUsrInfo(); // 删除用户账号
	void alterUsrInfo(); // 修改用户账号
	void findUsrInfo(); // 修改用户账号
	void sortUsr(); // 将用户账号重新编号
	void clearUsr(); // 清空所有用户账号数据
	void addUsrInfo(); // 增加用户账号

public:
	void mainUser();
	UserManage();
	~UserManage();

};
