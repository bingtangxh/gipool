#ifdef _WIN32
#include <conio.h>
#else
// #include <ncurses.h>
#define _XOPEN_SOURCE 700
#endif
#include <locale.h>
#include "gipool.h"
#include "localize.h"
#include "confunc.h"
#include "conmenus.h"

void printTestInfo();
void beforeTerminate();

int main(int argc,char** argv)
{
    initConsole();
    if(argc>1) {
        if (!strcmp(argv[1],"/?")) {
            help();
            return 0;
        }
    }
    initDynamicThings();
    do { localizedNames=(char**) malloc(charCount*sizeof(char*)); } while (localizedNames==NULL);
    localizeNames(CharMap,localizedNames);
    printCompileTime();
    ENDL;
    printf("Count of characters and pool info with errors: %d",checkIntegrity());
    ENDL;
    ENDL;
    _mainMenu();
    freeDynamicThings();
    freeLocalizedNames();
    // beforeTerminate();
    return 0;
}

void printTestInfo(){

}

void beforeTerminate(){
#ifdef _WIN32
    puts("Press any key to exit...");
#else
    puts("Press ENTER to exit...");
#endif
    PAUSE;
}