#include "ManageSystem.h"

/// <summary>
/// 构造函数，用于连接MySQL服务器
/// </summary>
ManageSystem::ManageSystem() {
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
}

ManageSystem::~ManageSystem() {
	mysql_close(&mysql);
}

/// <summary>
/// 打印登陆界面
/// </summary>
void ManageSystem::printLogin() {
	cout << "**************************" << endl;
	cout << "*********请登录***********" << endl;
	cout << "请输入用户名：";
	cin >> user_name;
	cout << "请输入密码";
	cin >> user_pwd;
}

/// <summary>
/// 实现登录功能
/// </summary>
/// <returns>登录用户为普通用户时返回0，为root用户时返回1</returns>
void ManageSystem::loginPage() {
	while (true) {
#pragma region 打印界面，获取用户名密码并形成SQL语句
		system("cls");
		printLogin();
		string querySql = "select user_pwd, authority from usr_info where user_name = \"" + user_name + "\"";
#pragma endregion

#pragma region 执行SQL并获取结果集
		if (mysql_query(&mysql, querySql.c_str())) {
			cout << "error_msg:" << mysql_error(&mysql) << endl;
			exit(-1);
		}
		res = mysql_store_result(&mysql);
#pragma endregion

#pragma region 根据数据库中数据判断是否登录成功
		if (row = mysql_fetch_row(res)) {
			if (user_pwd == row[0]) {
				isRoot = row[1][0] - '0';
				break;
			}
			else {
				cout << "密码错误！" << endl;
				system("pause");
				continue;
			}
		}
		else {
			cout << "用户名不存在，请重新输入" << endl;
			system("pause");
		}
#pragma endregion
	} // while_true
}// login_page


void ManageSystem::mainPage() {
	loginPage(); // 执行登录功能

	if(isRoot){
		while (true) {
			system("cls");
			cout << "0、退出系统" << endl;
			cout << "1、用户管理系统" << endl;
			cout << "2、员工管理系统" << endl;
			cout << "请选择要进入的系统（0/1/2）：";

			int sysChoice;
			if (! (cin >> sysChoice)) { // 读取并检测输入，如果输入非法，cin会置为false，并进入错误状态
				cin.clear(); // 将cin的错误状态清除
				cin.ignore(INT_MAX, '\n'); // 清除输入缓冲区
				cout << "输入有误，请重新输入" << endl;
				cin.get();
				continue;
			}
			
			if (sysChoice == 0) {
				cout << "退出成功" << endl;
				system("pause");
				exit(0);
			}
			else if(sysChoice == 1){
				um.mainUser();
			}
			else if(sysChoice == 2){
				em.mainEmpRoot();
			}
			else {
				cout << "输入错误，请重新输入" << endl;
				system("pause");
			}
		}
	}
	else {
		em.mainEmpOrdinary();
	}
}