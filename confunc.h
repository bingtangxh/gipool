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
const int choiceMenu(const wchar_t* menuItems[], int itemCount, const wchar_t* title);
_Bool localizeNames(Char_Map CharMap1[], char* localizedNames[]);
void clearInputBuffer();
void freeLocalizedNames();

void initConsole() {
#ifdef _WIN32
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);
#else
    setlocale(LC_CTYPE, "");
#endif

}

void putPool(Wish_Pool WishPool1)
{
    int fiveCount = sizeof(WishPool1.up5) == 0 ? 0 : (int)(sizeof(WishPool1.up5) / sizeof(WishPool1.up5[0]));
    int fourCount = sizeof(WishPool1.up4) == 0 ? 0 : (int)(sizeof(WishPool1.up4) / sizeof(WishPool1.up4[0]));
    printf("%hu.%hu.%hu\t%u.%hu.%hu\t%u.%hu.%hu\t", WishPool1.major, WishPool1.minor, WishPool1.half, WishPool1.startY, WishPool1.startM, WishPool1.startD, WishPool1.endY, WishPool1.endM, WishPool1.endD);
    for (int i = 0; i < fiveCount && WishPool1.up5[i] != 0; i++)
    {
        //printW(CharMap[WishPool1.up5[i]].name_cn);
        printf("%s ", localizedNames[WishPool1.up5[i]]==NULL?"":localizedNames[WishPool1.up5[i]]);
    }
    for (int i = 0; i < fourCount && WishPool1.up4[i] != 0; i++)
    {
        //printW(CharMap[WishPool1.up4[i]].name_cn);
        printf("%s ", localizedNames[WishPool1.up4[i]]==NULL?"":localizedNames[WishPool1.up4[i]]);
    }
    putchar('\n');
}

void printCompileTime() {
    char date[DATE_LENGTH] = __DATE__;
    convertCompileTime(date);
    printf("Compiled at %s %s\n", date, __TIME__);
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
                if ((localizedItemNames[i] = localize(menuItems[i])) == NULL) localizedItemNames[i] = "";
            }
        }

        char* localizedTitle = localize(title);
        if (localizedTitle != NULL) {
            if (strlen(localizedTitle) - 4 > maxItemLength) {
                maxItemLength = strlen(localizedTitle) - 4;
            }
            titleLineSpaces = maxItemLength + 4 - strlen(localizedTitle) + longestIndexLength;
            printW(L"╔");
            for (int i = 0; i < longestIndexLength + maxItemLength + 6; i++) { printW(L"═"); }
            printW(L"╗"); ENDL;

            printW(L"║"); SPACE;
            for (int i = 0; i < titleLineSpaces / 2 + titleLineSpaces % 2; i++) { SPACE; }
            printf("%s", localizedTitle);
            for (int i = 0; i < titleLineSpaces / 2; i++) { SPACE; }
            SPACE;
            printW(L"║"); ENDL;
            printW(L"╟");
            for (int i = 0; i < longestIndexLength + 4; i++) { printW(L"─"); }
            printW(L"┬");
            for (int i = 0; i < maxItemLength + 1; i++) { printW(L"─"); }
            printW(L"╢"); ENDL;
        }
        else {
            printW(L"╔");
            for (int i = 0; i < longestIndexLength + 4; i++) { printW(L"═"); }
            printW(L"╤");
            for (int i = 0; i < maxItemLength + 1; i++) { printW(L"═"); }
            printW(L"╗"); ENDL;
        }
        for (int i = 0,currentIndexColLen=0; i < itemCount; i++) {
            printW(L"║");
            currentIndexColLen = printf(" [%d] ", i == itemCount - 1 ? 0 : i + 1);
            currentIndexColLen = currentIndexColLen < 0 ? 0 : currentIndexColLen;
            gaptoMax_num = longestIndexLength - currentIndexColLen +4;
            for (int j = 0; j < gaptoMax_num; j++) SPACE;
            // printf(" %*d ", longestIndexLength, i + 1);
            printW(L"│");
            if (localizedItemNames[i] != NULL) {
                printf(" %s ", localizedItemNames[i]);
                gaptoMax = maxItemLength - 1 - strlen(localizedItemNames[i]);
                for (int j = 0; j < gaptoMax; j++) SPACE;
                printW(L"║"); ENDL;
            }
            else ENDL;
        }
        printW(L"╚");
        for (int i = 0; i < longestIndexLength + 4; i++) { printW(L"═"); }
        printW(L"╧");
        for (int i = 0; i < maxItemLength + 1; i++) { printW(L"═"); }
        printW(L"╝"); ENDL;
        ENDL;
        printf("Please select an option (0-%d) and press ENTER: ", itemCount-1);
        for (int i = 0; i < itemCount; i++) {
            free(localizedItemNames[i]);
            localizedItemNames[i] = NULL;
        }
        free(localizedItemNames);
        localizedItemNames = NULL;
        free(localizedTitle);
        localizedTitle = NULL;
        int choice = INT_MIN;
        do {
            GETNUM(choice);
            clearInputBuffer();
            if (choice == -1) {
                CLS;
                goto redisplayMenu;
            }
            else if (choice < 0 || choice > itemCount-1) {
                printf("Invalid choice. Type -1 to redisplay the menu. (0-%d): ", itemCount-1);
            }
            else { break; }
        } while (1);
        return choice;
    }
}

_Bool localizeNames(Char_Map CharMap1[], char* localizedNames[]) {
    _Bool result = 0;
    for (int i = 0, conved = 0; i < charCount; i++) {        
        if ((localizedNames[i] = localize(CharMap1[i].name_cn)) ==NULL) result = 1;
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