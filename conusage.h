#pragma once
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

int choiceOneCharacter4Test() {
    int result;
    printf("\ncharCount = %d\n", charCount);
    ENDL;
    fputs("Please type a char index number, type -1 to go back: ", stdout);
    do {
        GETNUM(result);
        clearInputBuffer();
        if (result == -1) {
            CLS;
            break;
        }
        else if (result < 0 || result > charCount-1) {
            printf("Invalid choice. Type -1 to go back. (0-%d): ", charCount-1);
        }
        else { break; }
    } while (1);
    return result;
}
