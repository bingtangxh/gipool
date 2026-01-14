#include <conio.h>
#include "gipool.h"
#include "conmenus.h"
#include "localize.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
#define CLS system("cls")
#else
#define CLS system("clear")
#endif

#ifdef _MSC_VER
#define GETNUM(num) scanf_s("%d",&num)
#else
#define GETNUM(num) scanf("%d",&num)
#endif

#define ENDL putchar('\n')
#define SPACE putchar(' ')

char** localizedNames = NULL;

void initConsole();
void putPool(Wish_Pool WishPool1);
void printCompileTime();
void printTestInfo();
void beforeTerminate();
const int choiceMenu(const wchar_t* menuItems[], int itemCount, const wchar_t* title);
_Bool localizeNames(Char_Map CharMap1[], char* localizedNames[]);
void clearInputBuffer();
void freeLocalizedNames();

int main(int argc,char** argv)
{
    initConsole();
    initDynamicThings();
    do { localizedNames = (char**)malloc(charCount * sizeof(char*)); } while (localizedNames == NULL);
    localizeNames(CharMap, localizedNames);
	printTestInfo();
    ENDL;
	printCompileTime();
    freeDynamicThings();
    freeLocalizedNames();
    printf("Count of characters and pool info with errors: %d\n",checkIntegrity());
    ENDL;
	choiceMenu(mainMenu, ARRAY_SIZE(mainMenu), L"原神祈愿卡池信息工具我不管我就要让它变长看看能不能正常工作");
    // beforeTerminate();
    return 0;
}

void initConsole() {
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);
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
        if((
                    daysPassedSinceLastUP[arrangedInOrderOfDays[i]]!=INT_MIN
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
	char ending = '\0';
#ifdef _MSC_VER
    ending=_getch();
#else
    ending=getch();
#endif
}

const int choiceMenu(const wchar_t* menuItems[], int itemCount, const wchar_t* title) {
redisplayMenu:
    {
        size_t currentExpectedLength = 0, maxItemLength = 0, longestItemIndex = 0, gaptoMax = 0, gaptoMax_num = 0, longestIndexLength = 0, destSize = 0, titleLineSpaces = 0;
        char** localizedItemNames = NULL;
        for (int temp = itemCount; temp > 0; temp /= 10) { longestIndexLength++; }
        localizedItemNames = (char**)malloc(itemCount * sizeof(char*));
        for (int i = 0; i < itemCount; i++) {
            if ((currentExpectedLength = localizedLen(menuItems[i])) > maxItemLength) {
                maxItemLength = currentExpectedLength;
                longestItemIndex = i;
            }
        }
        if (localizedItemNames != NULL) {
            for (int i = 0; i < itemCount; i++) {
                localizedItemNames[i] = localize(menuItems[i]);
            }
        }

        char* localizedTitle = localize(title);
        if (localizedTitle != NULL) {
            if (strlen(localizedTitle) - 5 > maxItemLength) {
                maxItemLength = strlen(localizedTitle) - 5;
            }
            titleLineSpaces = maxItemLength + 5 - strlen(localizedTitle) + longestIndexLength;
            printW(L"╔");
            for (int i = 0; i < longestIndexLength + maxItemLength + 7; i++) { printW(L"═"); }
            printW(L"╗"); ENDL;

            printW(L"║"); SPACE;
            for (int i = 0; i < titleLineSpaces / 2 + titleLineSpaces % 2; i++) SPACE;
            printf("%s", localizedTitle);
            for (int i = 0; i < titleLineSpaces / 2; i++) SPACE;
            printW(L" ║\n");
            printW(L"╟");
            for (int i = 0; i < longestIndexLength + 4; i++) printW(L"─");
            printW(L"┬");
            for (int i = 0; i < maxItemLength + 2; i++) printW(L"─");
            printW(L"╢\n");
        }
        else {
            printW(L"╔");
            for (int i = 0; i < longestIndexLength + 4; i++) { printW(L"═"); }
            printW(L"╤");
            for (int i = 0; i < maxItemLength + 2; i++) { printW(L"═"); }
            printW(L"╗"); ENDL;
        }
        for (int i = 0; i < itemCount; i++) {
            printW(L"║");
            gaptoMax_num = longestIndexLength - printf(" [%d] ", i + 1) + 4;
            for (int j = 0; j < gaptoMax_num; j++) SPACE;
            // printf(" %*d ", longestIndexLength, i + 1);
            printW(L"│");
            if (localizedItemNames[i] != NULL) {
                printf(" %s ", localizedItemNames[i]);
                gaptoMax = maxItemLength - strlen(localizedItemNames[i]);
                for (int j = 0; j < gaptoMax; j++) SPACE;
                printW(L"║\n");
            }
            else ENDL;
        }
        printW(L"╚");
        for (int i = 0; i < longestIndexLength + 4; i++) printW(L"═");
        printW(L"╧");
        for (int i = 0; i < maxItemLength + 2; i++) printW(L"═");
        printW(L"╝\n"); ENDL;
        printf("Please select an option (1-%d) and press ENTER: ", itemCount);
        for (int i = 0; i < itemCount; i++) {
            free(localizedItemNames[i]);
            localizedItemNames[i] = NULL;
        }
        free(localizedItemNames);
        localizedItemNames = NULL;
        free(localizedTitle);
        localizedTitle = NULL;
        int choice = 0;
        char current = '\0';
        do {
            GETNUM(choice);
            if (choice == -1) {
                CLS;
                goto redisplayMenu;
            }
            else if (choice < 1 || choice > itemCount) {
                clearInputBuffer();
                printf("Invalid choice. Type -1 to redisplay the menu. (1-%d): ", itemCount);
            }
            else { break; }
        } while (1);
        return choice;
    }
}

_Bool localizeNames(Char_Map CharMap1[], char* localizedNames[]) {
    _Bool result = 0;
    for (int i = 0, conved = 0; i < charCount; i++) {
        int destSize = WideCharToMultiByte(CP_ACP, 0, CharMap1[i].name_cn, -1, NULL, 0, NULL, NULL);
        localizedNames[i] = (char*)malloc(sizeof(char) * (destSize + 0));
        conved = WideCharToMultiByte(CP_ACP, 0, CharMap1[i].name_cn, -1, localizedNames[i], destSize, NULL, NULL);
        if (conved == 0) result = 1;
    }
    return result;
    // result 返回 1 为未完成，返回 0 为成功完成。
}

void clearInputBuffer() {
    char current = '\0';                              
    do {
        current = getchar();                        
    } while (current != '\n' && current != EOF && current != '\0');
}

void freeLocalizedNames() {
    if (localizedNames != NULL) {
        for (int i = 0; i < charCount; i++) {
            if (localizedNames[i] != NULL) {
                free(localizedNames[i]);
                localizedNames[i] = NULL;
            }
        }
        free(localizedNames);
        localizedNames = NULL;
    }
}