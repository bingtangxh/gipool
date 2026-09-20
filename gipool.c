#include "gipool.h"

#include "gipool.h"

PoolLinkList* PoolLinkLists=NULL;
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
uint8_t visionColor[]={7, 9, 33, 43, 99, 46, 159, 220, 7};
#endif


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
    return 0;
}

void printTestInfo(void)
{
}

void exitDuetoFatalError(const int code,const char* message,const int id)
{
    // 切勿将该函数的 message 参数交由不可靠的用户输入决定，否则可能会导致格式化字符串漏洞
    fputs(message, stderr);
    if(id>=0) {
        fprintf(stderr,"\nThe error occurred when id is: %d. ",id);
    }
    else { fputc('\n', stderr); }
    fputs("gipool cannot continue running.\n", stderr);
    // 确保 mainMenu 结束后和 exitDuetoFatalError 函数调用时，都释放动态申请的内存且两种退出方式流程相同
    freeDynamicThings();
    beforeTerminate();
    exit(code);
}
