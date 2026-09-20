#include "gipool.h"

size_t localizedMemLen(const wchar_t* source)
{
#ifdef _WIN32
    return (size_t)WideCharToMultiByte(CP_ACP,0,(LPCWSTR)source,-1,NULL,0,NULL,NULL);
#else
    return wcstombs(NULL,source,0)+1;
#endif
}

size_t localizedVisualLen(const wchar_t* source)
{
#ifdef _WIN32
    LPWORD widthArray=(LPWORD)malloc(sizeof(WORD)*(wcslen(source)+1));
    RETURN_IF_NULL(widthArray, 0, -1);
    if(GetStringTypeW(CT_CTYPE3,source,(int)wcslen(source),widthArray)==0) {
        free(widthArray);
        return 0;
    }
    size_t visualLen=0;
    for(size_t i=0; i<wcslen(source); i++) {
        if(widthArray[i]&C3_FULLWIDTH) visualLen+=2;
        else if(widthArray[i]&C3_HALFWIDTH) visualLen+=1;
        else if(widthArray[i]&C3_IDEOGRAPH) visualLen+=2;
        else visualLen+=1;
    }
    free(widthArray);
    return visualLen;
#else
    return (size_t)wcswidth(source,wcslen(source));
#endif
}

char* localize(const wchar_t* source)
{
#ifdef _WIN32
    int destSize=WideCharToMultiByte(CP_ACP,0,(LPCWSTR)source,-1,NULL,0,NULL,NULL);
    if(destSize==0) return NULL;
    char* localizedStr=(char*)malloc(sizeof(char)*(size_t)destSize);
    RETURN_IF_NULL(localizedStr, NULL, -1);
    if(WideCharToMultiByte(CP_ACP,0,(LPCWSTR)source,-1,localizedStr,destSize,NULL,NULL)==0) {
        free(localizedStr);
        return NULL;
    }
    return localizedStr;
#else
    size_t destSize=wcstombs(NULL,source,0)+1;
    if(destSize==0) return NULL;
    char* localizedStr=(char*)malloc(sizeof(char)*destSize);
    RETURN_IF_NULL(localizedStr, NULL, -1);
    if(wcstombs(localizedStr,source,destSize)==0) {
        free(localizedStr);
        return NULL;
    }
    return localizedStr;
#endif
}
