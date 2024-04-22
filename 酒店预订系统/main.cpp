#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
// 用户结构体定义
typedef struct User
{
    char ID[20];   // 用户ID
    char pwd[20];  // 用户密码
    int authority; // 用户权限: 1前台、2顾客、3管理员
} User;
typedef struct Room
{
    int roomId;        // 房间号
    float area;        // 面积
    float price;       // 价格
    char roomType[20]; // 大床、双床、套房
} Rooms;
typedef struct Customers
{
    char ID[20];   // 用户id
    char type[20]; // 客人类型[大使、铂金、黄金、普通]
    float discout; // 对应折扣
} Customers;
typedef struct Reservation
{
    char CustomerId[20]; // 用户名
    int roomId;          // 房间号
    float price;         // 价格
    char startDate[20];  // 开始日期
    char endDate[20];    // 结束日期
    int isCheckedIn;     // 是否入住
} Reservations;
User users[1000];
int numUsers = 0;
Rooms rooms[100];                // 房间信息数组，假设最多有100个房间
int numRooms = 0;                // 房间数量
Customers customers[1000];       // 客户信息数组，假设最多有1000个客户
int numCustomers = 0;            // 客户数量
Reservations reservations[1000]; // 预订信息数组，假设最多有1000条预订记录
int numReservations = 0;         // 预订数量
User *currentUser = NULL;
// 显示当前时间及问候语
void showTime(int status);

// 从文件加载用户信息
void loadUser();

// 将用户信息保存到文件
void saveUser();

// 主菜单函数
int mainMenu();

// 登录函数
int login();

// 客房管理子菜单函数
void roomManagementMenu();

// 客人管理子菜单函数
void customerManagementMenu();

// 信息统计函数
void informationStatistics();

// 密码维护函数
void passwordMaintenance();

// 信息查询子菜单函数
void informationQueryMenu();

// 信息排序子菜单函数
void informationSortMenu();

// 管理员菜单函数
void adminMenu();

// 入住管理子菜单函数
void checkInManagementMenu();

// 前台菜单函数
void frontDeskMenu();
void customerMenu();
// 显示当前时间及问候语
void showTime(int status)
{
    time_t it = time(NULL);
    struct tm *ptr = localtime(&it);

    // 根据当前时间段输出问候语
    if (ptr->tm_hour >= 5 && ptr->tm_hour < 9)
        printf("早上好！\n");
    else if (ptr->tm_hour >= 9 && ptr->tm_hour < 12)
        printf("上午好！\n");
    else if (ptr->tm_hour >= 12 && ptr->tm_hour < 14)
        printf("中午好！\n");
    else if (ptr->tm_hour >= 14 && ptr->tm_hour < 18)
        printf("下午好！\n");
    else
        printf("晚上好！\n");

    // 显示当前时间
    printf("现在是%4d年%02d月%02d日 %02d:%02d:%02d\n",
           ptr->tm_year + 1900, ptr->tm_mon + 1, ptr->tm_mday, ptr->tm_hour, ptr->tm_min, ptr->tm_sec);

    // 根据状态显示对应信息
    if (status == 1)
        printf("欢迎使用旅馆管理系统！\n");
    else if (status == 0)
        printf("谢谢使用旅馆管理系统！\n");
}
// 从文件加载用户信息
void loadUser()
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        printf("无法打开用户文件。\n");
        return;
    }

    // 逐行读取用户信息并存储到数组中
    while (fscanf(file, "%s %s %d", users[numUsers].ID, users[numUsers].pwd, &users[numUsers].authority) != EOF)
    {
        numUsers++;
    }

    fclose(file);
}

// 将用户信息保存到文件
void saveUser()
{
    FILE *file = fopen("users.txt", "w");
    if (file == NULL)
    {
        printf("无法创建用户文件。\n");
        return;
    }

    // 逐行将用户信息写入文件
    for (int i = 0; i < numUsers; i++)
    {
        fprintf(file, "%s %s %d\n", users[i].ID, users[i].pwd, users[i].authority);
    }

    fclose(file);
}

// 主菜单函数
int mainMenu()
{
    int choice;
    printf("\t\t\t      酒店管理系统\n");
    printf("\t\t\t===============================\n");
    printf("\t\t\t      1 : 前台登录\n");
    printf("\t\t\t      2 : 顾客登录\n");
    printf("\t\t\t      3 : 管理员登录\n");
    printf("\t\t\t      0 : 退出\n");
    printf("\t\t\t===============================\n");
    printf("请选择：\n");
    scanf("%d", &choice);

    // 验证输入有效性
    while (choice < 0 || choice > 3)
    {
        printf("输入错误，请重新输入：\n");
        scanf("%d", &choice);
    }

    // 根据选择调用显示时间函数
    showTime(choice != 0 ? 1 : 0);
    return choice;
}
// 登录函数
int login()
{
    char userID[20];
    char password[20];
    printf("请输入用户ID：");
    scanf("%s", userID);
    printf("请输入密码：");
    scanf("%s", password);

    // 在这里进行用户验证的逻辑，你可以根据用户ID和密码从用户数组中查找用户，并验证用户类型和密码是否匹配
    // 如果验证通过，返回用户类型；否则返回0表示登录失败
    for (int i = 0; i < numUsers; ++i)
    {
        if (strcmp(users[i].ID, userID) == 0 && strcmp(users[i].pwd, password) == 0)
        {
            currentUser = &users[i];
            return users[i].authority;
        }
    }
    // 登录失败
    return 0;
}
void menu()
{
    int choice;

    // 主菜单循环
    do
    {
        system("cls");
        // 显示主菜单并获取用户选择
        choice = mainMenu();

        // 根据用户选择进行相应操作
        switch (choice)
        {
        case 1: // 前台登录
        {
            int userType = login(); // 调用登录函数
            if (userType == 1)
            {

                printf("前台登录成功！\n");
                Sleep(100);
                system("cls");
                frontDeskMenu();
            }
            else
            {
                printf("前台登录失败，请检查用户名和密码。\n");
            }
            break;
        }
        case 2: // 顾客登录
        {
            int userType = login(); // 调用登录函数
            if (userType == 2)
            {
                // 这里可以调用顾客操作的函数
                printf("顾客登录成功！\n");
                Sleep(100);
                system("cls");
                customerMenu();
            }
            else
            {
                printf("顾客登录失败，请检查用户名和密码。\n");
            }
            break;
        }
        case 3: // 管理员登录
        {
            int userType = login(); // 调用登录函数
            if (userType == 3)
            {
                // 这里可以调用管理员操作的函数
                printf("管理员登录成功！\n");
                Sleep(100);
                system("cls");
                adminMenu();
            }
            else
            {
                printf("管理员登录失败，请检查用户名和密码。\n");
            }
            break;
        }
        case 0: // 退出
            printf("正在退出系统...\n");
            break;
        default:
            printf("输入错误，请重新选择。\n");
            break;
        }

    } while (choice != 0);
}
// 客房管理子菜单函数
void roomManagementMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\t客房管理\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : 添加客房\n");
        printf("\t\t      2 : 删除客房\n");
        printf("\t\t      3 : 修改客房信息\n");
        printf("\t\t      4 : 查询客房信息\n");
        printf("\t\t      0 : 返回上级菜单\n");
        printf("\t\t==============================\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            // 添加客房函数调用
            break;
        case 2:
            // 删除客房函数调用
            break;
        case 3:
            // 修改客房信息函数调用
            break;
        case 4:
            // 查询客房信息函数调用
            break;
        case 0:
            printf("返回上级菜单...\n");
            break;
        default:
            printf("输入错误，请重新选择。\n");
            break;
        }
        system("pause");
    } while (choice != 0);
}

// 客人管理子菜单函数
void customerManagementMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\t客人管理\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : 添加客人\n");
        printf("\t\t      2 : 删除客人\n");
        printf("\t\t      3 : 修改客人信息\n");
        printf("\t\t      4 : 查询客人信息\n");
        printf("\t\t      0 : 返回上级菜单\n");
        printf("\t\t==============================\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            // 添加客人函数调用
            break;
        case 2:
            // 删除客人函数调用
            break;
        case 3:
            // 修改客人信息函数调用
            break;
        case 4:
            // 查询客人信息函数调用
            break;
        case 0:
            printf("返回上级菜单...\n");
            break;
        default:
            printf("输入错误，请重新选择。\n");
            break;
        }
        system("pause");
    } while (choice != 0);
}

// 信息统计函数
void informationStatistics()
{
    // 根据你的描述，编写相应的统计功能
}

// 密码维护函数
void passwordMaintenance()
{
    char oldPassword[20];
    char newPassword[20];
    char confirmPassword[20];

    printf("请输入旧密码：");
    scanf("%s", oldPassword);

    // 检查输入的旧密码是否与当前用户的密码匹配
    if (strcmp(oldPassword, currentUser->pwd) != 0)
    {
        printf("旧密码输入错误，请重新操作。\n");
        return;
    }

    printf("请输入新密码：");
    scanf("%s", newPassword);

    printf("请确认新密码：");
    scanf("%s", confirmPassword);

    // 检查两次输入的密码是否匹配
    if (strcmp(newPassword, confirmPassword) != 0)
    {
        printf("两次输入的密码不匹配，请重新操作。\n");
        return;
    }

    // 更新当前客人的密码
    strcpy(currentUser->pwd, newPassword);
}

// 信息查询子菜单函数
void informationQueryMenu()
{
    // 编写信息查询功能
}

// 信息排序子菜单函数
void informationSortMenu()
{
    // 编写信息排序功能
}
// 管理员菜单函数
void adminMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\t管理员菜单\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : 客房管理\n");
        printf("\t\t      2 : 客人管理\n");
        printf("\t\t      3 : 信息查询\n");
        printf("\t\t      4 : 信息排序\n");
        printf("\t\t      5 : 信息统计\n");
        printf("\t\t      6 : 密码维护\n");
        printf("\t\t      0 : 退出\n");
        printf("\t\t==============================\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            roomManagementMenu(); // 进入客房管理子菜单
            break;
        case 2:
            customerManagementMenu(); // 进入客人管理子菜单
            break;
        case 3:
            informationQueryMenu(); // 信息查询
            break;
        case 4:
            informationSortMenu(); // 信息排序
            break;
        case 5:
            informationStatistics(); // 进入信息统计功能
            break;
        case 6:
            passwordMaintenance(); // 进入密码维护功能
            break;
        case 0:
            printf("正在退出管理员菜单...\n");
            break;
        default:
            printf("输入错误，请重新选择。\n");
            break;
        }
        system("pause");
    } while (choice != 0);
}
// 入住管理子菜单函数
void checkInManagementMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\t入住管理\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : 办理入住手续\n");
        printf("\t\t      2 : 分配客房\n");
        printf("\t\t      0 : 返回上级菜单\n");
        printf("\t\t==============================\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            // 办理入住手续函数调用
            break;
        case 2:
            // 分配客房函数调用
            break;
        case 0:
            printf("返回上级菜单...\n");
            break;
        default:
            printf("输入错误，请重新选择。\n");
            break;
        }
        system("pause");
    } while (choice != 0);
}

// 前台菜单函数
void frontDeskMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\t前台菜单\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : 入住管理\n");
        printf("\t\t      2 : 信息查询\n");
        printf("\t\t      3 : 信息排序\n");
        printf("\t\t      4 : 信息统计\n");
        printf("\t\t      5 : 个人信息维护\n");
        printf("\t\t      0 : 退出\n");
        printf("\t\t==============================\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            checkInManagementMenu(); // 进入入住管理子菜单
            break;
        case 2:
            informationQueryMenu(); // 进入信息查询子菜单
            break;
        case 3:
            informationSortMenu(); // 进入信息排序子菜单
            break;
        case 4:
            informationStatistics(); // 进入信息统计子菜单
            break;
        case 5:
            passwordMaintenance();
            break;
        case 0:
            printf("正在退出前台菜单...\n");
            break;
        default:
            printf("输入错误，请重新选择。\n");
            break;
        }
        system("pause");
    } while (choice != 0);
}

int main()
{
    loadUser();
    menu();
    saveUser();
    return 0;
}
void customerQuery()
{
}
void customerSort()
{
}
void customerStatistics()
{
}
void myReservation()
{
}
void customerMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\t客人菜单\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : 信息查询\n");
        printf("\t\t      2 : 信息排序\n");
        printf("\t\t      3 : 信息统计\n");
        printf("\t\t      4 : 预订管理\n");
        printf("\t\t      5 : 密码维护\n");
        printf("\t\t      0 : 退出\n");
        printf("\t\t==============================\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            customerQuery(); // 进入信息查询子菜单
            break;
        case 2:
            customerSort(); // 进入信息排序子菜单
            break;
        case 3:
            customerStatistics();
            break;
        case 4:
            myReservation();
            break;
        case 5:
            passwordMaintenance(); // 进入密码维护功能
            break;
        case 0:
            printf("正在退出客人菜单...\n");
            break;
        default:
            printf("输入错误，请重新选择。\n");
            break;
        }
        system("pause");
    } while (choice != 0);
}
