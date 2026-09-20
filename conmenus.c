#include "gipool.h"

size_t splitResultLength = 0;
char *splitResultCurrent=NULL;
char* splitResult=NULL;

#define CHINESE_SPLITER ((int)(wcslen(CharMap[i].name_cn) & 31) == length)\
&&\
(CharMap[i].attrib != ROLE_TYPE_EXCLUDED)\

#define ENGLISH_SPLITER ((int)(strlen(CharMap[i].name) & 63) == length)\
&&\
(CharMap[i].attrib != ROLE_TYPE_EXCLUDED)\

const wchar_t* mainMenuItems[]={
    L"查询所有卡池",
    L"查询所有角色距离上次复刻已过天数",
    L"查询每一次单独角色卡池",
    L"显示一个赛诺冷笑话",
    L"退出"
};


const wchar_t* splitMainMenu[]={
    L"按照角色中文名有几个字筛选",
    L"按照角色英文名有几个字母筛选",
    L"按照角色神之眼类型筛选",
    L"直接输入角色编号（高级）",
    L"返回"
};

const wchar_t* splitByVisionType[] = {
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
                size_t choicedIndex = id2Index((unsigned int)choice);
                if(buildPoolLinkList(choicedIndex,WishPool))
                {
                    // 来到这里
                    // 仅说明构建链表失败，不一定是内存申请失败，也有可能是查询的角色不是4星或5星UP角色
                    // 所以先继续运行
                }
                ENDL;
                SetConsoleColorByCharacter(CharMap[choicedIndex]);
                printf("%s: ",CharMap[choicedIndex].name);
                ResetConsoleColor();
                printPoolLinkList(PoolLinkLists[choicedIndex]);
                ENDL;
                printf(
#ifdef _WIN32
                    "The above is all %s wish pool info, press any key for another character.",
#else
                    "The above is all %s wish pool info, press ENTER for another character.",
#endif
                    CharMap[choicedIndex].name);
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
    int selection = -1;
    do {
        {
            if (prevSlt == 0 || prevSlt == -1) {
                ENDL;
                prevSlt = choiceMenu(splitMainMenu, (int)ARRAY_SIZE(splitMainMenu), L"选择筛选角色的依据");
                ENDL;
            }
            switch (prevSlt) {
            case 1:
            {
                selection = choiceOneCharacterUsingChineseNameLength();
                break;
            }
            case 2:
            {
                selection = choiceOneCharacterUsingEnglishNameLength();
                break;
            }
            case 3:
            {
                selection = choiceOneCharacterUsingVisionType();
                break;
            }
            case 4:
            {
                selection = choiceOneCharacter4Test();
                if (id2Index(selection) == SIZE_MAX) {
                    printf("Your choice does not correspond to any character. Please try again.");
                    ENDL;
                    continue;
                }
                break;
            }
            case 0:
            default:
                return -1;
            }
            // switch 语句当中的 break 会来到这里
            if (selection == -1) {
                CLS;
                prevSlt = 0;
                continue;
            }
            else if (selection == -2)
            {
                continue;
            }
            else
            {
                return selection;
            }
            
        }
    } while(1);
}

void printAllPools(void)
{
    CLS;
    for (size_t i = 0; i < poolCount; i++) {
        putPool(WishPool[i]);
    }
}

void printDaysofAllLimited5StarCharacters(void)
{
    CLS;
    for (size_t i = 0; i < charCount; i++) {
        size_t index = (size_t)arrangedInOrderOfDays[i];
        if ((daysPassedSinceLastUP[index] != INT_MIN) &&
            ((CharMap[index].attrib == 9) || (CharMap[index].attrib == 5))) {
            for (size_t j = 0; j < localizedVisualLen(CharMap[longestChineseIndex].name_cn) - localizedVisualLen(CharMap[index].name_cn); j++) { SPACE; }
            SetConsoleColorByCharacter(CharMap[index]);
            printf("%s", localizedNames[index]);
            ResetConsoleColor();
            printf(" | %d", daysPassedSinceLastUP[index]);
            ENDL;
        }
    }
}

int choiceOneCharacter4Test(void)
{
    int result = -1;
    ENDL;
    printf("Please type a char index number, type -1 to go back (-1-%u): ", (unsigned int)charCount - 1);
    do {
        result = readIntInRange(-1, (int)charCount - 1, NULL);
        if (result == -1) {
            CLS;
            break;
        }
        else if (result < 0 || (unsigned int)result > charCount - 1) {
            printf("Invalid choice. Type -1 to go back. (0-%u): ", (unsigned int)charCount - 1);
        }
        else {
            break;
        }
    } while (1);
    return result;
}

int choiceOneCharacterwithSpliterBefore(int list[], size_t length)
{
    int result = -1;
    do {
        result = choiceOneCharacter4Test();
        if (result == -1) {
            return result;
        }
        if (1)
        {
            for (size_t i = 0; i < length; i++) {
                if (list[i] == result) {
                    if (id2Index(result) == SIZE_MAX)
                    {
                        printf("Your choice does not correspond to any character. Please try again.");
                        ENDL;
                        continue;
                    }
                    return result;
                }
            }
            printf("Your choice is not in the query list. Please try again.");
            ENDL;
            continue;
        }

    } while (1);
}

int choiceOneCharacterUsingChineseNameLength(void) {
    int foundAny = 0, found = 0, currentIndex = 0, selection = -1;
    int* foundList = NULL;
typeChineseName:
    {
        ENDL;
        printf("Please type how long the Chinese name is and press ENTER, type -1 or 0 for go back (-1-%zu): ", longestChineseNameLength);
        int length = 0;
        do {
            length = readIntInRange(-1, (int)longestChineseNameLength, NULL);
            if (length == -1 || length == 0) {
                return -1;
            }
            else if (length<0 || length>(int)longestChineseNameLength) {
                printf("Invalid choice. Type -1 or 0 to go back. (-1-%zu): ", longestChineseNameLength);
            }
            else {
                break;
            }
        } while (1);
        CLS;
        foundAny = 0;
        found = 0;
        for (size_t i = 0; i < charCount; i++) {
            if (CHINESE_SPLITER) {
                foundAny = 1;
                found++;
                SetConsoleColorByCharacter(CharMap[i]);
                printf("%u\t%s\t%s", index2Id(i), localizedNames[i], CharMap[i].name);
                ResetConsoleColor();
                ENDL;
            }
        }
        if (!foundAny) {
            printf("No character found with Chinese name length %d. Please try again.\n", length);
            goto typeChineseName;
        }
        foundList = (int*)malloc(sizeof(int) * (found + 1));
        RETURN_IF_NULL(foundList, -2,-1);
        // 这里只是筛选失败，应该返回重新询问角色名长度，不退出
        currentIndex = 0;
        for (size_t i = 0; i < charCount; i++) {
            if (CHINESE_SPLITER) {
                EXIT_IF_NULL(foundList, index2Id(i));
                foundList[currentIndex++] = index2Id(i);
            }
        }
        selection = choiceOneCharacterwithSpliterBefore(foundList, (size_t)found);
        if (selection == -1) {
            free(foundList);
            foundList = NULL;
            return -2;
        }
        else {
            free(foundList);
            foundList = NULL;
            return selection;
        }
    }
}

int choiceOneCharacterUsingEnglishNameLength(void) {
    int foundAny = 0, found = 0, currentIndex = 0, selection = -1;
    int* foundList = NULL;
typeEnglishName:
    {
        ENDL;
        printf("Please type how long the English name is and press ENTER, type -1 or 0 for go back (-1-%zu): ", longestEnglishNameLength);
        int length = 0;
        do {
            length = readIntInRange(-1, (int)longestEnglishNameLength, NULL);
            if (length == -1 || length == 0) {
                return -1;
            }
            else if (length<0 || length>(int)longestEnglishNameLength) {
                printf("Invalid choice. Type -1 or 0 to go back. (-1-%zu): ", longestEnglishNameLength);
            }
            else {
                break;
            }
        } while (1);

        CLS;
        foundAny = 0;
        found = 0;
        for (size_t i = 0; i < charCount; i++) {
            if (ENGLISH_SPLITER) {
                foundAny = 1;
                found++;
                SetConsoleColorByCharacter(CharMap[i]);
                printf("%u\t%s\t%s", index2Id(i), CharMap[i].name, localizedNames[i]);
                ResetConsoleColor();
                ENDL;
            }
        }
        if (!foundAny) {
            printf("No character found with English name length %d. Please try again.\n", length);
            goto typeEnglishName;
        }
        foundList = (int*)malloc(sizeof(int) * (found + 1));
        RETURN_IF_NULL(foundList, -2,-1);
        currentIndex = 0;
        for (size_t i = 0; i < charCount; i++) {
            if (ENGLISH_SPLITER) {
                if (foundList == NULL)
                {
                    puts("Unexpected null pointer foundList.");
                    exit(1);
                }
                else foundList[currentIndex++] = index2Id(i);
            }
        }
        selection = choiceOneCharacterwithSpliterBefore(foundList, (size_t)found);
        if (selection == -1) {
            free(foundList);
            foundList = NULL;
            return -2;
        }
        else {
            free(foundList);
            foundList = NULL;
            return selection;
        }
    }
}

int choiceOneCharacterUsingVisionType(void) {
    size_t found = 0, currentIndex = 0;
    int selection = -1;
    int* foundList = NULL;

    splitResultLength = getSplitResultExpectedLength();
    splitResult = (char*)malloc(sizeof(char) * (splitResultLength + 1));
    RETURN_IF_NULL(splitResult, -2,-1);
    int visionSelection = VISION_UNKNOWN;
    ENDL;
    visionSelection = choiceMenu(splitByVisionType, (int)ARRAY_SIZE(splitByVisionType), L"选择一个神之眼类型");
    int visionUserChoice = visionSelection;
    switch (visionSelection) {
    case 1: visionSelection = ANEMO; break;
    case 2: visionSelection = GEO; break;
    case 3: visionSelection = ELECTRO; break;
    case 4: visionSelection = DENDRO; break;
    case 5: visionSelection = HYDRO; break;
    case 6: visionSelection = PYRO; break;
    case 7: visionSelection = CRYO; break;
    case 0:
        return -1;
    }
    CLS;
    SetConsoleColorByVision((uint8_t)visionSelection);
    putws(splitByVisionType[visionUserChoice - 1]);
    ResetConsoleColor();
    ENDL;
    found = 0;
    splitResultCurrent = splitResult;
    for (size_t i = 0; i < charCount; i++) {
        if (CharMap[i].vision == visionSelection) {
            // 将上面的判断条件后面加个 ||1 用来测试字符串内存空间申请的够不够长
            found++;
            splitResultCurrent+=
#ifdef _MSC_VER
                sprintf_s(splitResultCurrent, splitResultLength-(splitResultCurrent - splitResult), "%3d | ", index2Id(i));
#else
                sprintf(splitResultCurrent,"%3d | ", index2Id(i));
#endif
            for (size_t j = 0; j < localizedVisualLen(CharMap[longestChineseIndex].name_cn) - localizedVisualLen(CharMap[i].name_cn); j++) { 
                splitResultCurrent +=
#ifdef _MSC_VER
                    sprintf_s(splitResultCurrent, splitResultLength - (splitResultCurrent - splitResult), " ");
#else
                    sprintf(splitResultCurrent, " ");
#endif
            }
            splitResultCurrent+=
#ifdef _MSC_VER
                sprintf_s(splitResultCurrent, splitResultLength - (splitResultCurrent - splitResult), "%s | %s", localizedNames[i], CharMap[i].name);
#else
                sprintf(splitResultCurrent,"%s | %s", localizedNames[i], CharMap[i].name);
#endif
            splitResultCurrent+=
#ifdef _MSC_VER
            sprintf_s(splitResultCurrent, splitResultLength - (splitResultCurrent - splitResult), "\n");
#else
            sprintf(splitResultCurrent, "\n");
#endif
        }
    }
    puts(splitResult);
    foundList = (int*)malloc(sizeof(int) * (found + 0));
    RETURN_IF_NULL(foundList, -2,-1);
    currentIndex = 0;
    for (size_t i = 0; i < charCount; i++) {
        if (CharMap[i].vision == visionSelection) {
            EXIT_IF_NULL(foundList, index2Id(i));
            if (currentIndex>=found) {
                puts("Unexpected index out of bounds in foundList.");
                exit(1);
            } else foundList[currentIndex++] = index2Id(i);
        }
    }
    printf("Found %zu characters.", found);
    ENDL;
    selection = choiceOneCharacterwithSpliterBefore(foundList, (size_t)found);
    if (selection == -1) {
        free(foundList);
        foundList = NULL;
        return -2;
    }
    else {
        free(foundList);
        foundList = NULL;
        return selection;
    }
}
