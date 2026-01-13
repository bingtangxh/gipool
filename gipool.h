#include "giplinfo.h"
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <limits.h>

#define ARRAY_SIZE(arr)                 \
    (                                   \
        sizeof(arr)==0?0:               \
        sizeof(arr)/sizeof((arr)[0])    \
    )

char** localizedNames;
int charCount,poolCount,longestIndex;
int* daysPassedSinceLastUP;
int* arrangedInOrderOfDays;
static const char* month_table[] = {
    "Jan","Feb","Mar","Apr","May","Jun",
    "Jul","Aug","Sep","Oct","Nov","Dec"
};
int month_to_number(const char* mon);
int checkIntegrity(void);
_Bool isPoolInOrder(int i);
void initDynamicThings(void);
void printW(const wchar_t* wstr);
void putPool(Wish_Pool WishPool1);
int findLongest(Char_Map CharMap1[]);
_Bool localizeNames(Char_Map CharMap1[],char* localizedNames[]);
void getDaysPassedSinceLastUp(void);
time_t makeTimeFromYMDHMS(uint16_t y,uint8_t m,uint8_t d,int hour,int min,int sec);
int daysSinceSinglePoolEnds(const Wish_Pool pool);
void swap(int* a,int* b);
int partition(int days[],int indices[],int low,int high);
void quickSort(int days[],int indices[],int low,int high);
void arrangeByDaysPassedSinceLastUp(void);
void freeDynamicThings(void);
int poolEndHour(uint8_t half);

int month_to_number(const char* mon)
{
    for (int i = 0; i < 12; ++i) {
        if (strcmp(mon, month_table[i]) == 0)
            return i + 1; // 1~12
    }
    return 0; // 非法月份
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
    localizedNames=(char**)malloc(charCount*sizeof(char*));
    localizeNames(CharMap,localizedNames);
    getDaysPassedSinceLastUp();
    arrangedInOrderOfDays=(int*)malloc(charCount*sizeof(int));
    for(int i=0;i<charCount;i++){
        arrangedInOrderOfDays[i]=i;
    }
    arrangeByDaysPassedSinceLastUp();
}

void printW(const wchar_t* wstr) {
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    WriteConsoleW(hConsole,wstr,(DWORD)wcslen(wstr),&written,NULL);
}

void putPool(Wish_Pool WishPool1)
{
    int fiveCount=sizeof(WishPool1.up5)==0?0:(int)(sizeof(WishPool1.up5)/sizeof(WishPool1.up5[0]));
    int fourCount=sizeof(WishPool1.up4)==0?0:(int)(sizeof(WishPool1.up4)/sizeof(WishPool1.up4[0]));
    printf("%hu.%hu.%hu\t%u.%hu.%hu\t%u.%hu.%hu\t",WishPool1.major,WishPool1.minor,WishPool1.half,WishPool1.startY,WishPool1.startM,WishPool1.startD,WishPool1.endY,WishPool1.endM,WishPool1.endD);
    for(int i=0;i<fiveCount&&WishPool1.up5[i]!=0;i++)
    {
        //printW(CharMap[WishPool1.up5[i]].name_cn);
        printf("%s ",localizedNames[WishPool1.up5[i]]);
    }
    for(int i=0;i<fourCount&&WishPool1.up4[i]!=0;i++)
    {
        //printW(CharMap[WishPool1.up4[i]].name_cn);
        printf("%s ",localizedNames[WishPool1.up4[i]]);
    }
    putchar('\n');
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

_Bool localizeNames(Char_Map CharMap1[],char* localizedNames[]){
    _Bool result=0;
    for(int i=0,conved=0;i<charCount;i++){
        int destSize=WideCharToMultiByte(CP_ACP,0,CharMap1[i].name_cn,-1,NULL,0,NULL,NULL);
        localizedNames[i]=(char*)malloc(sizeof(char)*(destSize+0));
        conved=WideCharToMultiByte(CP_ACP,0,CharMap1[i].name_cn,-1,localizedNames[i],destSize,NULL,NULL);
        if(conved==0) result=1;
    }
    return result;
    // result 返回 1 为未完成，返回 0 为成功完成。
}

void getDaysPassedSinceLastUp(void){
    // 这个函数在程序最开始初始化时调用，计算每一个角色的最后一个卡池的已结束天数
    // 因为函数最开始会将 daysPassedSinceLastUP 全局指针变量进行分配
    // 而这个全局变量的释放是由 freeDynamicThings() 进行
    // 所以请勿反复调用此函数
    daysPassedSinceLastUP=(int*)malloc(sizeof(int)*charCount);
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
        if (lastPoolIndex>=0)daysPassedSinceLastUP[c]=daysSinceSinglePoolEnds(WishPool[lastPoolIndex]);
        // 从未 UP 过（常驻 / 联动 / 特殊）
        else daysPassedSinceLastUP[c]=-1;
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
    // 包装函数，在 initDynamicThings() 中调用  
    // 对 arrangedInOrderOfDays 进行快速排序
    // 排序依据是 daysPassedSinceLastUP[arrangedInOrderOfDays[i]] 降序
    if(charCount>0) quickSort(daysPassedSinceLastUP,arrangedInOrderOfDays,0,charCount-1);
}

void freeDynamicThings(void){
    for(int i=0;i<charCount;i++){
        free(localizedNames[i]);
    }
    free(localizedNames);
    free(daysPassedSinceLastUP);
    free(arrangedInOrderOfDays);
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