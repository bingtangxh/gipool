#pragma once
#ifndef CONFUNC
#define CONFUNC
#ifndef GIPOOL
#include "gipool.h"
#endif
#ifndef LOCALIZE
#include "localize.h"
#endif

#define PUT_RIGHT_ALIGN_STAR_CHARACTER_NAME_FORMAT_LENGTH 10
#define PUT_LEFT_ALIGN_STAR_CHARACTER_NAME_FORMAT_LENGTH  11

#ifdef _WIN32
#include <conio.h>
#include <minwindef.h>
#define CLS system("cls")
#define ENDL putchar('\n')
#define putws(wstr) printW(wstr); putwchar(L'\n');
#ifdef _MSC_VER
#define PAUSE ending = _getch();
#define getch() _getch()
#define getche() _getche()
#else
#define PAUSE ending = getch();
#endif
#else
#define CLS system("clear")
#define ENDL putchar('\n')
// #define puts(str) puts(str); putchar('\r');
#define putws(wstr) printW(wstr); putwchar(L'\r'); putwchar(L'\n');
#define getche() \
    ending = getchar(); if(ending!=EOF&&ending!='\0'&&ending!='\n') {clearInputBuffer();}
#define getch() \
    ending = getchar(); if(ending!=EOF&&ending!='\0'&&ending!='\n') {clearInputBuffer();}
#define PAUSE ending = getchar(); if(ending!=EOF&&ending!='\0'&&ending!='\n') {clearInputBuffer();}
#endif

#define SPACE putchar(' ')

#ifdef _MSC_VER
#define GETNUM(num) scanf_s("%d",&num); clearInputBuffer();
#else
#define GETNUM(num) scanf("%d",&num); clearInputBuffer();
#endif

#if 1
const wchar_t singleEdge[]=L"─";
const wchar_t doubleEdge[]=L"═";
#else
const wchar_t singleEdge[]=L"-";
const wchar_t doubleEdge[]=L"=";
#endif


const char splitLine[]="-------------------------------------------------------------------------------------------------------";
#ifdef _WIN32

CONSOLE_SCREEN_BUFFER_INFO original;

WORD visionColor[]={
    FOREGROUND_BLUE |FOREGROUND_GREEN     |FOREGROUND_RED,           // 其他
    FOREGROUND_RED                        |FOREGROUND_INTENSITY,     // 火元素
    FOREGROUND_BLUE,                                                 // 水元素
    FOREGROUND_BLUE |FOREGROUND_GREEN     |FOREGROUND_INTENSITY,     // 风元素
    FOREGROUND_RED  |FOREGROUND_BLUE      |FOREGROUND_INTENSITY,     // 雷元素
    FOREGROUND_GREEN                      |FOREGROUND_INTENSITY,     // 草元素
    FOREGROUND_BLUE                       |FOREGROUND_INTENSITY,     // 冰元素
    FOREGROUND_GREEN|FOREGROUND_RED       |FOREGROUND_INTENSITY,     // 岩元素
    FOREGROUND_BLUE |FOREGROUND_GREEN     |FOREGROUND_RED            // 未知
};
#else

uint8_t visionColor[]={
    7,          // 其他
    9,          // 火元素
    45,          // 水元素
    80,          // 风元素
    93,          // 雷元素
    46,          // 草元素
    45,          // 冰元素
    220,          // 岩元素
    7           // 未知
};

#endif

void initConsole();
void putPool(_WishPool WishPool1);
void printCompileTime();
const int typeMenu(const wchar_t* menuItems[],int itemCount,const wchar_t* title);
const int choiceMenu(const wchar_t* menuItems[],int itemCount,const wchar_t* title);
_Bool localizeNames(_CharMap CharMap1[],char* localizedNames[]);
void clearInputBuffer();
void freeLocalizedNames();
void printPoolLinkList(PoolLinkList current);
int readIntInRange(int min,int max,const int* _default);
void pause();
_Bool SetConsoleColorByCharacter(_CharMap character);
_Bool ResetConsoleColor();

#ifdef _WIN32
DWORD printW(const wchar_t*);
#else
int printW(const wchar_t* wstr);
#endif

void initConsole() {
#ifdef _WIN32
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);
#else
    setlocale(LC_CTYPE,"");
#endif
}

void putPool(_WishPool WishPool1)
{
    if (WishPool1.half>=10) { puts(splitLine); }
    size_t fiveCount=sizeof(WishPool1.up5)==0 ? 0 : (int) (sizeof(WishPool1.up5)/sizeof(WishPool1.up5[0]));
    for(size_t i=0; i<fiveCount; i++) {
        if (WishPool1.up5[i]==0) {
            fiveCount=i;
            break;
        }
    }
    size_t fourCount=sizeof(WishPool1.up4)==0 ? 0 : (int) (sizeof(WishPool1.up4)/sizeof(WishPool1.up4[0]));
    printf("%hu.%hu.%hu\t%u.%hu.%hu\t%u.%hu.%hu\t | ",WishPool1.major,WishPool1.minor,WishPool1.half,WishPool1.startY,WishPool1.startM,WishPool1.startD,WishPool1.endY,WishPool1.endM,WishPool1.endD);

    for (size_t i=0; i<2-fiveCount&&WishPool1.half<10; i++){
        for(size_t j=0;j<=localizedVisualLen(CharMap[longestChineseIndex].name_cn);j++){
            SPACE;
        }
        if (WishPool1.half<10) { printf("| "); }
    }
    for (size_t i=0; i<fiveCount&&WishPool1.up5[i]!=0; i++)
    {
        for(size_t j=0;
            (j<localizedVisualLen(CharMap[longestChineseIndex].name_cn)-localizedVisualLen(CharMap[WishPool1.up5[i]].name_cn))&&WishPool1.half<10;
        j++){ SPACE; }
        //printW(CharMap[WishPool1.up5[i]].name_cn);
        SetConsoleColorByCharacter(CharMap[WishPool1.up5[i]]);
        printf("%s ",localizedNames[WishPool1.up5[i]]==NULL ? "" : localizedNames[WishPool1.up5[i]]);
        ResetConsoleColor();
        if (WishPool1.half<10) { printf("| "); }
    }
    for (size_t i=0; i<fourCount&&WishPool1.up4[i]!=0; i++)
    {
        SetConsoleColorByCharacter(CharMap[WishPool1.up4[i]]);
        printf("%s ",localizedNames[WishPool1.up4[i]]==NULL ? "" : localizedNames[WishPool1.up4[i]]);
        ResetConsoleColor();

        for(size_t j=0;j<localizedVisualLen(CharMap[longestChineseIndex].name_cn)-localizedVisualLen(CharMap[WishPool1.up4[i]].name_cn);j++){
            SPACE;
        }

    }
    ENDL;
    if (WishPool1.half>=10) { puts(splitLine); }
}

void printCompileTime() {
    char date[DATE_LENGTH]=__DATE__;
    convertCompileTime(date);
    printf("Compiled at %s %s",date,__TIME__);
    ENDL;
}

const int typeMenu(const wchar_t* menuItems[],int itemCount,const wchar_t* title) {
    do
    {
        size_t currentExpectedLength=0,maxItemLength=0,longestItemIndex=0,gaptoMax=0,gaptoMax_num=0,longestIndexLength=0,titleLineSpaces=0;
        // size_t destSize=0;
        char** localizedItemNames=NULL;
        for (int temp=itemCount; temp>0; temp/=10) { longestIndexLength++; }
        localizedItemNames=(char**) malloc(itemCount*sizeof(char*));
        for (int i=0; i<itemCount; i++) {
            if ((currentExpectedLength=localizedVisualLen(menuItems[i]))>maxItemLength) {
                maxItemLength=currentExpectedLength;
                longestItemIndex=i;
            }
        }
        if (localizedItemNames!=NULL) {
            for (int i=0; i<itemCount; i++) {
                if ((localizedItemNames[i]=localize(menuItems[i]))==NULL) localizedItemNames[i]=NULL;
            }
        }

        char* localizedTitle=localize(title);
        if (localizedTitle!=NULL) {
            if (localizedVisualLen(title)-4>maxItemLength) {
                maxItemLength=localizedVisualLen(title)-4;
            }
            titleLineSpaces=maxItemLength+5-localizedVisualLen(title)+longestIndexLength;
            printW(L"╔");
            for (size_t i=0; i<longestIndexLength+maxItemLength+7; i++) { printW(doubleEdge); }
            printW(L"╗"); ENDL;

            printW(L"║"); SPACE;
            for (size_t i=0; i<titleLineSpaces/2+titleLineSpaces%2; i++) { SPACE; }
            printf("%s",localizedTitle);
            for (size_t i=0; i<titleLineSpaces/2; i++) { SPACE; }
            SPACE;
            printW(L"║"); ENDL;
            printW(L"╟");
            for (size_t i=0; i<longestIndexLength+4; i++) { printW(singleEdge); }
            printW(L"┬");
            for (size_t i=0; i<maxItemLength+2; i++) { printW(singleEdge); }
            printW(L"╢"); ENDL;
        } else {
            printW(L"╔");
            for (size_t i=0; i<longestIndexLength+4; i++) { printW(doubleEdge); }
            printW(L"╤");
            for (size_t i=0; i<maxItemLength+2; i++) { printW(doubleEdge); }
            printW(L"╗"); ENDL;
        }
        for (int i=0,currentIndexColLen=0; i<itemCount; i++) {
            printW(L"║");
            currentIndexColLen=printf(" [%d] ",i==itemCount-1 ? 0 : i+1);
            currentIndexColLen=currentIndexColLen<0 ? 0 : currentIndexColLen;
            gaptoMax_num=longestIndexLength-currentIndexColLen+4;
            for (size_t j=0; j<gaptoMax_num; j++) SPACE;
            // printf(" %*d ", longestIndexLength, i + 1);
            printW(L"│");
            if (localizedItemNames[i]!=NULL) {
                printf(" %s ",localizedItemNames[i]);
                gaptoMax=maxItemLength-localizedVisualLen(menuItems[i]);
                for (size_t j=0; j<gaptoMax; j++) SPACE;
                printW(L"║"); ENDL;
            } else ENDL;
        }
        printW(L"╚");
        for (size_t i=0; i<longestIndexLength+4; i++) { printW(doubleEdge); }
        printW(L"╧");
        for (size_t i=0; i<maxItemLength+2; i++) { printW(doubleEdge); }
        printW(L"╝"); ENDL;
        ENDL;
        printf("Please select an option (0-%d) and press ENTER: ",itemCount-1);
        for (int i=0; i<itemCount; i++) {
            free(localizedItemNames[i]);
            localizedItemNames[i]=NULL;
        }
        free(localizedItemNames);
        localizedItemNames=NULL;
        free(localizedTitle);
        localizedTitle=NULL;
        int choice=INT_MIN;
        do {
            choice=readIntInRange(-1,itemCount-1,NULL);
            // clearInputBuffer();
            if (choice==-1) {
                CLS;
                break;
            } else if (choice < 0||choice > itemCount-1) {
                printf("Invalid choice. Type -1 to redisplay the menu. (0-%d): ",itemCount-1);
            } else { break; }
        } while (1);
        if (choice==-1) continue;
        else return choice;
    } while (1);
}

const int choiceMenu(const wchar_t* menuItems[],int itemCount,const wchar_t* title) {
    do {
        size_t currentExpectedLength=0,maxItemLength=0,longestItemIndex=0,gaptoMax=0,gaptoMax_num=0,longestIndexLength=1,titleLineSpaces=0;
        // size_t destSize=0;
        char** localizedItemNames=NULL;
        // for (int temp = itemCount; temp > 0; temp /= 10) { longestIndexLength++; }
        localizedItemNames=(char**) malloc(itemCount*sizeof(char*));
        for (int i=0; i<itemCount; i++) {
            if ((currentExpectedLength=localizedVisualLen(menuItems[i]))>maxItemLength) {
                maxItemLength=currentExpectedLength;
                longestItemIndex=i;
            }
        }
        if (localizedItemNames!=NULL) {
            for (int i=0; i<itemCount; i++) {
                if ((localizedItemNames[i]=localize(menuItems[i]))==NULL) localizedItemNames[i]=NULL;
            }
        }

        char* localizedTitle=localize(title);
        if (localizedTitle!=NULL) {
            if (localizedVisualLen(title)-4>maxItemLength) {
                maxItemLength=localizedVisualLen(title)-4;
            }
            titleLineSpaces=maxItemLength+5-localizedVisualLen(title)+longestIndexLength;
            printW(L"╔");
            for (size_t i=0; i<longestIndexLength+maxItemLength+7; i++) { printW(doubleEdge); }

            printW(L"╗"); ENDL;

            printW(L"║"); SPACE;
            for (size_t i=0; i<titleLineSpaces/2+titleLineSpaces%2; i++) { SPACE; }
            printf("%s",localizedTitle);
            for (size_t i=0; i<titleLineSpaces/2; i++) { SPACE; }
            SPACE;
            printW(L"║"); ENDL;
            printW(L"╟");
            for (size_t i=0; i<longestIndexLength+4; i++) { printW(singleEdge); }
            printW(L"┬");
            for (size_t i=0; i<maxItemLength+2; i++) { printW(singleEdge); }
            printW(L"╢"); ENDL;
        } else {
            printW(L"╔");
            for (size_t i=0; i<longestIndexLength+4; i++) { printW(doubleEdge); }
            printW(L"╤");
            for (size_t i=0; i<maxItemLength+2; i++) { printW(doubleEdge); }
            printW(L"╗"); ENDL;
        }
        for (int i=0,currentIndexColLen=0; i<itemCount; i++) {
            if (i>=35&&i!=itemCount-1) continue;
            else {
                printW(L"║");
                if (i<9) {
                    currentIndexColLen=printf(" [%d] ",
                                              i==itemCount-1 ? 0 :
                                              i+1
                    );
                } else if (i>=9&&i<itemCount-1&&i<35) {
                    currentIndexColLen=printf(" [%c] ",'A'+i-9);
                } else if (i==itemCount-1) {
                    currentIndexColLen=printf(" [%d] ",0);
                }
                currentIndexColLen=currentIndexColLen<0 ? 0 : currentIndexColLen;
                gaptoMax_num=longestIndexLength-currentIndexColLen+4;
                for (size_t j=0; j<gaptoMax_num; j++) SPACE;
                // printf(" %*d ", longestIndexLength, i + 1);
                printW(L"│");
                if (localizedItemNames[i]!=NULL) {
                    printf(" %s ",localizedItemNames[i]);
                    gaptoMax=maxItemLength-localizedVisualLen(menuItems[i]);
                    for (size_t j=0; j<gaptoMax; j++) SPACE;
                    printW(L"║"); ENDL;
                } else ENDL;
            }
        }
        printW(L"╚");
        for (size_t i=0; i<longestIndexLength+4; i++) { printW(doubleEdge); }
        printW(L"╧");
        for (size_t i=0; i<maxItemLength+2; i++) { printW(doubleEdge); }
        printW(L"╝"); ENDL;
        ENDL;
        printf(
#ifdef _WIN32
            "Please select an option (0-%c): ",
#else
            "Please select an option and press ENTER (0-%c): ",
#endif
               itemCount<=10 ? itemCount-1+'0' :
               itemCount<=35 ? itemCount-11+'A' : 'Z'
        );
        for (int i=0; i<itemCount; i++) {
            free(localizedItemNames[i]);
            localizedItemNames[i]=NULL;
        }
        free(localizedItemNames);
        localizedItemNames=NULL;
        free(localizedTitle);
        localizedTitle=NULL;
        int choice=-1;
        do {
#ifndef _WIN32
            fflush(stdout);
#endif
            choice=
#ifdef _MSC_VER
                _getche();
#else
                getche();
#endif
            if ('A'<=choice&&choice<='Z') {
                choice-='A';
                choice+=10;
            } else if ('a'<=choice&&choice<='z') {
                choice-='a';
                choice+=10;
            } else if ('0'<=choice&&choice<='9') {
                choice-='0';
            } else {
                // 到这就不可能是正常输入，直接赋一个 -1 让下面直接进行错误处理
                choice=-1;
            }
            // 注意这里没有 else
            // 这里是已经处理完输入了
            // 下面是边界检测，如果没有问题就 break 然后 return
            if (choice < 0||choice > itemCount-1) {
                ENDL;
                printf("Invalid choice. Please try again. (0-%c): ",
                       itemCount<=10 ? itemCount-1+'0' :
                       itemCount<=35 ? itemCount-11+'A' : 'Z'
                );
            } else { break; }
        } while (1);
        return choice;
    } while (1);
}


_Bool localizeNames(_CharMap CharMap1[],char* _localizedNames[]) {
    _Bool result=0;
    for (size_t i=0; i<charCount; i++) {
        if ((_localizedNames[i]=localize(CharMap1[i].name_cn))==NULL) result=1;
    }
    return result;
    // result 返回 1 为未完成，返回 0 为成功完成。
}

void clearInputBuffer() {
    int current='\0';
    do {
        current=getchar();
    } while (current!='\n'&&current!=EOF&&current!='\0');
}

void freeLocalizedNames() {
    if (localizedNames!=NULL) {
        for (size_t i=0; i<charCount; i++) {
            if (localizedNames[i]!=NULL) {
                free(localizedNames[i]);
                localizedNames[i]=NULL;
            }
        }
        free(localizedNames);
        localizedNames=NULL;
    }
}

void printPoolLinkList(PoolLinkList current) {
    if (current==NULL) { ENDL; puts("This character have't been UP yet."); }
    for (int i=0; current!=NULL; i++) {
        if (i%3==0) ENDL;
        printf("%hu.%hu.%hu\t",current->major,current->minor,current->half);
        current=current->next;
    }
    ENDL;
}

int readIntInRange(int min,int max,const int* _default)
{
    char buf[64],*end;
    long val;
    while (1)
    {
        if (!fgets(buf,sizeof(buf),stdin))
        {
            if (feof(stdin))
            {
                ENDL;
                printf("EOF detected. Exiting.");
                ENDL;
                exit(1);
            }
            clearerr(stdin);
            continue;
        }
        if (buf[0]=='\n')
        {
            if (_default!=NULL) { return *_default; }
            printf("Empty input. Enter a number between %d and %d: ",min,max);
            continue;
        }
        if (!strchr(buf,'\n'))
        {
            printf("Input too long. Enter a number between %d and %d: ",min,max);
            clearInputBuffer();
            continue;
        }
        errno=0;
        val=strtol(buf,&end,10);
        if (end==buf||(*end!='\n'&&*end!='\0'))
        {
            printf("Invalid input. Enter a valid integer between %d and %d: ",min,max);
            continue;
        }
        if (errno==ERANGE||val < min||val > max)
        {
            printf("Out of range. Enter a number between %d and %d: ",min,max);
            continue;
        }
        return (int) val;
    }
}

void pause()
{
    ending='\0';
#ifdef _WIN32
#ifdef _MSC_VER
    ending=_getch();
#else
    ending=(char) getch();
#endif
#else
    do {
        ending=getchar();
    } while (ending!='\n'&&ending!=EOF&&ending!='\0');
#endif
}


#ifdef _WIN32
DWORD printW(const wchar_t* wstr) {
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    WriteConsoleW(hConsole,wstr,(DWORD) wcslen(wstr),&written,NULL);
    return written;
}
#else
int printW(const wchar_t* wstr) {
     return printf("%ls",wstr);
}
#endif

_Bool SetConsoleColorByCharacter(_CharMap character) {
#ifdef _WIN32
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleScreenBufferInfo(hConsole,&original)) {
        return 0;
    }
    SetConsoleTextAttribute(hConsole,visionColor[character.vision]);
    return 1;
#else
    printf("\033[38;5;%um",visionColor[character.vision]);
#endif
}

_Bool ResetConsoleColor() {
#ifdef _WIN32
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,original.wAttributes);
    return 1;
#else
    printf("\033[0m");
#endif
}

#endif   // #ifndef CONFUNC
