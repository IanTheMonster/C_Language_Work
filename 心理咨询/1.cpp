#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_USERS 100
#define MAX_STUDENTS 100
#define MAX_TESTS 100
#define MAX_ARTICLES 100
// �û��ṹ��
typedef struct User
{
    char username[100];
    char password[100];
    int role; // 0��ʾѧ����1��ʾ��ʦ
} User;
// ѧ���ṹ��
typedef struct
{
    char username[100]; // ѧ���û���
    float mentalScore;  // ����÷�
    char id[100];       // ѧ��ID
    char name[100];     // ѧ������
    int age;            // ѧ������
} Student;
// ������Ŀ�ṹ��
typedef struct
{
    int testNo;        // ���Ա��
    char content[100]; // ��������
} Test;
// ���½ṹ��
typedef struct
{
    char date[100];  // ��������
    char title[100]; // ���±���
    char words[100]; // ��������
} Article;
// �û�����
User users[MAX_USERS];
int numUsers = 0;
User currentUser = {"", "", -1}; // ��ʼ����ǰ�û�Ϊ��
// ��ʼ��ѧ����Ϣ
Student currentStudent = {"", 0.0, "", "", 0}; // ��ѧ���ṹ������ĸ��ֶγ�ʼ��Ϊ�ջ�0
// ѧ������
Student students[MAX_STUDENTS];
int numStudents = 0;
// ������Ŀ����
Test tests[MAX_TESTS];
int numTests = 0;
// ��������
Article articles[MAX_ARTICLES];
int numArticles = 0;
void studentMenu();
void teacherMenu();
// �û���¼����
// �û���¼����
void userLogin()
{
    char username[100];
    char password[100];
    int role;

    printf("��ӭʹ��������ѯϵͳ\n");
    printf("��ѡ���¼��ɫ��\n");
    printf("1. ѧ��\n");
    printf("2. ��ʦ\n");
    printf("��ѡ�������");
    scanf("%d", &role);

    printf("�������û�����");
    scanf("%s", username);
    printf("���������룺");
    scanf("%s", password);

    if (role == 1)
    {
        // ѧ����¼�߼�
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
            printf("ѧ����¼�ɹ�\n");
            studentMenu();
        }
        else
        {
            printf("�û������������\n");
        }
    }
    else if (role == 2)
    {
        // ��ʦ��¼�߼�
        if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0)
        {
            printf("��ʦ��¼�ɹ�\n");
            teacherMenu();
        }
        else
        {
            printf("�û������������\n");
        }
    }
    else
    {
        printf("��Ч��ѡ��\n");
    }
}
// ѧ���˵�����
void studentMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n==================== ѧ���˵� ====================\n");
        printf("����,%s!\n", currentStudent.name);
        printf("1. �鿴/�޸ĸ�����Ϣ\n");
        printf("2. ���������֪ʶ������\n");
        printf("3. �����������\n");
        printf("4. ����ʦ��ѯ��������\n");
        printf("5. �˳�\n");
        printf("===================================================\n");
        printf("��ѡ�������");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("��ѡ���˲鿴/�޸ĸ�����Ϣ\n");
            // TODO: ���ò鿴/�޸ĸ�����Ϣ�ĺ���
            break;
        case 2:
            printf("��ѡ�������������֪ʶ������\n");
            // TODO: �������������֪ʶ�����µĺ���
            break;
        case 3:
            printf("��ѡ���˽����������\n");
            // TODO: ���ý���������Եĺ���
            break;
        case 4:
            printf("��ѡ��������ʦ��ѯ��������\n");
            // TODO: ��������ʦ��ѯ��������ĺ���
            break;
        case 5:
            printf("��ѡ�����˳�\n");
            break;
        default:
            printf("��Ч��ѡ������������\n");
        }
        system("pause");
    } while (choice != 5);
}
// ��ʦ�˵�����
void teacherMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n==================== ��ʦ�˵� ====================\n");
        printf("1. ����ѧ����Ϣ\n");
        printf("2. ɾ��ѧ����Ϣ\n");
        printf("3. �޸�ѧ����Ϣ\n");
        printf("4. ����ѧ����Ϣ\n");
        printf("5. ��������֪ʶ����\n");
        printf("6. ɾ������֪ʶ����\n");
        printf("7. �޸�����֪ʶ����\n");
        printf("8. ��������֪ʶ����\n");
        printf("9. ����������������Ŀ\n");
        printf("10. ɾ��������������Ŀ\n");
        printf("11. ��ʾ����������������Ŀ\n");
        printf("12. ��ʾ���������Խ��\n");
        printf("13. ��ѧ�����������Խ������\n");
        printf("14. �鿴���ظ�ѧ��˽��\n");
        printf("0. �˳�\n");
        printf("===================================================\n");
        printf("��ѡ�������");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("��ѡ��������ѧ����Ϣ\n");
            // TODO: ��������ѧ����Ϣ�ĺ���
            break;
        case 2:
            printf("��ѡ����ɾ��ѧ����Ϣ\n");
            // TODO: ����ɾ��ѧ����Ϣ�ĺ���
            break;
        case 3:
            printf("��ѡ�����޸�ѧ����Ϣ\n");
            // TODO: �����޸�ѧ����Ϣ�ĺ���
            break;
        case 4:
            printf("��ѡ���˲���ѧ����Ϣ\n");
            // TODO: ���ò���ѧ����Ϣ�ĺ���
            break;
        case 5:
            printf("��ѡ������������֪ʶ����\n");
            // TODO: ������������֪ʶ���µĺ���
            break;
        case 6:
            printf("��ѡ����ɾ������֪ʶ����\n");
            // TODO: ����ɾ������֪ʶ���µĺ���
            break;
        case 7:
            printf("��ѡ�����޸�����֪ʶ����\n");
            // TODO: �����޸�����֪ʶ���µĺ���
            break;
        case 8:
            printf("��ѡ���˲�������֪ʶ����\n");
            // TODO: ���ò�������֪ʶ���µĺ���
            break;
        case 9:
            printf("��ѡ��������������������Ŀ\n");
            // TODO: ��������������������Ŀ�ĺ���
            break;
        case 10:
            printf("��ѡ����ɾ��������������Ŀ\n");
            // TODO: ����ɾ��������������Ŀ�ĺ���
            break;
        case 11:
            printf("��ѡ������ʾ����������������Ŀ\n");
            // TODO: ������ʾ����������������Ŀ�ĺ���
            break;
        case 12:
            printf("��ѡ������ʾ���������Խ��\n");
            // TODO: ������ʾ���������Խ���ĺ���
            break;
        case 13:
            printf("��ѡ���˶�ѧ�����������Խ������\n");
            // TODO: ���ö�ѧ�����������Խ������ĺ���
            break;
        case 14:
            printf("��ѡ���˲鿴���ظ�ѧ��˽��\n");
            // TODO: ���ò鿴���ظ�ѧ��˽�ĵĺ���
            break;
        case 0:
            printf("��ѡ�����˳�\n");
            break;
        default:
            printf("��Ч��ѡ������������\n");
        }
        system("pause");
    } while (choice != 0);
}
