#pragma once
#ifndef CONUSAGE
#define CONUSAGE
#ifndef CONFUNC
#include "confunc.h"
#endif
#ifndef CONMENUS
#include "conmenus.h"
#endif

#define PUT_ALL_CHARACTERS_DAYS_SINCE_LAST_UP_FORMAT_LENGTH 10
char putAllCharactersDaysSinceLastUpFormat[PUT_ALL_CHARACTERS_DAYS_SINCE_LAST_UP_FORMAT_LENGTH]="";

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
#ifdef _MSC_VER
    strcpy_s(putAllCharactersDaysSinceLastUpFormat,PUT_ALL_CHARACTERS_DAYS_SINCE_LAST_UP_FORMAT_LENGTH,"%");
    sprintf_s(putAllCharactersDaysSinceLastUpFormat+strlen(putAllCharactersDaysSinceLastUpFormat),PUT_ALL_CHARACTERS_DAYS_SINCE_LAST_UP_FORMAT_LENGTH-strlen(putAllCharactersDaysSinceLastUpFormat),"%u",strlen(localizedNames[longestChineseIndex]));
    strcat_s(putAllCharactersDaysSinceLastUpFormat,PUT_ALL_CHARACTERS_DAYS_SINCE_LAST_UP_FORMAT_LENGTH,"s | %d");
#else
    strcat(putAllCharactersDaysSinceLastUpFormat,"%");
    sprintf(putAllCharactersDaysSinceLastUpFormat+strlen(putAllCharactersDaysSinceLastUpFormat),"%u",strlen(localizedNames[longestChineseIndex]));
    strcat(putAllCharactersDaysSinceLastUpFormat,"s | %d");
#endif
    CLS;
    for (size_t i=0; i<charCount; i++) {
        if ((
            daysPassedSinceLastUP[arrangedInOrderOfDays[i]]!=INT_MIN
            )&&((
                CharMap[arrangedInOrderOfDays[i]].attrib==9
                )||(
                    CharMap[arrangedInOrderOfDays[i]].attrib==5
                    ))) {
            printf(putAllCharactersDaysSinceLastUpFormat,localizedNames[arrangedInOrderOfDays[i]],daysPassedSinceLastUP[arrangedInOrderOfDays[i]]/*,localizedVisualLen(CharMap[arrangedInOrderOfDays[i]].name_cn)*/);
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
            printf("Invalid choice. Type -1 to go back. (0-%u): ",charCount-1);
        } else { break; }
    } while (1);
    return result;
}

#endif