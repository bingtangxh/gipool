#include "gipool.h"

#define MAX_CHOICE_MENU_ITEMS 35 // 1~9,A~Z

#ifndef _WIN32

int getche_my(void) {
    int ending=getchar();
    if(ending!=EOF&&ending!='\0'&&ending!='\n') { clearInputBuffer(); }
    return ending;
}

int getch_my(void) {
    int ending=getchar();
    if(ending!=EOF&&ending!='\0'&&ending!='\n') { clearInputBuffer(); }
    return ending;
}

#endif // _WIN32

void initConsole(void)
{
#ifndef _WIN32
    setlocale(LC_CTYPE,"");
#endif
}

void putPool(const WishPoolType WishPool1)
{
    size_t fiveCount=ARRAY_SIZE(WishPool1.up5);
    size_t fourCount=ARRAY_SIZE(WishPool1.up4);

    if(WishPool1.half>=10) {
        puts(splitLine);
    }

    for(size_t i=0; i<fiveCount; i++) {
        if(WishPool1.up5[i]==0) {
            fiveCount=i;
            break;
        }
    }

    printf("%hu.%hu.%hu\t%u.%hu.%hu\t%u.%hu.%hu\t | ",
        WishPool1.major,WishPool1.minor,WishPool1.half,
        WishPool1.startY,WishPool1.startM,WishPool1.startD,
        WishPool1.endY,WishPool1.endM,WishPool1.endD);

    for(size_t i=0; i<2-fiveCount&&WishPool1.half<10; i++) {
        for(size_t j=0; j<=localizedVisualLen(CharMap[longestChineseIndex].name_cn); j++) {
            SPACE;
        }
        printf("| ");
    }

    for(size_t i=0; i<fiveCount&&WishPool1.up5[i]!=0; i++) {
        size_t currentIndex = id2Index(WishPool1.up5[i]);
        for(size_t j=0;
            j<localizedVisualLen(CharMap[longestChineseIndex].name_cn)-localizedVisualLen(CharMap[currentIndex].name_cn)&&
            WishPool1.half<10;
            j++) {
            SPACE;
        }
        SetConsoleColorByCharacter(CharMap[currentIndex]);
        printf("%s ",localizedNames[currentIndex]==NULL ? "" : localizedNames[currentIndex]);
        ResetConsoleColor();
        if(WishPool1.half<10) {
            printf("| ");
        }
    }

    for(size_t i=0; i<fourCount&&WishPool1.up4[i]!=0; i++) {
        size_t currentIndex = id2Index(WishPool1.up4[i]);
        SetConsoleColorByCharacter(CharMap[currentIndex]);
        printf("%s ",localizedNames[currentIndex]==NULL ? "" : localizedNames[currentIndex]);
        ResetConsoleColor();

        for(size_t j=0; j<localizedVisualLen(CharMap[longestChineseIndex].name_cn)-
            localizedVisualLen(CharMap[currentIndex].name_cn);
            j++) {
            SPACE;
        }
    }
    ENDL;

    if(WishPool1.half>=10) {
        puts(splitLine);
    }
}

void printCompileTime(void)
{
    char date[DATE_LENGTH]=__DATE__;
    convertCompileTime(date);
    printf("Compiled at %s %s",date,__TIME__);
    ENDL;
}

int typeMenu(const wchar_t* menuItems[],int itemCount,const wchar_t* title)
{
    do {
        size_t currentExpectedLength=0,maxItemLength=0,gaptoMax=0,gaptoMax_num=0,longestIndexLength=0,titleLineSpaces=0;
        char** localizedItemNames=NULL;

        for(int temp=itemCount; temp>0; temp/=10) {
            longestIndexLength++;
        }
        localizedItemNames=(char**)malloc(itemCount*sizeof(char*));
        EXIT_IF_NULL(localizedItemNames, -1);
        for(int i=0; i<itemCount; i++) {
            if((currentExpectedLength=localizedVisualLen(menuItems[i]))>maxItemLength) {
                maxItemLength=currentExpectedLength;
            }
        }
        if(localizedItemNames!=NULL) {
            for(int i=0; i<itemCount; i++) {
                localizedItemNames[i]=localize(menuItems[i]);
            }
        }

        char* localizedTitle=localize(title);
        if(localizedTitle!=NULL) {
            if((localizedVisualLen(title)>=4)&&localizedVisualLen(title)-4>maxItemLength) {
                maxItemLength=localizedVisualLen(title)-4;
            }
            titleLineSpaces=maxItemLength+5-localizedVisualLen(title)+longestIndexLength;
            printW(L"╔");
            for(size_t i=0; i<longestIndexLength+maxItemLength+7; i++) { printW(doubleEdge); }
            printW(L"╗"); ENDL;

            printW(L"║"); SPACE;
            for(size_t i=0; i<titleLineSpaces/2+titleLineSpaces%2; i++) { SPACE; }
            printf("%s",localizedTitle);
            for(size_t i=0; i<titleLineSpaces/2; i++) { SPACE; }
            SPACE;
            printW(L"║"); ENDL;
            printW(L"╟");
            for(size_t i=0; i<longestIndexLength+4; i++) { printW(singleEdge); }
            printW(L"┬");
            for(size_t i=0; i<maxItemLength+2; i++) { printW(singleEdge); }
            printW(L"╢"); ENDL;
        }
        else {
            printW(L"╔");
            for(size_t i=0; i<longestIndexLength+4; i++) { printW(doubleEdge); }
            printW(L"╤");
            for(size_t i=0; i<maxItemLength+2; i++) { printW(doubleEdge); }
            printW(L"╗"); ENDL;
        }
        for(int i=0,currentIndexColLen=0; i<itemCount; i++) {
            printW(L"║");
            currentIndexColLen=printf(" [%d] ",i==itemCount-1 ? 0 : i+1);
            currentIndexColLen=currentIndexColLen<0 ? 0 : currentIndexColLen;
            gaptoMax_num=longestIndexLength-(size_t)currentIndexColLen+4;
            for(size_t j=0; j<gaptoMax_num; j++) SPACE;
            printW(L"│");
            if(localizedItemNames[i]!=NULL) {
                printf(" %s ",localizedItemNames[i]);
                gaptoMax=maxItemLength-localizedVisualLen(menuItems[i]);
                for(size_t j=0; j<gaptoMax; j++) SPACE;
                printW(L"║"); ENDL;
            }
            else ENDL;
        }
        printW(L"╚");
        for(size_t i=0; i<longestIndexLength+4; i++) { printW(doubleEdge); }
        printW(L"╧");
        for(size_t i=0; i<maxItemLength+2; i++) { printW(doubleEdge); }
        printW(L"╝"); ENDL;
        ENDL;
        printf("Please select an option (0-%d) and press ENTER: ",itemCount-1);
        for(int i=0; i<itemCount; i++) {
            free(localizedItemNames[i]);
        }
        free(localizedItemNames);
        free(localizedTitle);
        do {
            int choice=readIntInRange(-1,itemCount-1,NULL);
            if(choice==-1) {
                CLS;
                break;
            }
            else if(choice < 0||choice > itemCount-1) {
                printf("Invalid choice. Type -1 to redisplay the menu. (0-%d): ",itemCount-1);
            }
            else {
                return choice;
            }
        } while(1);
    } while(1);
}

int choiceMenu(const wchar_t* menuItems[],int itemCount,const wchar_t* title)
{
    do {
        size_t currentExpectedLength=0,maxItemLength=0,gaptoMax=0,gaptoMax_num=0,longestIndexLength=1,titleLineSpaces=0;
        char** localizedItemNames=(char**)malloc(itemCount*sizeof(char*));
        EXIT_IF_NULL(localizedItemNames, -1);
        for(int i=0; i<itemCount; i++) {
            if((currentExpectedLength=localizedVisualLen(menuItems[i]))>maxItemLength) {
                maxItemLength=currentExpectedLength;
            }
        }
        if(localizedItemNames!=NULL) {
            for(int i=0; i<itemCount; i++) {
                localizedItemNames[i]=localize(menuItems[i]);
            }
        }

        char* localizedTitle=localize(title);
        if(localizedTitle!=NULL) {
            if((localizedVisualLen(title)>=4)&&localizedVisualLen(title)-4>maxItemLength) {
                maxItemLength=localizedVisualLen(title)-4;
            }
            titleLineSpaces=maxItemLength+5-localizedVisualLen(title)+longestIndexLength;
            printW(L"╔");
            for(size_t i=0; i<longestIndexLength+maxItemLength+7; i++) { printW(doubleEdge); }
            printW(L"╗"); ENDL;

            printW(L"║"); SPACE;
            for(size_t i=0; i<titleLineSpaces/2+titleLineSpaces%2; i++) { SPACE; }
            printf("%s",localizedTitle);
            for(size_t i=0; i<titleLineSpaces/2; i++) { SPACE; }
            SPACE;
            printW(L"║"); ENDL;
            printW(L"╟");
            for(size_t i=0; i<longestIndexLength+4; i++) { printW(singleEdge); }
            printW(L"┬");
            for(size_t i=0; i<maxItemLength+2; i++) { printW(singleEdge); }
            printW(L"╢"); ENDL;
        }
        else {
            printW(L"╔");
            for(size_t i=0; i<longestIndexLength+4; i++) { printW(doubleEdge); }
            printW(L"╤");
            for(size_t i=0; i<maxItemLength+2; i++) { printW(doubleEdge); }
            printW(L"╗"); ENDL;
        }
        for(int i=0,currentIndexColLen=0; i<itemCount; i++) {
            if(i>=MAX_CHOICE_MENU_ITEMS&&i!=itemCount-1) {
                continue;
            }
            printW(L"║");
            if(i<9) {
                currentIndexColLen=printf(" [%d] ",i==itemCount-1 ? 0 : i+1);
            }
            else if(i>=9&&i<itemCount-1&&i<MAX_CHOICE_MENU_ITEMS) {
                currentIndexColLen=printf(" [%c] ",'A'+i-9);
            }
            else if(i==itemCount-1) {
                currentIndexColLen=printf(" [%d] ",0);
            }
            currentIndexColLen=currentIndexColLen<0 ? 0 : currentIndexColLen;
            gaptoMax_num=longestIndexLength-(size_t)currentIndexColLen+4;
            for(size_t j=0; j<gaptoMax_num; j++) SPACE;
            printW(L"│");
            if(localizedItemNames[i]!=NULL) {
                printf(" %s ",localizedItemNames[i]);
                gaptoMax=maxItemLength-localizedVisualLen(menuItems[i]);
                for(size_t j=0; j<gaptoMax; j++) SPACE;
                printW(L"║"); ENDL;
            }
            else ENDL;
        }
        printW(L"╚");
        for(size_t i=0; i<longestIndexLength+4; i++) { printW(doubleEdge); }
        printW(L"╧");
        for(size_t i=0; i<maxItemLength+2; i++) { printW(doubleEdge); }
        printW(L"╝"); ENDL;
        ENDL;
        printf(
#ifdef _WIN32
            "Please select an option (0-%c): ",
#else
            "Please select an option and press ENTER (0-%c): ",
#endif
            itemCount<=10 ? itemCount-1+'0' : itemCount<=MAX_CHOICE_MENU_ITEMS ? itemCount-11+'A' : 'Z'
        );
        for(int i=0; i<itemCount; i++) {
            free(localizedItemNames[i]);
        }
        free(localizedItemNames);
        free(localizedTitle);
        do {
            int choice;
#ifndef _WIN32
            fflush(stdout);
#endif
            choice=
#ifdef _MSC_VER
                _getche();
#else
                getche();
#endif
            if('A'<=choice&&choice<='Z') {
                choice-='A';
                choice+=10;
            }
            else if('a'<=choice&&choice<='z') {
                choice-='a';
                choice+=10;
            }
            else if('0'<=choice&&choice<='9') {
                choice-='0';
            }
            else {
                choice=-1;
            }
            if(choice < 0||choice > itemCount-1) {
                ENDL;
                printf("Invalid choice. Please try again. (0-%c): ",
                    itemCount<=10 ? itemCount-1+'0' :
                    itemCount<=MAX_CHOICE_MENU_ITEMS ? itemCount-11+'A' : 'Z');
            }
            else {
                return choice;
            }
        } while(1);
    } while(1);
}

void clearInputBuffer(void)
{
    int current='\0';
    do {
        current=getchar();
    } while(current!='\n'&&current!=EOF&&current!='\0'&&current!='\x1a');
}

void printPoolLinkList(PoolLinkList current)
{
    if(current==NULL) { ENDL; puts("This character have't been UP yet."); }
    for(int i=0; current!=NULL; i++) {
        if(i%3==0) ENDL;
        printf("%hu.%hu.%hu\t",current->major,current->minor,current->half);
        current=current->next;
    }
    ENDL;
}

int readIntInRange(int min,int max,const int* defaultValue)
{
    char buf[64],*end;
    long val;
    while(1) {
        if(!fgets(buf,sizeof(buf),stdin)) {
            // 说明 fgets 读取失败，可能是 EOF 或者其他错误
            if(feof(stdin)) {
                if (defaultValue != NULL) { return *defaultValue; }
                printf("EOF detected. Enter a number (%d-%d): ", min, max);
                continue;
            }
            // 如果有错误却又不是 EOF，清除错误标志并继续
            clearerr(stdin);
            continue;
        }
        if(buf[0]=='\n') {
            if(defaultValue!=NULL) { return *defaultValue; }
            printf("Empty input. Enter a number (%d-%d): ",min,max);
            continue;
        }
        // char ending = buf[63];
        if(!strchr(buf,'\n')&&!strchr(buf, '\x1a')) {
            // '\n' 和 '\x1a' 都不在 buf 中，说明输入太长了 
            printf("Input too long. Enter a number (%d-%d): ",min,max);
            // 只有这一种情况说明输入没有读完，需要清空输入缓冲区
            clearInputBuffer();
            continue;
        }
        // 到这就不需要清空输入缓冲区了
        errno=0;
        val=strtol(buf,&end,10);
        if(end==buf||(*end!='\n'&&*end!='\0'&&*end!='\r'&&*end!=EOF && *end != '\x1a')) {
            printf("Invalid input. Enter a valid integer (%d-%d): ",min,max);
            continue;
        }
        if(errno==ERANGE||val < min||val > max) {
            printf("Out of range. Enter a number (%d-%d): ",min,max);
            continue;
        }
        return (int)val;
    }
}

void pauseConsole(void)
{
    ending='\0';
#ifdef _WIN32
#ifdef _MSC_VER
    ending=_getch();
#else
    ending=(char)getch();
#endif
#else
    do {
        ending=getchar();
    } while(ending!='\n'&&ending!=EOF&&ending!='\0');
#endif
}

#ifdef _WIN32
DWORD printW(const wchar_t* wstr)
{
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    WriteConsoleW(hConsole,wstr,(DWORD)wcslen(wstr),&written,NULL);
    return written;
}
#else
int printW(const wchar_t* wstr)
{
    return printf("%ls",wstr);
}
#endif

_Bool SetConsoleColorByCharacter(const CharMapType character)
{
    return SetConsoleColorByVision(character.vision);
}

_Bool SetConsoleColorByVision(uint8_t vision)
{
#ifdef _WIN32
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    if(!GetConsoleScreenBufferInfo(hConsole,&original)) {
        return 0;
    }
    SetConsoleTextAttribute(hConsole,visionColor[vision]);
    return 1;
#else
    printf("\033[38;5;%um",visionColor[vision]);
    return 1;
#endif
}

_Bool ResetConsoleColor(void)
{
#ifdef _WIN32
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,original.wAttributes);
    return 1;
#else
    printf("\033[0m");
    return 1;
#endif
}

size_t id2Index(int id)
{
    for(size_t i=0; i<charCount; i++) {
        if(CharMap[i].id==id) {
            return i;
        }
    }
    return SIZE_MAX;
}

int index2Id(size_t index)
{
    if(index>=charCount) {
        return -1;
    }
    return CharMap[index].id;
}

void beforeTerminate(void)
{
#ifdef _WIN32
    puts("Press any key to exit...");
#else
    puts("Press ENTER to exit...");
#endif
    PAUSE;
}
