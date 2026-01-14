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
            for (int i = 0; i < poolCount; i++) {
                putPool(WishPool[i]);
            }
            ENDL;
            puts("The above is all wish pool info, press any key to go back.");
            break;
        }
        case 2:
        {
            for (int i = 0; i < charCount; i++) {
                if ((
                    daysPassedSinceLastUP[arrangedInOrderOfDays[i]] != INT_MIN
                    ) && ((
                        CharMap[arrangedInOrderOfDays[i]].attrib == 9
                        ) || (
                            CharMap[arrangedInOrderOfDays[i]].attrib == 5
                            ))) {
                    printf("%12s\t%d\n", localizedNames[arrangedInOrderOfDays[i]], daysPassedSinceLastUP[arrangedInOrderOfDays[i]]);
                }
                else {
                    // printf("%12s\t%u\n",localizedNames[arrangedInOrderOfDays[i]],CharMap[arrangedInOrderOfDays[i]].attrib);
                }
            }
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
