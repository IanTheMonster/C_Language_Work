#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ����Ա���ṹ��
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

// ȫ�ֱ��������ڴ洢Ա����Ϣ
Employee employees[100];
int numEmployees = 0;

// ��������
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
        printf("�޷����ļ���\n");
        return;
    }

    for (int i = 0; i < numEmployees; i++)
    {
        fprintf(file, "%s %s %s %s %s,%s %.2f %s\n", employees[i].name, employees[i].department, employees[i].hometown, employees[i].birthdate, employees[i].education, employees[i].position, employees[i].salary, employees[i].awards_punishments);
    }

    fclose(file);
    printf("Ա����Ϣ�ѱ��浽�ļ���\n");
}

void loadFromFile()
{
    FILE *file = fopen("employees.txt", "r");
    if (file == NULL)
    {
        printf("�޷����ļ���\n");
        return;
    }

    numEmployees = 0;
    while (fscanf(file, "%s %s %s %s %s %s %f %s\n", employees[numEmployees].name, employees[numEmployees].department, employees[numEmployees].hometown, employees[numEmployees].birthdate, employees[numEmployees].education, employees[numEmployees].position, &employees[numEmployees].salary, employees[numEmployees].awards_punishments) != EOF)
    {
        numEmployees++;
    }

    fclose(file);
    printf("���ļ������� %d ��Ա����Ϣ��\n", numEmployees);
}
void login()
{
    char username[50];
    char password[50];
    int attempts = 0;
    const int maxAttempts = 3;
    const char *correctUsername = "admin";
    const char *correctPassword = "admin";

    printf("===== ��¼ =====\n");
    do
    {
        printf("�û���: ");
        scanf("%s", username);
        printf("����: ");
        scanf("%s", password);

        if (strcmp(username, correctUsername) == 0 && strcmp(password, correctPassword) == 0)
        {
            printf("��¼�ɹ���\n");
            return;
        }
        else
        {
            attempts++;
            printf("��¼ʧ�ܣ����������롣\n");
        }
    } while (attempts < maxAttempts);

    printf("��¼�����������ƣ������˳���\n");
    exit(1);
}
int main()
{
    // ��¼��֤
    login();

    // ����Ա����Ϣ
    loadFromFile();

    int choice;

    do
    {
        printf("\n===== ���¹���ϵͳ =====\n");
        printf("1. �����¼�¼\n");
        printf("2. ɾ����¼\n");
        printf("3. �޸ļ�¼\n");
        printf("4. ��������ѯ\n");
        printf("5. �����Ų�ѯ\n");
        printf("6. �����¼\n");
        printf("7. �˳�\n");
        printf("8. ͳ�Ʋ�����Ϣ\n");
        printf("==========================\n");
        printf("��ѡ�����: ");
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
            saveToFile(); // �˳�ǰ����Ա����Ϣ���ļ�
            printf("��лʹ�����¹���ϵͳ��\n");
            break;
        case 8:
            statisticsByDepartment();
            break;
        default:
            printf("��Ч��ѡ�������ѡ��\n");
        }
    } while (choice != 7);

    return 0;
}
void addRecord()
{
    int numToAdd;
    printf("\n������Ҫ��ӵ�Ա��������");
    scanf("%d", &numToAdd);

    if (numEmployees + numToAdd > 100)
    {
        printf("��ӵ�Ա�����������˿����ɵ����ޣ��޷�������ӡ�\n");
        return;
    }

    for (int i = 0; i < numToAdd; i++)
    {
        Employee newEmployee;
        printf("\n������� %d ��Ա������Ϣ��\n", i + 1);
        printf("����: ");
        scanf("%s", newEmployee.name);

        // ����Ƿ������ͬ������Ա����¼
        for (int j = 0; j < numEmployees; j++)
        {
            if (strcmp(employees[j].name, newEmployee.name) == 0)
            {
                printf("�Ѵ�������Ϊ %s ��Ա����¼�����������롣\n", newEmployee.name);
                i--; // �������뵱ǰԱ����Ϣ
                break;
            }
        }

        if (i >= 0) // ���û���ظ��������������¼��������Ϣ
        {
            printf("����: ");
            scanf("%s", newEmployee.department);
            printf("����: ");
            scanf("%s", newEmployee.hometown);
            printf("����������: ");
            scanf("%s", newEmployee.birthdate);
            printf("ѧ��/ѧλ: ");
            scanf("%s", newEmployee.education);
            printf("ְ��/ְ��: ");
            scanf("%s", newEmployee.position);
            printf("����: ");
            scanf("%f", &newEmployee.salary);
            printf("���ͼ�¼: ");
            scanf("%s", newEmployee.awards_punishments);

            employees[numEmployees++] = newEmployee;
        }
    }

    printf("Ա����Ϣ��ӳɹ���\n");
}
void deleteRecord()
{
    if (numEmployees == 0)
    {
        printf("Ա����¼Ϊ�գ��޷�ɾ����\n");
        return;
    }

    char name[50];
    printf("\n������Ҫɾ����Ա������: ");
    scanf("%s", name);

    int found = 0;
    for (int i = 0; i < numEmployees; i++)
    {
        if (strcmp(employees[i].name, name) == 0)
        {
            printf("�ҵ�Ա�� %s ����Ϣ��\n", name);
            printf("����: %s\n", employees[i].name);
            printf("����: %s\n", employees[i].department);
            printf("����: %s\n", employees[i].hometown);
            printf("����������: %s\n", employees[i].birthdate);
            printf("ѧ��/ѧλ: %s\n", employees[i].education);
            printf("ְ��/ְ��: %s\n", employees[i].position);
            printf("����: %.2f\n", employees[i].salary);
            printf("���ͼ�¼: %s\n", employees[i].awards_punishments);

            char confirm;
            printf("ȷ��ɾ����Ա����Ϣ��(y/n): ");
            scanf(" %c", &confirm);
            if (confirm == 'y' || confirm == 'Y')
            {
                for (int j = i; j < numEmployees - 1; j++)
                {
                    employees[j] = employees[j + 1];
                }
                numEmployees--;
                printf("Ա�� %s ����Ϣ��ɾ����\n", name);
            }
            else
            {
                printf("��ȡ��ɾ��������\n");
            }

            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("δ�ҵ�Ա�� %s ����Ϣ��\n", name);
    }
}
void modifyRecord()
{
    char name[50];
    printf("\n������Ҫ�޸ĵ�Ա������: ");
    scanf("%s", name);

    int found = 0;
    for (int i = 0; i < numEmployees; i++)
    {
        if (strcmp(employees[i].name, name) == 0)
        {
            printf("\n��ѡ��Ҫ�޸ĵ���Ϣ��\n");
            printf("1. ����\n");
            printf("2. ����\n");
            printf("3. ����������\n");
            printf("4. ѧ��/ѧλ\n");
            printf("5. ְ��/ְ��\n");
            printf("6. ����\n");
            printf("7. ���ͼ�¼\n");
            printf("��ѡ�����: ");

            int choice;
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                printf("����: ");
                scanf("%s", employees[i].department);
                break;
            case 2:
                printf("����: ");
                scanf("%s", employees[i].hometown);
                break;
            case 3:
                printf("����������: ");
                scanf("%s", employees[i].birthdate);
                break;
            case 4:
                printf("ѧ��/ѧλ: ");
                scanf("%s", employees[i].education);
                break;
            case 5:
                printf("ְ��/ְ��: ");
                scanf("%s", employees[i].position);
                break;
            case 6:
                printf("����: ");
                scanf("%f", &employees[i].salary);
                break;
            case 7:
                printf("���ͼ�¼: ");
                scanf("%s", employees[i].awards_punishments);
                break;
            default:
                printf("��Ч��ѡ�������ѡ��\n");
                break;
            }

            found = 1;
            printf("Ա�� %s ����Ϣ���޸ġ�\n", name);
            break;
        }
    }

    if (!found)
    {
        printf("δ�ҵ�Ա�� %s ����Ϣ��\n", name);
    }
}

void searchByName()
{
    char name[50];
    printf("\n������Ҫ��ѯ��Ա������: ");
    scanf("%s", name);

    int found = 0;
    for (int i = 0; i < numEmployees; i++)
    {
        if (strcmp(employees[i].name, name) == 0)
        {
            printf("����: %s\n", employees[i].name);
            printf("����: %s\n", employees[i].department);
            printf("����: %s\n", employees[i].hometown);
            printf("����������: %s\n", employees[i].birthdate);
            printf("ѧ��/ѧλ: %s\n", employees[i].education);
            printf("ְ��/ְ��: %s\n", employees[i].position);
            printf("����: %.2f\n", employees[i].salary);
            printf("���ͼ�¼: %s\n", employees[i].awards_punishments);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("δ�ҵ�Ա�� %s ����Ϣ��\n", name);
    }
}

void searchByDepartment()
{
    char department[50];
    printf("\n������Ҫ��ѯ�Ĳ���: ");
    scanf("%s", department);

    int found = 0;
    for (int i = 0; i < numEmployees; i++)
    {
        if (strcmp(employees[i].department, department) == 0)
        {
            printf("����: %s\n", employees[i].name);
            printf("����: %s\n", employees[i].department);
            printf("����: %s\n", employees[i].hometown);
            printf("����������: %s\n", employees[i].birthdate);
            printf("ѧ��/ѧλ: %s\n", employees[i].education);
            printf("ְ��/ְ��: %s\n", employees[i].position);
            printf("����: %.2f\n", employees[i].salary);
            printf("���ͼ�¼: %s\n", employees[i].awards_punishments);
            found = 1;
            printf("\n");
        }
    }

    if (!found)
    {
        printf("δ�ҵ�����Ϊ %s ��Ա����Ϣ��\n", department);
    }
}

void accessControl()
{
    char username[50];
    char password[50];
    printf("\n�����û���: ");
    scanf("%s", username);
    printf("��������: ");
    scanf("%s", password);
    printf("�û������������óɹ���\n");
}
// ������������
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

    printf("Ա����Ϣ�Ѱ���������\n");
}

// ������������
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

    printf("Ա����Ϣ�Ѱ���������\n");
}

// �����¼����
void sortRecords()
{
    int choice;

    printf("\n��ѡ������ʽ��\n");
    printf("1. ����������\n");
    printf("2. ����������\n");
    printf("��ѡ�����: ");
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
        printf("��Ч��ѡ�������ѡ��\n");
        break;
    }
}
void statisticsByDepartment()
{
    char department[50];
    printf("\n������Ҫͳ�ƵĲ���: ");
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
        printf("δ�ҵ�����Ϊ %s ��Ա����Ϣ��\n", department);
    }
    else
    {
        float averageSalary = totalSalary / numEmployeesInDepartment;
        printf("���� %s ��ͳ����Ϣ��\n", department);
        printf("Ա������: %d\n", numEmployeesInDepartment);
        printf("ƽ������: %.2f\n", averageSalary);
    }
}
