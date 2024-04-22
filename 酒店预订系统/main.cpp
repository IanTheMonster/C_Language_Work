#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
// �û��ṹ�嶨��
typedef struct User
{
    char ID[20];   // �û�ID
    char pwd[20];  // �û�����
    int authority; // �û�Ȩ��: 1ǰ̨��2�˿͡�3����Ա
} User;
typedef struct Room
{
    int roomId;        // �����
    float area;        // ���
    float price;       // �۸�
    char roomType[20]; // �󴲡�˫�����׷�
} Rooms;
typedef struct Customers
{
    char ID[20];   // �û�id
    char type[20]; // ��������[��ʹ�����𡢻ƽ���ͨ]
    float discout; // ��Ӧ�ۿ�
} Customers;
typedef struct Reservation
{
    char CustomerId[20]; // �û���
    int roomId;          // �����
    float price;         // �۸�
    char startDate[20];  // ��ʼ����
    char endDate[20];    // ��������
    int isCheckedIn;     // �Ƿ���ס
} Reservations;
User users[1000];
int numUsers = 0;
Rooms rooms[100];                // ������Ϣ���飬���������100������
int numRooms = 0;                // ��������
Customers customers[1000];       // �ͻ���Ϣ���飬���������1000���ͻ�
int numCustomers = 0;            // �ͻ�����
Reservations reservations[1000]; // Ԥ����Ϣ���飬���������1000��Ԥ����¼
int numReservations = 0;         // Ԥ������
User *currentUser = NULL;
// ��ʾ��ǰʱ�估�ʺ���
void showTime(int status);

// ���ļ������û���Ϣ
void loadUser();

// ���û���Ϣ���浽�ļ�
void saveUser();

// ���˵�����
int mainMenu();

// ��¼����
int login();

// �ͷ������Ӳ˵�����
void roomManagementMenu();

// ���˹����Ӳ˵�����
void customerManagementMenu();

// ��Ϣͳ�ƺ���
void informationStatistics();

// ����ά������
void passwordMaintenance();

// ��Ϣ��ѯ�Ӳ˵�����
void informationQueryMenu();

// ��Ϣ�����Ӳ˵�����
void informationSortMenu();

// ����Ա�˵�����
void adminMenu();

// ��ס�����Ӳ˵�����
void checkInManagementMenu();

// ǰ̨�˵�����
void frontDeskMenu();
void customerMenu();
// ��ʾ��ǰʱ�估�ʺ���
void showTime(int status)
{
    time_t it = time(NULL);
    struct tm *ptr = localtime(&it);

    // ���ݵ�ǰʱ�������ʺ���
    if (ptr->tm_hour >= 5 && ptr->tm_hour < 9)
        printf("���Ϻã�\n");
    else if (ptr->tm_hour >= 9 && ptr->tm_hour < 12)
        printf("����ã�\n");
    else if (ptr->tm_hour >= 12 && ptr->tm_hour < 14)
        printf("����ã�\n");
    else if (ptr->tm_hour >= 14 && ptr->tm_hour < 18)
        printf("����ã�\n");
    else
        printf("���Ϻã�\n");

    // ��ʾ��ǰʱ��
    printf("������%4d��%02d��%02d�� %02d:%02d:%02d\n",
           ptr->tm_year + 1900, ptr->tm_mon + 1, ptr->tm_mday, ptr->tm_hour, ptr->tm_min, ptr->tm_sec);

    // ����״̬��ʾ��Ӧ��Ϣ
    if (status == 1)
        printf("��ӭʹ���ùݹ���ϵͳ��\n");
    else if (status == 0)
        printf("ллʹ���ùݹ���ϵͳ��\n");
}
// ���ļ������û���Ϣ
void loadUser()
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        printf("�޷����û��ļ���\n");
        return;
    }

    // ���ж�ȡ�û���Ϣ���洢��������
    while (fscanf(file, "%s %s %d", users[numUsers].ID, users[numUsers].pwd, &users[numUsers].authority) != EOF)
    {
        numUsers++;
    }

    fclose(file);
}

// ���û���Ϣ���浽�ļ�
void saveUser()
{
    FILE *file = fopen("users.txt", "w");
    if (file == NULL)
    {
        printf("�޷������û��ļ���\n");
        return;
    }

    // ���н��û���Ϣд���ļ�
    for (int i = 0; i < numUsers; i++)
    {
        fprintf(file, "%s %s %d\n", users[i].ID, users[i].pwd, users[i].authority);
    }

    fclose(file);
}

// ���˵�����
int mainMenu()
{
    int choice;
    printf("\t\t\t      �Ƶ����ϵͳ\n");
    printf("\t\t\t===============================\n");
    printf("\t\t\t      1 : ǰ̨��¼\n");
    printf("\t\t\t      2 : �˿͵�¼\n");
    printf("\t\t\t      3 : ����Ա��¼\n");
    printf("\t\t\t      0 : �˳�\n");
    printf("\t\t\t===============================\n");
    printf("��ѡ��\n");
    scanf("%d", &choice);

    // ��֤������Ч��
    while (choice < 0 || choice > 3)
    {
        printf("����������������룺\n");
        scanf("%d", &choice);
    }

    // ����ѡ�������ʾʱ�亯��
    showTime(choice != 0 ? 1 : 0);
    return choice;
}
// ��¼����
int login()
{
    char userID[20];
    char password[20];
    printf("�������û�ID��");
    scanf("%s", userID);
    printf("���������룺");
    scanf("%s", password);

    // ����������û���֤���߼�������Ը����û�ID��������û������в����û�������֤�û����ͺ������Ƿ�ƥ��
    // �����֤ͨ���������û����ͣ����򷵻�0��ʾ��¼ʧ��
    for (int i = 0; i < numUsers; ++i)
    {
        if (strcmp(users[i].ID, userID) == 0 && strcmp(users[i].pwd, password) == 0)
        {
            currentUser = &users[i];
            return users[i].authority;
        }
    }
    // ��¼ʧ��
    return 0;
}
void menu()
{
    int choice;

    // ���˵�ѭ��
    do
    {
        system("cls");
        // ��ʾ���˵�����ȡ�û�ѡ��
        choice = mainMenu();

        // �����û�ѡ�������Ӧ����
        switch (choice)
        {
        case 1: // ǰ̨��¼
        {
            int userType = login(); // ���õ�¼����
            if (userType == 1)
            {

                printf("ǰ̨��¼�ɹ���\n");
                Sleep(100);
                system("cls");
                frontDeskMenu();
            }
            else
            {
                printf("ǰ̨��¼ʧ�ܣ������û��������롣\n");
            }
            break;
        }
        case 2: // �˿͵�¼
        {
            int userType = login(); // ���õ�¼����
            if (userType == 2)
            {
                // ������Ե��ù˿Ͳ����ĺ���
                printf("�˿͵�¼�ɹ���\n");
                Sleep(100);
                system("cls");
                customerMenu();
            }
            else
            {
                printf("�˿͵�¼ʧ�ܣ������û��������롣\n");
            }
            break;
        }
        case 3: // ����Ա��¼
        {
            int userType = login(); // ���õ�¼����
            if (userType == 3)
            {
                // ������Ե��ù���Ա�����ĺ���
                printf("����Ա��¼�ɹ���\n");
                Sleep(100);
                system("cls");
                adminMenu();
            }
            else
            {
                printf("����Ա��¼ʧ�ܣ������û��������롣\n");
            }
            break;
        }
        case 0: // �˳�
            printf("�����˳�ϵͳ...\n");
            break;
        default:
            printf("�������������ѡ��\n");
            break;
        }

    } while (choice != 0);
}
// �ͷ������Ӳ˵�����
void roomManagementMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\t�ͷ�����\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : ��ӿͷ�\n");
        printf("\t\t      2 : ɾ���ͷ�\n");
        printf("\t\t      3 : �޸Ŀͷ���Ϣ\n");
        printf("\t\t      4 : ��ѯ�ͷ���Ϣ\n");
        printf("\t\t      0 : �����ϼ��˵�\n");
        printf("\t\t==============================\n");
        printf("��ѡ��");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            // ��ӿͷ���������
            break;
        case 2:
            // ɾ���ͷ���������
            break;
        case 3:
            // �޸Ŀͷ���Ϣ��������
            break;
        case 4:
            // ��ѯ�ͷ���Ϣ��������
            break;
        case 0:
            printf("�����ϼ��˵�...\n");
            break;
        default:
            printf("�������������ѡ��\n");
            break;
        }
        system("pause");
    } while (choice != 0);
}

// ���˹����Ӳ˵�����
void customerManagementMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\t���˹���\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : ��ӿ���\n");
        printf("\t\t      2 : ɾ������\n");
        printf("\t\t      3 : �޸Ŀ�����Ϣ\n");
        printf("\t\t      4 : ��ѯ������Ϣ\n");
        printf("\t\t      0 : �����ϼ��˵�\n");
        printf("\t\t==============================\n");
        printf("��ѡ��");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            // ��ӿ��˺�������
            break;
        case 2:
            // ɾ�����˺�������
            break;
        case 3:
            // �޸Ŀ�����Ϣ��������
            break;
        case 4:
            // ��ѯ������Ϣ��������
            break;
        case 0:
            printf("�����ϼ��˵�...\n");
            break;
        default:
            printf("�������������ѡ��\n");
            break;
        }
        system("pause");
    } while (choice != 0);
}

// ��Ϣͳ�ƺ���
void informationStatistics()
{
    // ���������������д��Ӧ��ͳ�ƹ���
}

// ����ά������
void passwordMaintenance()
{
    char oldPassword[20];
    char newPassword[20];
    char confirmPassword[20];

    printf("����������룺");
    scanf("%s", oldPassword);

    // �������ľ������Ƿ��뵱ǰ�û�������ƥ��
    if (strcmp(oldPassword, currentUser->pwd) != 0)
    {
        printf("������������������²�����\n");
        return;
    }

    printf("�����������룺");
    scanf("%s", newPassword);

    printf("��ȷ�������룺");
    scanf("%s", confirmPassword);

    // �����������������Ƿ�ƥ��
    if (strcmp(newPassword, confirmPassword) != 0)
    {
        printf("������������벻ƥ�䣬�����²�����\n");
        return;
    }

    // ���µ�ǰ���˵�����
    strcpy(currentUser->pwd, newPassword);
}

// ��Ϣ��ѯ�Ӳ˵�����
void informationQueryMenu()
{
    // ��д��Ϣ��ѯ����
}

// ��Ϣ�����Ӳ˵�����
void informationSortMenu()
{
    // ��д��Ϣ������
}
// ����Ա�˵�����
void adminMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\t����Ա�˵�\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : �ͷ�����\n");
        printf("\t\t      2 : ���˹���\n");
        printf("\t\t      3 : ��Ϣ��ѯ\n");
        printf("\t\t      4 : ��Ϣ����\n");
        printf("\t\t      5 : ��Ϣͳ��\n");
        printf("\t\t      6 : ����ά��\n");
        printf("\t\t      0 : �˳�\n");
        printf("\t\t==============================\n");
        printf("��ѡ��");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            roomManagementMenu(); // ����ͷ������Ӳ˵�
            break;
        case 2:
            customerManagementMenu(); // ������˹����Ӳ˵�
            break;
        case 3:
            informationQueryMenu(); // ��Ϣ��ѯ
            break;
        case 4:
            informationSortMenu(); // ��Ϣ����
            break;
        case 5:
            informationStatistics(); // ������Ϣͳ�ƹ���
            break;
        case 6:
            passwordMaintenance(); // ��������ά������
            break;
        case 0:
            printf("�����˳�����Ա�˵�...\n");
            break;
        default:
            printf("�������������ѡ��\n");
            break;
        }
        system("pause");
    } while (choice != 0);
}
// ��ס�����Ӳ˵�����
void checkInManagementMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\t��ס����\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : ������ס����\n");
        printf("\t\t      2 : ����ͷ�\n");
        printf("\t\t      0 : �����ϼ��˵�\n");
        printf("\t\t==============================\n");
        printf("��ѡ��");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            // ������ס������������
            break;
        case 2:
            // ����ͷ���������
            break;
        case 0:
            printf("�����ϼ��˵�...\n");
            break;
        default:
            printf("�������������ѡ��\n");
            break;
        }
        system("pause");
    } while (choice != 0);
}

// ǰ̨�˵�����
void frontDeskMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\tǰ̨�˵�\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : ��ס����\n");
        printf("\t\t      2 : ��Ϣ��ѯ\n");
        printf("\t\t      3 : ��Ϣ����\n");
        printf("\t\t      4 : ��Ϣͳ��\n");
        printf("\t\t      5 : ������Ϣά��\n");
        printf("\t\t      0 : �˳�\n");
        printf("\t\t==============================\n");
        printf("��ѡ��");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            checkInManagementMenu(); // ������ס�����Ӳ˵�
            break;
        case 2:
            informationQueryMenu(); // ������Ϣ��ѯ�Ӳ˵�
            break;
        case 3:
            informationSortMenu(); // ������Ϣ�����Ӳ˵�
            break;
        case 4:
            informationStatistics(); // ������Ϣͳ���Ӳ˵�
            break;
        case 5:
            passwordMaintenance();
            break;
        case 0:
            printf("�����˳�ǰ̨�˵�...\n");
            break;
        default:
            printf("�������������ѡ��\n");
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
        printf("\n\t\t\t���˲˵�\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : ��Ϣ��ѯ\n");
        printf("\t\t      2 : ��Ϣ����\n");
        printf("\t\t      3 : ��Ϣͳ��\n");
        printf("\t\t      4 : Ԥ������\n");
        printf("\t\t      5 : ����ά��\n");
        printf("\t\t      0 : �˳�\n");
        printf("\t\t==============================\n");
        printf("��ѡ��");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            customerQuery(); // ������Ϣ��ѯ�Ӳ˵�
            break;
        case 2:
            customerSort(); // ������Ϣ�����Ӳ˵�
            break;
        case 3:
            customerStatistics();
            break;
        case 4:
            myReservation();
            break;
        case 5:
            passwordMaintenance(); // ��������ά������
            break;
        case 0:
            printf("�����˳����˲˵�...\n");
            break;
        default:
            printf("�������������ѡ��\n");
            break;
        }
        system("pause");
    } while (choice != 0);
}
