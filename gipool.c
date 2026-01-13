#include <conio.h>
#include "gipool.h"

void printCompileTime();
void beforeTerminate();

int main()
{
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);
    int gotten=0;
    initDynamicThings();
    for (int i=0;i<poolCount;i++){
        putPool(WishPool[i]);
    }
    for(int i=0;i<charCount;i++){
        if((1||daysPassedSinceLastUP[arrangedInOrderOfDays[i]])&&((CharMap[arrangedInOrderOfDays[i]].attrib==9)||(CharMap[arrangedInOrderOfDays[i]].attrib==5))){
            printf("%12s\t%d\n",localizedNames[arrangedInOrderOfDays[i]],daysPassedSinceLastUP[arrangedInOrderOfDays[i]]);
        } else {
            // printf("%12s\t%u\n",localizedNames[arrangedInOrderOfDays[i]],CharMap[arrangedInOrderOfDays[i]].attrib);
        }
    }
    putchar('\n');
    printCompileTime();
    freeDynamicThings();
    printf("Count of characters and pool info with errors: %d\n",checkIntegrity());
    putchar('\n');
    beforeTerminate();
    return 0;
}

void printCompileTime(){
    int gotten=0;
    char month_str[4];
    unsigned short month;
    char date[12];
    unsigned short day;
    unsigned int year;
    if((gotten=
#ifdef _MSC_VER
        sscanf_s(__DATE__,"%3s %hu %u",month_str,4,&day,&year)
#else
        sscanf(__DATE__,"%3s %hu %u",month_str,&day,&year)
#endif
    )==3
    &&
    (month=month_to_number(month_str))!=0
    )
    {
#ifdef _MSC_VER
        _snprintf_s(date,12,11,"%u-%hu-%hu",year,month,day);
#else
        snprintf(date,11,"%u-%hu-%hu",year,month,day);
#endif
    }
    else {
#ifdef _MSC_VER
        strcpy_s(date,12,__DATE__);
#else
        strcpy(date,__DATE__);
#endif
    }
    printf("Compiled at %s %s\n\n",date,__TIME__);
}

void beforeTerminate(){
    puts("Press any key to exit...");
#ifdef _MSC_VER
    _getch();
#else
    getch();
#endif
}