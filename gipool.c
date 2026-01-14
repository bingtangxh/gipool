#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "gipool.h"
#include "localize.h"
#include "conmenus.h"
#include "confunc.h"

void printTestInfo();
void beforeTerminate();

int main(int argc,char** argv)
{
    initConsole();
    initDynamicThings();
    do { localizedNames = (char**)malloc(charCount * sizeof(char*)); } while (localizedNames == NULL);
    localizeNames(CharMap, localizedNames);
	printTestInfo();
    ENDL;
	printCompileTime();
    freeDynamicThings();
    freeLocalizedNames();
    printf("Count of characters and pool info with errors: %d\n",checkIntegrity());
    ENDL;
	choiceMenu(mainMenu, ARRAY_SIZE(mainMenu), L"原神祈愿卡池信息工具我不管我就要让它变长看看能不能正常工作");
    // beforeTerminate();
    return 0;
}

void printTestInfo(){
    for(int i=0;i<poolCount;i++){
        putPool(WishPool[i]);
    }
    for(int i=0;i<charCount;i++){
        if((
                    daysPassedSinceLastUP[arrangedInOrderOfDays[i]]!=INT_MIN
            )&&((
                    CharMap[arrangedInOrderOfDays[i]].attrib==9
                )||(
                    CharMap[arrangedInOrderOfDays[i]].attrib==5
            ))){
            printf("%12s\t%d\n",localizedNames[arrangedInOrderOfDays[i]],daysPassedSinceLastUP[arrangedInOrderOfDays[i]]);
        }
        else{
            // printf("%12s\t%u\n",localizedNames[arrangedInOrderOfDays[i]],CharMap[arrangedInOrderOfDays[i]].attrib);
        }
    }
}

void beforeTerminate(){
    puts("Press any key to exit...");
	char ending = '\0';
#ifdef _MSC_VER
    ending=_getch();
#else
    ending=getch();
#endif
}
