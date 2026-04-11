#pragma once
#include "gipool.h"
#ifndef CONUSAGE
#define CONUSAGE
#ifndef CONFUNC
#include "confunc.h"
#endif
#ifndef CONMENUS
#include "conmenus.h"
#endif

void printAllPools();
void printDaysofAllLimited5StarCharacters();
int choiceOneCharacter4Test();

void printAllPools() {
    CLS;
    for (size_t i=0; i<poolCount; i++) {
        putPool(WishPool[i]);
    }
}

void printDaysofAllLimited5StarCharacters() {

    CLS;
    for (size_t i=0; i<charCount; i++) {
        if ((
            daysPassedSinceLastUP[arrangedInOrderOfDays[i]]!=INT_MIN
            )&&((
                CharMap[arrangedInOrderOfDays[i]].attrib==9
                )||(
                    CharMap[arrangedInOrderOfDays[i]].attrib==5
                    ))) {
            for (size_t j=0; j<localizedVisualLen(CharMap[longestChineseIndex].name_cn)-localizedVisualLen(CharMap[arrangedInOrderOfDays[i]].name_cn); j++){ SPACE; }
            SetConsoleColorByCharacter(CharMap[arrangedInOrderOfDays[i]]);  
            printf("%s",localizedNames[arrangedInOrderOfDays[i]]);
            ResetConsoleColor();
            printf(" | %d",daysPassedSinceLastUP[arrangedInOrderOfDays[i]]);
            ENDL;
        } else {
            // printf("%12s\t%u",localizedNames[arrangedInOrderOfDays[i]],CharMap[arrangedInOrderOfDays[i]].attrib);
            // ENDL;
        }
    }
}

int choiceOneCharacter4Test() {
    int result=-1;
    // ENDL;
    // printf("charCount = %d",charCount);
    // ENDL;
    ENDL;
    fputs("Please type a char index number, type -1 to go back: ",stdout);
    do {
        result=readIntInRange(INT_MIN,INT_MAX,NULL);
        // clearInputBuffer();
        if (result==-1) {
            CLS;
            break;
        } else if (result < 0||result > charCount-1) {
            printf("Invalid choice. Type -1 to go back. (0-%u): ",(unsigned int)charCount-1);
        } else { break; }
    } while (1);
    return result;
}

#endif
