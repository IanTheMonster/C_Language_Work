#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILE_NAME "guards.txt"
#define MAX_GUARDS 7
#define DAYS_IN_WEEK 7
const char *days[DAYS_IN_WEEK] = {"����", "��һ", "�ܶ�", "����", "����", "����", "����"};
typedef struct
{
    int id;
    char name[50];
    int off_days[DAYS_IN_WEEK]; // 1 ��ʾ��Ϣ��0 ��ʾ����
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
        getchar(); // ����������Ļ��з�

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
            printf("�˳�����\n");
            break;
        default:
            printf("��Ч��ѡ��\n");
        }
    } while (choice != 9);

    return 0;
}
void printMenu()
{
    printf("\n==========================\n");
    printf("   ����ֵ�ల��ϵͳ\n");
    printf("==========================\n");
    printf("1. ��ӱ���\n");
    printf("2. �޸ı�����Ϣ\n");
    printf("3. ɾ������\n");
    printf("4. ���ұ���\n");
    printf("5. ����������\n");
    printf("6. �г����б���\n");
    printf("7. ��ӡ�������ݷ���\n");
    printf("8. ��ӡÿ�չ������ű�\n");
    printf("9. �˳�\n");
    printf("==========================\n");
    printf("ѡ��һ������: ");
}

int loadData(Guard guards[])
{
    FILE *file = fopen(FILE_NAME, "r");
    if (!file)
        return 0; // �ļ��������򷵻�0

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
        printf("�Ѵﵽ��󱣰�������\n");
        return;
    }

    guards[size].id = getNewID(guards, size);
    printf("���뱣������: ");
    scanf("%49s", guards[size].name);
    for (int i = 0; i < DAYS_IN_WEEK; i++)
    {
        guards[size].off_days[i] = 0; // Initially set all days to working
    }

    size++;
    saveData(guards, size);
    printf("������ӳɹ���\n");
}
void modifyGuard()
{
    Guard guards[MAX_GUARDS];
    int size = loadData(guards);
    int id, found = -1;

    printf("����Ҫ�޸ĵı���ID: ");
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
        printf("δ�ҵ�ָ���ı�����\n");
        return;
    }

    printf("Ҫ�޸ĵı�����Ϣ���£�\n");
    printf("����ID: %d\n", guards[found].id);
    printf("1. ��������: %s\n", guards[found].name);
    printf("2. ����������:\n");
    for (int i = 0; i < DAYS_IN_WEEK; i++)
    {
        printf("   %s�� %s\n", days[i], guards[found].off_days[i] == 1 ? "��Ϣ" : "����");
    }

    printf("ѡ��Ҫ�޸ĵ��ֶ� (1-2): ");
    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf("�����µı�������: ");
        scanf("%49s", guards[found].name);
        break;
    case 2:
        printf("���������գ�\n");
        for (int i = 0; i < DAYS_IN_WEEK; i++)
        {
            printf("��%sΪ��Ϣ�� (1Ϊ��Ϣ, 0Ϊ����): ", days[i]);
            scanf("%d", &guards[found].off_days[i]);
        }
        break;
    default:
        printf("��Ч��ѡ��\n");
        return;
    }

    saveData(guards, size);
    printf("������Ϣ�Ѹ��¡�\n");
}

void deleteGuard()
{
    Guard guards[MAX_GUARDS];
    int size = loadData(guards);
    int id, found = -1;

    printf("����Ҫɾ���ı���ID: ");
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
        printf("δ�ҵ�ָ���ı�����\n");
        return;
    }

    printf("ȷ��ɾ�����±�����Ϣ��\n");
    printf("����ID: %d\n", guards[found].id);
    printf("��������: %s\n", guards[found].name);
    printf("����������: ");
    for (int i = 0; i < DAYS_IN_WEEK; i++)
    {
        if (guards[found].off_days[i] == 1)
        {
            printf("%s ", days[i]);
        }
    }
    printf("\n");

    printf("ȷ��ɾ����(y/n): ");
    char confirmation;
    scanf(" %c", &confirmation);

    if (confirmation != 'y' && confirmation != 'Y')
    {
        printf("ȡ��ɾ����\n");
        return;
    }

    for (int i = found; i < size - 1; i++)
    {
        guards[i] = guards[i + 1];
    }

    size--;
    saveData(guards, size);
    printf("����ɾ���ɹ���\n");
}

void findGuard()
{
    Guard guards[MAX_GUARDS];
    int size = loadData(guards);
    int id, found = -1;

    printf("����Ҫ���ҵı���ID: ");
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
        printf("δ�ҵ�ָ���ı�����\n");
        return;
    }

    printf("����ID: %d\n", guards[found].id);
    printf("��������: %s\n", guards[found].name);
    printf("������Ϣ��: ");
    for (int i = 0; i < DAYS_IN_WEEK; i++)
    {
        if (guards[found].off_days[i] == 1)
        {
            printf("%s ", i == 0 ? "����" : i == 1 ? "��һ"
                                        : i == 2   ? "�ܶ�"
                                        : i == 3   ? "����"
                                        : i == 4   ? "����"
                                        : i == 5   ? "����"
                                                   : "����");
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
    printf("���뱣��ID����������: ");
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
        printf("δ�ҵ�ָ���ı�����\n");
        return;
    }

    printf("�������� (0=����, 1=��һ, ..., 6=����): ");
    scanf("%d", &day);
    printf("����Ϊ��Ϣ(1)���ǹ���(0)��");
    scanf("%d", &set);
    guards[found].off_days[day] = set;

    saveData(guards, size);
    printf("���������óɹ���\n");
}

void listGuards()
{
    Guard guards[MAX_GUARDS];
    int size = loadData(guards);

    if (size == 0)
    {
        printf("û�б�����Ϣ��\n");
        return;
    }

    printf("�����б�:\n");
    for (int i = 0; i < size; i++)
    {
        printf("����ID: %d, ����: %s, ������: ", guards[i].id, guards[i].name);
        for (int j = 0; j < DAYS_IN_WEEK; j++)
        {
            if (guards[i].off_days[j] == 1)
            {
                printf("%s ", j == 0 ? "����" : j == 1 ? "��һ"
                                            : j == 2   ? "�ܶ�"
                                            : j == 3   ? "����"
                                            : j == 4   ? "����"
                                            : j == 5   ? "����"
                                                       : "����");
            }
        }
        printf("\n");
    }
}
void printScheduleTable()
{
    Guard guards[MAX_GUARDS];
    int count = loadData(guards);
    const char *days[] = {"����", "��һ", "�ܶ�", "����", "����", "����", "����"};

    printf("\nÿ�ձ����������ű�\n");
    printf("| %-6s | %-40s |\n", "����", "���չ����ı���");
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
    const char *days[] = {"����", "��һ", "�ܶ�", "����", "����", "����", "����"};
    Guard guards[MAX_GUARDS];
    int count = loadData(guards);

    printf("\n���б��������ݷ�����\n");
    printf("| %-4s | %-20s | %-30s |\n", "ID", "��������", "������");
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
