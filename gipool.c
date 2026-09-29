#include "gipool.h"

PoolLinkList* PoolLinkLists=NULL;
AllocationNode* allocatedMemoryList=NULL;
size_t charCount=0;
size_t poolCount=0;
size_t longestChineseIndex=0;
size_t longestChineseNameLength=0;
size_t longestEnglishIndex=0;
size_t longestEnglishNameLength=0;
int ending='\0';
int* daysPassedSinceLastUP=NULL;
int* arrangedInOrderOfDays=NULL;
char** localizedNames=NULL;
const int maxCharactersInCharPool=2;

const wchar_t singleEdge[]=L"─";
const wchar_t doubleEdge[]=L"═";
const char splitLine[]="-------------------------------------------------------------------------------------------------------";

#ifdef _WIN32
CONSOLE_SCREEN_BUFFER_INFO original;
WORD visionColor[]={
    FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED,
    FOREGROUND_RED|FOREGROUND_INTENSITY,
    FOREGROUND_BLUE|FOREGROUND_INTENSITY,
    FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY,
    FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY,
    FOREGROUND_GREEN|FOREGROUND_INTENSITY,
    FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY,
    FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY,
    FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED
};
#else
uint8_t visionColor[]={ 7, 9, 33, 43, 99, 46, 159, 220, 7 };
#endif

_Bool appendAllocation(void* address)
{
    AllocationNode* node=NULL;
    AllocationNode* current=NULL;

    if(address==NULL||isAllocationInList(address)) {
        return 0;
    }

    node=(AllocationNode*)malloc(sizeof(*node));
    if(node==NULL) {
        return 0;
    }
    node->address=address;
    node->next=NULL;

    if(allocatedMemoryList==NULL) {
        allocatedMemoryList=node;
        return 1;
    }

    current=allocatedMemoryList;
    while(current->next!=NULL) {
        current=current->next;
    }
    current->next=node;
    return 1;
}

_Bool removeAllocation(void* address)
{
    AllocationNode* current=NULL;
    AllocationNode* previous=NULL;

    if(address==NULL) {
        return 0;
    }

    current=allocatedMemoryList;
    while(current!=NULL) {
        if(current->address==address) {
            if(previous==NULL) {
                allocatedMemoryList=current->next;
            }
            else {
                previous->next=current->next;
            }
            free(current);
            return 1;
        }
        previous=current;
        current=current->next;
    }
    return 0;
}

_Bool isAllocationInList(const void* address)
{
    AllocationNode* current=NULL;

    if(address==NULL) {
        return 0;
    }

    current=allocatedMemoryList;
    while(current!=NULL) {
        if(current->address==address) {
            return 1;
        }
        current=current->next;
    }
    return 0;
}

void* trackedMalloc(size_t size)
{
    void* address=NULL;

    address=malloc(size);
    if(address==NULL) {
        return NULL;
    }
    if(!appendAllocation(address)) {
        free(address);
        return NULL;
    }
    return address;
}

void trackedFree(void* address)
{
    if(address==NULL) {
        return;
    }
    if(removeAllocation(address)) {
        free(address);
    }
}

void freeAllTrackedAllocations(void)
{
    AllocationNode* current=NULL;
    AllocationNode* next=NULL;

    current=allocatedMemoryList;
    allocatedMemoryList=NULL;
    while(current!=NULL) {
        next=current->next;
        free(current->address);
        free(current);
        current=next;
    }
}


int main(int argc,char** argv)
{
    initConsole();
    if(argc>1) {
        if(!strcmp(argv[1],"/?")) {
            help();
            return 0;
        }
    }
    initDynamicThings();
    printCompileTime();
    ENDL;
    printf("Count of characters and pool info with errors: %d",checkIntegrity());
    printTestInfo();
    ENDL;
    mainMenu();
    // 确保 mainMenu 结束后和 exitDuetoFatalError 函数调用时，都释放动态申请的内存且两种退出方式流程相同
    freeDynamicThings();
    freeAllTrackedAllocations();
    return 0;
}
// Codex write test: September 29, 2026.

void printTestInfo(void)
{
}

void exitDuetoFatalError(const int code,const char* message,const int id)
{
    // 切勿将该函数的 message 参数交由不可靠的用户输入决定，否则可能会导致格式化字符串漏洞
    fputs(message,stderr);
    if(id>=0) {
        fprintf(stderr,"\nThe error occurred when id is: %d. ",id);
    }
    else { fputc('\n',stderr); }
    fputs("gipool cannot continue running.\n",stderr);
    // 确保 mainMenu 结束后和 exitDuetoFatalError 函数调用时，都释放动态申请的内存且两种退出方式流程相同
    freeDynamicThings();
    freeAllTrackedAllocations();
    beforeTerminate();
    exit(code);
}
