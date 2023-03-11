#include "UserManage.h"

void UserManage::printMain() {
	cout << "**************************" << endl;
	cout << "***欢迎使用用户管理系统***" << endl;
	cout << "******0-退出管理系统******" << endl;
	cout << "******1-显示用户账号******" << endl;
	cout << "******2-查找用户账号******" << endl;
	cout << "******3-重 新 编 号*******" << endl;
	cout << "******4-增加用户账号******" << endl;
	cout << "******5-删除用户账号******" << endl;
	cout << "******6-修改用户账号******" << endl;
	cout << "******7-清空所有账号******" << endl;
	cout << "**************************" << endl;
	cout << "**************************" << endl;
}

void UserManage::printTableHead() {
	for (int i = 0; i < usrFields.size(); i++) {
		cout << setw(20) << left << usrFields[i];
	}
	cout << endl;
}

/// <summary>
/// 添加员工信息
/// </summary>
void UserManage::addUsrInfo() {

	while (true) { // 输入信息有误时循环输入

#pragma region 打印界面，读入并处理数据
		system("cls");
		cout << "创建账号" << endl;
		cout << "请输入用户名（20个字符以内）：";
		string new_name;
		cin >> new_name;
		cout << "请输入密码（20个字符以内）：";
		string new_pwd;
		cin >> new_pwd;
		cout << "是否有root权限（0/1）：";
		char new_root;
		cin >> new_root;
#pragma endregion	

#pragma region 根据读入数据形成SQL语句 并执行
		string querySQL = "insert into usr_info (user_name, user_pwd, authority) values (\""
			+ new_name + "\",\"" + new_pwd + "\",\"" + new_root + "\")";

		if (mysql_query(&mysql, querySQL.c_str())) {
			cout << "error_msg:" << mysql_error(&mysql) << endl;
			cout << "输入格式有误，请重新输入" << endl;
			system("pause");
			continue;
		}

		cout << "添加成功" << endl;
		system("pause");
		break;

	} // while_true
#pragma endregion
}

/// <summary>
/// 显示所有员工信息
/// </summary>
void UserManage::showUsrInfo() {

#pragma region 查询表中数据并保存结果集
	string querySql = "select * from usr_info";
	mysql_query(&mysql, querySql.c_str());
	res = mysql_store_result(&mysql);
#pragma endregion

#pragma region 打印表头
	printTableHead();
#pragma endregion

#pragma region 打印表中信息
	while (row = mysql_fetch_row(res)) {
		for (int i = 0; i < usrFields.size(); i++) {
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
void UserManage::deleteUsrInfo() {
	while (true) {
#pragma region 打印提示信息并获取输入
		system("cls");

		cout << "请输入要删除的账号的用户名（输入q退出）：";
		string strInfo;
		cin >> strInfo;
		if (strInfo == "q") {
			break;
		}
#pragma endregion

#pragma region 删除指定用户名
		else {
			string querySQL = "delete from usr_info where user_name = \"" + strInfo + "\"";
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
				cout << "删除失败：未找到用户名为" << strInfo << "的账号信息" << endl;
				system("pause");
			}
		}
#pragma endregion
	} // while-true
}


void UserManage::alterUsrInfo() {
	while (true) {
#pragma region 读入要修改的员工信息
		system("cls");
		cout << "请输入要修改账号的用户名（按q退出）：";
		string targetName;
		cin >> targetName;
		if (targetName == "q") {
			break;
		}
#pragma endregion

#pragma region 判断用户名是否存在
		string querySQL = "select * from usr_info where user_name = \"" + targetName + "\"";
		if (mysql_query(&mysql, querySQL.c_str())) {
			cout << "输入格式有误，请重新输入" << endl;
			cout << "error_msg:" << mysql_error(&mysql) << endl;
			continue;
		}
		res = mysql_store_result(&mysql);
		if (row = mysql_fetch_row(res)) { // 用户名存在的话就打印账号信息
			printTableHead();
			for (int i = 0; i < usrFields.size(); i++) {
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
			cout << "要修改的账户不存在，请重新输入" << endl;
			system("pause");
			continue;
		}
		mysql_free_result(res);
#pragma endregion

#pragma region 选择要修改的信息项
		vector<string> hintFields = { "密码", "用户权限"};
		vector<string> tableFields = { "user_pwd", "authority"};
		int targetField; // 保存用户选择项
		string strInfo; // 保存用户要修改的信息
		cout << "1.密码\t2.用户权限 " << endl;
		while (true) { // 获取并检测用户选择
			cout << "请输入要修改的信息项编号：";
			cin.sync();
			char temp;
			cin >> temp;
			if (temp == '1' || temp == '2') {
				targetField = temp - '0';
				break;
			}
			else {
				cout << "您的选择有误，请重新选择" << endl;
				system("pause");
				continue;
			}
		}
#pragma endregion

#pragma region 读入修改后的信息并进行修改
		cout << "请输入新的" << hintFields[targetField - 1] << ":";
		cin >> strInfo;
		string alterSQL = "update usr_info set " + tableFields[targetField - 1] + "=\"" + strInfo + "\" where user_name = \"" + targetName + "\"";
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
void UserManage::findUsrInfo() {
	while (true) {
#pragma region 打印界面并获取输入
		system("cls");
		string input;
		cout << "请输入用户名查找账号（输入q退出）：";
		cin >> input;
		if (input == "q") {
			break;
		}
#pragma endregion

#pragma region 根据输入的用户名查找
		string querySql = "select * from usr_info where user_name = \"" + input + "\"";
		if (mysql_query(&mysql, querySql.c_str())) {
			cout << "error_msg:" << mysql_error(&mysql) << endl;
			system("pause");
		}
		res = mysql_store_result(&mysql);
		if (mysql_num_rows(res) == 0) {
			cout << "用户名为" << input << "的账号不存在" << endl;
			system("pause");
		}
		else {
			// 打印查询到的信息
			printTableHead();
			while (row = mysql_fetch_row(res)) {
				for (int i = 0; i < usrFields.size(); i++) {
					if (row[i]) {
						cout << setw(20) << left << row[i];
					}
					else {
						cout << setw(20) << left << "（空）";
					}
				}
				cout << endl;
			}
			system("pause");
		} // if-else
#pragma endregion
	}

	mysql_free_result(res); // 释放结果集
}

/// <summary>
/// 重新为员工分配序号
/// </summary>
void UserManage::sortUsr() {
	// 直接先将id列删除再创建，就可以重新排序
	mysql_query(&mysql, "ALTER TABLE usr_info DROP id");
	mysql_query(&mysql, "ALTER TABLE usr_info ADD id INT PRIMARY KEY AUTO_INCREMENT first");
	cout << "重新排序成功" << endl;
	system("pause");
}

/// <summary>
/// 清空用户账号表
/// </summary>
void UserManage::clearUsr() {
	char confirm = 'n';
	cout << "是否确定清空数据库（y/n）：";
	cin >> confirm;
	if (confirm == 'y' || confirm == 'Y') {
		if (mysql_query(&mysql, "delete from usr_info")) {
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

void UserManage::mainUser() {
	while (true) {
#pragma region 打印界面，获取要执行的操作码
		system("cls");
		printMain();
		cout << "请输入需要使用的功能编号：";
		int input;
		if(!(cin >> input)){ // 读入并检测数据
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "输入有误，请重新输入" << endl;
			cin.get();
			continue;
		}
#pragma endregion
#pragma region 根据操作码执行对应操作
		switch (input) {
		case Optcode::QUIT: // 退出
			cout << "退出成功" << endl;
			system("pause");
			return;
			break;
		case Optcode::ADD: // 添加员工
			addUsrInfo();
			break;
		case Optcode::SHOW: // 显示所有员工信息
			showUsrInfo();
			system("pause");
			break;
		case Optcode::DEL: // 删除离职员工
			deleteUsrInfo();
			break;
		case Optcode::FIND: // 查找员工
			findUsrInfo();
			break;
		case Optcode::SORT: // 重新排序
			sortUsr();
			break;
		case Optcode::CLEAR: // 清除所有信息
			clearUsr();
			break;
		case Optcode::ALTER: // 修改员工信息
			alterUsrInfo();
			break;
		default:
			cout << "选择有误，请重新选择" << endl;
			system("pause");
			break;
		} // switch-case
#pragma endregion
	}
}

UserManage::UserManage() {
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

#pragma region 初始化usr_info的表头信息
	if (mysql_query(&mysql, "select * from usr_info where id = 0")) {
		cout << "error_msg:" << mysql_error(&mysql) << endl;
	}
	res = mysql_store_result(&mysql);
	fieldNums = mysql_num_fields(res);
	usrFields.reserve(fieldNums);
	fields = mysql_fetch_fields(res);
	for (int i = 0; i < fieldNums; i++) {
		usrFields.push_back(fields[i].name);
	}
	mysql_free_result(res);
#pragma endregion
}

UserManage::~UserManage() {
	mysql_close(&mysql);
}