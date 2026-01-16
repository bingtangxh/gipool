#include <conio.h>
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
    initDynamicThings();
    do { localizedNames=(char**) malloc(charCount*sizeof(char*)); } while (localizedNames==NULL);
    localizeNames(CharMap,localizedNames);
    printCompileTime();
    ENDL;
    printf("Count of characters and pool info with errors: %d\n",checkIntegrity());
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
    puts("Press any key to exit...");
    PAUSE;
}