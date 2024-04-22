#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILE_NAME "guards.txt"
#define MAX_GUARDS 7
#define DAYS_IN_WEEK 7
const char *days[DAYS_IN_WEEK] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};
typedef struct
{
    int id;
    char name[50];
    int off_days[DAYS_IN_WEEK]; // 1 表示休息，0 表示工作
} Guard;

void addGuard();
void modifyGuard();
void deleteGuard();
void findGuard();
void setOffDay();
void printAllSchedules(int numGuards);
void listGuards();
int loadData(Guard guards[]);
void saveData(Guard guards[], int size);
int getNewID(Guard guards[], int size);
void printScheduleTable();
void printMenu();
int main()
{
    int choice;

    do
    {
        printMenu();
        scanf("%d", &choice);
        getchar(); // 清除缓冲区的换行符

        switch (choice)
        {
        case 1:
            addGuard();
            break;
        case 2:
            modifyGuard();
            break;
        case 3:
            deleteGuard();
            break;
        case 4:
            findGuard();
            break;
        case 5:
            setOffDay();
            break;
        case 6:
            listGuards();
            break;
        case 7:
            printAllSchedules(MAX_GUARDS);
            break;
        case 8:
            printScheduleTable();
            break;
        case 9:
            printf("退出程序。\n");
            break;
        default:
            printf("无效的选择。\n");
        }
    } while (choice != 9);

    return 0;
}
void printMenu()
{
    printf("\n==========================\n");
    printf("   保安值班安排系统\n");
    printf("==========================\n");
    printf("1. 添加保安\n");
    printf("2. 修改保安信息\n");
    printf("3. 删除保安\n");
    printf("4. 查找保安\n");
    printf("5. 设置轮休日\n");
    printf("6. 列出所有保安\n");
    printf("7. 打印所有轮休方案\n");
    printf("8. 打印每日工作安排表\n");
    printf("9. 退出\n");
    printf("==========================\n");
    printf("选择一个操作: ");
}

int loadData(Guard guards[])
{
    FILE *file = fopen(FILE_NAME, "r");
    if (!file)
        return 0; // 文件不存在则返回0

    int size = 0;
    while (fscanf(file, "%d %s", &guards[size].id, guards[size].name) == 2)
    {
        for (int i = 0; i < DAYS_IN_WEEK; i++)
        {
            fscanf(file, "%d", &guards[size].off_days[i]);
        }
        size++;
    }
    fclose(file);
    return size;
}

void saveData(Guard guards[], int size)
{
    FILE *file = fopen(FILE_NAME, "w");
    for (int i = 0; i < size; i++)
    {
        fprintf(file, "%d %s", guards[i].id, guards[i].name);
        for (int j = 0; j < DAYS_IN_WEEK; j++)
        {
            fprintf(file, " %d", guards[i].off_days[j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int getNewID(Guard guards[], int size)
{
    int max = 0;
    for (int i = 0; i < size; i++)
    {
        if (guards[i].id > max)
            max = guards[i].id;
    }
    return max + 1;
}

void addGuard()
{
    Guard guards[MAX_GUARDS];
    int size = loadData(guards);

    if (size >= MAX_GUARDS)
    {
        printf("已达到最大保安数量。\n");
        return;
    }

    guards[size].id = getNewID(guards, size);
    printf("输入保安姓名: ");
    scanf("%49s", guards[size].name);
    for (int i = 0; i < DAYS_IN_WEEK; i++)
    {
        guards[size].off_days[i] = 0; // Initially set all days to working
    }

    size++;
    saveData(guards, size);
    printf("保安添加成功。\n");
}
void modifyGuard()
{
    Guard guards[MAX_GUARDS];
    int size = loadData(guards);
    int id, found = -1;

    printf("输入要修改的保安ID: ");
    scanf("%d", &id);

    for (int i = 0; i < size; i++)
    {
        if (guards[i].id == id)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("未找到指定的保安。\n");
        return;
    }

    printf("要修改的保安信息如下：\n");
    printf("保安ID: %d\n", guards[found].id);
    printf("1. 保安姓名: %s\n", guards[found].name);
    printf("2. 轮休日设置:\n");
    for (int i = 0; i < DAYS_IN_WEEK; i++)
    {
        printf("   %s： %s\n", days[i], guards[found].off_days[i] == 1 ? "休息" : "工作");
    }

    printf("选择要修改的字段 (1-2): ");
    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf("输入新的保安姓名: ");
        scanf("%49s", guards[found].name);
        break;
    case 2:
        printf("设置轮休日：\n");
        for (int i = 0; i < DAYS_IN_WEEK; i++)
        {
            printf("周%s为休息日 (1为休息, 0为工作): ", days[i]);
            scanf("%d", &guards[found].off_days[i]);
        }
        break;
    default:
        printf("无效的选择。\n");
        return;
    }

    saveData(guards, size);
    printf("保安信息已更新。\n");
}

void deleteGuard()
{
    Guard guards[MAX_GUARDS];
    int size = loadData(guards);
    int id, found = -1;

    printf("输入要删除的保安ID: ");
    scanf("%d", &id);

    for (int i = 0; i < size; i++)
    {
        if (guards[i].id == id)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("未找到指定的保安。\n");
        return;
    }

    printf("确认删除以下保安信息：\n");
    printf("保安ID: %d\n", guards[found].id);
    printf("保安姓名: %s\n", guards[found].name);
    printf("保安轮休日: ");
    for (int i = 0; i < DAYS_IN_WEEK; i++)
    {
        if (guards[found].off_days[i] == 1)
        {
            printf("%s ", days[i]);
        }
    }
    printf("\n");

    printf("确认删除？(y/n): ");
    char confirmation;
    scanf(" %c", &confirmation);

    if (confirmation != 'y' && confirmation != 'Y')
    {
        printf("取消删除。\n");
        return;
    }

    for (int i = found; i < size - 1; i++)
    {
        guards[i] = guards[i + 1];
    }

    size--;
    saveData(guards, size);
    printf("保安删除成功。\n");
}

void findGuard()
{
    Guard guards[MAX_GUARDS];
    int size = loadData(guards);
    int id, found = -1;

    printf("输入要查找的保安ID: ");
    scanf("%d", &id);

    for (int i = 0; i < size; i++)
    {
        if (guards[i].id == id)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("未找到指定的保安。\n");
        return;
    }

    printf("保安ID: %d\n", guards[found].id);
    printf("保安姓名: %s\n", guards[found].name);
    printf("保安休息日: ");
    for (int i = 0; i < DAYS_IN_WEEK; i++)
    {
        if (guards[found].off_days[i] == 1)
        {
            printf("%s ", i == 0 ? "周日" : i == 1 ? "周一"
                                        : i == 2   ? "周二"
                                        : i == 3   ? "周三"
                                        : i == 4   ? "周四"
                                        : i == 5   ? "周五"
                                                   : "周六");
        }
    }
    printf("\n");
}

void setOffDay()
{

    Guard guards[MAX_GUARDS];
    int size = loadData(guards);
    int id, day, found = -1, set;
    listGuards();
    printf("输入保安ID设置轮休日: ");
    scanf("%d", &id);

    for (int i = 0; i < size; i++)
    {
        if (guards[i].id == id)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("未找到指定的保安。\n");
        return;
    }

    printf("输入日子 (0=周日, 1=周一, ..., 6=周六): ");
    scanf("%d", &day);
    printf("设置为休息(1)还是工作(0)：");
    scanf("%d", &set);
    guards[found].off_days[day] = set;

    saveData(guards, size);
    printf("轮休日设置成功。\n");
}

void listGuards()
{
    Guard guards[MAX_GUARDS];
    int size = loadData(guards);

    if (size == 0)
    {
        printf("没有保安信息。\n");
        return;
    }

    printf("保安列表:\n");
    for (int i = 0; i < size; i++)
    {
        printf("保安ID: %d, 姓名: %s, 轮休日: ", guards[i].id, guards[i].name);
        for (int j = 0; j < DAYS_IN_WEEK; j++)
        {
            if (guards[i].off_days[j] == 1)
            {
                printf("%s ", j == 0 ? "周日" : j == 1 ? "周一"
                                            : j == 2   ? "周二"
                                            : j == 3   ? "周三"
                                            : j == 4   ? "周四"
                                            : j == 5   ? "周五"
                                                       : "周六");
            }
        }
        printf("\n");
    }
}
void printScheduleTable()
{
    Guard guards[MAX_GUARDS];
    int count = loadData(guards);
    const char *days[] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};

    printf("\n每日保安工作安排表：\n");
    printf("| %-6s | %-40s |\n", "日子", "当日工作的保安");
    printf("-------------------------------------------------------------\n");

    for (int day = 0; day < DAYS_IN_WEEK; day++)
    {
        printf("| %-6s | ", days[day]);
        bool first = true;
        for (int i = 0; i < count; i++)
        {
            if (guards[i].off_days[day] == 0)
            {
                if (!first)
                    printf(", ");
                printf("%s", guards[i].name);
                first = false;
            }
        }
        printf(" |\n");
    }
}
void printAllSchedules(int numGuards)
{
    const char *days[] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};
    Guard guards[MAX_GUARDS];
    int count = loadData(guards);

    printf("\n所有保安的轮休方案：\n");
    printf("| %-4s | %-20s | %-30s |\n", "ID", "保安姓名", "轮休日");
    printf("-------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("| %-4d | %-20s | ", guards[i].id, guards[i].name);
        bool first = true;
        for (int j = 0; j < DAYS_IN_WEEK; j++)
        {
            if (guards[i].off_days[j] == 1)
            {
                if (!first)
                    printf(", ");
                printf("%s", days[j]);
                first = false;
            }
        }
        printf(" |\n");
    }
}
