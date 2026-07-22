#pragma once

#ifndef GIPOOL_H
#define GIPOOL_H

#ifndef _WIN32
#define _XOPEN_SOURCE 700
#endif

#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

#define DATE_LENGTH 12
#define ARRAY_SIZE(arr) (sizeof(arr) == 0 ? 0 : sizeof(arr) / sizeof((arr)[0]))

#ifdef _WIN32
#define CLS system("cls")
#define ENDL putchar('\n')
#define putws(wstr) do { printW(wstr); putwchar(L'\n'); } while(0)
#ifdef _MSC_VER
#define PAUSE ending = _getch()
#define getch() _getch()
#define getche() _getche()
#else
#define PAUSE ending = getch()
#endif
#else
#define CLS system("clear")
#define ENDL putchar('\n')
#define putws(wstr) do {printW(wstr); putwchar(L'\r'); putwchar(L'\n'); }while(0)
#define getche() \
    do { \
        ending = getchar(); \
        if (ending != EOF && ending != '\0' && ending != '\n') { clearInputBuffer(); } \
    } while(0)
#define getch() \
    do { \
    ending = getchar(); \
    if (ending != EOF && ending != '\0' && ending != '\n') { clearInputBuffer(); } \
    } while(0)
#define PAUSE \
    do { \
    ending = getchar(); \
    if (ending != EOF && ending != '\0' && ending != '\n') { clearInputBuffer(); } \
    } while(0)
#endif

#define SPACE putchar(' ')

#ifdef _MSC_VER
#define GETNUM(num) do { scanf_s("%d", &(num)); clearInputBuffer(); } while(0)
#else
#define GETNUM(num) do { scanf("%d", &(num)); clearInputBuffer(); } while(0)
#endif

typedef uint32_t RoleMeta;

typedef enum vision {
    VISION_OTHER,
    PYRO,
    HYDRO,
    ANEMO,
    ELECTRO,
    DENDRO,
    CRYO,
    GEO,
    VISION_UNKNOWN
} Vision;

typedef enum roleType {
    ROLE_TYPE_TRAVELER_AETHER,
    ROLE_TYPE_TRAVELER_LUMINE,
    ROLE_TYPE_COLLAB,
    ROLE_TYPE_FOUR_STAR=4,
    ROLE_TYPE_LIMITED_FIVE_STAR,
    ROLE_TYPE_UNKNOWN,
    ROLE_TYPE_EXCLUDED=256
} RoleType;

#define ENCODE_POOL_VERSION_FOR_PERMANENT_5_STAR(major, minor, half) \
    (((major) << 6) | (((minor) & 0xF) << 2) | ((half) & 0x3))

#define MAKE_ROLE_META_P(major, minor, half) \
    ((ENCODE_POOL_VERSION_FOR_PERMANENT_5_STAR((major), (minor), (half)) << 3) | 3)

#define GET_ROLE_TYPE(meta) ((meta) & 0x7)
#define GET_MAJOR_P(meta) (((meta) >> 9) & 0xFF)
#define GET_MINOR_P(meta) (((meta) >> 5) & 0x0F)
#define GET_HALF_P(meta) (((meta) >> 3) & 0x03)

#define ENCODE_POOL_INFO_LL(major, minor, half) \
    (((major) << 24) | ((minor) << 16) | ((half) << 8))

#define GET_MAJOR_LL(meta) (((meta) >> 24) & 0xFF)
#define GET_MINOR_LL(meta) (((meta) >> 16) & 0xFF)
#define GET_HALF_LL(meta) (((meta) >> 8) & 0xFF)

typedef struct characterMap {
    const unsigned int id;
    const wchar_t name_cn[20];
    const char name[40];
    const uint8_t vision;
    const uint32_t attrib;
} CharMapType;

typedef struct weaponMap {
    const unsigned int id;
    const wchar_t name_cn[20];
    const char name[40];
    const unsigned int stars;
    const uint8_t type;
} WeaponMap;

typedef struct wishPool {
    unsigned int up5[24];
    unsigned int up4[24];
    unsigned int weapon[24];
    uint8_t major;
    uint8_t minor;
    uint8_t half;
    uint16_t startY;
    uint8_t startM;
    uint8_t startD;
    uint16_t endY;
    uint8_t endM;
    uint8_t endD;
} WishPoolType;

typedef struct _poolNode {
    uint8_t major;
    uint8_t minor;
    uint8_t half;
    struct _poolNode* next;
} _PoolNode,*PoolLinkList,**PoolLinkListArray;

extern CharMapType CharMap[];
extern WishPoolType WishPool[];

void getCharandPoolCount(void);

extern PoolLinkList* PoolLinkLists;
extern size_t charCount;
extern size_t poolCount;
extern size_t longestChineseIndex;
extern size_t longestChineseNameLength;
extern size_t longestEnglishIndex;
extern size_t longestEnglishNameLength;
extern int ending;
extern int* daysPassedSinceLastUP;
extern int* arrangedInOrderOfDays;
extern char** localizedNames;
extern const int maxCharactersInCharPool;

extern const wchar_t singleEdge[];
extern const wchar_t doubleEdge[];
extern const char splitLine[];

#ifdef _WIN32
extern CONSOLE_SCREEN_BUFFER_INFO original;
extern WORD visionColor[];
#else
extern uint8_t visionColor[];
#endif

_Bool convertCompileTime(char* date);
_Bool buildPoolLinkList(size_t index,WishPoolType WishPools[]);
void help(void);

void initConsole(void);
void printCompileTime(void);
int typeMenu(const wchar_t* menuItems[],int itemCount,const wchar_t* title);
int choiceMenu(const wchar_t* menuItems[],int itemCount,const wchar_t* title);
_Bool localizeNamesArray(CharMapType CharMap1[],char* targetLocalizedNames[]);
void freeLocalizedNames(void);
void printPoolLinkList(PoolLinkList current);
int readIntInRange(int min,int max,const int* defaultValue);
_Bool SetConsoleColorByCharacter(CharMapType character);
_Bool ResetConsoleColor(void);

void initDynamicThings(void);
int checkIntegrity(void);
void freeDynamicThings(void);

void putPool(WishPoolType WishPool1);

#ifdef _WIN32
DWORD printW(const wchar_t* wstr);
#else
int printW(const wchar_t* wstr);
#endif

size_t localizedMemLen(const wchar_t* source);
size_t localizedVisualLen(const wchar_t* source);
char* localize(const wchar_t* source);

void printAllPools(void);
void printDaysofAllLimited5StarCharacters(void);
int choiceOneCharacter4Test(void);

void mainMenu(void);

int cynoJoke(void);

#endif

