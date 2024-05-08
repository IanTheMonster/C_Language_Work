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

// ����ṹ�嶨��
typedef struct Room
{
    int roomId;        // �����
    float area;        // ���
    float price;       // �۸�
    char roomType[20]; // ��������
    int isOccupied;    // �Ƿ����
    struct Room *next; // ָ����һ�������ָ��
} Room;

// �ͻ��ṹ�嶨��
typedef struct Customer
{
    char ID[20];           // �ͻ�ID
    char type[20];         // �ͻ�����
    float discount;        // �ۿ�
    struct Customer *next; // ָ����һ���ͻ���ָ��
} Customer;

// Ԥ���ṹ�嶨��
typedef struct Reservation
{
    char CustomerId[20];      // �û���
    int roomId;               // �����
    float price;              // �۸�
    char startDate[20];       // ��ʼ����
    char endDate[20];         // ��������
    char roomType[20];        // ��������
    int isCheckedIn;          // �Ƿ���ס
    struct Reservation *next; // ָ����һ��Ԥ����ָ��
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

// ȫ�ֱ������û�����͵�ǰ�û�
User users[1000];
int numUsers = 0;
User *currentUser = NULL;

// ȫ�ֱ��������䡢�ͻ���Ԥ��������ͷָ��
Room *roomsList = NULL;
Customer *customersList = NULL;
Reservation *reservationsList = NULL;
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
// ��ӿͷ���������
void addRoom();
// ɾ���ͷ���������
void deleteRoom();
// �޸Ŀͷ���Ϣ��������
void modifyRoom();
// ��ѯ�ͷ���Ϣ��������
void queryRoom();
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
// �Ƚ��������ڵ��Ⱥ�˳�����date1����date2�����ظ��������date1����date2�����������������ȣ�����0
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
        printf("�޷������ͷ��ļ���\n");
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
        printf("�޷��򿪿ͷ��ļ���\n");
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
        printf("�޷����������ļ���\n");
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
        printf("�޷��򿪿����ļ���\n");
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
        printf("�޷�����Ԥ���ļ���\n");
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
        printf("�޷���Ԥ���ļ���\n");
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
// ��ӿͷ�����
void addRoom()
{
    char choice; // ���ڼ�¼�û��Ƿ������ӵ�ѡ��

    do
    {
        Room *newRoom = (Room *)malloc(sizeof(Room)); // �����¿ͷ����ڴ�ռ�

        // ��ȡ�û�����Ŀͷ���Ϣ
        printf("�������¿ͷ�����Ϣ��\n");
        printf("����ţ�");
        scanf("%d", &(newRoom->roomId));
        printf("�����");
        scanf("%f", &(newRoom->area));

        // ��ʾ�������Ͳ˵����û�ѡ��
        printf("��ѡ�񷿼����ͣ�\n");
        printf("1. �� (�۸�150)\n");
        printf("2. ˫�� (�۸�200)\n");
        printf("3. �׷� (�۸�400)\n");
        int roomTypeChoice;
        scanf("%d", &roomTypeChoice);

        // �����û�ѡ�����÷������ͺͼ۸�
        switch (roomTypeChoice)
        {
        case 1:
            strcpy(newRoom->roomType, "��");
            newRoom->price = 150;
            break;
        case 2:
            strcpy(newRoom->roomType, "˫��");
            newRoom->price = 200;
            break;
        case 3:
            strcpy(newRoom->roomType, "�׷�");
            newRoom->price = 400;
            break;
        default:
            printf("��Ч��ѡ���������Ĭ������Ϊ�󴲣��۸�Ĭ������Ϊ150��\n");
            strcpy(newRoom->roomType, "��");
            newRoom->price = 150;
            break;
        }

        // �����¿ͷ�����һ��ָ��Ϊ��ǰ�����ͷָ�룬������뵽����ͷ��
        newRoom->next = roomsList;
        roomsList = newRoom;

        printf("�ͷ���ӳɹ���\n");

        // ѯ���û��Ƿ�������
        printf("�Ƿ������ӿͷ���Ϣ��(y/n): ");
        scanf(" %c", &choice); // ע������Ŀո�����������һ�������еĻ��з�

    } while (choice == 'y' || choice == 'Y'); // ����û����� 'y' �� 'Y' ��������
}
// ɾ���ͷ�����
void deleteRoom()
{
    int roomNumber;
    char choice;

    do
    {
        Room *prev = NULL;
        Room *current = roomsList;

        printf("������Ҫɾ���Ŀͷ��ķ���ţ�");
        scanf("%d", &roomNumber);

        // ���������ҵ�Ҫɾ���ķ���
        while (current != NULL && current->roomId != roomNumber)
        {
            prev = current;
            current = current->next;
        }

        // ����ҵ���Ҫɾ���ķ���
        if (current != NULL)
        {
            // ���Ҫɾ����������ĵ�һ���ڵ�
            if (prev == NULL)
            {
                roomsList = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            // �ͷ��ڴ�
            free(current);
            printf("�ͷ�ɾ���ɹ���\n");
        }
        else
        {
            printf("δ�ҵ��÷���ŵĿͷ���\n");
        }

        // ѯ���û��Ƿ����ɾ��
        printf("�Ƿ����ɾ���ͷ���(y/n): ");
        scanf(" %c", &choice); // ע������Ŀո�����������һ�������еĻ��з�

    } while (choice == 'y' || choice == 'Y'); // ����û����� 'y' �� 'Y' �����ɾ��
}
// �޸Ŀͷ���Ϣ����
void modifyRoom()
{
    int roomNumber;
    char choice;

    do
    {
        Room *current = roomsList;

        printf("������Ҫ�޸���Ϣ�Ŀͷ��ķ���ţ�");
        scanf("%d", &roomNumber);

        // ���������ҵ�Ҫ�޸���Ϣ�Ŀͷ�
        while (current != NULL && current->roomId != roomNumber)
        {
            current = current->next;
        }

        // ����ҵ���Ҫ�޸���Ϣ�Ŀͷ�
        if (current != NULL)
        {
            int fieldChoice;
            printf("��ѡ��Ҫ�޸ĵ��ֶΣ�\n");
            printf("1. ���\n");
            printf("2. �۸�\n");
            printf("3. ��������\n");
            printf("��ѡ��");
            scanf("%d", &fieldChoice);

            // �����û�ѡ�������Ӧ����Ϣ
            switch (fieldChoice)
            {
            case 1:
                printf("�������µ������");
                scanf("%f", &(current->area));
                break;
            case 2:
                printf("�������µļ۸�");
                scanf("%f", &(current->price));
                break;
            case 3:
                printf("�������µķ������ͣ�");
                scanf("%s", current->roomType);
                break;
            default:
                printf("��Ч��ѡ�\n");
                break;
            }

            printf("�ͷ���Ϣ�޸ĳɹ���\n");
        }
        else
        {
            printf("δ�ҵ��÷���ŵĿͷ���\n");
        }

        // ѯ���û��Ƿ�����޸�
        printf("�Ƿ�����޸Ŀͷ���Ϣ��(y/n): ");
        scanf(" %c", &choice); // ע������Ŀո�����������һ�������еĻ��з�

    } while (choice == 'y' || choice == 'Y'); // ����û����� 'y' �� 'Y' ������޸�
}
// �򵥲�ѯ�ͷ�����
void simpleSearchRoom()
{
    int roomNumber;
    printf("������Ҫ��ѯ�Ŀͷ�����ţ�");
    scanf("%d", &roomNumber);

    // �������в��ҷ����ƥ��Ŀͷ��������Ϣ
    Room *current = roomsList;
    while (current != NULL)
    {
        if (current->roomId == roomNumber)
        {
            // ����ͷ���Ϣ
            printf("����ţ�%d\n", current->roomId);
            printf("�����%f\n", current->area);
            printf("�۸�%f\n", current->price);
            printf("�������ͣ�%s\n", current->roomType);
            printf("\n");
            return; // ��ѯ���ͷ���ֱ�ӷ���
        }
        current = current->next;
    }

    // ���û���ҵ�ƥ��Ŀͷ�
    printf("δ�ҵ������Ϊ%d�Ŀͷ���\n", roomNumber);
}
// ģ����ѯ�ͷ�����
void fuzzySearchRoom()
{
    char keyword[100];
    printf("�����뷿�������йز�ѯ�ؼ��ʣ�");
    scanf("%s", keyword);

    // �������в��Ұ����ؼ��ʵĿͷ��������Ϣ
    Room *current = roomsList;
    while (current != NULL)
    {
        // ʹ��strstr�����жϷ����������Ƿ�����ؼ���
        if (strstr(current->roomType, keyword) != NULL)
        {
            // ����ͷ���Ϣ
            printf("����ţ�%d\n", current->roomId);
            printf("�����%f\n", current->area);
            printf("�۸�%f\n", current->price);
            printf("�������ͣ�%s\n", current->roomType);
            printf("\n");
        }
        current = current->next;
    }
}
// ��ѯ�ͷ��Ƿ�գ�ĳʱ��Σ�
void checkRoomAvailability()
{
    char startDate[20];
    char endDate[20];
    int roomNumber;

    printf("������Ҫ��ѯ�Ŀͷ�����ţ�");
    scanf("%d", &roomNumber);
    printf("��������ʼ���ڣ���ʽ��YYYY-MM-DD����");
    scanf("%s", startDate);
    printf("������������ڣ���ʽ��YYYY-MM-DD����");
    scanf("%s", endDate);

    // ��Ԥ����Ϣ�в����Ƿ�����ָ�������ʱ����ص���Ԥ����¼
    for (Reservation *currentReservation = reservationsList; currentReservation != NULL; currentReservation = currentReservation->next)
    {
        if (currentReservation->roomId == roomNumber)
        {
            // ���ʱ������ص�����ͷ�����
            if ((compareDates(currentReservation->startDate, endDate) < 0 || compareDates(currentReservation->endDate, startDate) > 0) && currentReservation->isCheckedIn == 1)
            {
                printf("�ͷ������Ϊ%d��ָ��ʱ����ڲ��ա�\n", roomNumber);
                return;
            }
        }
    }

    // û���ҵ��ص���Ԥ����¼����ͷ���
    printf("�ͷ������Ϊ%d��ָ��ʱ����ڿ��С�\n", roomNumber);
}

// ��ѯĳ����Ԥ����Ϣ
void searchCustomerReservation()
{
    char customerID[20];

    printf("������Ҫ��ѯԤ����Ϣ�Ŀ���ID��");
    scanf("%s", customerID);

    // ��Ԥ����Ϣ�в�����ָ������IDƥ���Ԥ����¼
    printf("����IDΪ%s��Ԥ����Ϣ���£�\n", customerID);
    for (Reservation *currentReservation = reservationsList; currentReservation != NULL; currentReservation = currentReservation->next)
    {
        if (strcmp(currentReservation->CustomerId, customerID) == 0)
        {
            printf("����ţ�%d\n", currentReservation->roomId);
            printf("�۸�%f\n", currentReservation->price);
            printf("��ʼ���ڣ�%s\n", currentReservation->startDate);
            printf("�������ڣ�%s\n", currentReservation->endDate);
            printf("�Ƿ���ס��%s\n", currentReservation->isCheckedIn ? "��" : "��");
            printf("\n");
        }
    }
}
// ��ʾ���з�����Ϣ����
void displayAllRooms()
{
    printf("�����\t\t���\t\t�۸�\t\t��������\n");
    printf("===============================================================\n");

    Room *currentRoom = roomsList;
    while (currentRoom != NULL)
    {
        printf("%d\t\t%.2f\t\t%.2f\t\t%s\n", currentRoom->roomId, currentRoom->area, currentRoom->price, currentRoom->roomType, currentRoom->isOccupied ? "��ռ��" : "����");
        currentRoom = currentRoom->next;
    }
}

// ��ѯ�ͷ�����
void queryRoom()
{
    int choice;

    do
    {
        printf("��ѡ���ѯ��ʽ��\n");
        printf("1. �򵥲�ѯ�ͷ���Ϣ\n");
        printf("2. ģ����ѯ�ͷ���Ϣ\n");
        printf("3. ��ѯ�ͷ��Ƿ�գ�ĳʱ��Σ�\n");
        printf("4. ��ѯ����Ԥ����Ϣ\n");
        printf("0. �����ϼ��˵�\n");
        printf("��ѡ��");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            simpleSearchRoom(); // �򵥲�ѯ
            break;
        case 2:
            fuzzySearchRoom(); // ģ����ѯ
            break;
        case 3:
            checkRoomAvailability(); // ��ѯ�ͷ��Ƿ��
            break;
        case 4:
            searchCustomerReservation(); // ��ѯ����Ԥ����Ϣ
            break;
        case 0:
            printf("�����ϼ��˵�...\n");
            break;
        default:
            printf("��Ч��ѡ�������ѡ��\n");
            break;
        }

    } while (choice != 0);
}

// �ͷ�����˵�����
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
        printf("\t\t      5 : ��ʾ���пͷ���Ϣ\n");
        printf("\t\t      0 : �����ϼ��˵�\n");
        printf("\t\t==============================\n");
        printf("��ѡ��");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addRoom(); // ��ӿͷ���������
            break;
        case 2:
            deleteRoom(); // ɾ���ͷ���������
            break;
        case 3:
            modifyRoom(); // �޸Ŀͷ���Ϣ��������
            break;
        case 4:
            queryRoom(); // ��ѯ�ͷ���Ϣ��������
            break;
        case 5:
            displayAllRooms();
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
// ��ӿ��˺���
void addCustomer()
{
    Customer *newCustomer = (Customer *)malloc(sizeof(Customer));
    if (newCustomer == NULL)
    {
        printf("�ڴ����ʧ�ܡ�\n");
        return;
    }

    printf("���������ID��");
    scanf("%s", newCustomer->ID);
    printf("������������ͣ�");
    scanf("%s", newCustomer->type);
    printf("����������ۿۣ�");
    scanf("%f", &newCustomer->discount);

    // ���¿��˽ڵ���뵽����ͷ��
    newCustomer->next = customersList;
    customersList = newCustomer;

    printf("������ӳɹ���\n");
}

// ɾ�����˺���
void deleteCustomer()
{
    char customerID[20];
    printf("������Ҫɾ���Ŀ���ID��");
    scanf("%s", customerID);

    Customer *prevCustomer = NULL;
    Customer *currentCustomer = customersList;

    // �����������Ҫɾ���Ŀ��˽ڵ�
    while (currentCustomer != NULL && strcmp(currentCustomer->ID, customerID) != 0)
    {
        prevCustomer = currentCustomer;
        currentCustomer = currentCustomer->next;
    }

    // ����ҵ���Ҫɾ���Ŀ��˽ڵ�
    if (currentCustomer != NULL)
    {
        // ���Ҫɾ����������ͷ���ڵ�
        if (prevCustomer == NULL)
        {
            customersList = currentCustomer->next;
        }
        else
        {
            prevCustomer->next = currentCustomer->next;
        }

        free(currentCustomer);
        printf("����ɾ���ɹ���\n");
    }
    else
    {
        printf("δ�ҵ�Ҫɾ���Ŀ��ˡ�\n");
    }
}
// �޸Ŀ�����Ϣ����
void modifyCustomer()
{
    char customerID[20];
    printf("������Ҫ�޸���Ϣ�Ŀ���ID��");
    scanf("%s", customerID);

    Customer *currentCustomer = customersList;

    // �����������Ҫ�޸���Ϣ�Ŀ��˽ڵ�
    while (currentCustomer != NULL && strcmp(currentCustomer->ID, customerID) != 0)
    {
        currentCustomer = currentCustomer->next;
    }

    // ����ҵ���Ҫ�޸���Ϣ�Ŀ��˽ڵ�
    if (currentCustomer != NULL)
    {
        int choice;
        printf("��ѡ��Ҫ�޸ĵ��ֶΣ�\n");
        printf("1. ��������\n");
        printf("2. �����ۿ�\n");
        printf("��ѡ��");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("��ѡ��������ͣ�\n");
            printf("1. ����\n");
            printf("2. �ƽ�\n");
            printf("3. �׽�\n");
            printf("4. ��ʹ\n");
            printf("��ѡ��");
            int typeChoice;
            scanf("%d", &typeChoice);
            switch (typeChoice)
            {
            case 1:
                strcpy(currentCustomer->type, "����");
                currentCustomer->discount = 0.9; // �����ۿ�
                break;
            case 2:
                strcpy(currentCustomer->type, "�ƽ�");
                currentCustomer->discount = 0.85; // �ƽ��ۿ�
                break;
            case 3:
                strcpy(currentCustomer->type, "�׽�");
                currentCustomer->discount = 0.8; // �׽��ۿ�
                break;
            case 4:
                strcpy(currentCustomer->type, "��ʹ");
                currentCustomer->discount = 0.75; // ��ʹ�ۿ�
                break;
            default:
                printf("�������������ѡ��\n");
                return;
            }
            break;
        case 2:
            printf("����������ۿۣ�");
            scanf("%f", &currentCustomer->discount);
            break;
        default:
            printf("�������������ѡ��\n");
            return;
        }

        printf("������Ϣ�޸ĳɹ���\n");
    }
    else
    {
        printf("δ�ҵ�Ҫ�޸���Ϣ�Ŀ��ˡ�\n");
    }
}

// ��ѯ������Ϣ����
void searchCustomer()
{
    char customerID[20];
    printf("������Ҫ��ѯ��Ϣ�Ŀ���ID��");
    scanf("%s", customerID);

    Customer *currentCustomer = customersList;

    // �����������Ҫ��ѯ��Ϣ�Ŀ��˽ڵ�
    while (currentCustomer != NULL && strcmp(currentCustomer->ID, customerID) != 0)
    {
        currentCustomer = currentCustomer->next;
    }

    // ����ҵ���Ҫ��ѯ��Ϣ�Ŀ��˽ڵ�
    if (currentCustomer != NULL)
    {
        printf("����ID��%s\n", currentCustomer->ID);
        printf("�������ͣ�%s\n", currentCustomer->type);
        printf("�����ۿۣ�%f\n", currentCustomer->discount);
    }
    else
    {
        printf("δ�ҵ�Ҫ��ѯ��Ϣ�Ŀ��ˡ�\n");
    }
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
            printf("�����ϼ��˵�...\n");
            break;
        default:
            printf("�������������ѡ��\n");
            break;
        }
        system("pause");
    } while (choice != 0);
}

// ������������֮�����������ĺ���
int dateDifference(const char *start, const char *end)
{
    struct tm tmStart = {0}, tmEnd = {0};
    int year, month, day;

    // ������ʼ����
    sscanf(start, "%d-%d-%d", &year, &month, &day);
    tmStart.tm_year = year - 1900; // tm_year����1900������������
    tmStart.tm_mon = month - 1;    // tm_mon����������ģ�1��Ϊ0��
    tmStart.tm_mday = day;

    // ������������
    sscanf(end, "%d-%d-%d", &year, &month, &day);
    tmEnd.tm_year = year - 1900;
    tmEnd.tm_mon = month - 1;
    tmEnd.tm_mday = day;

    // ����Сʱ��ϢΪ0
    tmStart.tm_hour = 0;
    tmEnd.tm_hour = 0;

    // ת��Ϊtime_t
    time_t tStart = mktime(&tmStart);
    time_t tEnd = mktime(&tmEnd);

    // ȷ��������Ч
    if (tStart == (time_t)(-1) || tEnd == (time_t)(-1))
    {
        return -1; // ���������Ч���򷵻�-1���ʵ��Ĵ������
    }

    // ������������
    return (int)((tEnd - tStart) / (24 * 3600));
}

// ��ͳ�ƺ���
void informationStatistics()
{
    char startDate[11], endDate[11];
    printf("������ͳ�ƵĿ�ʼ���� (YYYY-MM-DD): ");
    scanf("%10s", startDate);
    printf("������ͳ�ƵĽ������� (YYYY-MM-DD): ");
    scanf("%10s", endDate);

    int totalDays = dateDifference(startDate, endDate) + 1;
    if (totalDays <= 0)
    {
        printf("�������ڱ�����ڿ�ʼ���ڡ�\n");
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
        printf("�Ƶ���û�з������ݡ�\n");
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
            // ����Ԥ�����ǵ�ʱ����ڵ�ʵ������
            char effectiveStart[11], effectiveEnd[11];
            strcpy(effectiveStart, (compareDates(resIter->startDate, startDate) < 0) ? startDate : resIter->startDate);
            strcpy(effectiveEnd, (compareDates(resIter->endDate, endDate) > 0) ? endDate : resIter->endDate);
            int coveredDays = dateDifference(effectiveStart, effectiveEnd) + 1;

            bookedRoomDays += coveredDays; // Ԥ�������ۼ�
            if (resIter->isCheckedIn)
            {
                occupiedRoomDays += coveredDays; // ��ס�����ۼ�
            }
        }
        resIter = resIter->next;
    }

    double bookingRate = (double)bookedRoomDays / totalRoomDays * 100.0;
    double occupancyRate = (double)occupiedRoomDays / totalRoomDays * 100.0;

    printf("ͳ��ʱ��Σ�%s �� %s\n", startDate, endDate);
    printf("����������%d��ͳ����������%d\n", totalRooms, totalDays);
    printf("Ԥ���ʣ�%.2f%%\n", bookingRate);
    printf("��ס�ʣ�%.2f%%\n", occupancyRate);
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
// ��Ϣ�����Ӳ˵�����
void informationQueryMenu()
{
    queryRoom();
}
// ���ܣ�����ÿ���������ס������������ס�����������򷿼�
void sortByRoomOccupancy()
{
    if (roomsList == NULL)
    {
        printf("û�з�����Ϣ�ɹ�����\n");
        return;
    }

    // ��һ��������ÿ���������ס����
    Room *roomPtr = roomsList;
    int roomCount = 0;
    while (roomPtr != NULL)
    {
        roomCount++;
        roomPtr = roomPtr->next;
    }

    // �����������洢���估����ס����
    RoomOccupancy *occupancies = (RoomOccupancy *)malloc(sizeof(RoomOccupancy) * roomCount);
    roomPtr = roomsList;
    int index = 0;
    while (roomPtr != NULL)
    {
        occupancies[index].roomId = roomPtr->roomId;
        occupancies[index].occupancyCount = 0; // ��ʼ����ס����Ϊ0

        // ����Ԥ���б�ͳ����ס����
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

    // �ڶ�����ʹ�ò��������㷨����ס������������
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

    // ��ӡ�����ķ�����Ϣ
    printf("�����\t��ס����\n");
    for (int i = 0; i < roomCount; i++)
    {
        printf("%d\t%d\n", occupancies[i].roomId, occupancies[i].occupancyCount);
    }

    // �ͷ��ڴ�
    free(occupancies);
}

// ���ܣ��������ԤԼ��������
void sortByReservationCount()
{
    if (roomsList == NULL)
    {
        printf("û�з�����Ϣ�ɹ�����\n");
        return;
    }

    // ��һ��������ÿ�������ԤԼ����
    Room *roomPtr = roomsList;
    int roomCount = 0;
    while (roomPtr != NULL)
    {
        roomCount++;
        roomPtr = roomPtr->next;
    }

    // �����������洢���估��ԤԼ����
    RoomReservationCount *reservationCounts = (RoomReservationCount *)malloc(sizeof(RoomReservationCount) * roomCount);
    if (!reservationCounts)
    {
        printf("�ڴ����ʧ�ܡ�\n");
        return;
    }

    roomPtr = roomsList;
    int index = 0;
    while (roomPtr != NULL)
    {
        reservationCounts[index].roomId = roomPtr->roomId;
        reservationCounts[index].reservationCount = 0; // ��ʼ��ԤԼ����Ϊ0

        // ����Ԥ���б�ͳ��ÿ�������ԤԼ����
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

    // �ڶ�����ʹ�ò��������㷨��ԤԼ������������
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

    // ��ӡ�����ķ�����Ϣ
    printf("�����\tԤԼ����\n");
    for (int i = 0; i < roomCount; i++)
    {
        printf("%d\t%d\n", reservationCounts[i].roomId, reservationCounts[i].reservationCount);
    }

    // �ͷ��ڴ�
    free(reservationCounts);
}

// ���ܣ�������ס���������
void sortByCustomerStays()
{
    if (customersList == NULL)
    {
        printf("û�п�����Ϣ�ɹ�����\n");
        return;
    }

    // ��һ��������ÿ�����˵�ס�����
    Customer *customerPtr = customersList;
    int customerCount = 0;
    while (customerPtr != NULL)
    {
        customerCount++;
        customerPtr = customerPtr->next;
    }

    // �����������洢���˼���ס�����
    CustomerStayCount *stayCounts = (CustomerStayCount *)malloc(sizeof(CustomerStayCount) * customerCount);
    if (!stayCounts)
    {
        printf("�ڴ����ʧ�ܡ�\n");
        return;
    }

    customerPtr = customersList;
    int index = 0;
    while (customerPtr != NULL)
    {
        strcpy(stayCounts[index].customerId, customerPtr->ID);
        stayCounts[index].stayCount = 0; // ��ʼ��ס�����Ϊ0

        // ����Ԥ���б�ͳ��ÿ�����˵���ס����
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

    // �ڶ�����ʹ�ò��������㷨��ס�������������
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

    // ��ӡ�����Ŀ�����Ϣ
    printf("����ID\tס�����\n");
    for (int i = 0; i < customerCount; i++)
    {
        printf("%s\t%d\n", stayCounts[i].customerId, stayCounts[i].stayCount);
    }

    // �ͷ��ڴ�
    free(stayCounts);
}

// ��Ϣ�����Ӳ˵�����
void informationSortMenu()
{
    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\t��Ϣ����\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : ��������ס��������\n");
        printf("\t\t      2 : ��ԤԼ��������\n");
        printf("\t\t      3 : ������ס���������\n");
        printf("\t\t      0 : �����ϼ��˵�\n");
        printf("\t\t==============================\n");
        printf("��ѡ��");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            sortByRoomOccupancy(); // ��������ס��������
            break;
        case 2:
            sortByReservationCount(); // ��ԤԼ��������
            break;
        case 3:
            sortByCustomerStays(); // ������ס���������
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
// ������ס��������
void checkInProcedure()
{
    char customerID[20];
    char startDate[20], endDate[20];
    int roomTypeChoice, roomFound = 0;
    Room *selectedRoom = NULL;

    printf("���������ID��");
    scanf("%s", customerID);

    // ����Ƿ���ڸÿ���Ԥ����Ϣ
    Reservation *currentReservation = reservationsList;
    while (currentReservation != NULL)
    {
        if (strcmp(currentReservation->CustomerId, customerID) == 0 && !currentReservation->isCheckedIn)
        {
            // �ҵ��˸ÿ��˵�Ԥ����Ϣ��δ��ס������Ԥ���ķ����
            printf("�ÿ�����Ԥ�����䣬������ԤԼ��Ϣ��\n");
            printf("����ID��%s\n", currentReservation->CustomerId);
            printf("����ţ�%d\n", currentReservation->roomId);
            printf("�۸�%.2f\n", currentReservation->price);
            printf("ԤԼ��ʼʱ�䣺%s\n", currentReservation->startDate);
            printf("ԤԼ����ʱ�䣺%s\n", currentReservation->endDate);
            printf("�Ƿ���ס��%s\n", currentReservation->isCheckedIn ? "��" : "��");

            currentReservation->isCheckedIn = 1; // ���Ϊ����ס
            roomFound = 1;

            // ��Ǹ÷���Ϊ����ס״̬
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
            printf("��סԤԼ����ɹ�!\n");
            return;
        }
        currentReservation = currentReservation->next;
    }

    if (!roomFound)
    {
        // û��Ԥ����Ϣ�������µķ������
        printf("δ�ҵ�Ԥ����Ϣ��������µķ�����䡣\n");
        printf("��������ʼ���ڣ���ʽ��YYYY-MM-DD����");
        scanf("%s", startDate);
        printf("������������ڣ���ʽ��YYYY-MM-DD����");
        scanf("%s", endDate);

        printf("��ѡ�񷿼����ͣ�\n");
        printf("1. ��\n");
        printf("2. ˫��\n");
        printf("3. �׷�\n");
        printf("��ѡ��");
        scanf("%d", &roomTypeChoice);

        char roomType[20];
        switch (roomTypeChoice)
        {
        case 1:
            strcpy(roomType, "��");
            break;
        case 2:
            strcpy(roomType, "˫��");

            break;
        case 3:
            strcpy(roomType, "�׷�");
            break;
        default:
            printf("��Ч��ѡ�ѡ��Ĭ��Ϊ�󴲡�\n");
            strcpy(roomType, "��");
            break;
        }

        Room *currentRoom = roomsList;
        while (currentRoom != NULL)
        {
            if (strcmp(currentRoom->roomType, roomType) == 0 && !currentRoom->isOccupied)
            {
                // ����Ƿ������ʱ����ص���Ԥ��
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
                    currentRoom->isOccupied = 1; // ��Ƿ���Ϊ����ס
                    printf("����ͷ��ɹ�������ţ�%d\n", currentRoom->roomId);
                    roomFound = 1;

                    // ��ӵ�Ԥ���б�
                    Reservation *newReservation = (Reservation *)malloc(sizeof(Reservation));
                    if (newReservation != NULL)
                    {
                        strcpy(newReservation->CustomerId, customerID);
                        newReservation->roomId = currentRoom->roomId;
                        newReservation->price = currentRoom->price; // �ٶ��۸񲻱�
                        strcpy(newReservation->startDate, startDate);
                        strcpy(newReservation->endDate, endDate);
                        newReservation->isCheckedIn = 1; // �Ѿ���ס
                        newReservation->next = reservationsList;
                        reservationsList = newReservation;
                    }
                    break; // �ҵ����÷��䣬����ѭ��
                }
            }
            currentRoom = currentRoom->next;
        }

        if (!roomFound)
        {
            // û�п��÷���
            printf("��Ǹ����ǰû�з��������Ŀ��пͷ���\n");
        }
    }
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
        printf("\t\t      1 : ������ס\n");
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
            checkInProcedure();
            break;
        case 2:
            informationQueryMenu();
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

    printf("�������ѯ����ʼ���ڣ���ʽ��YYYY-MM-DD����");
    scanf("%s", startDate);
    printf("�������ѯ�Ľ������ڣ���ʽ��YYYY-MM-DD����");
    scanf("%s", endDate);
    printf("�����뷿�����ͣ��磺�󴲡�˫�����׷�����");
    scanf("%s", roomType);

    printf("\n��Ԥ���ķ����б����£�\n");
    printf("%-10s %-10s %-15s %-10s\n", "�����", "��������", "�۸�", "���"); // ����ͷ�������

    Room *currentRoom = roomsList;
    while (currentRoom != NULL)
    {
        if (strcmp(currentRoom->roomType, roomType) == 0 && !currentRoom->isOccupied)
        {
            int roomAvailable = 1; // ���跿�����

            // ����Ԥ���б��Լ�鷿����ָ��ʱ���Ƿ�Ԥ��
            Reservation *currentReservation = reservationsList;
            while (currentReservation != NULL)
            {
                if (currentReservation->roomId == currentRoom->roomId &&
                    !(compareDates(currentReservation->endDate, startDate) < 0 || compareDates(currentReservation->startDate, endDate) > 0))
                {
                    roomAvailable = 0; // ����������ѯʱ����ѱ�Ԥ��
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
        printf("û���ҵ����������Ŀ��пͷ���\n");
    }
}
// ���������������û�ѡ�����򷿼�
void customerSort()
{
    if (roomsList == NULL || roomsList->next == NULL)
    {
        return; // �б�Ϊ�ջ�ֻ��һ��Ԫ����������
    }

    Room *sorted = NULL;       // �µ�����������ͷָ��
    Room *current = roomsList; // ��ǰ��������ԭ����ڵ�
    Room *next = NULL;
    int sortType;
    // �û�ѡ����������
    printf("��ѡ���������ͣ�\n");
    printf("1. �������������\n");
    printf("2. ������۸�����\n");
    printf("������ѡ��1��2����");
    scanf("%d", &sortType);
    while (current != NULL)
    {
        next = current->next; // ������һ���ڵ�

        // ��������Ĳ������
        if (sorted == NULL || (sortType == 1 && sorted->area >= current->area) || (sortType == 2 && sorted->price >= current->price))
        {
            // ���뵽���������ͷ��
            current->next = sorted;
            sorted = current;
        }
        else
        {
            // Ѱ�Ҳ���λ��
            Room *temp = sorted;
            while (temp->next != NULL && ((sortType == 1 && temp->next->area < current->area) || (sortType == 2 && temp->next->price < current->price)))
            {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }

        current = next; // �ƶ�����һ��Ԫ��
    }

    roomsList = sorted; // ���·�������ͷָ��
    printf("�����Ѿ����!\n");
    displayAllRooms();
}
// ͳ�ƺ���������ͳ����ס��Ϣ
void customerStatistics()
{
    char startDate[20], endDate[20];
    printf("������ͳ�Ƶ���ʼ���ڣ���ʽ��YYYY-MM-DD����");
    scanf("%s", startDate);
    printf("������ͳ�ƵĽ������ڣ���ʽ��YYYY-MM-DD����");
    scanf("%s", endDate);

    int totalReservations = 0;         // ����ס����
    int periodReservations = 0;        // ָ��ʱ����ڵ���ס����
    int roomTypeCounts[3] = {0, 0, 0}; // �������ͼ���������Ϊ���󴲡�˫�����׷���

    // ����Ԥ���б����ͳ������
    for (Reservation *res = reservationsList; res != NULL; res = res->next)
    {
        if (!strcmp(res->CustomerId, currentUser->ID))
        {
            totalReservations++; // �ۼ�����ס����
            // ���Ԥ���Ƿ���ָ�����ڷ�Χ��
            if (compareDates(res->startDate, endDate) <= 0 && compareDates(res->endDate, startDate) >= 0)
            {
                periodReservations++; // �ۼӷ���ʱ��ε���ס����
            }

            // ���ݷ��������ۼӼ���
            if (strcmp(res->roomType, "��") == 0)
            {
                roomTypeCounts[0]++;
            }
            else if (strcmp(res->roomType, "˫��") == 0)
            {
                roomTypeCounts[1]++;
            }
            else if (strcmp(res->roomType, "�׷�") == 0)
            {
                roomTypeCounts[2]++;
            }
        }
    }
    printf("%s,����!\n", currentUser->ID);
    printf("����ס������%d\n", totalReservations);
    printf("�� %s �� %s ��ʱ�������ס������%d\n", startDate, endDate, periodReservations);
    printf("�󴲷���ס������%d\n", roomTypeCounts[0]);
    printf("˫������ס������%d\n", roomTypeCounts[1]);
    printf("�׷���ס������%d\n", roomTypeCounts[2]);
}
// ԤԼ���亯��
void myReservation()
{
    // �����µ�ԤԼ�ڵ�
    Reservation *newReservation = (Reservation *)malloc(sizeof(Reservation));
    if (newReservation == NULL)
    {
        printf("�ڴ����ʧ��\n");
        return;
    }

    // ����ԤԼ��Ϣ
    printf("��ȷ�������û�����%s\n", currentUser->ID);
    strcpy(newReservation->CustomerId, currentUser->ID);
    displayAllRooms(); // ��ʾ���з�����Ϣ���û�ѡ��
    printf("��������ҪԤԼ�ķ���ţ�");
    scanf("%d", &(newReservation->roomId));
    // ���ҷ����Ƿ����
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
        printf("δ�ҵ��÷���ţ����������롣\n");
        free(newReservation);
        return;
    }
    printf("������ԤԼ�Ŀ�ʼ���ڣ���ʽ��YYYY-MM-DD����");
    scanf("%s", newReservation->startDate);
    printf("������ԤԼ�Ľ������ڣ���ʽ��YYYY-MM-DD����");
    scanf("%s", newReservation->endDate);

    // ��鷿���Ƿ����ԤԼ��¼
    Reservation *currentReservation = reservationsList;
    while (currentReservation != NULL)
    {
        if (currentReservation->roomId == newReservation->roomId &&
            compareDates(currentReservation->endDate, newReservation->startDate) >= 0 &&
            compareDates(currentReservation->startDate, newReservation->endDate) <= 0)
        {
            printf("�����ʱ���ѱ�ԤԼ����ѡ��������������ԤԼ���ڡ�\n");
            free(newReservation);
            return;
        }
        currentReservation = currentReservation->next;
    }
    // ��ԤԼ��Ϣ���뵽ԤԼ����ͷ��
    newReservation->isCheckedIn = 0; // ��δ��ס
    newReservation->next = reservationsList;
    reservationsList = newReservation;
    printf("ԤԼ�ɹ���\n");
}
// ȡ��Ԥ������
void cancelReservation()
{
    if (reservationsList == NULL)
    {
        printf("��ǰ��Ԥ����Ϣ��\n");
        return;
    }

    char customerId[20];
    strcpy(customerId, currentUser->ID);
    Reservation *currentReservation = reservationsList;
    Reservation *prevReservation = NULL;
    int count = 0; // �����������ڼ�¼Ԥ����Ϣ�����

    // ��ӡ��������Ԥ����Ϣ
    printf("��������������Ԥ����Ϣ��\n");
    while (currentReservation != NULL)
    {
        if (strcmp(currentReservation->CustomerId, customerId) == 0)
        {
            printf("%d. ����ţ�%d����ʼ���ڣ�%s���������ڣ�%s\n", count + 1, currentReservation->roomId, currentReservation->startDate, currentReservation->endDate);
            count++;
        }
        currentReservation = currentReservation->next;
    }

    if (count == 0)
    {
        printf("δ�ҵ�����û�����ص�Ԥ����Ϣ��\n");
        return;
    }

    int selection;
    printf("������Ҫȡ��Ԥ������Ϣ��ţ�");
    scanf("%d", &selection);

    if (selection < 1 || selection > count)
    {
        printf("������Ч����������ȷ����š�\n");
        return;
    }

    // ���±��������ҵ�Ҫȡ����Ԥ����Ϣ
    currentReservation = reservationsList;
    prevReservation = NULL;
    count = 0; // ���ü�����

    // �ҵ�Ҫȡ����Ԥ����Ϣ
    while (currentReservation != NULL)
    {
        if (strcmp(currentReservation->CustomerId, customerId) == 0)
        {
            count++;
            if (count == selection)
            {
                if (prevReservation == NULL)
                {
                    // ���Ҫȡ����Ԥ����Ϣ�ǵ�һ���ڵ�
                    reservationsList = currentReservation->next;
                }
                else
                {
                    // ���Ҫȡ����Ԥ����Ϣ���ǵ�һ���ڵ�
                    prevReservation->next = currentReservation->next;
                }

                printf("��ȡ��Ԥ���ķ���ţ�%d\n", currentReservation->roomId);
                free(currentReservation);
                printf("Ԥ��ȡ���ɹ���\n");
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
        printf("\n\t\t\t���˲˵�\n");
        printf("\t\t==============================\n");
        printf("\t\t      1 : ��Ϣ��ѯ\n");
        printf("\t\t      2 : ��Ϣ����\n");
        printf("\t\t      3 : ��Ϣͳ��\n");
        printf("\t\t      4 : Ԥ������\n");
        printf("\t\t      5 : ȡ��Ԥ��\n");
        printf("\t\t      6 : ����ά��\n");
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
            cancelReservation();
            break;
        case 6:
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
