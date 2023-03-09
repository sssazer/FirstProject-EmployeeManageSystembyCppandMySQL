# 创建员工表
DROP TABLE emp_info;

CREATE TABLE emp_info
AS 
SELECT employee_id AS emp_id, last_name AS NAME, phone_number, job_id, salary, manager_id
FROM atguigudb.`employees`;
ALTER TABLE emp_info
MODIFY emp_id INT PRIMARY KEY AUTO_INCREMENT;
DESC emp_info;

SELECT * FROM atguigudb.`employees`;
SELECT * FROM emp_info;
DESC emp_info;
# 建用户表
DROP TABLE usr_info;

CREATE TABLE usr_info(
id INT PRIMARY KEY AUTO_INCREMENT,
user_name VARCHAR(20) NOT NULL COMMENT "用户账号",
user_pwd VARCHAR(20) NOT NULL COMMENT "用户密码",
authority BOOL NOT NULL DEFAULT FALSE COMMENT "用户权限,true表示root用户，false表示普通用户"
);

ALTER TABLE usr_info
MODIFY user_name VARCHAR(20) NOT NULL UNIQUE;

DESC usr_info;

# 向用户表中添加测试用户
INSERT INTO usr_info(user_name, user_pwd)
VALUES("test","123456");

INSERT INTO usr_info(user_name, user_pwd, authority)
VALUES("test_root", "1", 1);

SELECT * FROM usr_info;

# 修改员工信息表
DESC emp_info;
ALTER TABLE emp_info
DROP COLUMN first_name, 
DROP COLUMN email, 
DROP COLUMN phone_number, 
DROP COLUMN hire_date, 
DROP COLUMN commission_pct, 
DROP COLUMN department_id;

ALTER TABLE emp_info
CHANGE employee_id emp_id INT;

ALTER TABLE emp_info
CHANGE last_name NAME VARCHAR(25);

UPDATE usr_info
SET user_name = 'root'
WHERE id = 2;

SELECT *
FROM emp_info
WHERE emp_id = "wangyize" OR NAME = "wangyize";
DESC emp_info;

UPDATE emp_info
SET NAME = "nanbao"
WHERE emp_id = "wangyize" OR NAME = "wangyize";

INSERT INTO emp_info(NAME, phone_number, job_id, salary, manager_id)
VALUES("wang", "12314", "stu", "0", NULL);
SELECT * FROM emp_info
WHERE NAME = "wang";