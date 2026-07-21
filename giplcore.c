#include "gipool.h"

static const char* month_table[]={
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

unsigned short shortmonth_to_number(const char* mon)
{
    for(unsigned short i=0; i<12; i++) {
        if(strcmp(mon,month_table[i])==0) {
            return i+1;
        }
    }
    return 0;
}

_Bool convertCompileTime(char* date)
{
    int gotten=0;
    char month_str[4];
    unsigned short month=0;
    unsigned short day=0;
    unsigned int year=0;

    if((gotten=
#ifdef _MSC_VER
        sscanf_s(__DATE__,"%3s %hu %u",month_str,4U,&day,&year)
#else
        sscanf(__DATE__,"%3s %hu %u",month_str,&day,&year)
#endif
        )==3&&
        (month=shortmonth_to_number(month_str))!=0) {
#ifdef _MSC_VER
        _snprintf_s(date,DATE_LENGTH,DATE_LENGTH-1,"%u-%hu-%hu",year,month,day);
#else
        snprintf(date,DATE_LENGTH,"%u-%hu-%hu",year,month,day);
#endif
        return 0;
    }

#ifdef _MSC_VER
    strcpy_s(date,DATE_LENGTH,__DATE__);
#else
    strcpy(date,__DATE__);
#endif
    return 1;
}

_Bool isPoolInOrder(int i)
{
    if((WishPool[i].major>WishPool[i+1].major)||
        ((WishPool[i].major==WishPool[i+1].major)&&(WishPool[i].minor>WishPool[i+1].minor))||
        ((WishPool[i].major<WishPool[i+1].major)&&(WishPool[i+1].minor>0))||
        (!((WishPool[i].major<WishPool[i+1].major)||
            ((WishPool[i].major==WishPool[i+1].major)&&(WishPool[i].minor<WishPool[i+1].minor)))&&
            ((WishPool[i].half%10)>(WishPool[i+1].half%10)))||
        (makeTimeFromYMDHMS(WishPool[i].startY,WishPool[i].startM,WishPool[i].startD,poolEndHour(WishPool[i].half),0,0)>=
            makeTimeFromYMDHMS(WishPool[i].endY,WishPool[i].endM,WishPool[i].endD,poolEndHour(WishPool[i].half),0,0))||
        ((makeTimeFromYMDHMS(WishPool[i].endY,WishPool[i].endM,WishPool[i].endD,poolEndHour(WishPool[i].half),0,0)>
            makeTimeFromYMDHMS(WishPool[i+1].startY,WishPool[i+1].startM,WishPool[i+1].startD,poolEndHour(WishPool[i].half),0,0))&&
            ((WishPool[i].half%10)<(WishPool[i+1].half%10)))) {
        return 1;
    }
    return 0;
}

void initDynamicThings(void)
{
    getCharandPoolCount();
    longestChineseIndex=(size_t)findLongest(CharMap);
    longestChineseNameLength=wcslen(CharMap[longestChineseIndex].name_cn);
    longestEnglishIndex=(size_t)findLongestEnglish(CharMap);
    longestEnglishNameLength=strlen(CharMap[longestEnglishIndex].name);

    getDaysPassedSinceLastUp();

    do {
        arrangedInOrderOfDays=(int*)malloc(charCount*sizeof(int));
    } while(arrangedInOrderOfDays==NULL);

    for(size_t i=0; i<charCount; i++) {
        arrangedInOrderOfDays[i]=(int)i;
    }
    arrangeByDaysPassedSinceLastUp();

    do {
        PoolLinkLists=(PoolLinkList*)malloc(sizeof(PoolLinkList)*charCount);
    } while(PoolLinkLists==NULL);

    for(size_t i=0; i<charCount; i++) {
        PoolLinkLists[i]=NULL;
    }
}

int checkIntegrity(void)
{
    // 该函数用到了 charCount 和 poolCount 这两个全局变量
    // 所以在调用该函数之前必须先调用 getCharandPoolCount() 或者 initDynamicThings() 函数来初始化这两个变量
    // 不过该函数也只会在 main 函数中被调用了
    int errorlevel=0;
    size_t excludedPoolIndex=0;

    for(unsigned int i=0; i<charCount; i++) {
        if(CharMap[i].attrib==ROLE_TYPE_EXCLUDED) {
            excludedPoolIndex++;
            continue;
        }
        if(CharMap[i].id!=i-excludedPoolIndex) {
            errorlevel++;
            excludedPoolIndex++;
        }
    }

    for(int i=0; i+1<(int)poolCount; i++) {
        if(isPoolInOrder(i)) {
            errorlevel++;
        }
    }

    return errorlevel;
}

int findLongest(_CharMap CharMap1[])
{
    size_t currentLen=0;
    size_t maxLen=0;
    int maxIndex=-1;

    for(size_t i=0; i<charCount; i++) {
        currentLen=wcslen(CharMap1[i].name_cn);
        if(currentLen>maxLen) {
            maxIndex=(int)i;
            maxLen=currentLen;
        }
    }
    return maxIndex;
}

int findLongestEnglish(_CharMap CharMap1[])
{
    size_t currentLen=0;
    size_t maxLen=0;
    int maxIndex=-1;

    for(size_t i=0; i<charCount; i++) {
        currentLen=strlen(CharMap1[i].name);
        if(currentLen>maxLen) {
            maxIndex=(int)i;
            maxLen=currentLen;
        }
    }
    return maxIndex;
}

void getDaysPassedSinceLastUp(void)
{
    if(daysPassedSinceLastUP!=NULL) {
        free(daysPassedSinceLastUP);
    }

    do {
        daysPassedSinceLastUP=(int*)malloc(sizeof(int)*charCount);
    } while(daysPassedSinceLastUP==NULL);

    for(size_t c=0; c<charCount; c++) {
        int lastPoolIndex=-1;

        if(poolCount<=0) {
            daysPassedSinceLastUP[c]=INT_MIN;
            continue;
        }

        for(size_t p=poolCount-1; p+1!=0; p--) {
            for(size_t i=0; i<24&&(CharMap[c].attrib==4 ? WishPool[p].up4[i] : WishPool[p].up5[i])!=0; i++) {
                if((CharMap[c].attrib==4 ? WishPool[p].up4[i] : WishPool[p].up5[i])==CharMap[c].id) {
                    lastPoolIndex=(int)p;
                    goto FOUND;
                }
            }
        }
    FOUND:
        if(lastPoolIndex>=0) {
            daysPassedSinceLastUP[c]=daysSinceSinglePoolEnds(WishPool[lastPoolIndex]);
        }
        else {
            daysPassedSinceLastUP[c]=INT_MIN;
        }
    }
}

time_t makeTimeFromYMDHMS(uint16_t y,uint8_t m,uint8_t d,int hour,int min,int sec)
{
    struct tm t={0};
    t.tm_year=y-1900;
    t.tm_mon=m-1;
    t.tm_mday=d;
    t.tm_hour=hour;
    t.tm_min=min;
    t.tm_sec=sec;
    t.tm_isdst=-1;
    return mktime(&t);
}

int daysSinceSinglePoolEnds(_WishPool pool)
{
    time_t now=time(NULL);
    int hour=poolEndHour(pool.half);
    time_t end;
    double diff;

    if(hour==INT_MIN) {
        return INT_MIN;
    }

    end=makeTimeFromYMDHMS(pool.endY,pool.endM,pool.endD,hour,0,0);
    diff=difftime(now,end)/(24*60*60);

    if(diff<0) {
        return ((int)diff)-1;
    }
    if(diff>0) {
        return ((int)diff)+1;
    }
    return 0;
}

void swap(int* a,int* b)
{
    if(a==b||*a==*b) {
        return;
    }

    *a+=*b;
    *b=*a-*b;
    *a-=*b;
}

int partition(int days[],int indices[],int low,int high)
{
    int pivot=days[indices[high]];
    int i=low-1;

    for(int j=low; j<=high-1; j++) {
        if(days[indices[j]]>pivot) {
            i++;
            swap(&indices[i],&indices[j]);
        }
    }

    swap(&indices[i+1],&indices[high]);
    return i+1;
}

void quickSort(int days[],int indices[],int low,int high)
{
    if(low<high) {
        int pi=partition(days,indices,low,high);
        quickSort(days,indices,low,pi-1);
        quickSort(days,indices,pi+1,high);
    }
}

void arrangeByDaysPassedSinceLastUp(void)
{
    if(charCount>0) {
        quickSort(daysPassedSinceLastUP,arrangedInOrderOfDays,0,(int)charCount-1);
    }
}

void freeDynamicThings(void)
{
    PoolLinkList currentNode=NULL;
    PoolLinkList currentNext=NULL;

    free(daysPassedSinceLastUP);
    daysPassedSinceLastUP=NULL;

    free(arrangedInOrderOfDays);
    arrangedInOrderOfDays=NULL;

    if(PoolLinkLists!=NULL) {
        for(size_t i=0; i<charCount; i++) {
            currentNode=PoolLinkLists[i];
            while(currentNode!=NULL) {
                currentNext=currentNode->next;
                free(currentNode);
                currentNode=currentNext;
            }
        }
        free(PoolLinkLists);
        PoolLinkLists=NULL;
    }
}

int poolEndHour(uint8_t half)
{
    switch(half%10) {
    case 1:
        return 18;
    case 2:
        return 15;
    case 3:
        return 0;
    default:
        return INT_MIN;
    }
}

_Bool buildPoolLinkList(size_t index,_WishPool WishPools[])
{
    PoolLinkList current=NULL;
    PoolLinkList currentNext=NULL;
    size_t fiveCount=ARRAY_SIZE(WishPools[0].up5);
    size_t fourCount=ARRAY_SIZE(WishPools[0].up4);

    if(PoolLinkLists==NULL) {
        return 1;
    }

    if(PoolLinkLists[index]!=NULL) {
        current=PoolLinkLists[index];
        while(current!=NULL) {
            currentNext=current->next;
            free(current);
            current=currentNext;
        }
        PoolLinkLists[index]=NULL;
    }

    if(CharMap[index].attrib==5) {
        for(size_t i=0; i<poolCount; i++) {
            for(size_t j=0; j<fiveCount&&WishPools[i].up5[j]!=0; j++) {
                if(WishPools[i].up5[j]==index) {
                    do {
                        currentNext=createPoolNode(WishPools[i]);
                    } while(currentNext==NULL);

                    if(PoolLinkLists[index]==NULL) {
                        PoolLinkLists[index]=current=currentNext;
                    }
                    else {
                        current->next=currentNext;
                        current=current->next;
                    }
                }
            }
        }
        return 0;
    }

    if(CharMap[index].attrib==4) {
        for(size_t i=0; i<poolCount; i++) {
            for(size_t j=0; j<fourCount&&WishPools[i].up4[j]!=0; j++) {
                if(WishPools[i].up4[j]==index) {
                    do {
                        currentNext=createPoolNode(WishPools[i]);
                    } while(currentNext==NULL);

                    if(PoolLinkLists[index]==NULL) {
                        PoolLinkLists[index]=current=currentNext;
                    }
                    else if(current!=NULL) {
                        current->next=currentNext;
                        current=current->next;
                    }
                    else {
                        puts("Error: 'current' is NULL while building pool link list for 4-star character.\r\nPlease report this bug to the developer.\r");
                        free(currentNext);
                        return 1;
                    }
                }
            }
        }
        return 0;
    }

    return 1;
}

PoolLinkList createPoolNode(_WishPool WishPool1)
{
    PoolLinkList target=(PoolLinkList)malloc(sizeof(_PoolNode));
    if(target==NULL) {
        return NULL;
    }

    target->major=WishPool1.major;
    target->minor=WishPool1.minor;
    target->half=WishPool1.half;
    target->next=NULL;
    return target;
}

void help(void)
{
    puts("Genshin Impact Wish Pool Information Tool\r");
    puts("\r");
    puts("Copyright (c) 2025-2026 BingtangXH.\r");
    puts("May the Anemo God bless you.\r");
    puts("\r");
}
