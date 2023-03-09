#pragma once
#include<iostream>
#include<vector>
#include<mysql.h>
#include<string>
#include<iomanip>

using namespace std;

class EmployeeManage {
private:
	MYSQL mysql;
	MYSQL_RES* res; // 存储查询结果集 res作为参数传入或者跨函数时就会变为null！
	MYSQL_ROW row; // 读取结果集
	int fieldNums; // 当前查询结果集的字段数
	MYSQL_FIELD* fields; // 当前查询结果集的表头信息
	vector<string> empFields; // 保存emp_info表的表头信息

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

	int optIndex;

	void printMain(bool isRoot); // 打印主页面
	void showEmpInfo(); // 显示所有职工信息
	void printTableHead(); // 打印表头信息
	void deleteEmpInfo(); // 删除离职员工信息
	void alterEmpInfo(); // 修改员工信息
	void findEmpInfo(); // 修改员工信息	
	void sortById(); // 将员工重新编号
	void clearAll(); // 清空所有数据
	void addEmpInfo(); // 增加职工信息

public:
	void mainEmpRoot();
	void mainEmpOrdinary();
	EmployeeManage();
	~EmployeeManage();
};
