#pragma once
#define CONUSAGE
#ifndef CONFUNC
#include "confunc.h"
#endif

void printAllPools();
void printDaysofAllLimited5StarCharacters();




void printAllPools() {
    CLS;
    for (int i = 0; i < poolCount; i++) {
        putPool(WishPool[i]);
    }
}

void printDaysofAllLimited5StarCharacters() {
    CLS;
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
}