#include "gipool.h"

static const wchar_t* mainMenuItems[]={
    L"查询所有卡池",
    L"查询所有角色距离上次复刻已过天数",
    L"查询每一次单独角色卡池",
    L"显示一个赛诺冷笑话",
    L"退出"
};

static const wchar_t* splitMainMenu[]={
    L"按照角色中文名有几个字筛选",
    L"按照角色英文名有几个字母筛选",
    L"按照角色神之眼类型筛选",
    L"直接输入角色编号（高级）",
    L"返回"
};

static const wchar_t* splitByVisionType[]={
    L"风元素",
    L"岩元素",
    L"雷元素",
    L"草元素",
    L"水元素",
    L"火元素",
    L"冰元素",
    L"返回"
};

void mainMenu(void)
{
    static int prevSlt=0;
    while(1) {
    main_menu:
        if(prevSlt==0||prevSlt==-1) {
            ENDL;
            prevSlt=choiceMenu(mainMenuItems,(int)ARRAY_SIZE(mainMenuItems),L"原神祈愿卡池信息工具");
        }
        switch(prevSlt) {
        case 1:
            prevSlt=0;
            printAllPools();
            ENDL;
#ifdef _WIN32
            puts("The above is all wish pool info, press any key to go back.");
#else
            puts("The above is all wish pool info, press ENTER to go back.");
#endif
            PAUSE;
            break;
        case 2:
            prevSlt=0;
            printDaysofAllLimited5StarCharacters();
            ENDL;
#ifdef _WIN32
            puts("The above is days of all limited 5 star characters, press any key to go back.");
#else
            puts("The above is days of all limited 5 star characters, press ENTER to go back.");
#endif
            PAUSE;
            break;
        case 3:
            ENDL;
            do {
                int choice=choiceOneCharacter();
                if(choice==-1) {
                    CLS;
                    prevSlt=0;
                    goto main_menu;
                }
                buildPoolLinkList((size_t)choice,WishPool);
                ENDL;
                printf("%s: ",CharMap[choice].name);
                printPoolLinkList(PoolLinkLists[choice]);
                ENDL;
                printf(
#ifdef _WIN32
                    "The above is all %s wish pool info, press any key for another character.",
#else
                    "The above is all %s wish pool info, press ENTER for another character.",
#endif
                    CharMap[choice].name);
                ENDL;
                PAUSE;
                CLS;
            } while(1);
        case 4:
            prevSlt=0;
            ENDL;
            cynoJoke();
            break;
        case 0:
        default:
            return;
        }
        CLS;
    }
}

int choiceOneCharacter(void)
{
    static int prevSlt=0;
    do {
    choice_one_character:
        if(prevSlt==0||prevSlt==-1) {
            ENDL;
            prevSlt=choiceMenu(splitMainMenu,(int)ARRAY_SIZE(splitMainMenu),L"选择筛选角色的依据");
            ENDL;
        }
        switch(prevSlt) {
        case 1:
        {
            ENDL;
            printf("Please type how long the Chinese name is and press ENTER, type 0 for go back (0-20): ");
            int length=0;
            do {
                length=readIntInRange(0,20,NULL);
                if(length==0) {
                    CLS;
                    prevSlt=0;
                    goto choice_one_character;
                }
                else if(length<=0||length>20) {
                    printf("Invalid choice. Type 0 to go back. (0-20): ");
                }
                else {
                    break;
                }
            } while(1);
            CLS;
            for(size_t i=0; i<charCount; i++) {
                if((int)(wcslen(CharMap[i].name_cn)&31)==length) {
                    SetConsoleColorByCharacter(CharMap[i]);
                    printf("%u\t%s\t%s",(unsigned int)i,localizedNames[i],CharMap[i].name);
                    ResetConsoleColor();
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
                length=readIntInRange(0,40,NULL);
                if(length==0) {
                    CLS;
                    prevSlt=0;
                    goto choice_one_character;
                }
                else if(length<=0||length>40) {
                    printf("Invalid choice. Type 0 to go back. (0-40): ");
                }
                else {
                    break;
                }
            } while(1);
            CLS;
            for(size_t i=0; i<charCount; i++) {
                if((int)(strlen(CharMap[i].name)&63)==length) {
                    SetConsoleColorByCharacter(CharMap[i]);
                    printf("%u\t%s\t%s",(unsigned int)i,CharMap[i].name,localizedNames[i]);
                    ResetConsoleColor();
                    ENDL;
                }
            }
            break;
        }
        case 3:
        {
            int visionSelection=VISION_UNKNOWN;
            ENDL;
            visionSelection=choiceMenu(splitByVisionType,(int)ARRAY_SIZE(splitByVisionType),L"选择一个神之眼类型");
            switch(visionSelection) {
            case 1: visionSelection=ANEMO; break;
            case 2: visionSelection=GEO; break;
            case 3: visionSelection=ELECTRO; break;
            case 4: visionSelection=DENDRO; break;
            case 5: visionSelection=HYDRO; break;
            case 6: visionSelection=PYRO; break;
            case 7: visionSelection=CRYO; break;
            case 0:
                CLS;
                prevSlt=0;
                continue;
            }
            CLS;
            for(size_t i=0; i<charCount; i++) {
                if(CharMap[i].vision==visionSelection) {
                    printf("%3zu | ",i);
                    for(size_t j=0; j<localizedVisualLen(CharMap[longestChineseIndex].name_cn)-localizedVisualLen(CharMap[i].name_cn); j++) { SPACE; }
                    printf("%s | %s",localizedNames[i],CharMap[i].name);
                    ENDL;
                }
            }
            break;
        }
        case 4:
            break;
        case 0:
        default:
            return -1;
        }
        {
            int selection=choiceOneCharacter4Test();
            if(selection==-1) {
                if(prevSlt==4) prevSlt=0;
                continue;
            }
            return selection;
        }
    } while(1);
}
