#define CONMENUS
#ifndef GIPOOL
#include "gipool.h"
#endif
#ifndef LOCALIZE
#include "localize.h"
#endif
#ifndef CONFUNC
#include "confunc.h"
#endif
#ifndef CONUSAGE
#include "conusage.h"
#endif
const wchar_t* mainMenu[]={
        L"查询所有卡池",
        L"查询所有角色距离上次复刻已过天数",
        L"查询每一次单独角色卡池",
        L"退出"
};

const wchar_t* splitMainMenu[]={
    L"按照角色中文名有几个字筛选",
    L"按照角色英文名有几个字母筛选",
    L"按照角色神之眼类型筛选",
    L"直接输入角色编号（高级）",
    L"返回"
};

const wchar_t* splitByVisionType[]={
    L"风元素",
    L"岩元素",
    L"雷元素",
    L"草元素",
    L"水元素",
    L"火元素",
    L"冰元素",
    L"返回"
};

// int prevSlt = 0;

void _mainMenu();
int choiceOneCharacter();

void _mainMenu()
{
    static int prevSlt=0;
    while (1) {
    mainMenu:
        if (prevSlt==0||prevSlt==-1) {
            ENDL;
            prevSlt=choiceMenu(mainMenu,ARRAY_SIZE(mainMenu),L"原神祈愿卡池信息工具");
        }
        switch (prevSlt)
        {
            case 1:
            {
                prevSlt=0;
                printAllPools();
                ENDL;
                puts("The above is all wish pool info, press any key to go back.");
                break;
            }
            case 2:
            {
                prevSlt=0;
                printDaysofAllLimited5StarCharacters();
                ENDL;
                puts("The above is days of all limited 5 star characters, press any key to go back.");
                break;
            }
            case 3:
            {
                ENDL;
                do {
                    int choice=choiceOneCharacter();
                    if (choice==-1) { CLS;  prevSlt=0; goto mainMenu; } else {
                        buildPoolLinkList(choice,WishPool);
                        ENDL;
                        printf("%s: ",CharMap[choice].name);
                        printPoolLinkList(PoolLinkLists[choice]);
                        ENDL;
                        printf("The above is all %s wish pool info, press any key for another character.",CharMap[choice].name);
                        ENDL;
                        PAUSE;
                        CLS;
                        continue;
                    }
                } while (1);
            }
            case 0:
            default:
                goto exit_mainMenu;
        }
        PAUSE;
        CLS;
    }
exit_mainMenu:
    return;
}

int choiceOneCharacter() {
    static int prevSlt=0;
    do {
    choiceOneCharacter:
        if (prevSlt==0||prevSlt==-1)
        {
            ENDL;
            prevSlt=choiceMenu(splitMainMenu,ARRAY_SIZE(splitMainMenu),L"选择筛选角色的依据");
            ENDL;
        }
        switch (prevSlt) {
            case 1:
            {
                ENDL;
                printf("Please type how long the Chinese name is and press ENTER, type 0 for go back (0-20): ");
                int length=0;
                do {
                    GETNUM(length);
                    clearInputBuffer();
                    if (length==0) {
                        CLS;
                        prevSlt=0;
                        goto choiceOneCharacter;
                    } else if (length<=0||length>20) {
                        printf("Invalid choice. Type 0 to go back. (0-20): ");
                    } else { break; }
                } while (1);
                CLS;
                for (int i=0; i<charCount; i++) {
                    if (wcslen(CharMap[i].name_cn)==length) {
                        printf("%d\t%s\t%s",i,localizedNames[i],CharMap[i].name);
                        ENDL;
                    }
                }
                break;
            }
            case 2:
            {
                ENDL;
                printf("Please type how long the English name is and press ENTER, type 0 for go back (0-40): ");
                int length=0;
                do {
                    GETNUM(length);
                    clearInputBuffer();
                    if (length==0) {
                        CLS;
                        prevSlt=0;
                        goto choiceOneCharacter;
                    } else if (length<=0||length>40) {
                        printf("Invalid choice. Type 0 to go back. (0-40): ");
                    } else { break; }
                } while (1);
                CLS;
                for (int i=0; i<charCount; i++) {
                    if (strlen(CharMap[i].name)==length) {
                        printf("%d\t%s\t%s",i,CharMap[i].name,localizedNames[i]);
                        ENDL;
                    }
                }
                break;
            }
            case 3:
            {
                int visionSelection=VISION_UNKNOWN;
                ENDL;
                visionSelection=choiceMenu(splitByVisionType,ARRAY_SIZE(splitByVisionType),L"选择一个神之眼类型");
                switch (visionSelection)
                {
                    case 1:
                        visionSelection=ANEMO;
                        break;
                    case 2:
                        visionSelection=GEO;
                        break;
                    case 3:
                        visionSelection=ELECTRO;
                        break;
                    case 4:
                        visionSelection=DENDRO;
                        break;
                    case 5:
                        visionSelection=HYDRO;
                        break;
                    case 6:
                        visionSelection=PYRO;
                        break;
                    case 7:
                        visionSelection=CRYO;
                        break;
                    case 0:
                        CLS;
                        prevSlt=0;
                        continue;
                }
                CLS;
                for (int i=0; i<charCount; i++) {
                    if (CharMap[i].vision==visionSelection) {
                        printf("%d %s %s",i,localizedNames[i],CharMap[i].name);
                        ENDL;
                    }
                }

                break;
            }
            case 4:
            {
                break;
            }
            case 0:
            default:
                return -1;
        }
        int selection=-1;
        selection=choiceOneCharacter4Test();
        if (selection==-1) {
            // 只在 prevSlt==4 的时候重置选取
            // 因为只有是 4 的时候是直接调用 choiceOneCharacter4Test()
            // 其他时候都是先调取别的过滤逻辑再调用 choiceOneCharacter4Test()
            // 那么可以让那些对应选项带的前置逻辑来负责将 prevSlt 置 0
            if (prevSlt==4) prevSlt=0;
            continue;
        } else return selection;
        PAUSE;
        CLS;
        continue;
    } while (1);
}