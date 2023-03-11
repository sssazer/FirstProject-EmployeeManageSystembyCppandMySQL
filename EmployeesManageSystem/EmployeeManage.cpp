#include "EmployeeManage.h"

void EmployeeManage::printTableHead() {

	for (int i = 0; i < empFields.size(); i++) {
		cout << setw(20) << left << empFields[i];
	}
	cout << endl;
}

/// <summary>
/// 打印主功能界面
/// </summary>
/// <param name="isRoot"></param>
void EmployeeManage::printMain(bool isRoot) {
	if (isRoot) {
		cout << "**************************" << endl;
		cout << "***欢迎使用职工管理系统***" << endl;
		cout << "******0-退出管理系统******" << endl;
		cout << "******1-显示职工信息******" << endl;
		cout << "******2-查找职工信息******" << endl;
		cout << "******3-按照编号排序******" << endl;
		cout << "******4-增加职工信息******" << endl;
		cout << "******5-删除离职职工******" << endl;
		cout << "******6-修改职工信息******" << endl;
		cout << "******7-清空所有数据******" << endl;
		cout << "**************************" << endl;
		cout << "**************************" << endl;
	}
	else {
		cout << "**************************" << endl;
		cout << "***欢迎使用职工管理系统***" << endl;
		cout << "******0-退出管理系统******" << endl;
		cout << "******1-显示职工信息******" << endl;
		cout << "******2-查找职工信息******" << endl;
		cout << "******3-按照编号排序******" << endl;
		cout << "**************************" << endl;
		cout << "**************************" << endl;
	}
}

#pragma region 职工管理系统函数
/// <summary>
/// 添加员工信息
/// </summary>
void EmployeeManage::addEmpInfo() {

	char* infoChars = new char[100];
	cin.get();
	while (true) { // 输入信息有误时循环输入

#pragma region 打印界面和读入数据
		system("cls");
		cout << "请按以下格式输入员工信息，中间用空格分隔，暂时没有的信息填null，输入q退出" << endl;
		cout << "员工姓名 电话号码 职位 工资 管理者ID" << endl;

		// 将整行数据读入		
		cin.getline(infoChars, 100);
		string infoStr = infoChars;
		infoChars += '\0';
		if (infoStr == "q") {
			break;
		}
#pragma endregion	

#pragma region 处理读入的数据并形成SQL语句
		string querySQL = "insert into emp_info (name, phone_number, job_id, salary, manager_id) values (";
		int i = 0;
		int j;
		// 以空格为分隔符分隔输入的各类信息，并补充至SQL语句中
		while ((j = infoStr.find(" ", i)) != -1) {
			string subInfo = infoStr.substr(i, j - i);
			i = j + 1;
			if (subInfo == "null" || subInfo == "NULL") {
				querySQL = querySQL + subInfo + ",";
			}
			else {
				querySQL = querySQL + "\"" + subInfo + "\",";
			}
		}
		if (i < infoStr.size()) {
			string subInfo = infoStr.substr(i, infoStr.size() - i);
			if (subInfo == "null" || subInfo == "NULL") {
				querySQL = querySQL + subInfo + ",";
			}
			else {
				querySQL = querySQL + "\"" + subInfo + "\",";
			}
		}
		querySQL.replace(querySQL.size() - 1, 1, ")");
#pragma endregion

#pragma region 执行SQL语句，插入员工信息
		// 执行SQL语句
		if (mysql_query(&mysql, querySQL.c_str())) {
			cout << "error_msg:" << mysql_error(&mysql) << endl;
			cout << "输入信息有误，请重新输入" << endl;
			system("pause");
			continue;
		}
		cout << "添加成功" << endl;
		system("pause");
		break;
	} // while_true
#pragma endregion

	delete[] infoChars;
}

/// <summary>
/// 显示所有员工信息
/// </summary>
void EmployeeManage::showEmpInfo() {

#pragma region 查询表中数据并保存结果集
	string querySql = "select * from emp_info";
	mysql_query(&mysql, querySql.c_str());
	res = mysql_store_result(&mysql);
#pragma endregion

#pragma region 打印表头
	printTableHead();
#pragma endregion

#pragma region 打印表中信息
	while (row = mysql_fetch_row(res)) {
		for (int i = 0; i < empFields.size(); i++) {
			if (row[i]) {
				cout << setw(20) << left << row[i];
			}
			else {
				cout << setw(20) << left << "（无）";
			}
		}
		cout << endl;
	}
#pragma endregion

	// 释放结果集
	mysql_free_result(res);
}

/// <summary>
/// 通过ID或姓名删除员工信息
/// </summary>
void EmployeeManage::deleteEmpInfo() {
	while (true) {
#pragma region 打印提示信息并获取输入
		system("cls");

		cout << "请输入要删除的员工ID或姓名(输入q退出):";
		string strInfo;
		cin >> strInfo;
		if (strInfo == "q") {
			break;
		}
#pragma endregion

#pragma region 输入ID
		if (strInfo[0] >= '0' && strInfo[0] <= '9') {
			string querySQL = "delete from emp_info where emp_id = \"" + strInfo + "\"";
			if (mysql_query(&mysql, querySQL.c_str())) {
				cout << "输入格式有误，请检查您的输入" << endl;
				cout << "error_msg:" << mysql_error(&mysql) << endl;
				system("pause");
			}
			else if (mysql_affected_rows(&mysql)) {
				cout << "删除成功" << endl;
				system("pause");
				break;
			}
			else {
				cout << "删除失败，未找到ID为" << strInfo << "的员工" << endl;
				system("pause");
			}
		}
#pragma endregion

#pragma region 输入姓名
		else {
			string querySQL = "delete from emp_info where name = \"" + strInfo + "\"";
			if (mysql_query(&mysql, querySQL.c_str())) {
				cout << "输入格式有误，请检查您的输入" << endl;
				cout << "error_msg:" << mysql_error(&mysql) << endl;
				system("pause");
			}
			else if (mysql_affected_rows(&mysql)) {
				cout << "删除成功" << endl;
				system("pause");
				break;
			}
			else {
				cout << "删除失败：未找到姓名为" << strInfo << "的员工" << endl;
				system("pause");
			}
		}
#pragma endregion
	} // while-true
}


void EmployeeManage::alterEmpInfo() {
	while (true) {
#pragma region 读入要修改的员工信息
		system("cls");
		cout << "请输入要修改的员工ID（按q退出）：";
		string targetId;
		cin >> targetId;
		if (targetId == "q") {
			break;
		}
#pragma endregion

#pragma region 判断员工是否存在
		string querySQL = "select * from emp_info where emp_id = \"" + targetId + "\"";
		if (mysql_query(&mysql, querySQL.c_str())) {
			cout << "输入格式有误，请重新输入" << endl;
			cout << "error_msg:" << mysql_error(&mysql) << endl;
			continue;
		}
		res = mysql_store_result(&mysql);
		if (row = mysql_fetch_row(res)) { // 员工存在的话就打印员工信息
			printTableHead();
			for (int i = 0; i < empFields.size(); i++) {
				if (row[i]) {
					cout << setw(20) << left << row[i];
				}
				else {
					cout << setw(20) << left << "（空）";
				}
			}
			cout << endl;
		}
		else {
			cout << "要修改的员工不存在，请重新输入" << endl;
			system("pause");
			continue;
		}
		mysql_free_result(res);
#pragma endregion

#pragma region 选择要修改的信息项
		vector<string> hintFields = { "姓名","电话","岗位","薪资","管理者ID" };
		vector<string> tableFields = { "name", "phone_number", "job_id", "salary", "manager_id" };
		int targetField; // 保存用户选择项
		string strInfo; // 保存用户要修改的信息
		cout << "1.姓名  2.电话  3.岗位  4.薪资  5.管理者ID" << endl;
		while (true) { // 获取并检测用户选择
			cout << "请输入要修改的信息项编号：";
			if (cin >> targetField) {
				break;
			}
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "您的选择有误，请重新输入" << endl;
			system("pause");
			continue;
		}
#pragma endregion

#pragma region 读入修改后的信息并进行修改
		cout << "请输入新的" << hintFields[targetField - 1] << ":";
		cin >> strInfo;
		string alterSQL = "update emp_info set " + tableFields[targetField - 1] + "=\"" + strInfo + "\" where emp_id = \"" + targetId + "\"";
		if (mysql_query(&mysql, alterSQL.c_str())) {
			cout << "修改失败" << endl;
			cout << "error_msg:" << mysql_error(&mysql) << endl;
			system("pause");
			continue;
		}
		// 反馈修改结果
		if (mysql_affected_rows(&mysql)) {
			cout << "修改成功" << endl;
			system("pause");
			continue;
		}
		else {
			cout << "修改失败" << endl;
			system("pause");
			continue;
		}
#pragma endregion
	}
}

/// <summary>
/// 通过姓名或id查找员工
/// </summary>
void EmployeeManage::findEmpInfo() {
	while (true) {
#pragma region 打印界面并获取输入
		system("cls");
		string input;
		cout << "请输入id或名字查询员工（输入q退出）：";
		cin >> input;
		if (input == "q") {
			break;
		}
#pragma endregion
		// 判断输入的是id还是名字
#pragma region 输入ID
		if (input[0] >= '0' && input[1] <= '9') {
			string querySql = "select * from emp_info where emp_id = \"" + input + "\"";
			if (mysql_query(&mysql, querySql.c_str())) {
				cout << "error_msg:" << mysql_error(&mysql) << endl;
				system("pause");
			}
			res = mysql_store_result(&mysql);
			if (mysql_num_rows(res) == 0) {
				cout << "id为" << input << "的员工不存在" << endl;
				system("pause");
			}
			else {
				// 打印查询到的信息
				printTableHead(); // 打印表头
				while (row = mysql_fetch_row(res)) {
					for (int i = 0; i < empFields.size(); i++) {
						if (row[i]) {
							cout << setw(20) << left << row[i];
						}
						else {
							cout << setw(20) << left << "（空）";
						}
					}
				}
				system("pause");
			} // if-else
		}
#pragma endregion

#pragma region 输入名字
		else {
			string querySql = "select * from emp_info where name = \"" + input + "\"";
			if (mysql_query(&mysql, querySql.c_str())) {
				cout << "error_msg:" << mysql_error(&mysql) << endl;
				system("pause");
			}
			res = mysql_store_result(&mysql);
			if (mysql_num_rows(res) == 0) {
				cout << "name为" << input << "的员工不存在" << endl;
				system("pause");
			}
			else {
				// 打印查询到的信息
				printTableHead();
				while (row = mysql_fetch_row(res)) {
					for (int i = 0; i < empFields.size(); i++) {
						if (row[i]) {
							cout << setw(20) << left << row[i];
						}
						else {
							cout << setw(20) << left << "（空）";
						}
					}
				}
				system("pause");
			} // if-else
		}
#pragma endregion
	}

	mysql_free_result(res); // 释放结果集
}

/// <summary>
/// 重新为员工分配序号
/// </summary>
void EmployeeManage::sortById() {
	// 直接先将id列删除再创建，就可以重新排序
	mysql_query(&mysql, "ALTER TABLE emp_info DROP emp_id");
	mysql_query(&mysql, "ALTER TABLE emp_info ADD emp_id INT PRIMARY KEY AUTO_INCREMENT first");
	cout << "重新排序成功" << endl;
	system("pause");
}

/// <summary>
/// 清空员工表
/// </summary>
void EmployeeManage::clearAll() {
	char confirm = 'n';
	cout << "是否确定清空数据库（y/n）：";
	cin >> confirm;
	if (confirm == 'y' || confirm == 'Y') {
		if (mysql_query(&mysql, "delete from emp_info")) {
			cout << "清空失败" << endl;
			cout << "error_msg:" << mysql_error(&mysql) << endl;
			system("pause");
		}
		else {
			cout << "清空成功" << endl;
			system("pause");
		}
	}
}

#pragma endregion

void EmployeeManage::mainEmpRoot() {
		while (true) {
#pragma region 打印界面，获取要执行的操作码
			system("cls");
			printMain(1);
			cout << "请输入需要使用的功能编号：";
			if (!(cin >> optIndex)) { // 读入并检测操作代码
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				cout << "输入有误，请重新输入" << endl;
				system("pause");
				continue;
			}
#pragma endregion
#pragma region 根据操作码执行对应操作
			switch (optIndex) {
			case Optcode::QUIT: // 退出
				cout << "退出成功" << endl;
				system("pause");
				return;
				break;
			case Optcode::ADD: // 添加员工
				addEmpInfo();
				break;
			case Optcode::SHOW: // 显示所有员工信息
				showEmpInfo();
				system("pause");
				break;
			case Optcode::DEL: // 删除离职员工
				deleteEmpInfo();
				break;
			case Optcode::FIND: // 查找员工
				findEmpInfo();
				break;
			case Optcode::SORT: // 重新排序
				sortById();
				break;
			case Optcode::CLEAR: // 清除所有信息
				clearAll();
				break;
			case Optcode::ALTER: // 修改员工信息
				alterEmpInfo();
				break;
			default:
				cout << "选择有误，请重新选择" << endl;
				system("pause");
				break;
			} // switch-case
#pragma endregion
		}
}

void EmployeeManage::mainEmpOrdinary() {
	while (true) {
#pragma region 打印界面，获取操作码
		system("cls");
		printMain(0);
		cout << "请输入需要使用的功能编号：";
		if (!(cin >> optIndex)) { // 读入并检测操作代码
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "输入有误，请重新输入" << endl;
			system("pause");
			continue;
		}
#pragma endregion
#pragma region 根据操作码执行对应操作
		switch (optIndex) {
		case Optcode::QUIT: // 退出
			cout << "退出成功" << endl;
			system("pause");
			return;
			break;
		case Optcode::SHOW: // 显示所有员工信息
			showEmpInfo();
			system("pause");
			break;
		case Optcode::FIND: // 查找员工信息
			findEmpInfo();
			break;
		case Optcode::SORT: // 排序
			sortById();
			break;
		default:
			cout << "选择有误，请重新选择" << endl;
			system("pause");
			break;
		} // switch-case
#pragma endregion
	}
}

EmployeeManage::EmployeeManage() {
#pragma region 登录数据库
	if (!mysql_init(&mysql)) {
		cout << "数据库初始化失败，请联系管理员！" << endl;
		exit(-1);
	}
	if (!mysql_real_connect(&mysql, "127.0.0.1", "root", "abc123", "manage_system_db", 3306, NULL, 0)) {
		cout << "数据库连接失败，请联系管理员！" << endl;
		cout << "error_msg:" << mysql_error(&mysql) << endl;
		exit(-1);
	}
#pragma endregion

#pragma region 初始化emp_info的表头信息
	if (mysql_query(&mysql, "select * from emp_info where emp_id = 0")) {
		cout << "error_msg:" << mysql_error(&mysql) << endl;
	}
	res = mysql_store_result(&mysql);
	fieldNums = mysql_num_fields(res);
	empFields.reserve(fieldNums);
	fields = mysql_fetch_fields(res);
	for (int i = 0; i < fieldNums; i++) {
		empFields.push_back(fields[i].name);
	}
	mysql_free_result(res);
#pragma endregion
}

EmployeeManage::~EmployeeManage() {
	mysql_close(&mysql);
}