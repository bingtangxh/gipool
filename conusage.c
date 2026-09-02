#include "gipool.h"

void printAllPools(void)
{
    CLS;
    for(size_t i=0; i<poolCount; i++) {
        putPool(WishPool[i]);
    }
}

void printDaysofAllLimited5StarCharacters(void)
{
    CLS;
    for(size_t i=0; i<charCount; i++) {
        size_t index=(size_t)arrangedInOrderOfDays[i];
        if((daysPassedSinceLastUP[index]!=INT_MIN)&&
            ((CharMap[index].attrib==9)||(CharMap[index].attrib==5))) {
            for(size_t j=0; j<localizedVisualLen(CharMap[longestChineseIndex].name_cn)-localizedVisualLen(CharMap[index].name_cn); j++) { SPACE; }
            SetConsoleColorByCharacter(CharMap[index]);
            printf("%s",localizedNames[index]);
            ResetConsoleColor();
            printf(" | %d",daysPassedSinceLastUP[index]);
            ENDL;
        }
    }
}

int choiceOneCharacter4Test(void)
{
    int result=-1;
    ENDL;
    fputs("Please type a char index number, type -1 to go back: ",stdout);
    do {
        result=readIntInRange(INT_MIN,INT_MAX,NULL);
        if(result==-1) {
            CLS;
            break;
        }
        else if(result < 0||(unsigned int)result > charCount-1) {
            printf("Invalid choice. Type -1 to go back. (0-%u): ",(unsigned int)charCount-1);
        }
        else {
            break;
        }
    } while(1);
    return result;
}
