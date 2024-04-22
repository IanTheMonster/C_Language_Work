#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Device
{
    int id;
    char name[50];
    char category[50];
    char status[20];
};

struct DeviceNode
{
    struct Device data;
    struct DeviceNode *next;
};

struct Repair
{
    int deviceId;
    char repairDate[50];
    char repairPerson[50];
    char description[200];
};

struct RepairNode
{
    struct Repair data;
    struct RepairNode *next;
};

struct LendReturnRecord
{
    int deviceId;
    char date[20];
    char person[50];
    char type[20];
};

struct LendReturnNode
{
    struct LendReturnRecord data;
    struct LendReturnNode *next;
};

struct ChangeRecord
{
    int deviceId;
    char originName[50];
    char newName[50];
    char date[20];
};

struct ChangeNode
{
    struct ChangeRecord data;
    struct ChangeNode *next;
};

// ȫ�ֱ���������ͷ�ڵ�
struct DeviceNode *deviceHead = NULL;
struct RepairNode *repairHead = NULL;
struct LendReturnNode *lendReturnHead = NULL;
struct ChangeNode *changeHead = NULL;
int checkDeviceExistence(int deviceId);
// Function prototypes
void deviceMenu();
void addDevice();
void deleteDevice();
void modifyDevice();
void searchDevice();
void displayDevices();
void repairMenu();
void addRepairRecord();
void deleteRepairRecord();
void modifyRepairRecord();
void searchRepairRecord();
void displayRepairRecords();
void lendReturnMenu();
void addLendReturnRecord();
void deleteLendReturnRecord();
void modifyLendReturnRecord();
void searchLendReturnRecord();
void displayLendReturnRecords();
void changeMenu();
void addChangeRecord();
void deleteChangeRecord();
void modifyChangeRecord();
void searchChangeRecord();
void displayChangeRecords();
void saveDevices(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("�޷����ļ�д�롣\n");
        return;
    }
    struct DeviceNode *current = deviceHead;
    while (current != NULL)
    {
        fprintf(file, "%d,%s,%s,%s\n", current->data.id, current->data.name, current->data.category, current->data.status);
        current = current->next;
    }
    fclose(file);
    printf("�豸��Ϣ�ѱ��档\n");
}

void saveRepairs(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("�޷����ļ�д�롣\n");
        return;
    }
    struct RepairNode *current = repairHead;
    while (current != NULL)
    {
        fprintf(file, "%d,%s,%s,%s\n", current->data.deviceId, current->data.repairDate, current->data.repairPerson, current->data.description);
        current = current->next;
    }
    fclose(file);
    printf("ά�޼�¼�ѱ��档\n");
}

void saveLendReturns(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("�޷����ļ�д�롣\n");
        return;
    }
    struct LendReturnNode *current = lendReturnHead;
    while (current != NULL)
    {
        fprintf(file, "%d,%s,%s,%s\n", current->data.deviceId, current->data.date, current->data.person, current->data.type);
        current = current->next;
    }
    fclose(file);
    printf("�軹��¼�ѱ��档\n");
}
void addLendReturnRecord()
{
    struct LendReturnRecord newRecord;
    printf("\n=== ��ӽ軹��¼ ===\n");

    // �����豸ID��������Ƿ�������豸�б���
    printf("�������豸ID: ");
    scanf("%d", &newRecord.deviceId);
    if (!checkDeviceExistence(newRecord.deviceId))
    {
        printf("�豸IDΪ %d ���豸�����ڣ�������Ӹ��豸��\n", newRecord.deviceId);
        return;
    }

    printf("������軹����: ");
    scanf("%s", newRecord.date);
    printf("������軹��: ");
    scanf("%s", newRecord.person);
    printf("������軹���ͣ����/�黹��: ");
    scanf("%s", newRecord.type);

    // �����½ڵ�
    struct LendReturnNode *newNode = (struct LendReturnNode *)malloc(sizeof(struct LendReturnNode));
    newNode->data = newRecord;
    newNode->next = NULL;

    // �������Ϊ�գ��½ڵ㼴Ϊͷ�ڵ�
    if (lendReturnHead == NULL)
    {
        lendReturnHead = newNode;
    }
    else
    {
        // ���������ҵ����һ���ڵ�
        struct LendReturnNode *current = lendReturnHead;
        while (current->next != NULL)
        {
            current = current->next;
        }
        // ���½ڵ����ӵ����һ���ڵ����
        current->next = newNode;
    }
    printf("�軹��¼��ӳɹ���\n");
}
void deleteLendReturnRecord()
{
    if (lendReturnHead == NULL)
    {
        printf("���޽軹��¼��\n");
        return;
    }

    int targetId;
    printf("������Ҫɾ���Ľ軹��¼���豸ID: ");
    scanf("%d", &targetId);

    struct LendReturnNode *current = lendReturnHead;
    struct LendReturnNode *prev = NULL;

    // �����������Ŀ��軹��¼
    while (current != NULL && current->data.deviceId != targetId)
    {
        prev = current;
        current = current->next;
    }

    // ���δ�ҵ�Ŀ��軹��¼
    if (current == NULL)
    {
        printf("δ�ҵ��豸IDΪ %d �Ľ軹��¼��\n", targetId);
        return;
    }

    // �ҵ�Ŀ��軹��¼��ɾ���ڵ�
    if (prev == NULL)
    {
        // ���Ŀ��軹��¼��ͷ�ڵ�
        lendReturnHead = current->next;
    }
    else
    {
        // ���Ŀ��軹��¼����ͷ�ڵ�
        prev->next = current->next;
    }

    // �ͷ��ڴ�
    free(current);
    printf("�豸IDΪ %d �Ľ軹��¼ɾ���ɹ���\n", targetId);
}
void modifyLendReturnRecord()
{
    if (lendReturnHead == NULL)
    {
        printf("���޽軹��¼��\n");
        return;
    }

    int targetId;
    printf("������Ҫ�޸ĵĽ軹��¼���豸ID: ");
    scanf("%d", &targetId);

    struct LendReturnNode *current = lendReturnHead;

    // �����������Ŀ��軹��¼
    while (current != NULL && current->data.deviceId != targetId)
    {
        current = current->next;
    }

    // ���δ�ҵ�Ŀ��軹��¼
    if (current == NULL)
    {
        printf("δ�ҵ��豸IDΪ %d �Ľ軹��¼��\n", targetId);
        return;
    }

    // �ҵ�Ŀ��軹��¼���޸Ľ軹��Ϣ
    printf("�������µĽ軹����: ");
    scanf("%s", current->data.date);
    printf("�������µĽ軹��: ");
    scanf("%s", current->data.person);
    printf("�������µĽ軹���ͣ����/�黹��: ");
    scanf("%s", current->data.type);

    printf("�豸IDΪ %d �Ľ軹��¼�޸ĳɹ���\n", targetId);
}
void searchLendReturnRecord()
{
    if (lendReturnHead == NULL)
    {
        printf("���޽軹��¼��\n");
        return;
    }

    int targetId;
    printf("������Ҫ��ѯ�Ľ軹��¼���豸ID: ");
    scanf("%d", &targetId);

    struct LendReturnNode *current = lendReturnHead;

    // �����������Ŀ��軹��¼
    while (current != NULL && current->data.deviceId != targetId)
    {
        current = current->next;
    }

    // ���δ�ҵ�Ŀ��軹��¼
    if (current == NULL)
    {
        printf("δ�ҵ��豸IDΪ %d �Ľ軹��¼��\n", targetId);
        return;
    }

    // �ҵ�Ŀ��軹��¼������軹��Ϣ
    printf("\n�軹����: %s\n", current->data.date);
    printf("�軹��: %s\n", current->data.person);
    printf("�軹����: %s\n", current->data.type);
}
void displayLendReturnRecords()
{
    if (lendReturnHead == NULL)
    {
        printf("���޽軹��¼��\n");
        return;
    }

    printf("\n=== ���н軹��¼ ===\n");
    printf("| %-10s | %-20s | %-20s | %-20s |\n", "�豸ID", "�軹����", "�軹��", "�軹����");
    printf("|------------|----------------------|----------------------|----------------------|\n");

    struct LendReturnNode *current = lendReturnHead;

    // ������������軹��¼��Ϣ
    while (current != NULL)
    {
        printf("| %-10d | %-20s | %-20s | %-20s |\n",
               current->data.deviceId, current->data.date, current->data.person, current->data.type);
        current = current->next;
    }
}

void saveChanges(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("�޷����ļ�д�롣\n");
        return;
    }
    struct ChangeNode *current = changeHead;
    while (current != NULL)
    {
        fprintf(file, "%d,%s,%s,%s\n", current->data.deviceId, current->data.originName, current->data.newName, current->data.date);
        current = current->next;
    }
    fclose(file);
    printf("�����¼�ѱ��档\n");
}
void loadDevices(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("�޷����ļ���ȡ��\n");
        return;
    }
    struct DeviceNode *current = NULL;
    deviceHead = NULL; // Reset head before loading new data
    struct Device temp;
    while (fscanf(file, "%d,%49[^,],%49[^,],%19s\n", &temp.id, temp.name, temp.category, temp.status) == 4)
    {
        struct DeviceNode *newNode = (struct DeviceNode *)malloc(sizeof(struct DeviceNode));
        newNode->data = temp;
        newNode->next = NULL;
        if (current == NULL)
        {
            deviceHead = newNode;
        }
        else
        {
            current->next = newNode;
        }
        current = newNode;
    }
    fclose(file);
    printf("�豸��Ϣ�Ѽ��ء�\n");
}

void loadRepairs(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("�޷����ļ���ȡ��\n");
        return;
    }
    struct RepairNode *current = NULL;
    repairHead = NULL; // Reset head before loading new data
    struct Repair temp;
    while (fscanf(file, "%d,%49[^,],%49[^,],%199[^\n]\n", &temp.deviceId, temp.repairDate, temp.repairPerson, temp.description) == 4)
    {
        struct RepairNode *newNode = (struct RepairNode *)malloc(sizeof(struct RepairNode));
        newNode->data = temp;
        newNode->next = NULL;
        if (current == NULL)
        {
            repairHead = newNode;
        }
        else
        {
            current->next = newNode;
        }
        current = newNode;
    }
    fclose(file);
    printf("ά�޼�¼�Ѽ��ء�\n");
}

void loadLendReturns(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("�޷����ļ���ȡ��\n");
        return;
    }
    struct LendReturnNode *current = NULL;
    lendReturnHead = NULL; // Reset head before loading new data
    struct LendReturnRecord temp;
    while (fscanf(file, "%d,%19[^,],%49[^,],%19s\n", &temp.deviceId, temp.date, temp.person, temp.type) == 4)
    {
        struct LendReturnNode *newNode = (struct LendReturnNode *)malloc(sizeof(struct LendReturnNode));
        newNode->data = temp;
        newNode->next = NULL;
        if (current == NULL)
        {
            lendReturnHead = newNode;
        }
        else
        {
            current->next = newNode;
        }
        current = newNode;
    }
    fclose(file);
    printf("�軹��¼�Ѽ��ء�\n");
}

void loadChanges(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("�޷����ļ���ȡ��\n");
        return;
    }
    struct ChangeNode *current = NULL;
    changeHead = NULL; // Reset head before loading new data
    struct ChangeRecord temp;
    while (fscanf(file, "%d,%49[^,],%49[^,],%19s\n", &temp.deviceId, temp.originName, temp.newName, temp.date) == 4)
    {
        struct ChangeNode *newNode = (struct ChangeNode *)malloc(sizeof(struct ChangeNode));
        newNode->data = temp;
        newNode->next = NULL;
        if (current == NULL)
        {
            changeHead = newNode;
        }
        else
        {
            current->next = newNode;
        }
        current = newNode;
    }
    fclose(file);
    printf("�����¼�Ѽ��ء�\n");
}

// ����豸ID�Ƿ�������豸�б���
int checkDeviceExistence(int deviceId)
{
    struct DeviceNode *current = deviceHead;
    while (current != NULL)
    {
        if (current->data.id == deviceId)
        {
            return 1; // �豸ID����
        }
        current = current->next;
    }
    return 0; // �豸ID������
}
int main()
{
    int choice;
    // �ڳ���ʼʱ�Զ��������м�¼
    loadDevices("devices.txt");
    loadRepairs("repairs.txt");
    loadLendReturns("lendreturns.txt");
    loadChanges("changes.txt");

    do
    {
        printf("\n=== �豸����ϵͳ���˵� ===\n");
        printf("1. �豸����\n");
        printf("2. ά�޼�¼����\n");
        printf("3. �軹��¼����\n");
        printf("4. �����¼����\n");
        printf("0. �˳�ϵͳ\n");
        printf("��ѡ�������");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            deviceMenu();
            break;
        case 2:
            repairMenu();
            break;
        case 3:
            lendReturnMenu();
            break;
        case 4:
            changeMenu();
            break;
        case 0:
            printf("ллʹ�ã��ټ���\n");
            // �ڳ���ʼʱ�Զ��������м�¼
            saveDevices("devices.txt");
            saveRepairs("repairs.txt");
            saveLendReturns("lendreturns.txt");
            saveChanges("changes.txt");
            break;
        default:
            printf("��Ч��ѡ�����������롣\n");
        }
    } while (choice != 0);
    return 0;
}

void deviceMenu()
{
    int choice;
    do
    {
        printf("\n--- �豸����˵� ---\n");
        printf("1. ����豸\n");
        printf("2. ɾ���豸\n");
        printf("3. �޸��豸��Ϣ\n");
        printf("4. ��ѯ�豸\n");
        printf("5. ��ʾ�����豸\n");
        printf("0. �������˵�\n");
        printf("��ѡ�������");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            addDevice();
            break;
        case 2:
            deleteDevice();
            break;
        case 3:
            modifyDevice();
            break;
        case 4:
            searchDevice();
            break;
        case 5:
            displayDevices();
            break;
        case 0:
            break;
        default:
            printf("��Ч��ѡ�����������롣\n");
        }
    } while (choice != 0);
}
void addDevice()
{
    struct Device newDevice;
    printf("\n=== ����豸 ===\n");
    printf("�������豸ID: ");
    scanf("%d", &newDevice.id);
    printf("�������豸����: ");
    scanf("%s", newDevice.name);
    printf("�������豸���: ");
    scanf("%s", newDevice.category);
    printf("�������豸״̬: ");
    scanf("%s", newDevice.status);

    // �����½ڵ�
    struct DeviceNode *newNode = (struct DeviceNode *)malloc(sizeof(struct DeviceNode));
    newNode->data = newDevice;
    newNode->next = NULL;

    // �������Ϊ�գ��½ڵ㼴Ϊͷ�ڵ�
    if (deviceHead == NULL)
    {
        deviceHead = newNode;
    }
    else
    {
        // ���������ҵ����һ���ڵ�
        struct DeviceNode *current = deviceHead;
        while (current->next != NULL)
        {
            current = current->next;
        }
        // ���½ڵ����ӵ����һ���ڵ����
        current->next = newNode;
    }
    printf("�豸��ӳɹ���\n");
}

void displayDevices()
{
    if (deviceHead == NULL)
    {
        printf("�����豸��Ϣ��\n");
        return;
    }

    printf("\n=== �����豸��Ϣ ===\n");
    printf("| %-10s | %-20s | %-20s | %-10s |\n", "�豸ID", "�豸����", "�豸���", "�豸״̬");
    printf("|------------|----------------------|----------------------|------------|\n");

    struct DeviceNode *current = deviceHead;

    // ������������豸��Ϣ
    while (current != NULL)
    {
        printf("| %-10d | %-20s | %-20s | %-10s |\n",
               current->data.id, current->data.name, current->data.category, current->data.status);
        current = current->next;
    }
}

void searchDevice()
{
    if (deviceHead == NULL)
    {
        printf("�����豸��Ϣ��\n");
        return;
    }

    int targetId;
    printf("������Ҫ��ѯ���豸ID: ");
    scanf("%d", &targetId);

    struct DeviceNode *current = deviceHead;

    // �����������Ŀ���豸
    while (current != NULL && current->data.id != targetId)
    {
        current = current->next;
    }

    // ���δ�ҵ�Ŀ���豸
    if (current == NULL)
    {
        printf("δ�ҵ��豸IDΪ %d ���豸��\n", targetId);
        return;
    }

    // �ҵ�Ŀ���豸������豸��Ϣ
    printf("\n�豸ID: %d\n", current->data.id);
    printf("�豸����: %s\n", current->data.name);
    printf("�豸���: %s\n", current->data.category);
    printf("�豸״̬: %s\n", current->data.status);
}

void modifyDevice()
{
    if (deviceHead == NULL)
    {
        printf("�����豸��Ϣ��\n");
        return;
    }

    int targetId;
    printf("������Ҫ�޸ĵ��豸ID: ");
    scanf("%d", &targetId);

    struct DeviceNode *current = deviceHead;

    // �����������Ŀ���豸
    while (current != NULL && current->data.id != targetId)
    {
        current = current->next;
    }

    // ���δ�ҵ�Ŀ���豸
    if (current == NULL)
    {
        printf("δ�ҵ��豸IDΪ %d ���豸��\n", targetId);
        return;
    }

    // �ҵ�Ŀ���豸���޸��豸��Ϣ
    printf("�������µ��豸����: ");
    scanf("%s", current->data.name);
    printf("�������µ��豸���: ");
    scanf("%s", current->data.category);
    printf("�������µ��豸״̬: ");
    scanf("%s", current->data.status);

    printf("�豸IDΪ %d ���豸��Ϣ�޸ĳɹ���\n", targetId);
}

void deleteDevice()
{
    if (deviceHead == NULL)
    {
        printf("�����豸��Ϣ��\n");
        return;
    }

    int targetId;
    printf("������Ҫɾ�����豸ID: ");
    scanf("%d", &targetId);

    struct DeviceNode *current = deviceHead;
    struct DeviceNode *prev = NULL;

    // �����������Ŀ���豸
    while (current != NULL && current->data.id != targetId)
    {
        prev = current;
        current = current->next;
    }

    // ���δ�ҵ�Ŀ���豸
    if (current == NULL)
    {
        printf("δ�ҵ��豸IDΪ %d ���豸��\n", targetId);
        return;
    }

    // �ҵ�Ŀ���豸��ɾ���ڵ�
    if (prev == NULL)
    {
        // ���Ŀ���豸��ͷ�ڵ�
        deviceHead = current->next;
    }
    else
    {
        // ���Ŀ���豸����ͷ�ڵ�
        prev->next = current->next;
    }

    // �ͷ��ڴ�
    free(current);
    printf("�豸IDΪ %d ���豸ɾ���ɹ���\n", targetId);
}

void repairMenu()
{
    int choice;
    do
    {
        printf("\n--- ά�޼�¼���� ---\n");
        printf("1. ���ά�޼�¼\n");
        printf("2. ɾ��ά�޼�¼\n");
        printf("3. �޸�ά�޼�¼\n");
        printf("4. ��ѯά�޼�¼\n");
        printf("5. ��ʾ����ά�޼�¼\n");
        printf("0. �������˵�\n");
        printf("��ѡ�������");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            addRepairRecord();
            break;
        case 2:
            deleteRepairRecord();
            break;
        case 3:
            modifyRepairRecord();
            break;
        case 4:
            searchRepairRecord();
            break;
        case 5:
            displayRepairRecords();
            break;
        case 0:
            break;
        default:
            printf("��Ч��ѡ�����������롣\n");
        }
    } while (choice != 0);
}
void addRepairRecord()
{
    struct Repair newRepair;
    printf("\n=== ���ά�޼�¼ ===\n");

    // �����豸ID����������Ƿ�������豸�б���
    printf("�������豸ID: ");
    scanf("%d", &newRepair.deviceId);
    if (!checkDeviceExistence(newRepair.deviceId))
    {
        printf("�豸IDΪ %d ���豸�����ڣ�������Ӹ��豸��\n", newRepair.deviceId);
        return;
    }

    printf("������ά������: ");
    scanf("%s", newRepair.repairDate);
    printf("������ά����Ա: ");
    scanf("%s", newRepair.repairPerson);
    printf("������ά������: ");
    scanf("%s", newRepair.description);

    // �����½ڵ�
    struct RepairNode *newNode = (struct RepairNode *)malloc(sizeof(struct RepairNode));
    newNode->data = newRepair;
    newNode->next = NULL;

    // �������Ϊ�գ��½ڵ㼴Ϊͷ�ڵ�
    if (repairHead == NULL)
    {
        repairHead = newNode;
    }
    else
    {
        // ���������ҵ����һ���ڵ�
        struct RepairNode *current = repairHead;
        while (current->next != NULL)
        {
            current = current->next;
        }
        // ���½ڵ����ӵ����һ���ڵ����
        current->next = newNode;
    }
    printf("ά�޼�¼��ӳɹ���\n");
}
void lendReturnMenu()
{
    int choice;
    do
    {
        printf("\n--- �軹��¼���� ---\n");
        printf("1. ��ӽ軹��¼\n");
        printf("2. ɾ���軹��¼\n");
        printf("3. �޸Ľ軹��¼\n");
        printf("4. ��ѯ�軹��¼\n");
        printf("5. ��ʾ���н軹��¼\n");
        printf("0. �������˵�\n");
        printf("��ѡ�������");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            addLendReturnRecord();
            break;
        case 2:
            deleteLendReturnRecord();
            break;
        case 3:
            modifyLendReturnRecord();
            break;
        case 4:
            searchLendReturnRecord();
            break;
        case 5:
            displayLendReturnRecords();
            break;
        case 0:
            break;
        default:
            printf("��Ч��ѡ�����������롣\n");
        }
    } while (choice != 0);
}
void deleteRepairRecord()
{
    if (repairHead == NULL)
    {
        printf("����ά�޼�¼��\n");
        return;
    }

    int targetId;
    printf("������Ҫɾ����ά�޼�¼���豸ID: ");
    scanf("%d", &targetId);

    struct RepairNode *current = repairHead;
    struct RepairNode *prev = NULL;

    // �����������Ŀ��ά�޼�¼
    while (current != NULL && current->data.deviceId != targetId)
    {
        prev = current;
        current = current->next;
    }

    // ���δ�ҵ�Ŀ��ά�޼�¼
    if (current == NULL)
    {
        printf("δ�ҵ��豸IDΪ %d ��ά�޼�¼��\n", targetId);
        return;
    }

    // �ҵ�Ŀ��ά�޼�¼��ɾ���ڵ�
    if (prev == NULL)
    {
        // ���Ŀ��ά�޼�¼��ͷ�ڵ�
        repairHead = current->next;
    }
    else
    {
        // ���Ŀ��ά�޼�¼����ͷ�ڵ�
        prev->next = current->next;
    }

    // �ͷ��ڴ�
    free(current);
    printf("�豸IDΪ %d ��ά�޼�¼ɾ���ɹ���\n", targetId);
}
void modifyRepairRecord()
{
    if (repairHead == NULL)
    {
        printf("����ά�޼�¼��\n");
        return;
    }

    int targetId;
    printf("������Ҫ�޸ĵ�ά�޼�¼���豸ID: ");
    scanf("%d", &targetId);

    struct RepairNode *current = repairHead;

    // �����������Ŀ��ά�޼�¼
    while (current != NULL && current->data.deviceId != targetId)
    {
        current = current->next;
    }

    // ���δ�ҵ�Ŀ��ά�޼�¼
    if (current == NULL)
    {
        printf("δ�ҵ��豸IDΪ %d ��ά�޼�¼��\n", targetId);
        return;
    }

    // �ҵ�Ŀ��ά�޼�¼���޸�ά����Ϣ
    printf("�������µ�ά������: ");
    scanf("%s", current->data.repairDate);
    printf("�������µ�ά����Ա: ");
    scanf("%s", current->data.repairPerson);
    printf("�������µ�ά������: ");
    scanf("%s", current->data.description);

    printf("�豸IDΪ %d ��ά�޼�¼�޸ĳɹ���\n", targetId);
}
void searchRepairRecord()
{
    if (repairHead == NULL)
    {
        printf("����ά�޼�¼��\n");
        return;
    }

    int targetId;
    printf("������Ҫ��ѯ��ά�޼�¼���豸ID: ");
    scanf("%d", &targetId);

    struct RepairNode *current = repairHead;

    // �����������Ŀ��ά�޼�¼
    while (current != NULL && current->data.deviceId != targetId)
    {
        current = current->next;
    }

    // ���δ�ҵ�Ŀ��ά�޼�¼
    if (current == NULL)
    {
        printf("δ�ҵ��豸IDΪ %d ��ά�޼�¼��\n", targetId);
        return;
    }

    // �ҵ�Ŀ��ά�޼�¼�����ά����Ϣ
    printf("\nά������: %s\n", current->data.repairDate);
    printf("ά����Ա: %s\n", current->data.repairPerson);
    printf("ά������: %s\n", current->data.description);
}
void displayRepairRecords()
{
    if (repairHead == NULL)
    {
        printf("����ά�޼�¼��\n");
        return;
    }

    printf("\n=== ����ά�޼�¼ ===\n");
    printf("| %-10s | %-20s | %-20s | %-50s |\n", "�豸ID", "ά������", "ά����Ա", "ά������");
    printf("|------------|----------------------|----------------------|--------------------------------------------------|\n");

    struct RepairNode *current = repairHead;

    // �����������ά�޼�¼��Ϣ
    while (current != NULL)
    {
        printf("| %-10d | %-20s | %-20s | %-50s |\n",
               current->data.deviceId, current->data.repairDate, current->data.repairPerson, current->data.description);
        current = current->next;
    }
}
void addChangeRecord()
{
    struct ChangeRecord newChange;
    printf("\n=== ��ӱ����¼ ===\n");

    // �����豸ID����������Ƿ�������豸�б���
    printf("�������豸ID: ");
    scanf("%d", &newChange.deviceId);
    if (!checkDeviceExistence(newChange.deviceId))
    {
        printf("�豸IDΪ %d ���豸�����ڣ�������Ӹ��豸��\n", newChange.deviceId);
        return;
    }

    printf("������ԭ�豸����: ");
    scanf("%s", newChange.originName);
    printf("���������豸����: ");
    scanf("%s", newChange.newName);
    printf("������������: ");
    scanf("%s", newChange.date);

    // �����½ڵ�
    struct ChangeNode *newNode = (struct ChangeNode *)malloc(sizeof(struct ChangeNode));
    newNode->data = newChange;
    newNode->next = NULL;

    // �������Ϊ�գ��½ڵ㼴Ϊͷ�ڵ�
    if (changeHead == NULL)
    {
        changeHead = newNode;
    }
    else
    {
        // ���������ҵ����һ���ڵ�
        struct ChangeNode *current = changeHead;
        while (current->next != NULL)
        {
            current = current->next;
        }
        // ���½ڵ����ӵ����һ���ڵ����
        current->next = newNode;
    }
    printf("�����¼��ӳɹ���\n");
}

void deleteChangeRecord()
{
    if (changeHead == NULL)
    {
        printf("���ޱ����¼��\n");
        return;
    }

    int targetId;
    printf("������Ҫɾ���ı����¼���豸ID: ");
    scanf("%d", &targetId);

    struct ChangeNode *current = changeHead;
    struct ChangeNode *prev = NULL;

    // �����������Ŀ������¼
    while (current != NULL && current->data.deviceId != targetId)
    {
        prev = current;
        current = current->next;
    }

    // ���δ�ҵ�Ŀ������¼
    if (current == NULL)
    {
        printf("δ�ҵ��豸IDΪ %d �ı����¼��\n", targetId);
        return;
    }

    // �ҵ�Ŀ������¼��ɾ���ڵ�
    if (prev == NULL)
    {
        // ���Ŀ������¼��ͷ�ڵ�
        changeHead = current->next;
    }
    else
    {
        // ���Ŀ������¼����ͷ�ڵ�
        prev->next = current->next;
    }

    // �ͷ��ڴ�
    free(current);
    printf("�豸IDΪ %d �ı����¼ɾ���ɹ���\n", targetId);
}

void modifyChangeRecord()
{
    if (changeHead == NULL)
    {
        printf("���ޱ����¼��\n");
        return;
    }

    int targetId;
    printf("������Ҫ�޸ĵı����¼���豸ID: ");
    scanf("%d", &targetId);

    struct ChangeNode *current = changeHead;

    // �����������Ŀ������¼
    while (current != NULL && current->data.deviceId != targetId)
    {
        current = current->next;
    }

    // ���δ�ҵ�Ŀ������¼
    if (current == NULL)
    {
        printf("δ�ҵ��豸IDΪ %d �ı����¼��\n", targetId);
        return;
    }

    // �ҵ�Ŀ������¼���޸ı����Ϣ
    printf("�������µ�ԭ�豸����: ");
    scanf("%s", current->data.originName);
    printf("�������µ��豸����: ");
    scanf("%s", current->data.newName);
    printf("�������µı������: ");
    scanf("%s", current->data.date);

    printf("�豸IDΪ %d �ı����¼�޸ĳɹ���\n", targetId);
}

void searchChangeRecord()
{
    if (changeHead == NULL)
    {
        printf("���ޱ����¼��\n");
        return;
    }

    int targetId;
    printf("������Ҫ��ѯ�ı����¼���豸ID: ");
    scanf("%d", &targetId);

    struct ChangeNode *current = changeHead;

    // �����������Ŀ������¼
    while (current != NULL && current->data.deviceId != targetId)
    {
        current = current->next;
    }

    // ���δ�ҵ�Ŀ������¼
    if (current == NULL)
    {
        printf("δ�ҵ��豸IDΪ %d �ı����¼��\n", targetId);
        return;
    }

    // �ҵ�Ŀ������¼����������Ϣ
    printf("\nԭ�豸����: %s\n", current->data.originName);
    printf("���豸����: %s\n", current->data.newName);
    printf("�������: %s\n", current->data.date);
}

void displayChangeRecords()
{
    if (changeHead == NULL)
    {
        printf("���ޱ����¼��\n");
        return;
    }

    printf("\n=== ���б����¼ ===\n");
    printf("| %-10s | %-20s | %-20s | %-20s |\n", "�豸ID", "ԭ�豸����", "���豸����", "�������");
    printf("|------------|----------------------|----------------------|----------------------|\n");

    struct ChangeNode *current = changeHead;

    // ����������������¼��Ϣ
    while (current != NULL)
    {
        printf("| %-10d | %-20s | %-20s | %-20s |\n",
               current->data.deviceId, current->data.originName, current->data.newName, current->data.date);
        current = current->next;
    }
}

void changeMenu()
{
    int choice;
    do
    {
        printf("\n--- �����¼���� ---\n");
        printf("1. ��ӱ����¼\n");
        printf("2. ɾ�������¼\n");
        printf("3. �޸ı����¼\n");
        printf("4. ��ѯ�����¼\n");
        printf("5. ��ʾ���б����¼\n");
        printf("0. �������˵�\n");
        printf("��ѡ�������");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            addChangeRecord();
            break;
        case 2:
            deleteChangeRecord();
            break;
        case 3:
            modifyChangeRecord();
            break;
        case 4:
            searchChangeRecord();
            break;
        case 5:
            displayChangeRecords();
            break;
        case 0:
            break;
        default:
            printf("��Ч��ѡ�����������롣\n");
        }
    } while (choice != 0);
}