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

// 房间结构体定义
typedef struct Room
{
    int roomId;        // 房间号
    float area;        // 面积
    float price;       // 价格
    char roomType[20]; // 房间类型
    int isOccupied;    // 是否空闲
    struct Room *next; // 指向下一个房间的指针
} Room;

// 客户结构体定义
typedef struct Customer
{
    char ID[20];           // 客户ID
    char type[20];         // 客户类型
    float discount;        // 折扣
    struct Customer *next; // 指向下一个客户的指针
} Customer;

// 预订结构体定义
typedef struct Reservation
{
    char CustomerId[20];      // 用户名
    int roomId;               // 房间号
    float price;              // 价格
    char startDate[20];       // 开始日期
    char endDate[20];         // 结束日期
    char roomType[20];        // 房间类型
    int isCheckedIn;          // 是否入住
    struct Reservation *next; // 指向下一个预订的指针
} Reservation;
typedef struct RoomOccupancy
{
    int roomId;
    int occupancyCount;
} RoomOccupancy;
typedef struct RoomReservationCount
{
    int roomId;
    int reservationCount;
} RoomReservationCount;
typedef struct CustomerStayCount
{
    char customerId[20];
    int stayCount;
} CustomerStayCount;

// 全局变量：用户数组和当前用户
User users[1000];
int numUsers = 0;
User *currentUser = NULL;

// 全局变量：房间、客户和预订的链表头指针
Room *roomsList = NULL;
Customer *customersList = NULL;
Reservation *reservationsList = NULL;
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
// 添加客房函数声明
void addRoom();
// 删除客房函数声明
void deleteRoom();
// 修改客房信息函数声明
void modifyRoom();
// 查询客房信息函数声明
void queryRoom();
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
// 比较两个日期的先后顺序，如果date1早于date2，返回负数；如果date1晚于date2，返回正数；如果相等，返回0
int compareDates(char *date1, char *date2)
{
    struct tm tm1, tm2;

    sscanf(date1, "%d-%d-%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday);
    sscanf(date2, "%d-%d-%d", &tm2.tm_year, &tm2.tm_mon, &tm2.tm_mday);

    tm1.tm_year -= 1900;
    tm2.tm_year -= 1900;
    tm1.tm_mon -= 1;
    tm2.tm_mon -= 1;

    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);

    if (time1 < time2)
        return -1;
    else if (time1 > time2)
        return 1;
    else
        return 0;
}
void saveRooms()
{
    FILE *file = fopen("rooms.txt", "w");
    if (file == NULL)
    {
        printf("无法创建客房文件。\n");
        return;
    }

    Room *current = roomsList;
    while (current != NULL)
    {
        fprintf(file, "%d %f %f %s %d\n",
                current->roomId, current->area, current->price,
                current->roomType, current->isOccupied);
        current = current->next;
    }

    fclose(file);
}
void loadRooms()
{
    FILE *file = fopen("rooms.txt", "r");
    if (file == NULL)
    {
        printf("无法打开客房文件。\n");
        return;
    }

    Room *current = NULL, *last = NULL;
    while (!feof(file))
    {
        Room *newRoom = (Room *)malloc(sizeof(Room));
        if (fscanf(file, "%d %f %f %s %d",
                   &newRoom->roomId, &newRoom->area, &newRoom->price,
                   newRoom->roomType, &newRoom->isOccupied) == 5)
        {
            newRoom->next = NULL;
            if (last == NULL)
            {
                roomsList = newRoom;
            }
            else
            {
                last->next = newRoom;
            }
            last = newRoom;
        }
        else
        {
            free(newRoom);
            break;
        }
    }

    fclose(file);
}
void saveCustomers()
{
    FILE *file = fopen("customers.txt", "w");
    if (file == NULL)
    {
        printf("无法创建客人文件。\n");
        return;
    }

    Customer *current = customersList;
    while (current != NULL)
    {
        fprintf(file, "%s %s %f\n",
                current->ID, current->type, current->discount);
        current = current->next;
    }

    fclose(file);
}
void loadCustomers()
{
    FILE *file = fopen("customers.txt", "r");
    if (file == NULL)
    {
        printf("无法打开客人文件。\n");
        return;
    }

    Customer *current = NULL, *last = NULL;
    while (!feof(file))
    {
        Customer *newCustomer = (Customer *)malloc(sizeof(Customer));
        if (fscanf(file, "%s %s %f",
                   newCustomer->ID, newCustomer->type, &newCustomer->discount) == 3)
        {
            newCustomer->next = NULL;
            if (last == NULL)
            {
                customersList = newCustomer;
            }
            else
            {
                last->next = newCustomer;
            }
            last = newCustomer;
        }
        else
        {
            free(newCustomer);
            break;
        }
    }

    fclose(file);
}
void saveReservations()
{
    FILE *file = fopen("reservations.txt", "w");
    if (file == NULL)
    {
        printf("无法创建预订文件。\n");
        return;
    }

    Reservation *current = reservationsList;
    while (current != NULL)
    {
        fprintf(file, "%s %d %f %s %s %s %d\n",
                current->CustomerId, current->roomId, current->price,
                current->startDate, current->endDate,
                current->roomType, current->isCheckedIn);
        current = current->next;
    }

    fclose(file);
}
void loadReservations()
{
    FILE *file = fopen("reservations.txt", "r");
    if (file == NULL)
    {
        printf("无法打开预订文件。\n");
        return;
    }

    Reservation *current = NULL, *last = NULL;
    while (!feof(file))
    {
        Reservation *newReservation = (Reservation *)malloc(sizeof(Reservation));
        if (fscanf(file, "%s %d %f %s %s %s %d",
                   newReservation->CustomerId, &newReservation->roomId, &newReservation->price,
                   newReservation->startDate, newReservation->endDate,
                   newReservation->roomType, &newReservation->isCheckedIn) == 7)
        {
            newReservation->next = NULL;
            if (last == NULL)
            {
                reservationsList = newReservation;
            }
            else
            {
                last->next = newReservation;
            }
            last = newReservation;
        }
        else
        {
            free(newReservation);
            break;
        }
    }

    fclose(file);
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
// 添加客房函数
void addRoom()
{
    char choice; // 用于记录用户是否继续添加的选择

    do
    {
        Room *newRoom = (Room *)malloc(sizeof(Room)); // 分配新客房的内存空间

        // 获取用户输入的客房信息
        printf("请输入新客房的信息：\n");
        printf("房间号：");
        scanf("%d", &(newRoom->roomId));
        printf("面积：");
        scanf("%f", &(newRoom->area));

        // 显示房间类型菜单供用户选择
        printf("请选择房间类型：\n");
        printf("1. 大床 (价格：150)\n");
        printf("2. 双床 (价格：200)\n");
        printf("3. 套房 (价格：400)\n");
        int roomTypeChoice;
        scanf("%d", &roomTypeChoice);

        // 根据用户选择设置房间类型和价格
        switch (roomTypeChoice)
        {
        case 1:
            strcpy(newRoom->roomType, "大床");
            newRoom->price = 150;
            break;
        case 2:
            strcpy(newRoom->roomType, "双床");
            newRoom->price = 200;
            break;
        case 3:
            strcpy(newRoom->roomType, "套房");
            newRoom->price = 400;
            break;
        default:
            printf("无效的选项，房间类型默认设置为大床，价格默认设置为150。\n");
            strcpy(newRoom->roomType, "大床");
            newRoom->price = 150;
            break;
        }

        // 设置新客房的下一个指针为当前链表的头指针，将其插入到链表头部
        newRoom->next = roomsList;
        roomsList = newRoom;

        printf("客房添加成功！\n");

        // 询问用户是否继续添加
        printf("是否继续添加客房信息？(y/n): ");
        scanf(" %c", &choice); // 注意这里的空格，用于消耗上一次输入中的换行符

    } while (choice == 'y' || choice == 'Y'); // 如果用户输入 'y' 或 'Y' 则继续添加
}
// 删除客房函数
void deleteRoom()
{
    int roomNumber;
    char choice;

    do
    {
        Room *prev = NULL;
        Room *current = roomsList;

        printf("请输入要删除的客房的房间号：");
        scanf("%d", &roomNumber);

        // 遍历链表找到要删除的房间
        while (current != NULL && current->roomId != roomNumber)
        {
            prev = current;
            current = current->next;
        }

        // 如果找到了要删除的房间
        if (current != NULL)
        {
            // 如果要删除的是链表的第一个节点
            if (prev == NULL)
            {
                roomsList = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            // 释放内存
            free(current);
            printf("客房删除成功！\n");
        }
        else
        {
            printf("未找到该房间号的客房。\n");
        }

        // 询问用户是否继续删除
        printf("是否继续删除客房？(y/n): ");
        scanf(" %c", &choice); // 注意这里的空格，用于消耗上一次输入中的换行符

    } while (choice == 'y' || choice == 'Y'); // 如果用户输入 'y' 或 'Y' 则继续删除
}
// 修改客房信息函数
void modifyRoom()
{
    int roomNumber;
    char choice;

    do
    {
        Room *current = roomsList;

        printf("请输入要修改信息的客房的房间号：");
        scanf("%d", &roomNumber);

        // 遍历链表找到要修改信息的客房
        while (current != NULL && current->roomId != roomNumber)
        {
            current = current->next;
        }

        // 如果找到了要修改信息的客房
        if (current != NULL)
        {
            int fieldChoice;
            printf("请选择要修改的字段：\n");
            printf("1. 面积\n");
            printf("2. 价格\n");
            printf("3. 房间类型\n");
            printf("请选择：");
            scanf("%d", &fieldChoice);

            // 根据用户选择更新相应的信息
            switch (fieldChoice)
            {
            case 1:
                printf("请输入新的面积：");
                scanf("%f", &(current->area));
                break;
            case 2:
                printf("请输入新的价格：");
                scanf("%f", &(current->price));
                break;
            case 3:
                printf("请输入新的房间类型：");
                scanf("%s", current->roomType);
                break;
            default:
                printf("无效的选项。\n");
                break;
            }

            printf("客房信息修改成功！\n");
        }
        else
        {
            printf("未找到该房间号的客房。\n");
        }

        // 询问用户是否继续修改
        printf("是否继续修改客房信息？(y/n): ");
        scanf(" %c", &choice); // 注意这里的空格，用于消耗上一次输入中的换行符

    } while (choice == 'y' || choice == 'Y'); // 如果用户输入 'y' 或 'Y' 则继续修改
}
// 简单查询客房函数
void simpleSearchRoom()
{
    int roomNumber;
    printf("请输入要查询的客房房间号：");
    scanf("%d", &roomNumber);

    // 在链表中查找房间号匹配的客房并输出信息
    Room *current = roomsList;
    while (current != NULL)
    {
        if (current->roomId == roomNumber)
        {
            // 输出客房信息
            printf("房间号：%d\n", current->roomId);
            printf("面积：%f\n", current->area);
            printf("价格：%f\n", current->price);
            printf("房间类型：%s\n", current->roomType);
            printf("\n");
            return; // 查询到客房后直接返回
        }
        current = current->next;
    }

    // 如果没有找到匹配的客房
    printf("未找到房间号为%d的客房。\n", roomNumber);
}
// 模糊查询客房函数
void fuzzySearchRoom()
{
    char keyword[100];
    printf("请输入房间类型有关查询关键词：");
    scanf("%s", keyword);

    // 在链表中查找包含关键词的客房并输出信息
    Room *current = roomsList;
    while (current != NULL)
    {
        // 使用strstr函数判断房间类型中是否包含关键词
        if (strstr(current->roomType, keyword) != NULL)
        {
            // 输出客房信息
            printf("房间号：%d\n", current->roomId);
            printf("面积：%f\n", current->area);
            printf("价格：%f\n", current->price);
            printf("房间类型：%s\n", current->roomType);
            printf("\n");
        }
        current = current->next;
    }
}
// 查询客房是否空（某时间段）
void checkRoomAvailability()
{
    char startDate[20];
    char endDate[20];
    int roomNumber;

    printf("请输入要查询的客房房间号：");
    scanf("%d", &roomNumber);
    printf("请输入起始日期（格式：YYYY-MM-DD）：");
    scanf("%s", startDate);
    printf("请输入结束日期（格式：YYYY-MM-DD）：");
    scanf("%s", endDate);

    // 在预订信息中查找是否有与指定房间和时间段重叠的预订记录
    for (Reservation *currentReservation = reservationsList; currentReservation != NULL; currentReservation = currentReservation->next)
    {
        if (currentReservation->roomId == roomNumber)
        {
            // 如果时间段有重叠，则客房不空
            if ((compareDates(currentReservation->startDate, endDate) < 0 || compareDates(currentReservation->endDate, startDate) > 0) && currentReservation->isCheckedIn == 1)
            {
                printf("客房房间号为%d在指定时间段内不空。\n", roomNumber);
                return;
            }
        }
    }

    // 没有找到重叠的预订记录，则客房空
    printf("客房房间号为%d在指定时间段内空闲。\n", roomNumber);
}

// 查询某客人预订信息
void searchCustomerReservation()
{
    char customerID[20];

    printf("请输入要查询预订信息的客人ID：");
    scanf("%s", customerID);

    // 在预订信息中查找与指定客人ID匹配的预订记录
    printf("客人ID为%s的预订信息如下：\n", customerID);
    for (Reservation *currentReservation = reservationsList; currentReservation != NULL; currentReservation = currentReservation->next)
    {
        if (strcmp(currentReservation->CustomerId, customerID) == 0)
        {
            printf("房间号：%d\n", currentReservation->roomId);
            printf("价格：%f\n", currentReservation->price);
            printf("开始日期：%s\n", currentReservation->startDate);
            printf("结束日期：%s\n", currentReservation->endDate);
            printf("是否入住：%s\n", currentReservation->isCheckedIn ? "是" : "否");
            printf("\n");
        }
    }
}
// 显示所有房间信息函数
void displayAllRooms()
{
    printf("房间号\t\t面积\t\t价格\t\t房间类型\n");
    printf("===============================================================\n");

    Room *currentRoom = roomsList;
    while (currentRoom != NULL)
    {
        printf("%d\t\t%.2f\t\t%.2f\t\t%s\n", currentRoom->roomId, currentRoom->area, currentRoom->price, currentRoom->roomType, currentRoom->isOccupied ? "已占用" : "空闲");
        currentRoom = currentRoom->next;
    }
}

// 查询客房函数
void queryRoom()
{
    int choice;

    do
    {
        printf("请选择查询方式：\n");
        printf("1. 简单查询客房信息\n");
        printf("2. 模糊查询客房信息\n");
        printf("3. 查询客房是否空（某时间段）\n");
        printf("4. 查询客人预订信息\n");
        printf("0. 返回上级菜单\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            simpleSearchRoom(); // 简单查询
            break;
        case 2:
            fuzzySearchRoom(); // 模糊查询
            break;
        case 3:
            checkRoomAvailability(); // 查询客房是否空
            break;
        case 4:
            searchCustomerReservation(); // 查询客人预订信息
            break;
        case 0:
            printf("返回上级菜单...\n");
            break;
        default:
            printf("无效的选项，请重新选择。\n");
            break;
        }

    } while (choice != 0);
}

// 客房管理菜单函数
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
        printf("\t\t      5 : 显示所有客房信息\n");
        printf("\t\t      0 : 返回上级菜单\n");
        printf("\t\t==============================\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addRoom(); // 添加客房函数调用
            break;
        case 2:
            deleteRoom(); // 删除客房函数调用
            break;
        case 3:
            modifyRoom(); // 修改客房信息函数调用
            break;
        case 4:
            queryRoom(); // 查询客房信息函数调用
            break;
        case 5:
            displayAllRooms();
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
// 添加客人函数
void addCustomer()
{
    Customer *newCustomer = (Customer *)malloc(sizeof(Customer));
    if (newCustomer == NULL)
    {
        printf("内存分配失败。\n");
        return;
    }

    printf("请输入客人ID：");
    scanf("%s", newCustomer->ID);
    printf("请输入客人类型：");
    scanf("%s", newCustomer->type);
    printf("请输入客人折扣：");
    scanf("%f", &newCustomer->discount);

    // 将新客人节点插入到链表头部
    newCustomer->next = customersList;
    customersList = newCustomer;

    printf("客人添加成功！\n");
}

// 删除客人函数
void deleteCustomer()
{
    char customerID[20];
    printf("请输入要删除的客人ID：");
    scanf("%s", customerID);

    Customer *prevCustomer = NULL;
    Customer *currentCustomer = customersList;

    // 遍历链表查找要删除的客人节点
    while (currentCustomer != NULL && strcmp(currentCustomer->ID, customerID) != 0)
    {
        prevCustomer = currentCustomer;
        currentCustomer = currentCustomer->next;
    }

    // 如果找到了要删除的客人节点
    if (currentCustomer != NULL)
    {
        // 如果要删除的是链表头部节点
        if (prevCustomer == NULL)
        {
            customersList = currentCustomer->next;
        }
        else
        {
            prevCustomer->next = currentCustomer->next;
        }

        free(currentCustomer);
        printf("客人删除成功！\n");
    }
    else
    {
        printf("未找到要删除的客人。\n");
    }
}
// 修改客人信息函数
void modifyCustomer()
{
    char customerID[20];
    printf("请输入要修改信息的客人ID：");
    scanf("%s", customerID);

    Customer *currentCustomer = customersList;

    // 遍历链表查找要修改信息的客人节点
    while (currentCustomer != NULL && strcmp(currentCustomer->ID, customerID) != 0)
    {
        currentCustomer = currentCustomer->next;
    }

    // 如果找到了要修改信息的客人节点
    if (currentCustomer != NULL)
    {
        int choice;
        printf("请选择要修改的字段：\n");
        printf("1. 客人类型\n");
        printf("2. 客人折扣\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("请选择客人类型：\n");
            printf("1. 白银\n");
            printf("2. 黄金\n");
            printf("3. 白金\n");
            printf("4. 大使\n");
            printf("请选择：");
            int typeChoice;
            scanf("%d", &typeChoice);
            switch (typeChoice)
            {
            case 1:
                strcpy(currentCustomer->type, "白银");
                currentCustomer->discount = 0.9; // 白银折扣
                break;
            case 2:
                strcpy(currentCustomer->type, "黄金");
                currentCustomer->discount = 0.85; // 黄金折扣
                break;
            case 3:
                strcpy(currentCustomer->type, "白金");
                currentCustomer->discount = 0.8; // 白金折扣
                break;
            case 4:
                strcpy(currentCustomer->type, "大使");
                currentCustomer->discount = 0.75; // 大使折扣
                break;
            default:
                printf("输入错误，请重新选择。\n");
                return;
            }
            break;
        case 2:
            printf("请输入客人折扣：");
            scanf("%f", &currentCustomer->discount);
            break;
        default:
            printf("输入错误，请重新选择。\n");
            return;
        }

        printf("客人信息修改成功！\n");
    }
    else
    {
        printf("未找到要修改信息的客人。\n");
    }
}

// 查询客人信息函数
void searchCustomer()
{
    char customerID[20];
    printf("请输入要查询信息的客人ID：");
    scanf("%s", customerID);

    Customer *currentCustomer = customersList;

    // 遍历链表查找要查询信息的客人节点
    while (currentCustomer != NULL && strcmp(currentCustomer->ID, customerID) != 0)
    {
        currentCustomer = currentCustomer->next;
    }

    // 如果找到了要查询信息的客人节点
    if (currentCustomer != NULL)
    {
        printf("客人ID：%s\n", currentCustomer->ID);
        printf("客人类型：%s\n", currentCustomer->type);
        printf("客人折扣：%f\n", currentCustomer->discount);
    }
    else
    {
        printf("未找到要查询信息的客人。\n");
    }
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
            addCustomer();
            break;
        case 2:
            deleteCustomer();
            break;
        case 3:
            modifyCustomer();
            break;
        case 4:
            searchCustomer();
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

// 计算两个日期之间的天数差异的函数
int dateDifference(const char *start, const char *end)
{
    struct tm tmStart = {0}, tmEnd = {0};
    int year, month, day;

    // 解析开始日期
    sscanf(start, "%d-%d-%d", &year, &month, &day);
    tmStart.tm_year = year - 1900; // tm_year是自1900年以来的年数
    tmStart.tm_mon = month - 1;    // tm_mon是零基索引的（1月为0）
    tmStart.tm_mday = day;

    // 解析结束日期
    sscanf(end, "%d-%d-%d", &year, &month, &day);
    tmEnd.tm_year = year - 1900;
    tmEnd.tm_mon = month - 1;
    tmEnd.tm_mday = day;

    // 设置小时信息为0
    tmStart.tm_hour = 0;
    tmEnd.tm_hour = 0;

    // 转换为time_t
    time_t tStart = mktime(&tmStart);
    time_t tEnd = mktime(&tmEnd);

    // 确保日期有效
    if (tStart == (time_t)(-1) || tEnd == (time_t)(-1))
    {
        return -1; // 如果日期无效，则返回-1或适当的错误代码
    }

    // 计算天数差异
    return (int)((tEnd - tStart) / (24 * 3600));
}

// 主统计函数
void informationStatistics()
{
    char startDate[11], endDate[11];
    printf("请输入统计的开始日期 (YYYY-MM-DD): ");
    scanf("%10s", startDate);
    printf("请输入统计的结束日期 (YYYY-MM-DD): ");
    scanf("%10s", endDate);

    int totalDays = dateDifference(startDate, endDate) + 1;
    if (totalDays <= 0)
    {
        printf("结束日期必须大于开始日期。\n");
        return;
    }

    int totalRooms = 0;
    Room *roomIter = roomsList;
    while (roomIter != NULL)
    {
        totalRooms++;
        roomIter = roomIter->next;
    }

    if (totalRooms == 0)
    {
        printf("酒店内没有房间数据。\n");
        return;
    }

    int totalRoomDays = totalDays * totalRooms;
    int bookedRoomDays = 0;
    int occupiedRoomDays = 0;

    Reservation *resIter = reservationsList;
    while (resIter != NULL)
    {
        if (compareDates(resIter->startDate, endDate) <= 0 && compareDates(resIter->endDate, startDate) >= 0)
        {
            // 计算预订覆盖的时间段内的实际天数
            char effectiveStart[11], effectiveEnd[11];
            strcpy(effectiveStart, (compareDates(resIter->startDate, startDate) < 0) ? startDate : resIter->startDate);
            strcpy(effectiveEnd, (compareDates(resIter->endDate, endDate) > 0) ? endDate : resIter->endDate);
            int coveredDays = dateDifference(effectiveStart, effectiveEnd) + 1;

            bookedRoomDays += coveredDays; // 预订天数累加
            if (resIter->isCheckedIn)
            {
                occupiedRoomDays += coveredDays; // 入住天数累加
            }
        }
        resIter = resIter->next;
    }

    double bookingRate = (double)bookedRoomDays / totalRoomDays * 100.0;
    double occupancyRate = (double)occupiedRoomDays / totalRoomDays * 100.0;

    printf("统计时间段：%s 至 %s\n", startDate, endDate);
    printf("房间总数：%d，统计总天数：%d\n", totalRooms, totalDays);
    printf("预订率：%.2f%%\n", bookingRate);
    printf("入住率：%.2f%%\n", occupancyRate);
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
// 信息排序子菜单函数
void informationQueryMenu()
{
    queryRoom();
}
// 功能：计算每个房间的入住次数，并按入住次数降序排序房间
void sortByRoomOccupancy()
{
    if (roomsList == NULL)
    {
        printf("没有房间信息可供排序。\n");
        return;
    }

    // 第一步：计算每个房间的入住次数
    Room *roomPtr = roomsList;
    int roomCount = 0;
    while (roomPtr != NULL)
    {
        roomCount++;
        roomPtr = roomPtr->next;
    }

    // 创建数组来存储房间及其入住次数
    RoomOccupancy *occupancies = (RoomOccupancy *)malloc(sizeof(RoomOccupancy) * roomCount);
    roomPtr = roomsList;
    int index = 0;
    while (roomPtr != NULL)
    {
        occupancies[index].roomId = roomPtr->roomId;
        occupancies[index].occupancyCount = 0; // 初始化入住次数为0

        // 遍历预订列表，统计入住次数
        Reservation *resPtr = reservationsList;
        while (resPtr != NULL)
        {
            if (resPtr->roomId == roomPtr->roomId && resPtr->isCheckedIn)
            {
                occupancies[index].occupancyCount++;
            }
            resPtr = resPtr->next;
        }
        roomPtr = roomPtr->next;
        index++;
    }

    // 第二步：使用插入排序算法按入住次数降序排序
    for (int i = 1; i < roomCount; i++)
    {
        RoomOccupancy temp = occupancies[i];
        int j = i - 1;
        while (j >= 0 && occupancies[j].occupancyCount < temp.occupancyCount)
        {
            occupancies[j + 1] = occupancies[j];
            j--;
        }
        occupancies[j + 1] = temp;
    }

    // 打印排序后的房间信息
    printf("房间号\t入住次数\n");
    for (int i = 0; i < roomCount; i++)
    {
        printf("%d\t%d\n", occupancies[i].roomId, occupancies[i].occupancyCount);
    }

    // 释放内存
    free(occupancies);
}

// 功能：按房间的预约次数排序
void sortByReservationCount()
{
    if (roomsList == NULL)
    {
        printf("没有房间信息可供排序。\n");
        return;
    }

    // 第一步：计算每个房间的预约次数
    Room *roomPtr = roomsList;
    int roomCount = 0;
    while (roomPtr != NULL)
    {
        roomCount++;
        roomPtr = roomPtr->next;
    }

    // 创建数组来存储房间及其预约次数
    RoomReservationCount *reservationCounts = (RoomReservationCount *)malloc(sizeof(RoomReservationCount) * roomCount);
    if (!reservationCounts)
    {
        printf("内存分配失败。\n");
        return;
    }

    roomPtr = roomsList;
    int index = 0;
    while (roomPtr != NULL)
    {
        reservationCounts[index].roomId = roomPtr->roomId;
        reservationCounts[index].reservationCount = 0; // 初始化预约次数为0

        // 遍历预订列表，统计每个房间的预约次数
        Reservation *resPtr = reservationsList;
        while (resPtr != NULL)
        {
            if (resPtr->roomId == roomPtr->roomId)
            {
                reservationCounts[index].reservationCount++;
            }
            resPtr = resPtr->next;
        }
        roomPtr = roomPtr->next;
        index++;
    }

    // 第二步：使用插入排序算法按预约次数降序排序
    for (int i = 1; i < roomCount; i++)
    {
        RoomReservationCount temp = reservationCounts[i];
        int j = i - 1;
        while (j >= 0 && reservationCounts[j].reservationCount < temp.reservationCount)
        {
            reservationCounts[j + 1] = reservationCounts[j];
            j--;
        }
        reservationCounts[j + 1] = temp;
    }

    // 打印排序后的房间信息
    printf("房间号\t预约次数\n");
    for (int i = 0; i < roomCount; i++)
    {
        printf("%d\t%d\n", reservationCounts[i].roomId, reservationCounts[i].reservationCount);
    }

    // 释放内存
    free(reservationCounts);
}

// 功能：按客人住店次数排序
void sortByCustomerStays()
{
    if (customersList == NULL)
    {
        printf("没有客人信息可供排序。\n");
        return;
    }

    // 第一步：计算每个客人的住店次数
    Customer *customerPtr = customersList;
    int customerCount = 0;
    while (customerPtr != NULL)
    {
        customerCount++;
        customerPtr = customerPtr->next;
    }

    // 创建数组来存储客人及其住店次数
    CustomerStayCount *stayCounts = (CustomerStayCount *)malloc(sizeof(CustomerStayCount) * customerCount);
    if (!stayCounts)
    {
        printf("内存分配失败。\n");
        return;
    }

    customerPtr = customersList;
    int index = 0;
    while (customerPtr != NULL)
    {
        strcpy(stayCounts[index].customerId, customerPtr->ID);
        stayCounts[index].stayCount = 0; // 初始化住店次数为0

        // 遍历预订列表，统计每个客人的入住次数
        Reservation *resPtr = reservationsList;
        while (resPtr != NULL)
        {
            if (strcmp(resPtr->CustomerId, customerPtr->ID) == 0 && resPtr->isCheckedIn)
            {
                stayCounts[index].stayCount++;
            }
            resPtr = resPtr->next;
        }
        customerPtr = customerPtr->next;
        index++;
    }

    // 第二步：使用插入排序算法按住店次数降序排序
    for (int i = 1; i < customerCount; i++)
    {
        CustomerStayCount temp = stayCounts[i];
        int j = i - 1;
        while (j >= 0 && stayCounts[j].stayCount < temp.stayCount)
        {
            stayCounts[j + 1] = stayCounts[j];
            j--;
        }
        stayCounts[j + 1] = temp;
    }

    // 打印排序后的客人信息
    printf("客人ID\t住店次数\n");
    for (int i = 0; i < customerCount; i++)
    {
        printf("%s\t%d\n", stayCounts[i].customerId, stayCounts[i].stayCount);
    }

    // 释放内存
    free(stayCounts);
}

// 信息排序子菜单函数
void informationSortMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\t信息排序\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : 按房间入住次数排序\n");
        printf("\t\t      2 : 按预约次数排序\n");
        printf("\t\t      3 : 按客人住店次数排序\n");
        printf("\t\t      0 : 返回上级菜单\n");
        printf("\t\t==============================\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            sortByRoomOccupancy(); // 按房间入住次数排序
            break;
        case 2:
            sortByReservationCount(); // 按预约次数排序
            break;
        case 3:
            sortByCustomerStays(); // 按客人住店次数排序
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
// 办理入住手续函数
void checkInProcedure()
{
    char customerID[20];
    char startDate[20], endDate[20];
    int roomTypeChoice, roomFound = 0;
    Room *selectedRoom = NULL;

    printf("请输入客人ID：");
    scanf("%s", customerID);

    // 检查是否存在该客人预订信息
    Reservation *currentReservation = reservationsList;
    while (currentReservation != NULL)
    {
        if (strcmp(currentReservation->CustomerId, customerID) == 0 && !currentReservation->isCheckedIn)
        {
            // 找到了该客人的预订信息且未入住，分配预订的房间号
            printf("该客人已预订房间，以下是预约信息：\n");
            printf("客人ID：%s\n", currentReservation->CustomerId);
            printf("房间号：%d\n", currentReservation->roomId);
            printf("价格：%.2f\n", currentReservation->price);
            printf("预约开始时间：%s\n", currentReservation->startDate);
            printf("预约结束时间：%s\n", currentReservation->endDate);
            printf("是否入住：%s\n", currentReservation->isCheckedIn ? "是" : "否");

            currentReservation->isCheckedIn = 1; // 标记为已入住
            roomFound = 1;

            // 标记该房间为已入住状态
            Room *currentRoom = roomsList;
            while (currentRoom != NULL)
            {
                if (currentRoom->roomId == currentReservation->roomId)
                {
                    currentRoom->isOccupied = 1;
                    break;
                }
                currentRoom = currentRoom->next;
            }
            printf("入住预约房间成功!\n");
            return;
        }
        currentReservation = currentReservation->next;
    }

    if (!roomFound)
    {
        // 没有预订信息，进行新的房间分配
        printf("未找到预订信息。请进行新的房间分配。\n");
        printf("请输入起始日期（格式：YYYY-MM-DD）：");
        scanf("%s", startDate);
        printf("请输入结束日期（格式：YYYY-MM-DD）：");
        scanf("%s", endDate);

        printf("请选择房间类型：\n");
        printf("1. 大床\n");
        printf("2. 双床\n");
        printf("3. 套房\n");
        printf("请选择：");
        scanf("%d", &roomTypeChoice);

        char roomType[20];
        switch (roomTypeChoice)
        {
        case 1:
            strcpy(roomType, "大床");
            break;
        case 2:
            strcpy(roomType, "双床");

            break;
        case 3:
            strcpy(roomType, "套房");
            break;
        default:
            printf("无效的选项，选择默认为大床。\n");
            strcpy(roomType, "大床");
            break;
        }

        Room *currentRoom = roomsList;
        while (currentRoom != NULL)
        {
            if (strcmp(currentRoom->roomType, roomType) == 0 && !currentRoom->isOccupied)
            {
                // 检查是否有与此时间段重叠的预订
                int available = 1;
                Reservation *checkRes = reservationsList;
                while (checkRes != NULL)
                {
                    if (checkRes->roomId == currentRoom->roomId &&
                        !(compareDates(checkRes->endDate, startDate) < 0 || compareDates(checkRes->startDate, endDate) > 0))
                    {
                        available = 0;
                        break;
                    }
                    checkRes = checkRes->next;
                }

                if (available)
                {
                    currentRoom->isOccupied = 1; // 标记房间为已入住
                    printf("分配客房成功，房间号：%d\n", currentRoom->roomId);
                    roomFound = 1;

                    // 添加到预订列表
                    Reservation *newReservation = (Reservation *)malloc(sizeof(Reservation));
                    if (newReservation != NULL)
                    {
                        strcpy(newReservation->CustomerId, customerID);
                        newReservation->roomId = currentRoom->roomId;
                        newReservation->price = currentRoom->price; // 假定价格不变
                        strcpy(newReservation->startDate, startDate);
                        strcpy(newReservation->endDate, endDate);
                        newReservation->isCheckedIn = 1; // 已经入住
                        newReservation->next = reservationsList;
                        reservationsList = newReservation;
                    }
                    break; // 找到可用房间，结束循环
                }
            }
            currentRoom = currentRoom->next;
        }

        if (!roomFound)
        {
            // 没有可用房间
            printf("抱歉，当前没有符合条件的空闲客房。\n");
        }
    }
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
        printf("\t\t      1 : 办理入住\n");
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
            checkInProcedure();
            break;
        case 2:
            informationQueryMenu();
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
    loadCustomers();
    loadRooms();
    loadReservations();
    menu();
    saveUser();
    saveCustomers();
    saveRooms();
    saveReservations();
    return 0;
}
void customerQuery()
{
    char startDate[20], endDate[20], roomType[20];
    int typeFound = 0;

    printf("请输入查询的起始日期（格式：YYYY-MM-DD）：");
    scanf("%s", startDate);
    printf("请输入查询的结束日期（格式：YYYY-MM-DD）：");
    scanf("%s", endDate);
    printf("请输入房间类型（如：大床、双床、套房）：");
    scanf("%s", roomType);

    printf("\n可预订的房间列表如下：\n");
    printf("%-10s %-10s %-15s %-10s\n", "房间号", "房间类型", "价格", "面积"); // 标题头，左对齐

    Room *currentRoom = roomsList;
    while (currentRoom != NULL)
    {
        if (strcmp(currentRoom->roomType, roomType) == 0 && !currentRoom->isOccupied)
        {
            int roomAvailable = 1; // 假设房间可用

            // 遍历预订列表以检查房间在指定时间是否被预定
            Reservation *currentReservation = reservationsList;
            while (currentReservation != NULL)
            {
                if (currentReservation->roomId == currentRoom->roomId &&
                    !(compareDates(currentReservation->endDate, startDate) < 0 || compareDates(currentReservation->startDate, endDate) > 0))
                {
                    roomAvailable = 0; // 房间在所查询时间段已被预订
                    break;
                }
                currentReservation = currentReservation->next;
            }

            if (roomAvailable)
            {
                printf("%-10d %-10s %-15.2f %-10.2f\n", currentRoom->roomId, currentRoom->roomType, currentRoom->price, currentRoom->area);
                typeFound = 1;
            }
        }
        currentRoom = currentRoom->next;
    }

    if (!typeFound)
    {
        printf("没有找到符合条件的空闲客房。\n");
    }
}
// 插入排序函数，按用户选择排序房间
void customerSort()
{
    if (roomsList == NULL || roomsList->next == NULL)
    {
        return; // 列表为空或只有一个元素无需排序
    }

    Room *sorted = NULL;       // 新的排序后的链表头指针
    Room *current = roomsList; // 当前遍历到的原链表节点
    Room *next = NULL;
    int sortType;
    // 用户选择排序类型
    printf("请选择排序类型：\n");
    printf("1. 按房间面积排序\n");
    printf("2. 按房间价格排序\n");
    printf("请输入选择（1或2）：");
    scanf("%d", &sortType);
    while (current != NULL)
    {
        next = current->next; // 保存下一个节点

        // 插入排序的插入操作
        if (sorted == NULL || (sortType == 1 && sorted->area >= current->area) || (sortType == 2 && sorted->price >= current->price))
        {
            // 插入到排序链表的头部
            current->next = sorted;
            sorted = current;
        }
        else
        {
            // 寻找插入位置
            Room *temp = sorted;
            while (temp->next != NULL && ((sortType == 1 && temp->next->area < current->area) || (sortType == 2 && temp->next->price < current->price)))
            {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }

        current = next; // 移动到下一个元素
    }

    roomsList = sorted; // 更新房间链表头指针
    printf("排序已经完成!\n");
    displayAllRooms();
}
// 统计函数，用于统计入住信息
void customerStatistics()
{
    char startDate[20], endDate[20];
    printf("请输入统计的起始日期（格式：YYYY-MM-DD）：");
    scanf("%s", startDate);
    printf("请输入统计的结束日期（格式：YYYY-MM-DD）：");
    scanf("%s", endDate);

    int totalReservations = 0;         // 总入住次数
    int periodReservations = 0;        // 指定时间段内的入住次数
    int roomTypeCounts[3] = {0, 0, 0}; // 房间类型计数（假设为：大床、双床、套房）

    // 遍历预订列表计算统计数据
    for (Reservation *res = reservationsList; res != NULL; res = res->next)
    {
        if (!strcmp(res->CustomerId, currentUser->ID))
        {
            totalReservations++; // 累加总入住次数
            // 检查预订是否在指定日期范围内
            if (compareDates(res->startDate, endDate) <= 0 && compareDates(res->endDate, startDate) >= 0)
            {
                periodReservations++; // 累加符合时间段的入住次数
            }

            // 根据房间类型累加计数
            if (strcmp(res->roomType, "大床") == 0)
            {
                roomTypeCounts[0]++;
            }
            else if (strcmp(res->roomType, "双床") == 0)
            {
                roomTypeCounts[1]++;
            }
            else if (strcmp(res->roomType, "套房") == 0)
            {
                roomTypeCounts[2]++;
            }
        }
    }
    printf("%s,您好!\n", currentUser->ID);
    printf("总入住次数：%d\n", totalReservations);
    printf("在 %s 到 %s 的时间段内入住次数：%d\n", startDate, endDate, periodReservations);
    printf("大床房入住次数：%d\n", roomTypeCounts[0]);
    printf("双床房入住次数：%d\n", roomTypeCounts[1]);
    printf("套房入住次数：%d\n", roomTypeCounts[2]);
}
// 预约房间函数
void myReservation()
{
    // 创建新的预约节点
    Reservation *newReservation = (Reservation *)malloc(sizeof(Reservation));
    if (newReservation == NULL)
    {
        printf("内存分配失败\n");
        return;
    }

    // 输入预约信息
    printf("请确认您的用户名：%s\n", currentUser->ID);
    strcpy(newReservation->CustomerId, currentUser->ID);
    displayAllRooms(); // 显示所有房间信息供用户选择
    printf("请输入需要预约的房间号：");
    scanf("%d", &(newReservation->roomId));
    // 查找房间是否存在
    Room *currentRoom = roomsList;
    int roomFound = 0;
    while (currentRoom != NULL)
    {
        if (currentRoom->roomId == newReservation->roomId)
        {
            roomFound = 1;
            strcpy(newReservation->roomType, currentRoom->roomType);
            break;
        }
        currentRoom = currentRoom->next;
    }
    if (!roomFound)
    {
        printf("未找到该房间号，请重新输入。\n");
        free(newReservation);
        return;
    }
    printf("请输入预约的开始日期（格式：YYYY-MM-DD）：");
    scanf("%s", newReservation->startDate);
    printf("请输入预约的结束日期（格式：YYYY-MM-DD）：");
    scanf("%s", newReservation->endDate);

    // 检查房间是否存在预约记录
    Reservation *currentReservation = reservationsList;
    while (currentReservation != NULL)
    {
        if (currentReservation->roomId == newReservation->roomId &&
            compareDates(currentReservation->endDate, newReservation->startDate) >= 0 &&
            compareDates(currentReservation->startDate, newReservation->endDate) <= 0)
        {
            printf("房间该时段已被预约，请选择其他房间或调整预约日期。\n");
            free(newReservation);
            return;
        }
        currentReservation = currentReservation->next;
    }
    // 将预约信息插入到预约链表头部
    newReservation->isCheckedIn = 0; // 尚未入住
    newReservation->next = reservationsList;
    reservationsList = newReservation;
    printf("预约成功！\n");
}
// 取消预订函数
void cancelReservation()
{
    if (reservationsList == NULL)
    {
        printf("当前无预订信息。\n");
        return;
    }

    char customerId[20];
    strcpy(customerId, currentUser->ID);
    Reservation *currentReservation = reservationsList;
    Reservation *prevReservation = NULL;
    int count = 0; // 计数器，用于记录预订信息的序号

    // 打印本人所有预订信息
    printf("以下是您的所有预订信息：\n");
    while (currentReservation != NULL)
    {
        if (strcmp(currentReservation->CustomerId, customerId) == 0)
        {
            printf("%d. 房间号：%d，开始日期：%s，结束日期：%s\n", count + 1, currentReservation->roomId, currentReservation->startDate, currentReservation->endDate);
            count++;
        }
        currentReservation = currentReservation->next;
    }

    if (count == 0)
    {
        printf("未找到与该用户名相关的预订信息。\n");
        return;
    }

    int selection;
    printf("请输入要取消预订的信息序号：");
    scanf("%d", &selection);

    if (selection < 1 || selection > count)
    {
        printf("输入无效，请输入正确的序号。\n");
        return;
    }

    // 重新遍历链表，找到要取消的预订信息
    currentReservation = reservationsList;
    prevReservation = NULL;
    count = 0; // 重置计数器

    // 找到要取消的预订信息
    while (currentReservation != NULL)
    {
        if (strcmp(currentReservation->CustomerId, customerId) == 0)
        {
            count++;
            if (count == selection)
            {
                if (prevReservation == NULL)
                {
                    // 如果要取消的预订信息是第一个节点
                    reservationsList = currentReservation->next;
                }
                else
                {
                    // 如果要取消的预订信息不是第一个节点
                    prevReservation->next = currentReservation->next;
                }

                printf("已取消预订的房间号：%d\n", currentReservation->roomId);
                free(currentReservation);
                printf("预订取消成功！\n");
                return;
            }
        }
        prevReservation = currentReservation;
        currentReservation = currentReservation->next;
    }
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
        printf("\t\t      4 : 预订房间\n");
        printf("\t\t      5 : 取消预订\n");
        printf("\t\t      6 : 密码维护\n");
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
            cancelReservation();
            break;
        case 6:
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
