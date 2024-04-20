#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义员工结构体
typedef struct
{
    char name[50];
    char department[50];
    char hometown[50];
    char birthdate[20];
    char education[50];
    char position[50];
    float salary;
    char awards_punishments[100];
} Employee;

// 全局变量，用于存储员工信息
Employee employees[100];
int numEmployees = 0;

// 函数声明
void addRecord();
void deleteRecord();
void modifyRecord();
void statisticsByDepartment();
void searchByName();
void searchByDepartment();
void accessControl();
void sortRecords();
void saveToFile()
{
    FILE *file = fopen("employees.txt", "w");
    if (file == NULL)
    {
        printf("无法打开文件。\n");
        return;
    }

    for (int i = 0; i < numEmployees; i++)
    {
        fprintf(file, "%s %s %s %s %s,%s %.2f %s\n", employees[i].name, employees[i].department, employees[i].hometown, employees[i].birthdate, employees[i].education, employees[i].position, employees[i].salary, employees[i].awards_punishments);
    }

    fclose(file);
    printf("员工信息已保存到文件。\n");
}

void loadFromFile()
{
    FILE *file = fopen("employees.txt", "r");
    if (file == NULL)
    {
        printf("无法打开文件。\n");
        return;
    }

    numEmployees = 0;
    while (fscanf(file, "%s %s %s %s %s %s %f %s\n", employees[numEmployees].name, employees[numEmployees].department, employees[numEmployees].hometown, employees[numEmployees].birthdate, employees[numEmployees].education, employees[numEmployees].position, &employees[numEmployees].salary, employees[numEmployees].awards_punishments) != EOF)
    {
        numEmployees++;
    }

    fclose(file);
    printf("从文件加载了 %d 条员工信息。\n", numEmployees);
}
void login()
{
    char username[50];
    char password[50];
    int attempts = 0;
    const int maxAttempts = 3;
    const char *correctUsername = "admin";
    const char *correctPassword = "admin";

    printf("===== 登录 =====\n");
    do
    {
        printf("用户名: ");
        scanf("%s", username);
        printf("密码: ");
        scanf("%s", password);

        if (strcmp(username, correctUsername) == 0 && strcmp(password, correctPassword) == 0)
        {
            printf("登录成功！\n");
            return;
        }
        else
        {
            attempts++;
            printf("登录失败，请重新输入。\n");
        }
    } while (attempts < maxAttempts);

    printf("登录次数超过限制，程序退出。\n");
    exit(1);
}
int main()
{
    // 登录验证
    login();

    // 加载员工信息
    loadFromFile();

    int choice;

    do
    {
        printf("\n===== 人事管理系统 =====\n");
        printf("1. 加入新记录\n");
        printf("2. 删除记录\n");
        printf("3. 修改记录\n");
        printf("4. 按姓名查询\n");
        printf("5. 按部门查询\n");
        printf("6. 排序记录\n");
        printf("7. 退出\n");
        printf("8. 统计部门信息\n");
        printf("==========================\n");
        printf("请选择操作: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addRecord();
            break;
        case 2:
            deleteRecord();
            break;
        case 3:
            modifyRecord();
            break;
        case 4:
            searchByName();
            break;
        case 5:
            searchByDepartment();
            break;
        case 6:
            sortRecords();
            break;
        case 7:
            saveToFile(); // 退出前保存员工信息到文件
            printf("感谢使用人事管理系统！\n");
            break;
        case 8:
            statisticsByDepartment();
            break;
        default:
            printf("无效的选项，请重新选择。\n");
        }
    } while (choice != 7);

    return 0;
}
void addRecord()
{
    int numToAdd;
    printf("\n请输入要添加的员工数量：");
    scanf("%d", &numToAdd);

    if (numEmployees + numToAdd > 100)
    {
        printf("添加的员工数量超过了可容纳的上限，无法继续添加。\n");
        return;
    }

    for (int i = 0; i < numToAdd; i++)
    {
        Employee newEmployee;
        printf("\n请输入第 %d 个员工的信息：\n", i + 1);
        printf("姓名: ");
        scanf("%s", newEmployee.name);

        // 检查是否存在相同姓名的员工记录
        for (int j = 0; j < numEmployees; j++)
        {
            if (strcmp(employees[j].name, newEmployee.name) == 0)
            {
                printf("已存在姓名为 %s 的员工记录，请重新输入。\n", newEmployee.name);
                i--; // 重新输入当前员工信息
                break;
            }
        }

        if (i >= 0) // 如果没有重复的姓名，则继续录入其他信息
        {
            printf("部门: ");
            scanf("%s", newEmployee.department);
            printf("籍贯: ");
            scanf("%s", newEmployee.hometown);
            printf("出生年月日: ");
            scanf("%s", newEmployee.birthdate);
            printf("学历/学位: ");
            scanf("%s", newEmployee.education);
            printf("职务/职称: ");
            scanf("%s", newEmployee.position);
            printf("工资: ");
            scanf("%f", &newEmployee.salary);
            printf("奖惩记录: ");
            scanf("%s", newEmployee.awards_punishments);

            employees[numEmployees++] = newEmployee;
        }
    }

    printf("员工信息添加成功！\n");
}
void deleteRecord()
{
    if (numEmployees == 0)
    {
        printf("员工记录为空，无法删除。\n");
        return;
    }

    char name[50];
    printf("\n请输入要删除的员工姓名: ");
    scanf("%s", name);

    int found = 0;
    for (int i = 0; i < numEmployees; i++)
    {
        if (strcmp(employees[i].name, name) == 0)
        {
            printf("找到员工 %s 的信息：\n", name);
            printf("姓名: %s\n", employees[i].name);
            printf("部门: %s\n", employees[i].department);
            printf("籍贯: %s\n", employees[i].hometown);
            printf("出生年月日: %s\n", employees[i].birthdate);
            printf("学历/学位: %s\n", employees[i].education);
            printf("职务/职称: %s\n", employees[i].position);
            printf("工资: %.2f\n", employees[i].salary);
            printf("奖惩记录: %s\n", employees[i].awards_punishments);

            char confirm;
            printf("确认删除该员工信息？(y/n): ");
            scanf(" %c", &confirm);
            if (confirm == 'y' || confirm == 'Y')
            {
                for (int j = i; j < numEmployees - 1; j++)
                {
                    employees[j] = employees[j + 1];
                }
                numEmployees--;
                printf("员工 %s 的信息已删除。\n", name);
            }
            else
            {
                printf("已取消删除操作。\n");
            }

            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("未找到员工 %s 的信息。\n", name);
    }
}
void modifyRecord()
{
    char name[50];
    printf("\n请输入要修改的员工姓名: ");
    scanf("%s", name);

    int found = 0;
    for (int i = 0; i < numEmployees; i++)
    {
        if (strcmp(employees[i].name, name) == 0)
        {
            printf("\n请选择要修改的信息：\n");
            printf("1. 部门\n");
            printf("2. 籍贯\n");
            printf("3. 出生年月日\n");
            printf("4. 学历/学位\n");
            printf("5. 职务/职称\n");
            printf("6. 工资\n");
            printf("7. 奖惩记录\n");
            printf("请选择操作: ");

            int choice;
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                printf("部门: ");
                scanf("%s", employees[i].department);
                break;
            case 2:
                printf("籍贯: ");
                scanf("%s", employees[i].hometown);
                break;
            case 3:
                printf("出生年月日: ");
                scanf("%s", employees[i].birthdate);
                break;
            case 4:
                printf("学历/学位: ");
                scanf("%s", employees[i].education);
                break;
            case 5:
                printf("职务/职称: ");
                scanf("%s", employees[i].position);
                break;
            case 6:
                printf("工资: ");
                scanf("%f", &employees[i].salary);
                break;
            case 7:
                printf("奖惩记录: ");
                scanf("%s", employees[i].awards_punishments);
                break;
            default:
                printf("无效的选项，请重新选择。\n");
                break;
            }

            found = 1;
            printf("员工 %s 的信息已修改。\n", name);
            break;
        }
    }

    if (!found)
    {
        printf("未找到员工 %s 的信息。\n", name);
    }
}

void searchByName()
{
    char name[50];
    printf("\n请输入要查询的员工姓名: ");
    scanf("%s", name);

    int found = 0;
    for (int i = 0; i < numEmployees; i++)
    {
        if (strcmp(employees[i].name, name) == 0)
        {
            printf("姓名: %s\n", employees[i].name);
            printf("部门: %s\n", employees[i].department);
            printf("籍贯: %s\n", employees[i].hometown);
            printf("出生年月日: %s\n", employees[i].birthdate);
            printf("学历/学位: %s\n", employees[i].education);
            printf("职务/职称: %s\n", employees[i].position);
            printf("工资: %.2f\n", employees[i].salary);
            printf("奖惩记录: %s\n", employees[i].awards_punishments);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("未找到员工 %s 的信息。\n", name);
    }
}

void searchByDepartment()
{
    char department[50];
    printf("\n请输入要查询的部门: ");
    scanf("%s", department);

    int found = 0;
    for (int i = 0; i < numEmployees; i++)
    {
        if (strcmp(employees[i].department, department) == 0)
        {
            printf("姓名: %s\n", employees[i].name);
            printf("部门: %s\n", employees[i].department);
            printf("籍贯: %s\n", employees[i].hometown);
            printf("出生年月日: %s\n", employees[i].birthdate);
            printf("学历/学位: %s\n", employees[i].education);
            printf("职务/职称: %s\n", employees[i].position);
            printf("工资: %.2f\n", employees[i].salary);
            printf("奖惩记录: %s\n", employees[i].awards_punishments);
            found = 1;
            printf("\n");
        }
    }

    if (!found)
    {
        printf("未找到部门为 %s 的员工信息。\n", department);
    }
}

void accessControl()
{
    char username[50];
    char password[50];
    printf("\n设置用户名: ");
    scanf("%s", username);
    printf("设置密码: ");
    scanf("%s", password);
    printf("用户名和密码设置成功！\n");
}
// 按姓名排序函数
void sortByFullName()
{
    for (int i = 0; i < numEmployees - 1; i++)
    {
        for (int j = 0; j < numEmployees - i - 1; j++)
        {
            if (strcmp(employees[j].name, employees[j + 1].name) > 0)
            {
                Employee temp = employees[j];
                employees[j] = employees[j + 1];
                employees[j + 1] = temp;
            }
        }
    }

    printf("员工信息已按姓名排序。\n");
}

// 按工资排序函数
void sortBySalary()
{
    for (int i = 0; i < numEmployees - 1; i++)
    {
        for (int j = 0; j < numEmployees - i - 1; j++)
        {
            if (employees[j].salary > employees[j + 1].salary)
            {
                Employee temp = employees[j];
                employees[j] = employees[j + 1];
                employees[j + 1] = temp;
            }
        }
    }

    printf("员工信息已按工资排序。\n");
}

// 排序记录函数
void sortRecords()
{
    int choice;

    printf("\n请选择排序方式：\n");
    printf("1. 按姓名排序\n");
    printf("2. 按工资排序\n");
    printf("请选择操作: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        sortByFullName();
        break;
    case 2:
        sortBySalary();
        break;
    default:
        printf("无效的选项，请重新选择。\n");
        break;
    }
}
void statisticsByDepartment()
{
    char department[50];
    printf("\n请输入要统计的部门: ");
    scanf("%s", department);

    int totalEmployees = 0;
    float totalSalary = 0;
    int numEmployeesInDepartment = 0;

    for (int i = 0; i < numEmployees; i++)
    {
        if (strcmp(employees[i].department, department) == 0)
        {
            totalEmployees++;
            totalSalary += employees[i].salary;
            numEmployeesInDepartment++;
        }
    }

    if (numEmployeesInDepartment == 0)
    {
        printf("未找到部门为 %s 的员工信息。\n", department);
    }
    else
    {
        float averageSalary = totalSalary / numEmployeesInDepartment;
        printf("部门 %s 的统计信息：\n", department);
        printf("员工总数: %d\n", numEmployeesInDepartment);
        printf("平均工资: %.2f\n", averageSalary);
    }
}
