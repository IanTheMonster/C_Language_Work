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

// 全局变量，链表头节点
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
        printf("无法打开文件写入。\n");
        return;
    }
    struct DeviceNode *current = deviceHead;
    while (current != NULL)
    {
        fprintf(file, "%d,%s,%s,%s\n", current->data.id, current->data.name, current->data.category, current->data.status);
        current = current->next;
    }
    fclose(file);
    printf("设备信息已保存。\n");
}

void saveRepairs(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("无法打开文件写入。\n");
        return;
    }
    struct RepairNode *current = repairHead;
    while (current != NULL)
    {
        fprintf(file, "%d,%s,%s,%s\n", current->data.deviceId, current->data.repairDate, current->data.repairPerson, current->data.description);
        current = current->next;
    }
    fclose(file);
    printf("维修记录已保存。\n");
}

void saveLendReturns(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("无法打开文件写入。\n");
        return;
    }
    struct LendReturnNode *current = lendReturnHead;
    while (current != NULL)
    {
        fprintf(file, "%d,%s,%s,%s\n", current->data.deviceId, current->data.date, current->data.person, current->data.type);
        current = current->next;
    }
    fclose(file);
    printf("借还记录已保存。\n");
}
void addLendReturnRecord()
{
    struct LendReturnRecord newRecord;
    printf("\n=== 添加借还记录 ===\n");

    // 输入设备ID并检查其是否存在于设备列表中
    printf("请输入设备ID: ");
    scanf("%d", &newRecord.deviceId);
    if (!checkDeviceExistence(newRecord.deviceId))
    {
        printf("设备ID为 %d 的设备不存在，请先添加该设备。\n", newRecord.deviceId);
        return;
    }

    printf("请输入借还日期: ");
    scanf("%s", newRecord.date);
    printf("请输入借还人: ");
    scanf("%s", newRecord.person);
    printf("请输入借还类型（借出/归还）: ");
    scanf("%s", newRecord.type);

    // 创建新节点
    struct LendReturnNode *newNode = (struct LendReturnNode *)malloc(sizeof(struct LendReturnNode));
    newNode->data = newRecord;
    newNode->next = NULL;

    // 如果链表为空，新节点即为头节点
    if (lendReturnHead == NULL)
    {
        lendReturnHead = newNode;
    }
    else
    {
        // 遍历链表找到最后一个节点
        struct LendReturnNode *current = lendReturnHead;
        while (current->next != NULL)
        {
            current = current->next;
        }
        // 将新节点链接到最后一个节点后面
        current->next = newNode;
    }
    printf("借还记录添加成功！\n");
}
void deleteLendReturnRecord()
{
    if (lendReturnHead == NULL)
    {
        printf("暂无借还记录。\n");
        return;
    }

    int targetId;
    printf("请输入要删除的借还记录的设备ID: ");
    scanf("%d", &targetId);

    struct LendReturnNode *current = lendReturnHead;
    struct LendReturnNode *prev = NULL;

    // 遍历链表查找目标借还记录
    while (current != NULL && current->data.deviceId != targetId)
    {
        prev = current;
        current = current->next;
    }

    // 如果未找到目标借还记录
    if (current == NULL)
    {
        printf("未找到设备ID为 %d 的借还记录。\n", targetId);
        return;
    }

    // 找到目标借还记录，删除节点
    if (prev == NULL)
    {
        // 如果目标借还记录是头节点
        lendReturnHead = current->next;
    }
    else
    {
        // 如果目标借还记录不是头节点
        prev->next = current->next;
    }

    // 释放内存
    free(current);
    printf("设备ID为 %d 的借还记录删除成功。\n", targetId);
}
void modifyLendReturnRecord()
{
    if (lendReturnHead == NULL)
    {
        printf("暂无借还记录。\n");
        return;
    }

    int targetId;
    printf("请输入要修改的借还记录的设备ID: ");
    scanf("%d", &targetId);

    struct LendReturnNode *current = lendReturnHead;

    // 遍历链表查找目标借还记录
    while (current != NULL && current->data.deviceId != targetId)
    {
        current = current->next;
    }

    // 如果未找到目标借还记录
    if (current == NULL)
    {
        printf("未找到设备ID为 %d 的借还记录。\n", targetId);
        return;
    }

    // 找到目标借还记录，修改借还信息
    printf("请输入新的借还日期: ");
    scanf("%s", current->data.date);
    printf("请输入新的借还人: ");
    scanf("%s", current->data.person);
    printf("请输入新的借还类型（借出/归还）: ");
    scanf("%s", current->data.type);

    printf("设备ID为 %d 的借还记录修改成功。\n", targetId);
}
void searchLendReturnRecord()
{
    if (lendReturnHead == NULL)
    {
        printf("暂无借还记录。\n");
        return;
    }

    int targetId;
    printf("请输入要查询的借还记录的设备ID: ");
    scanf("%d", &targetId);

    struct LendReturnNode *current = lendReturnHead;

    // 遍历链表查找目标借还记录
    while (current != NULL && current->data.deviceId != targetId)
    {
        current = current->next;
    }

    // 如果未找到目标借还记录
    if (current == NULL)
    {
        printf("未找到设备ID为 %d 的借还记录。\n", targetId);
        return;
    }

    // 找到目标借还记录，输出借还信息
    printf("\n借还日期: %s\n", current->data.date);
    printf("借还人: %s\n", current->data.person);
    printf("借还类型: %s\n", current->data.type);
}
void displayLendReturnRecords()
{
    if (lendReturnHead == NULL)
    {
        printf("暂无借还记录。\n");
        return;
    }

    printf("\n=== 所有借还记录 ===\n");
    printf("| %-10s | %-20s | %-20s | %-20s |\n", "设备ID", "借还日期", "借还人", "借还类型");
    printf("|------------|----------------------|----------------------|----------------------|\n");

    struct LendReturnNode *current = lendReturnHead;

    // 遍历链表并输出借还记录信息
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
        printf("无法打开文件写入。\n");
        return;
    }
    struct ChangeNode *current = changeHead;
    while (current != NULL)
    {
        fprintf(file, "%d,%s,%s,%s\n", current->data.deviceId, current->data.originName, current->data.newName, current->data.date);
        current = current->next;
    }
    fclose(file);
    printf("变更记录已保存。\n");
}
void loadDevices(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("无法打开文件读取。\n");
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
    printf("设备信息已加载。\n");
}

void loadRepairs(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("无法打开文件读取。\n");
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
    printf("维修记录已加载。\n");
}

void loadLendReturns(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("无法打开文件读取。\n");
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
    printf("借还记录已加载。\n");
}

void loadChanges(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("无法打开文件读取。\n");
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
    printf("变更记录已加载。\n");
}

// 检查设备ID是否存在于设备列表中
int checkDeviceExistence(int deviceId)
{
    struct DeviceNode *current = deviceHead;
    while (current != NULL)
    {
        if (current->data.id == deviceId)
        {
            return 1; // 设备ID存在
        }
        current = current->next;
    }
    return 0; // 设备ID不存在
}
int main()
{
    int choice;
    // 在程序开始时自动加载所有记录
    loadDevices("devices.txt");
    loadRepairs("repairs.txt");
    loadLendReturns("lendreturns.txt");
    loadChanges("changes.txt");

    do
    {
        printf("\n=== 设备管理系统主菜单 ===\n");
        printf("1. 设备管理\n");
        printf("2. 维修记录管理\n");
        printf("3. 借还记录管理\n");
        printf("4. 变更记录管理\n");
        printf("0. 退出系统\n");
        printf("请选择操作：");
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
            printf("谢谢使用，再见！\n");
            // 在程序开始时自动加载所有记录
            saveDevices("devices.txt");
            saveRepairs("repairs.txt");
            saveLendReturns("lendreturns.txt");
            saveChanges("changes.txt");
            break;
        default:
            printf("无效的选择，请重新输入。\n");
        }
    } while (choice != 0);
    return 0;
}

void deviceMenu()
{
    int choice;
    do
    {
        printf("\n--- 设备管理菜单 ---\n");
        printf("1. 添加设备\n");
        printf("2. 删除设备\n");
        printf("3. 修改设备信息\n");
        printf("4. 查询设备\n");
        printf("5. 显示所有设备\n");
        printf("0. 返回主菜单\n");
        printf("请选择操作：");
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
            printf("无效的选择，请重新输入。\n");
        }
    } while (choice != 0);
}
void addDevice()
{
    struct Device newDevice;
    printf("\n=== 添加设备 ===\n");
    printf("请输入设备ID: ");
    scanf("%d", &newDevice.id);
    printf("请输入设备名称: ");
    scanf("%s", newDevice.name);
    printf("请输入设备类别: ");
    scanf("%s", newDevice.category);
    printf("请输入设备状态: ");
    scanf("%s", newDevice.status);

    // 创建新节点
    struct DeviceNode *newNode = (struct DeviceNode *)malloc(sizeof(struct DeviceNode));
    newNode->data = newDevice;
    newNode->next = NULL;

    // 如果链表为空，新节点即为头节点
    if (deviceHead == NULL)
    {
        deviceHead = newNode;
    }
    else
    {
        // 遍历链表找到最后一个节点
        struct DeviceNode *current = deviceHead;
        while (current->next != NULL)
        {
            current = current->next;
        }
        // 将新节点链接到最后一个节点后面
        current->next = newNode;
    }
    printf("设备添加成功！\n");
}

void displayDevices()
{
    if (deviceHead == NULL)
    {
        printf("暂无设备信息。\n");
        return;
    }

    printf("\n=== 所有设备信息 ===\n");
    printf("| %-10s | %-20s | %-20s | %-10s |\n", "设备ID", "设备名称", "设备类别", "设备状态");
    printf("|------------|----------------------|----------------------|------------|\n");

    struct DeviceNode *current = deviceHead;

    // 遍历链表并输出设备信息
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
        printf("暂无设备信息。\n");
        return;
    }

    int targetId;
    printf("请输入要查询的设备ID: ");
    scanf("%d", &targetId);

    struct DeviceNode *current = deviceHead;

    // 遍历链表查找目标设备
    while (current != NULL && current->data.id != targetId)
    {
        current = current->next;
    }

    // 如果未找到目标设备
    if (current == NULL)
    {
        printf("未找到设备ID为 %d 的设备。\n", targetId);
        return;
    }

    // 找到目标设备，输出设备信息
    printf("\n设备ID: %d\n", current->data.id);
    printf("设备名称: %s\n", current->data.name);
    printf("设备类别: %s\n", current->data.category);
    printf("设备状态: %s\n", current->data.status);
}

void modifyDevice()
{
    if (deviceHead == NULL)
    {
        printf("暂无设备信息。\n");
        return;
    }

    int targetId;
    printf("请输入要修改的设备ID: ");
    scanf("%d", &targetId);

    struct DeviceNode *current = deviceHead;

    // 遍历链表查找目标设备
    while (current != NULL && current->data.id != targetId)
    {
        current = current->next;
    }

    // 如果未找到目标设备
    if (current == NULL)
    {
        printf("未找到设备ID为 %d 的设备。\n", targetId);
        return;
    }

    // 找到目标设备，修改设备信息
    printf("请输入新的设备名称: ");
    scanf("%s", current->data.name);
    printf("请输入新的设备类别: ");
    scanf("%s", current->data.category);
    printf("请输入新的设备状态: ");
    scanf("%s", current->data.status);

    printf("设备ID为 %d 的设备信息修改成功。\n", targetId);
}

void deleteDevice()
{
    if (deviceHead == NULL)
    {
        printf("暂无设备信息。\n");
        return;
    }

    int targetId;
    printf("请输入要删除的设备ID: ");
    scanf("%d", &targetId);

    struct DeviceNode *current = deviceHead;
    struct DeviceNode *prev = NULL;

    // 遍历链表查找目标设备
    while (current != NULL && current->data.id != targetId)
    {
        prev = current;
        current = current->next;
    }

    // 如果未找到目标设备
    if (current == NULL)
    {
        printf("未找到设备ID为 %d 的设备。\n", targetId);
        return;
    }

    // 找到目标设备，删除节点
    if (prev == NULL)
    {
        // 如果目标设备是头节点
        deviceHead = current->next;
    }
    else
    {
        // 如果目标设备不是头节点
        prev->next = current->next;
    }

    // 释放内存
    free(current);
    printf("设备ID为 %d 的设备删除成功。\n", targetId);
}

void repairMenu()
{
    int choice;
    do
    {
        printf("\n--- 维修记录管理 ---\n");
        printf("1. 添加维修记录\n");
        printf("2. 删除维修记录\n");
        printf("3. 修改维修记录\n");
        printf("4. 查询维修记录\n");
        printf("5. 显示所有维修记录\n");
        printf("0. 返回主菜单\n");
        printf("请选择操作：");
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
            printf("无效的选择，请重新输入。\n");
        }
    } while (choice != 0);
}
void addRepairRecord()
{
    struct Repair newRepair;
    printf("\n=== 添加维修记录 ===\n");

    // 输入设备ID，并检查其是否存在于设备列表中
    printf("请输入设备ID: ");
    scanf("%d", &newRepair.deviceId);
    if (!checkDeviceExistence(newRepair.deviceId))
    {
        printf("设备ID为 %d 的设备不存在，请先添加该设备。\n", newRepair.deviceId);
        return;
    }

    printf("请输入维修日期: ");
    scanf("%s", newRepair.repairDate);
    printf("请输入维修人员: ");
    scanf("%s", newRepair.repairPerson);
    printf("请输入维修描述: ");
    scanf("%s", newRepair.description);

    // 创建新节点
    struct RepairNode *newNode = (struct RepairNode *)malloc(sizeof(struct RepairNode));
    newNode->data = newRepair;
    newNode->next = NULL;

    // 如果链表为空，新节点即为头节点
    if (repairHead == NULL)
    {
        repairHead = newNode;
    }
    else
    {
        // 遍历链表找到最后一个节点
        struct RepairNode *current = repairHead;
        while (current->next != NULL)
        {
            current = current->next;
        }
        // 将新节点链接到最后一个节点后面
        current->next = newNode;
    }
    printf("维修记录添加成功！\n");
}
void lendReturnMenu()
{
    int choice;
    do
    {
        printf("\n--- 借还记录管理 ---\n");
        printf("1. 添加借还记录\n");
        printf("2. 删除借还记录\n");
        printf("3. 修改借还记录\n");
        printf("4. 查询借还记录\n");
        printf("5. 显示所有借还记录\n");
        printf("0. 返回主菜单\n");
        printf("请选择操作：");
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
            printf("无效的选择，请重新输入。\n");
        }
    } while (choice != 0);
}
void deleteRepairRecord()
{
    if (repairHead == NULL)
    {
        printf("暂无维修记录。\n");
        return;
    }

    int targetId;
    printf("请输入要删除的维修记录的设备ID: ");
    scanf("%d", &targetId);

    struct RepairNode *current = repairHead;
    struct RepairNode *prev = NULL;

    // 遍历链表查找目标维修记录
    while (current != NULL && current->data.deviceId != targetId)
    {
        prev = current;
        current = current->next;
    }

    // 如果未找到目标维修记录
    if (current == NULL)
    {
        printf("未找到设备ID为 %d 的维修记录。\n", targetId);
        return;
    }

    // 找到目标维修记录，删除节点
    if (prev == NULL)
    {
        // 如果目标维修记录是头节点
        repairHead = current->next;
    }
    else
    {
        // 如果目标维修记录不是头节点
        prev->next = current->next;
    }

    // 释放内存
    free(current);
    printf("设备ID为 %d 的维修记录删除成功。\n", targetId);
}
void modifyRepairRecord()
{
    if (repairHead == NULL)
    {
        printf("暂无维修记录。\n");
        return;
    }

    int targetId;
    printf("请输入要修改的维修记录的设备ID: ");
    scanf("%d", &targetId);

    struct RepairNode *current = repairHead;

    // 遍历链表查找目标维修记录
    while (current != NULL && current->data.deviceId != targetId)
    {
        current = current->next;
    }

    // 如果未找到目标维修记录
    if (current == NULL)
    {
        printf("未找到设备ID为 %d 的维修记录。\n", targetId);
        return;
    }

    // 找到目标维修记录，修改维修信息
    printf("请输入新的维修日期: ");
    scanf("%s", current->data.repairDate);
    printf("请输入新的维修人员: ");
    scanf("%s", current->data.repairPerson);
    printf("请输入新的维修描述: ");
    scanf("%s", current->data.description);

    printf("设备ID为 %d 的维修记录修改成功。\n", targetId);
}
void searchRepairRecord()
{
    if (repairHead == NULL)
    {
        printf("暂无维修记录。\n");
        return;
    }

    int targetId;
    printf("请输入要查询的维修记录的设备ID: ");
    scanf("%d", &targetId);

    struct RepairNode *current = repairHead;

    // 遍历链表查找目标维修记录
    while (current != NULL && current->data.deviceId != targetId)
    {
        current = current->next;
    }

    // 如果未找到目标维修记录
    if (current == NULL)
    {
        printf("未找到设备ID为 %d 的维修记录。\n", targetId);
        return;
    }

    // 找到目标维修记录，输出维修信息
    printf("\n维修日期: %s\n", current->data.repairDate);
    printf("维修人员: %s\n", current->data.repairPerson);
    printf("维修描述: %s\n", current->data.description);
}
void displayRepairRecords()
{
    if (repairHead == NULL)
    {
        printf("暂无维修记录。\n");
        return;
    }

    printf("\n=== 所有维修记录 ===\n");
    printf("| %-10s | %-20s | %-20s | %-50s |\n", "设备ID", "维修日期", "维修人员", "维修描述");
    printf("|------------|----------------------|----------------------|--------------------------------------------------|\n");

    struct RepairNode *current = repairHead;

    // 遍历链表并输出维修记录信息
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
    printf("\n=== 添加变更记录 ===\n");

    // 输入设备ID，并检查其是否存在于设备列表中
    printf("请输入设备ID: ");
    scanf("%d", &newChange.deviceId);
    if (!checkDeviceExistence(newChange.deviceId))
    {
        printf("设备ID为 %d 的设备不存在，请先添加该设备。\n", newChange.deviceId);
        return;
    }

    printf("请输入原设备名称: ");
    scanf("%s", newChange.originName);
    printf("请输入新设备名称: ");
    scanf("%s", newChange.newName);
    printf("请输入变更日期: ");
    scanf("%s", newChange.date);

    // 创建新节点
    struct ChangeNode *newNode = (struct ChangeNode *)malloc(sizeof(struct ChangeNode));
    newNode->data = newChange;
    newNode->next = NULL;

    // 如果链表为空，新节点即为头节点
    if (changeHead == NULL)
    {
        changeHead = newNode;
    }
    else
    {
        // 遍历链表找到最后一个节点
        struct ChangeNode *current = changeHead;
        while (current->next != NULL)
        {
            current = current->next;
        }
        // 将新节点链接到最后一个节点后面
        current->next = newNode;
    }
    printf("变更记录添加成功！\n");
}

void deleteChangeRecord()
{
    if (changeHead == NULL)
    {
        printf("暂无变更记录。\n");
        return;
    }

    int targetId;
    printf("请输入要删除的变更记录的设备ID: ");
    scanf("%d", &targetId);

    struct ChangeNode *current = changeHead;
    struct ChangeNode *prev = NULL;

    // 遍历链表查找目标变更记录
    while (current != NULL && current->data.deviceId != targetId)
    {
        prev = current;
        current = current->next;
    }

    // 如果未找到目标变更记录
    if (current == NULL)
    {
        printf("未找到设备ID为 %d 的变更记录。\n", targetId);
        return;
    }

    // 找到目标变更记录，删除节点
    if (prev == NULL)
    {
        // 如果目标变更记录是头节点
        changeHead = current->next;
    }
    else
    {
        // 如果目标变更记录不是头节点
        prev->next = current->next;
    }

    // 释放内存
    free(current);
    printf("设备ID为 %d 的变更记录删除成功。\n", targetId);
}

void modifyChangeRecord()
{
    if (changeHead == NULL)
    {
        printf("暂无变更记录。\n");
        return;
    }

    int targetId;
    printf("请输入要修改的变更记录的设备ID: ");
    scanf("%d", &targetId);

    struct ChangeNode *current = changeHead;

    // 遍历链表查找目标变更记录
    while (current != NULL && current->data.deviceId != targetId)
    {
        current = current->next;
    }

    // 如果未找到目标变更记录
    if (current == NULL)
    {
        printf("未找到设备ID为 %d 的变更记录。\n", targetId);
        return;
    }

    // 找到目标变更记录，修改变更信息
    printf("请输入新的原设备名称: ");
    scanf("%s", current->data.originName);
    printf("请输入新的设备名称: ");
    scanf("%s", current->data.newName);
    printf("请输入新的变更日期: ");
    scanf("%s", current->data.date);

    printf("设备ID为 %d 的变更记录修改成功。\n", targetId);
}

void searchChangeRecord()
{
    if (changeHead == NULL)
    {
        printf("暂无变更记录。\n");
        return;
    }

    int targetId;
    printf("请输入要查询的变更记录的设备ID: ");
    scanf("%d", &targetId);

    struct ChangeNode *current = changeHead;

    // 遍历链表查找目标变更记录
    while (current != NULL && current->data.deviceId != targetId)
    {
        current = current->next;
    }

    // 如果未找到目标变更记录
    if (current == NULL)
    {
        printf("未找到设备ID为 %d 的变更记录。\n", targetId);
        return;
    }

    // 找到目标变更记录，输出变更信息
    printf("\n原设备名称: %s\n", current->data.originName);
    printf("新设备名称: %s\n", current->data.newName);
    printf("变更日期: %s\n", current->data.date);
}

void displayChangeRecords()
{
    if (changeHead == NULL)
    {
        printf("暂无变更记录。\n");
        return;
    }

    printf("\n=== 所有变更记录 ===\n");
    printf("| %-10s | %-20s | %-20s | %-20s |\n", "设备ID", "原设备名称", "新设备名称", "变更日期");
    printf("|------------|----------------------|----------------------|----------------------|\n");

    struct ChangeNode *current = changeHead;

    // 遍历链表并输出变更记录信息
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
        printf("\n--- 变更记录管理 ---\n");
        printf("1. 添加变更记录\n");
        printf("2. 删除变更记录\n");
        printf("3. 修改变更记录\n");
        printf("4. 查询变更记录\n");
        printf("5. 显示所有变更记录\n");
        printf("0. 返回主菜单\n");
        printf("请选择操作：");
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
            printf("无效的选择，请重新输入。\n");
        }
    } while (choice != 0);
}