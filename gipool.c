#include "gipool.h"

#include "gipool.h"

PoolLinkList* PoolLinkLists=NULL;
size_t charCount=0;
size_t poolCount=0;
size_t longestChineseIndex=0;
size_t longestChineseNameLength=0;
size_t longestEnglishIndex=0;
size_t longestEnglishNameLength=0;
int ending='\0';
int* daysPassedSinceLastUP=NULL;
int* arrangedInOrderOfDays=NULL;
char** localizedNames=NULL;
const int maxCharactersInCharPool=2;

const wchar_t singleEdge[]=L"─";
const wchar_t doubleEdge[]=L"═";
const char splitLine[]="-------------------------------------------------------------------------------------------------------";

#ifdef _WIN32
CONSOLE_SCREEN_BUFFER_INFO original;
WORD visionColor[]={
    FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED,
    FOREGROUND_RED|FOREGROUND_INTENSITY,
    FOREGROUND_BLUE|FOREGROUND_INTENSITY,
    FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY,
    FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY,
    FOREGROUND_GREEN|FOREGROUND_INTENSITY,
    FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY,
    FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY,
    FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED
};
#else
uint8_t visionColor[]={7, 9, 33, 43, 99, 46, 159, 220, 7};
#endif


void printTestInfo(void);
void beforeTerminate(void);


void printTestInfo(void)
{
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

int main(int argc,char** argv)
{
    initConsole();
    if(argc>1) {
        if(!strcmp(argv[1],"/?")) {
            help();
            return 0;
        }
    }

    initDynamicThings();
    do {
        localizedNames=(char**)malloc(charCount*sizeof(char*));
    } while(localizedNames==NULL);

    localizeNamesArray(CharMap,localizedNames);
    printCompileTime();
    ENDL;
    printf("Count of characters and pool info with errors: %d",checkIntegrity());
    ENDL;
    mainMenu();
    freeDynamicThings();
    freeLocalizedNames();
    return 0;
}
