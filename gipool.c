#include <conio.h>
#include "gipool.h"
#include "conmenus.h"
#define ENDL putchar('\n')
void initConsole();
void printW(const wchar_t* wstr);
void putPool(Wish_Pool WishPool1);
void printCompileTime();
void printTestInfo();
void beforeTerminate();
void displayMenu(const wchar_t* menuItems[], int itemCount, const wchar_t* title);

int main(int argc,char** argv)
{
    initConsole();
    initDynamicThings();
	// printTestInfo();
    ENDL;
	printCompileTime();
    freeDynamicThings();
    printf("Count of characters and pool info with errors: %d\n",checkIntegrity());
    ENDL;
	displayMenu(mainMenu, ARRAY_SIZE(mainMenu), L"原神祈愿卡池信息工具");
    // beforeTerminate();
    return 0;
}

void initConsole() {
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);
}

void printW(const wchar_t* wstr) {
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    WriteConsoleW(hConsole,wstr,(DWORD)wcslen(wstr),&written,NULL);
}

void putPool(Wish_Pool WishPool1)
{
    int fiveCount=sizeof(WishPool1.up5)==0?0:(int)(sizeof(WishPool1.up5)/sizeof(WishPool1.up5[0]));
    int fourCount=sizeof(WishPool1.up4)==0?0:(int)(sizeof(WishPool1.up4)/sizeof(WishPool1.up4[0]));
    printf("%hu.%hu.%hu\t%u.%hu.%hu\t%u.%hu.%hu\t",WishPool1.major,WishPool1.minor,WishPool1.half,WishPool1.startY,WishPool1.startM,WishPool1.startD,WishPool1.endY,WishPool1.endM,WishPool1.endD);
    for(int i=0;i<fiveCount&&WishPool1.up5[i]!=0;i++)
    {
        //printW(CharMap[WishPool1.up5[i]].name_cn);
        printf("%s ",localizedNames[WishPool1.up5[i]]);
    }
    for(int i=0;i<fourCount&&WishPool1.up4[i]!=0;i++)
    {
        //printW(CharMap[WishPool1.up4[i]].name_cn);
        printf("%s ",localizedNames[WishPool1.up4[i]]);
    }
    putchar('\n');
}

void printCompileTime(){
    char date[DATE_LENGTH]=__DATE__;
    convertCompileTime(date);
    printf("Compiled at %s %s\n\n",date,__TIME__);
}

void printTestInfo(){
    for(int i=0;i<poolCount;i++){
        putPool(WishPool[i]);
    }
    for(int i=0;i<charCount;i++){
        if((1||
                    daysPassedSinceLastUP[arrangedInOrderOfDays[i]]
            )&&((
                    CharMap[arrangedInOrderOfDays[i]].attrib==9
                )||(
                    CharMap[arrangedInOrderOfDays[i]].attrib==5
            ))){
            printf("%12s\t%d\n",localizedNames[arrangedInOrderOfDays[i]],daysPassedSinceLastUP[arrangedInOrderOfDays[i]]);
        }
        else{
            // printf("%12s\t%u\n",localizedNames[arrangedInOrderOfDays[i]],CharMap[arrangedInOrderOfDays[i]].attrib);
        }
    }
}

void beforeTerminate(){
    puts("Press any key to exit...");
#ifdef _MSC_VER
    _getch();
#else
    getch();
#endif
}

void displayMenu(const wchar_t* menuItems[], int itemCount, const wchar_t* title) {
    size_t maxItemLength = 0, longestItemIndex = 0, gaptoMax = 0, gaptoMax_num = 0, longestIndexLength = 0;
    int currentExpectedLength = 0,destSize = 0,titleLineSpaces=0;
	for (int temp=itemCount;temp>0;temp/=10) longestIndexLength++;
	localizedNames = (char**)malloc(itemCount * sizeof(char*));
    for(int i=0;i<itemCount;i++){
        if((currentExpectedLength=(int)WideCharToMultiByte(CP_ACP,0,menuItems[i],-1,NULL,0,NULL,NULL))>maxItemLength) {
            maxItemLength=currentExpectedLength;
            longestItemIndex=i;
        }
	}
    for(int i=0;i<itemCount;i++){
        destSize=WideCharToMultiByte(CP_ACP,0,menuItems[i],-1,NULL,0,NULL,NULL);
        localizedNames[i]=(char*)malloc(sizeof(char)*(destSize+0));
        WideCharToMultiByte(CP_ACP,0,menuItems[i],-1,localizedNames[i],destSize,NULL,NULL);
	}
	// printW(L"╔");
	// printW(L"═");
    // printW(L"╗");
	// printW(L"║");
    printW(L"╔");
    for (int i = 0; i < longestIndexLength + maxItemLength + 7; i++) printW(L"═");
    printW(L"╗\n");
    destSize = WideCharToMultiByte(CP_ACP, 0, title, -1, NULL, 0, NULL, NULL);
    char* localizedTitle = (char*)malloc(sizeof(char) * (destSize + 0));
    WideCharToMultiByte(CP_ACP, 0, title, -1, localizedTitle, destSize, NULL, NULL);
    if (localizedTitle != NULL) {
        printW(L"║ ");
        if (strlen(localizedTitle) - 5 > maxItemLength) {
            maxItemLength = strlen(localizedTitle) - 5;
        }
        titleLineSpaces = maxItemLength + 5 - strlen(localizedTitle);
        for (int i = 0; i < titleLineSpaces / 2+1; i++) putchar(' ');
        printf("%s",localizedTitle);
        for (int i = 0; i < (titleLineSpaces / 2+titleLineSpaces%2); i++) putchar(' ');
        printW(L" ║\n");
        printW(L"╟");
        for (int i = 0; i < longestIndexLength + 4; i++) printW(L"─");
        printW(L"┬");
        for (int i = 0; i < maxItemLength + 2; i++) printW(L"─");
        printW(L"╢\n");
    }
    for (int i = 0; i < itemCount; i++) {
        printW(L"║");
        gaptoMax_num=longestIndexLength-printf(" [%d] ", i+1)+4;
        for (int j = 0; j < gaptoMax_num; j++) putchar(' ');
        // printf(" %*d ", longestIndexLength, i + 1);
        printW(L"│");
        printf(" %s ",localizedNames[i]);
        if (localizedNames[i]!=NULL) {
            gaptoMax = maxItemLength - strlen(localizedNames[i]);
            for (int j = 0; j < gaptoMax; j++) putchar(' ');
            printW(L"║\n");
        } else ENDL;
    }
    printW(L"╚");
    for (int i = 0; i < longestIndexLength + 4; i++) printW(L"═");
    printW(L"╧");
    for (int i = 0; i < maxItemLength + 2; i++) printW(L"═");
    printW(L"╝\n");
    printf("Please select an option (1-%d): ", itemCount);
}