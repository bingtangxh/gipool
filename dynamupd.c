#include "gipool.h"

// 该文件负责获取 gipldata.c 和 cynjoks.c 的最新版本

void mark4OnlineUpdating(char* label)
{
    if(label==NULL) {
        return;
    } else return;
}

char* onlineUpdatingURLPrefix="https://raw.githubusercontent.com/bingtangxh/gipool/main/";
char* onlineUpdatingdataURLSuffix="gipldata.c";
char* onlineUpdatingjokesURLSuffix="cynjoks.c";

_Bool useOnlineUpdating(void)
{
    // 该函数在返回 1 表示在线获取成功之前，还需要将 charCount poolCount jokeCount 以及 CuCharMap CuWishPool CuCynoJokes 等变量更新为在线获取的最新版本
    charCount=charCount;
    poolCount=poolCount;
    jokeCount=jokeCount;
    CuCharMap=CuCharMap;
    CuWishPool=CuWishPool;
    CuCynoJokes=CuCynoJokes;
    
    return 0;
}