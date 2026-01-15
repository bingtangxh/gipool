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
const wchar_t* mainMenu[] = {
        L"查询所有卡池",
        L"查询所有角色距离上次复刻已过天数",
        L"退出"
};

void _mainMenu()
{
    while (1) {
        switch (
            choiceMenu(mainMenu, ARRAY_SIZE(mainMenu), L"原神祈愿卡池信息工具")
            )
        {
        case 1:
        {
            printAllPools();
            ENDL;
            puts("The above is all wish pool info, press any key to go back.");
            break;
        }
        case 2:
        {
            printDaysofAllLimited5StarCharacters();
            ENDL;
            puts("The above is days of all limited 5 star characters, press any key to go back.");
            break;
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
