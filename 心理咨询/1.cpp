#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_USERS 100
#define MAX_STUDENTS 100
#define MAX_TESTS 100
#define MAX_ARTICLES 100
// 用户结构体
typedef struct User
{
    char username[100];
    char password[100];
    int role; // 0表示学生，1表示教师
} User;
// 学生结构体
typedef struct
{
    char username[100]; // 学生用户名
    float mentalScore;  // 心理得分
    char id[100];       // 学生ID
    char name[100];     // 学生姓名
    int age;            // 学生年龄
} Student;
// 测试题目结构体
typedef struct
{
    int testNo;        // 测试编号
    char content[100]; // 测试内容
} Test;
// 文章结构体
typedef struct
{
    char date[100];  // 发布日期
    char title[100]; // 文章标题
    char words[100]; // 文章内容
} Article;
// 用户数组
User users[MAX_USERS];
int numUsers = 0;
User currentUser = {"", "", -1}; // 初始化当前用户为空
// 初始化学生信息
Student currentStudent = {"", 0.0, "", "", 0}; // 将学生结构体变量的各字段初始化为空或0
// 学生数组
Student students[MAX_STUDENTS];
int numStudents = 0;
// 测试题目数组
Test tests[MAX_TESTS];
int numTests = 0;
// 文章数组
Article articles[MAX_ARTICLES];
int numArticles = 0;
void studentMenu();
void teacherMenu();
// 用户登录函数
// 用户登录函数
void userLogin()
{
    char username[100];
    char password[100];
    int role;

    printf("欢迎使用心理咨询系统\n");
    printf("请选择登录角色：\n");
    printf("1. 学生\n");
    printf("2. 教师\n");
    printf("请选择操作：");
    scanf("%d", &role);

    printf("请输入用户名：");
    scanf("%s", username);
    printf("请输入密码：");
    scanf("%s", password);

    if (role == 1)
    {
        // 学生登录逻辑
        int studentIndex = -1;
        for (int i = 0; i < numUsers; i++)
        {
            if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
            {
                studentIndex = i;
                currentUser = users[i];
                for (int j = 0; j < numStudents; j++)
                {
                    if (strcmp(users[j].username, username) == 0)
                    {
                        currentStudent = students[j];
                    }
                }
                break;
            }
        }
        if (studentIndex != -1)
        {
            printf("学生登录成功\n");
            studentMenu();
        }
        else
        {
            printf("用户名或密码错误\n");
        }
    }
    else if (role == 2)
    {
        // 教师登录逻辑
        if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0)
        {
            printf("教师登录成功\n");
            teacherMenu();
        }
        else
        {
            printf("用户名或密码错误\n");
        }
    }
    else
    {
        printf("无效的选择\n");
    }
}
// 学生菜单函数
void studentMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n==================== 学生菜单 ====================\n");
        printf("您好,%s!\n", currentStudent.name);
        printf("1. 查看/修改个人信息\n");
        printf("2. 浏览心理健康知识和文章\n");
        printf("3. 进行心理测试\n");
        printf("4. 向老师咨询心理问题\n");
        printf("5. 退出\n");
        printf("===================================================\n");
        printf("请选择操作：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("您选择了查看/修改个人信息\n");
            // TODO: 调用查看/修改个人信息的函数
            break;
        case 2:
            printf("您选择了浏览心理健康知识和文章\n");
            // TODO: 调用浏览心理健康知识和文章的函数
            break;
        case 3:
            printf("您选择了进行心理测试\n");
            // TODO: 调用进行心理测试的函数
            break;
        case 4:
            printf("您选择了向老师咨询心理问题\n");
            // TODO: 调用向老师咨询心理问题的函数
            break;
        case 5:
            printf("您选择了退出\n");
            break;
        default:
            printf("无效的选择，请重新输入\n");
        }
        system("pause");
    } while (choice != 5);
}
// 教师菜单函数
void teacherMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n==================== 教师菜单 ====================\n");
        printf("1. 增加学生信息\n");
        printf("2. 删除学生信息\n");
        printf("3. 修改学生信息\n");
        printf("4. 查找学生信息\n");
        printf("5. 新增心理知识文章\n");
        printf("6. 删除心理知识文章\n");
        printf("7. 修改心理知识文章\n");
        printf("8. 查找心理知识文章\n");
        printf("9. 增加心理健康测试题目\n");
        printf("10. 删除心理健康测试题目\n");
        printf("11. 显示所有心理健康测试题目\n");
        printf("12. 显示心理健康测试结果\n");
        printf("13. 对学生心理健康测试结果排序\n");
        printf("14. 查看并回复学生私聊\n");
        printf("0. 退出\n");
        printf("===================================================\n");
        printf("请选择操作：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("您选择了增加学生信息\n");
            // TODO: 调用增加学生信息的函数
            break;
        case 2:
            printf("您选择了删除学生信息\n");
            // TODO: 调用删除学生信息的函数
            break;
        case 3:
            printf("您选择了修改学生信息\n");
            // TODO: 调用修改学生信息的函数
            break;
        case 4:
            printf("您选择了查找学生信息\n");
            // TODO: 调用查找学生信息的函数
            break;
        case 5:
            printf("您选择了新增心理知识文章\n");
            // TODO: 调用新增心理知识文章的函数
            break;
        case 6:
            printf("您选择了删除心理知识文章\n");
            // TODO: 调用删除心理知识文章的函数
            break;
        case 7:
            printf("您选择了修改心理知识文章\n");
            // TODO: 调用修改心理知识文章的函数
            break;
        case 8:
            printf("您选择了查找心理知识文章\n");
            // TODO: 调用查找心理知识文章的函数
            break;
        case 9:
            printf("您选择了增加心理健康测试题目\n");
            // TODO: 调用增加心理健康测试题目的函数
            break;
        case 10:
            printf("您选择了删除心理健康测试题目\n");
            // TODO: 调用删除心理健康测试题目的函数
            break;
        case 11:
            printf("您选择了显示所有心理健康测试题目\n");
            // TODO: 调用显示所有心理健康测试题目的函数
            break;
        case 12:
            printf("您选择了显示心理健康测试结果\n");
            // TODO: 调用显示心理健康测试结果的函数
            break;
        case 13:
            printf("您选择了对学生心理健康测试结果排序\n");
            // TODO: 调用对学生心理健康测试结果排序的函数
            break;
        case 14:
            printf("您选择了查看并回复学生私聊\n");
            // TODO: 调用查看并回复学生私聊的函数
            break;
        case 0:
            printf("您选择了退出\n");
            break;
        default:
            printf("无效的选择，请重新输入\n");
        }
        system("pause");
    } while (choice != 0);
}
