#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "giplinfo.h"

#define DATE_LENGTH 12
#define ARRAY_SIZE(arr)                 \
    (                                   \
        sizeof(arr)==0?0:               \
        sizeof(arr)/sizeof((arr)[0])    \
    )


#ifdef _MSC_VER
#define PAUSE ending = _getch(); 
#else
#define PAUSE ending = getch();
#endif

int charCount=0,poolCount=0,longestIndex=0;
int* daysPassedSinceLastUP=NULL;
int* arrangedInOrderOfDays=NULL;
char** localizedNames = NULL;
char ending = '\0',current='\0';
static const char* month_table[] = {
    "Jan","Feb","Mar","Apr","May","Jun",
    "Jul","Aug","Sep","Oct","Nov","Dec"
};
_Bool convertCompileTime(char* date);
int month_to_number(const char* mon);
int checkIntegrity(void);
_Bool isPoolInOrder(int i);
void initDynamicThings(void);
int findLongest(Char_Map CharMap1[]);
void getDaysPassedSinceLastUp(void);
time_t makeTimeFromYMDHMS(uint16_t y,uint8_t m,uint8_t d,int hour,int min,int sec);
int daysSinceSinglePoolEnds(const Wish_Pool pool);
void swap(int* a,int* b);
int partition(int days[],int indices[],int low,int high);
void quickSort(int days[],int indices[],int low,int high);
void arrangeByDaysPassedSinceLastUp(void);
void freeDynamicThings(void);
int poolEndHour(uint8_t half);
void pause();

_Bool convertCompileTime(char* date) {
    int gotten = 0;
    char month_str[4];
    unsigned short month;
    unsigned short day;
    unsigned int year;
    if ((gotten =
#ifdef _MSC_VER
        sscanf_s(__DATE__, "%3s %hu %u", month_str, 4, &day, &year)
#else
        sscanf(__DATE__, "%3s %hu %u", month_str, &day, &year)
#endif
        ) == 3
        &&
        (month = month_to_number(month_str)) != 0
        )
    {
#ifdef _MSC_VER
        _snprintf_s(date, DATE_LENGTH, DATE_LENGTH - 1, "%u-%hu-%hu", year, month, day);
#else
        snprintf(date, DATE_LENGTH - 1, "%u-%hu-%hu", year, month, day);
#endif
        return 0;
    }
    else {
#ifdef _MSC_VER
        strcpy_s(date, DATE_LENGTH, __DATE__);
#else
        strcpy(date, __DATE__);
#endif
        return 1;
    }
}

int month_to_number(const char* mon)
{
    for(int i=0;i<12;i++) {
        if(strcmp(mon,month_table[i])==0) return i+1; // 1~12
    }
    return 0; // Invalid month
}

int checkIntegrity(void){
    int errorlevel=0;
    for(int i=0;i<(int)ARRAY_SIZE(CharMap);i++) {
        if(
            CharMap[i].id!=i
        ) errorlevel++;
    }
    for(int i=0;i+1<(int)ARRAY_SIZE(WishPool);i++) {
        if(
            isPoolInOrder(i)         
        ) errorlevel++;
    }
    return errorlevel;
}

_Bool isPoolInOrder(int i){
    if((WishPool[i].major>WishPool[i+1].major)                                                                                                ||
            ((WishPool[i].major==WishPool[i+1].major)&& (WishPool[i].minor>WishPool[i+1].minor))                                                   ||
            ((WishPool[i].major<WishPool[i+1].major)&& (WishPool[i+1].minor>0))                                                                    ||
            (!((WishPool[i].major<WishPool[i+1].major)||((WishPool[i].major==WishPool[i+1].major)&&(WishPool[i].minor<WishPool[i+1].minor)))&&((WishPool[i].half%10)>(WishPool[i+1].half%10)))  ||
            (makeTimeFromYMDHMS(WishPool[i].startY,WishPool[i].startM,WishPool[i].startD,poolEndHour(WishPool[i].half),0,0)>=makeTimeFromYMDHMS(WishPool[i].endY,WishPool[i].endM,WishPool[i].endD,poolEndHour(WishPool[i].half),0,0)) ||
            ((makeTimeFromYMDHMS(WishPool[i].endY,WishPool[i].endM,WishPool[i].endD,poolEndHour(WishPool[i].half),0,0)>makeTimeFromYMDHMS(WishPool[i+1].startY,WishPool[i+1].startM,WishPool[i+1].startD,poolEndHour(WishPool[i].half),0,0))&&((WishPool[i].half%10)<(WishPool[i+1].half%10))) ||
            0  
    ) return 1; else return 0;
}

void initDynamicThings(void){
    charCount=(int)ARRAY_SIZE(CharMap);
    poolCount=(int)ARRAY_SIZE(WishPool);
    longestIndex=findLongest(CharMap);
    getDaysPassedSinceLastUp();
	do { arrangedInOrderOfDays = (int*)malloc(charCount * sizeof(int)); } while (arrangedInOrderOfDays == NULL);
    for(int i=0;i<charCount;i++){
        arrangedInOrderOfDays[i]=i;
    }
    arrangeByDaysPassedSinceLastUp();
}

int findLongest(Char_Map CharMap1[]){
    size_t currentLen=0,maxLen=0;
    int maxIndex=-1;
    for(int i=0;i<charCount;i++){
        currentLen=wcslen(CharMap1[i].name_cn);
        if(currentLen>maxLen)
        {
            maxIndex=i;
            maxLen=currentLen;
        }
    }
    return maxIndex;
}

void getDaysPassedSinceLastUp(void){
    // 这个函数在程序最开始初始化时调用，计算每一个角色的最后一个卡池的已结束天数
    // 因为函数最开始会将 daysPassedSinceLastUP 全局指针变量进行分配
    // 而这个全局变量的释放是由 freeDynamicThings() 进行
    // 所以请勿反复调用此函数
    // 现在本程序规定未分配或 free 过的指针应当值是 NULL
    // 因此这个函数在检测到 daysPassedSinceLastUP 不是 NULL 就会直接 free
    // 然后直接再次分配，没有其他情况检测
    if(daysPassedSinceLastUP!=NULL) free(daysPassedSinceLastUP);
    do { daysPassedSinceLastUP = (int*)malloc(sizeof(int) * charCount);
        }while (daysPassedSinceLastUP == NULL);
    for (int c=0;c<charCount;c++)
    {
        int lastPoolIndex=-1;
        // 从后往前找，最近一次包含该角色的池
        for(int p=poolCount-1;p>=0;p--)
        {
            for(int i=0;i<24&&(CharMap[c].attrib==4?WishPool[p].up4[i]:WishPool[p].up5[i])!=0;i++)
            {
                if((CharMap[c].attrib==4?WishPool[p].up4[i]:WishPool[p].up5[i])==CharMap[c].id)
                {
                    lastPoolIndex=p;
                    goto FOUND;
                }
            }
        }
    FOUND:
        if (daysPassedSinceLastUP != NULL)
        {
            if (lastPoolIndex >= 0)daysPassedSinceLastUP[c] = daysSinceSinglePoolEnds(WishPool[lastPoolIndex]);
            // 从未 UP 过（常驻 / 联动 / 特殊）
            else daysPassedSinceLastUP[c] = INT_MIN;
        }
    }
}

time_t makeTimeFromYMDHMS(uint16_t y,uint8_t m,uint8_t d,int hour,int min,int sec){
    struct tm t={0};
    t.tm_year = y-1900;
    t.tm_mon  = m-1;
    t.tm_mday = d;
    t.tm_hour = hour;
    t.tm_min  = min;
    t.tm_sec  = sec;
    // 此处暂时硬编码一个 00:00:00 ，以后再看要不要细化到时间
    t.tm_isdst=-1;
    return mktime(&t);
}

int daysSinceSinglePoolEnds(const Wish_Pool pool)
{
    // 这个函数仅计算单个卡池的已结束天数
    time_t now=time(NULL);
    int hour=0,min=0,sec=0;
    hour=poolEndHour(pool.half);
    if(hour==INT_MIN) return INT_MIN;
    time_t end=makeTimeFromYMDHMS(pool.endY,pool.endM,pool.endD,hour,min,sec);
    double diff=difftime(now,end);
    diff/=24*60*60;
    if(diff<0) return ((int)diff)-1;
    else if(diff>0) return ((int)diff)+1;
    else return 0;
}

void swap(int* a,int* b) {
    if(a==b||*a==*b) return;
    else {
        *a+=*b;
        *b=*a-*b;
        *a-=*b;
    }
}

int partition(int days[],int indices[],int low,int high) {
    // 快速排序分区函数
    int pivot=days[indices[high]];  // 选择最后一个元素作为枢轴
    int i=low-1;                    // 较小元素的索引
    for(int j=low;j<=high-1;j++) {
        // 如果当前元素大于枢轴值（降序排序）
        if(days[indices[j]]>pivot) {
            i++;                    // 增加较小元素的索引
            // 交换 indices[i] 和 indices[j]
            swap(&indices[i],&indices[j]);
        }
    }
    // 将枢轴放到正确位置
            swap(&indices[i+1],&indices[high]);
    return i+1;
}

void quickSort(int days[],int indices[],int low,int high) {
    // 快速排序主函数
    if(low<high) {
        // 获取分区点
        int pi=partition(days,indices,low ,high);
        // 递归排序左右两部分
               quickSort(days,indices,low ,pi-1);
               quickSort(days,indices,pi+1,high);
    }
}

void arrangeByDaysPassedSinceLastUp() {
    // 这个是包装函数，在 initDynamicThings() 中调用  
    // 对 arrangedInOrderOfDays 进行快速排序
    // 排序依据是 daysPassedSinceLastUP[arrangedInOrderOfDays[i]] 降序
    if(charCount>0) quickSort(daysPassedSinceLastUP,arrangedInOrderOfDays,0,charCount-1);
}

void freeDynamicThings(void){
    free(daysPassedSinceLastUP);
    free(arrangedInOrderOfDays);
    daysPassedSinceLastUP=arrangedInOrderOfDays=NULL;
}

int poolEndHour(uint8_t half){
    switch(half%10) {
        case 1:{
            return 18;
        }
        case 2:{
            return 15;
        }
        case 3:{
            return 0; // 资料暂缺
        }
        default:{
            return INT_MIN;
        }
    }
}

void pause()
{
    char ending = '\0';
#ifdef _MSC_VER
    ending = _getch();
#else
    ending = getch();
#endif
}