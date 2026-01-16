#pragma once
#define CONFUNC
#ifndef GIPOOL
#include "gipool.h"
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

#ifdef _MSC_VER
#define PAUSE ending = _getch(); 
#else
#define PAUSE ending = getch();
#endif

void initConsole();
void putPool(Wish_Pool WishPool1);
void printCompileTime();
const int typeMenu(const wchar_t* menuItems[],int itemCount,const wchar_t* title);
const int choiceMenu(const wchar_t* menuItems[],int itemCount,const wchar_t* title);
_Bool localizeNames(Char_Map CharMap1[],char* localizedNames[]);
void clearInputBuffer();
void freeLocalizedNames();
void printPoolLinkList(PoolLinkList current);

void initConsole() {
#ifdef _WIN32
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);
#else
    setlocale(LC_CTYPE,"");
#endif

}

void putPool(Wish_Pool WishPool1)
{
    int fiveCount=sizeof(WishPool1.up5)==0 ? 0 : (int) (sizeof(WishPool1.up5)/sizeof(WishPool1.up5[0]));
    int fourCount=sizeof(WishPool1.up4)==0 ? 0 : (int) (sizeof(WishPool1.up4)/sizeof(WishPool1.up4[0]));
    printf("%hu.%hu.%hu\t%u.%hu.%hu\t%u.%hu.%hu\t",WishPool1.major,WishPool1.minor,WishPool1.half,WishPool1.startY,WishPool1.startM,WishPool1.startD,WishPool1.endY,WishPool1.endM,WishPool1.endD);
    for (int i=0; i<fiveCount&&WishPool1.up5[i]!=0; i++)
    {
        //printW(CharMap[WishPool1.up5[i]].name_cn);
        printf("%s ",localizedNames[WishPool1.up5[i]]==NULL ? "" : localizedNames[WishPool1.up5[i]]);
    }
    for (int i=0; i<fourCount&&WishPool1.up4[i]!=0; i++)
    {
        //printW(CharMap[WishPool1.up4[i]].name_cn);
        printf("%s ",localizedNames[WishPool1.up4[i]]==NULL ? "" : localizedNames[WishPool1.up4[i]]);
    }
    putchar('\n');
}

void printCompileTime() {
    char date[DATE_LENGTH]=__DATE__;
    convertCompileTime(date);
    printf("Compiled at %s %s\n",date,__TIME__);
}

const int typeMenu(const wchar_t* menuItems[],int itemCount,const wchar_t* title) {
    do
    {
        size_t currentExpectedLength=0,maxItemLength=0,longestItemIndex=0,gaptoMax=0,gaptoMax_num=0,longestIndexLength=0,destSize=0,titleLineSpaces=0;
        char** localizedItemNames=NULL;
        for (int temp=itemCount; temp>0; temp/=10) { longestIndexLength++; }
        localizedItemNames=(char**) malloc(itemCount*sizeof(char*));
        for (int i=0; i<itemCount; i++) {
            if ((currentExpectedLength=localizedLen(menuItems[i]))>maxItemLength) {
                maxItemLength=currentExpectedLength;
                longestItemIndex=i;
            }
        }
        if (localizedItemNames!=NULL) {
            for (int i=0; i<itemCount; i++) {
                if ((localizedItemNames[i]=localize(menuItems[i]))==NULL) localizedItemNames[i]="";
            }
        }

        char* localizedTitle=localize(title);
        if (localizedTitle!=NULL) {
            if (strlen(localizedTitle)-4>maxItemLength) {
                maxItemLength=strlen(localizedTitle)-4;
            }
            titleLineSpaces=maxItemLength+4-strlen(localizedTitle)+longestIndexLength;
            printW(L"╔");
            for (int i=0; i<longestIndexLength+maxItemLength+6; i++) { printW(L"═"); }
            printW(L"╗"); ENDL;

            printW(L"║"); SPACE;
            for (int i=0; i<titleLineSpaces/2+titleLineSpaces%2; i++) { SPACE; }
            printf("%s",localizedTitle);
            for (int i=0; i<titleLineSpaces/2; i++) { SPACE; }
            SPACE;
            printW(L"║"); ENDL;
            printW(L"╟");
            for (int i=0; i<longestIndexLength+4; i++) { printW(L"─"); }
            printW(L"┬");
            for (int i=0; i<maxItemLength+1; i++) { printW(L"─"); }
            printW(L"╢"); ENDL;
        } else {
            printW(L"╔");
            for (int i=0; i<longestIndexLength+4; i++) { printW(L"═"); }
            printW(L"╤");
            for (int i=0; i<maxItemLength+1; i++) { printW(L"═"); }
            printW(L"╗"); ENDL;
        }
        for (int i=0,currentIndexColLen=0; i<itemCount; i++) {
            printW(L"║");
            currentIndexColLen=printf(" [%d] ",i==itemCount-1 ? 0 : i+1);
            currentIndexColLen=currentIndexColLen<0 ? 0 : currentIndexColLen;
            gaptoMax_num=longestIndexLength-currentIndexColLen+4;
            for (int j=0; j<gaptoMax_num; j++) SPACE;
            // printf(" %*d ", longestIndexLength, i + 1);
            printW(L"│");
            if (localizedItemNames[i]!=NULL) {
                printf(" %s ",localizedItemNames[i]);
                gaptoMax=maxItemLength-1-strlen(localizedItemNames[i]);
                for (int j=0; j<gaptoMax; j++) SPACE;
                printW(L"║"); ENDL;
            } else ENDL;
        }
        printW(L"╚");
        for (int i=0; i<longestIndexLength+4; i++) { printW(L"═"); }
        printW(L"╧");
        for (int i=0; i<maxItemLength+1; i++) { printW(L"═"); }
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
            GETNUM(choice);
            clearInputBuffer();
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
        size_t currentExpectedLength=0,maxItemLength=0,longestItemIndex=0,gaptoMax=0,gaptoMax_num=0,longestIndexLength=1,destSize=0,titleLineSpaces=0;
        char** localizedItemNames=NULL;
        // for (int temp = itemCount; temp > 0; temp /= 10) { longestIndexLength++; }
        localizedItemNames=(char**) malloc(itemCount*sizeof(char*));
        for (int i=0; i<itemCount; i++) {
            if ((currentExpectedLength=localizedLen(menuItems[i]))>maxItemLength) {
                maxItemLength=currentExpectedLength;
                longestItemIndex=i;
            }
        }
        if (localizedItemNames!=NULL) {
            for (int i=0; i<itemCount; i++) {
                if ((localizedItemNames[i]=localize(menuItems[i]))==NULL) localizedItemNames[i]="";
            }
        }

        char* localizedTitle=localize(title);
        if (localizedTitle!=NULL) {
            if (strlen(localizedTitle)-4>maxItemLength) {
                maxItemLength=strlen(localizedTitle)-4;
            }
            titleLineSpaces=maxItemLength+4-strlen(localizedTitle)+longestIndexLength;
            printW(L"╔");
            for (int i=0; i<longestIndexLength+maxItemLength+6; i++) { printW(L"═"); }
            printW(L"╗"); ENDL;

            printW(L"║"); SPACE;
            for (int i=0; i<titleLineSpaces/2+titleLineSpaces%2; i++) { SPACE; }
            printf("%s",localizedTitle);
            for (int i=0; i<titleLineSpaces/2; i++) { SPACE; }
            SPACE;
            printW(L"║"); ENDL;
            printW(L"╟");
            for (int i=0; i<longestIndexLength+4; i++) { printW(L"─"); }
            printW(L"┬");
            for (int i=0; i<maxItemLength+1; i++) { printW(L"─"); }
            printW(L"╢"); ENDL;
        } else {
            printW(L"╔");
            for (int i=0; i<longestIndexLength+4; i++) { printW(L"═"); }
            printW(L"╤");
            for (int i=0; i<maxItemLength+1; i++) { printW(L"═"); }
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
                for (int j=0; j<gaptoMax_num; j++) SPACE;
                // printf(" %*d ", longestIndexLength, i + 1);
                printW(L"│");
                if (localizedItemNames[i]!=NULL) {
                    printf(" %s ",localizedItemNames[i]);
                    gaptoMax=maxItemLength-1-strlen(localizedItemNames[i]);
                    for (int j=0; j<gaptoMax; j++) SPACE;
                    printW(L"║"); ENDL;
                } else ENDL;
            }
        }
        printW(L"╚");
        for (int i=0; i<longestIndexLength+4; i++) { printW(L"═"); }
        printW(L"╧");
        for (int i=0; i<maxItemLength+1; i++) { printW(L"═"); }
        printW(L"╝"); ENDL;
        ENDL;
        printf("Please select an option (0-%c): ",
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
        char choice=-1;
        do {
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


_Bool localizeNames(Char_Map CharMap1[],char* localizedNames[]) {
    _Bool result=0;
    for (int i=0,conved=0; i<charCount; i++) {
        if ((localizedNames[i]=localize(CharMap1[i].name_cn))==NULL) result=1;
    }
    return result;
    // result 返回 1 为未完成，返回 0 为成功完成。
}

void clearInputBuffer() {
    char current='\0';
    do {
        current=getchar();
    } while (current!='\n'&&current!=EOF&&current!='\0');
}

void freeLocalizedNames() {
    if (localizedNames!=NULL) {
        for (int i=0; i<charCount; i++) {
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
    if (current==NULL) puts("\nThis character have't been UP yet.");
    for (int i=0; current!=NULL; i++) {
        if (i%3==0) ENDL;
        printf("%hu.%hu.%hu\t",current->major,current->minor,current->half);
        current=current->next;
    }
    ENDL;
}
