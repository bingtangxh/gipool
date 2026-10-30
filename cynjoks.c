#include "gipool.h"

// 此文件名称 cynjoks.c 已被 dynamupd.c 硬编码引用，请勿修改此文件名称，否则在线更新功能将不能工作

const CynoJoke* CuCynoJokes=NULL;
CynoJoke* onlineUpdatedCynoJokes=NULL;
int jokeCount=0;

const CynoJoke CynoJokes[]=
{
     CNJK(L"提瓦特哪个国家的建筑最不牢固？", L"须弥，因为须弥墙危（须弥蔷薇）")
    ,CNJK(L"夜兰手镯摔坏了应该找谁修？", L"找琴团长，因为琴能补镯（勤能补拙）")
    ,CNJK(L"为什么崩铁列车组不敢来提瓦特？", L"因为提瓦特有托马，托马撕小火车（托马斯小火车）")
    ,CNJK(L"甘雨小时候不会游泳，为什么掉到水里没事？", L"因为甘雨小时候胖胖的，肥而不溺（腻）")
    ,CNJK(L"为什么阿贝多老师成绩这么好？", L"因为他是贝多分（贝多芬）")
    ,CNJK(L"甘雨吃什么水果会马上消失？", L"杨梅（羊没）")
    ,CNJK(L"为什么不能握闲云的手？", L"因为那个是仙人掌！")
    ,CNJK(L"璃月人为什么都靠右走？", L"因为帝君保右（保佑）")
    ,CNJK(L"为什么大家都喜欢买璃月的快递？", L"因为璃月有帝君包邮（保佑）")
    ,CNJK(L"为什么迪卢克老爷这么有钱？", L"因为他是“挣亿人”（正义人）")
    ,CNJK(L"魈宝吃什么可以跳得很高？", L"仙跳强（墙）")
    ,CNJK(L"行秋写的诗歌要交给谁送到出版社？", L"给公子，因为诗要鸭运（押韵）")
    ,CNJK(L"荒泷一斗给谁画肖像画会很难看？", L"芙宁娜，因为这是鬼画芙（鬼画符）")
    ,CNJK(L"为什么那维莱特滋水的时候不消耗体力？", L"因为滋滋不倦（孜孜不倦）")
    ,CNJK(L"为什么那维莱特天赋叠满会发光？", L"因为他是那维light")
    ,CNJK(L"班尼特为什么不敢去枫丹？", L"因为枫丹有刺霉会（刺玫会）")
    ,CNJK(L"赛诺打了提纳里一下，为什么赛诺受伤了？", L"因为打到反射狐了（反射弧）")
    ,CNJK(L"雷神睡觉的时候害怕哪个角色？", L"吓沃雷（夏沃蕾）")
    ,CNJK(L"为什么美露莘要经常洗脖子？", L"因为蓝脖积泥（兰博基尼）")
    ,CNJK(L"为什么重云和申鹤不能坐同一条船", L"因为有姨的小船说翻就翻")
    ,CNJK(L"可莉炸飞了骗骗花，为什么被琴团长抓起来了？", L"因为这是“炸骗”行为！")
    ,CNJK(L"哪个角色最不适合说冷笑话？", L"魈宝，因为魈话不凉（消化不良）")
    ,CNJK(L"为什么艾尔海森打牌老是输？", L"因为他是大“输”记官（大书记官）")
    ,CNJK(L"莫娜最害怕哪个任务？", L"黄金梦乡，因为那是杀莫书（沙漠书）")
    ,CNJK(L"为什么大家都不敢和魈宝握手？", L"因为那是仙人掌")
    ,CNJK(L"为什么锅巴选了香菱做主人？", L"因为她们俩投缘（头圆）")
    ,CNJK(L"为什么芙宁娜和仆人一起走路会突然趴下？", L"因为这是仆芙前进（匍匐前进）")
    ,CNJK(L"心海看到谁会躲起来？", L"温迪，因为他喜欢摸鱼")
    ,CNJK(L"璃月人为什么不用种树？", L"因为有仙人来栽（哉）")
    ,CNJK(L"迪卢克和凯亚谁的腿比较长？", L"迪卢克腿长，因为他是火系，火腿长（火腿肠）")
    ,CNJK(L"阿贝多画哪个角色一笔就能完成？", L"魈宝，因为一笔勾魈（销）")
    ,CNJK(L"为什么公子背字母表只能背到AB？", L"因为公子踩到迟滞之水了！还在等CD！（CD：技能冷却）")
    ,CNJK(L"妮露沾了水为什么会变硬", L"因为变成水泥路了（水妮露）")
    ,CNJK(L"璃月人晚上看到谁容易饿？", L"魈宝，因为晚上看到魈就是看到夜魈（夜宵）")
    ,CNJK(L"提纳里和赛诺出去吃饭，一般是谁来买单？", L"赛诺，因为诺诺大方（落落大方）")
    ,CNJK(L"为什么赛诺考试总是拿满分？", L"因为赛诺擅长“审”题，所以题目把答案都招了！")
    ,CNJK(L"提瓦特哪个角色最不孤独？", L"卡维，他是建筑师，有游标卡尺，游标卡尺不估读（孤独）")
    ,CNJK(L"觉得赛诺说冷笑话很冷怎么办？", L"去墙角，那里有90度")
    ,CNJK(L"空哥和甘雨去便利店，为什么空哥被打了，甘雨没事", L"因为便利店24小时不打羊（打烊）")
    ,CNJK(L"为什么提瓦特没有蚊子？", L"因为水神没了，只剩六个神了，六神（花露水）可以驱蚊")
    ,CNJK(L"为什么冰系角色不怕赛诺讲冷笑话？", L"因为冰美式（没事）")
    ,CNJK(L"从前有一只鹿，是公的，它跑了起来，越跑越快，最后它变成了高速公鹿（路）", NULL)
    ,CNJK(L"如果让荧妹“拍蒙”了派蒙，派蒙会变成什么？", L"“荧击”食品")
    ,CNJK(L"为什么说宵宫的爸爸也是花羽会的绒翼龙？", L"因为宵宫爸爸“容易聋”")
    ,CNJK(L"杜林的一天为什么非常长？", L"因为度（杜）日如年")
    ,CNJK(L"请问杜林的家叫什么？", L"杜甫（府）")
    ,CNJK(L"欧洛伦和奶奶组队，算不算孙子冰法", NULL)
    ,CNJK(L"丘丘人穿着行秋一样的衣服，简称行丘", NULL)
    ,CNJK(L"玛拉妮拉着妮露去悠悠度假村，简称“玛”拉“妮”", NULL)
    ,CNJK(L"沃雅妮莎的老师拦住了奥黛塔，简称雅师拦黛", NULL)
    ,CNJK(L"为什么菲林斯晚上经过丛林不怕被袭击？", L"因为他本来就是“飞”“林”斯，直接就能飞过去")
    ,CNJK(L"法尔伽捏住了菲林斯，简称法“捏”斯", NULL)
    ,CNJK(L"为什么我们还不知道愚人众的第十席是谁？", L"因为第十席还在“实习”")
    ,CNJK(L"为什么冰系角色不怕可莉扔的炸弹？", L"因为“冰”不厌“炸”")
    ,CNJK(L"为什么妮露跳完了花神之舞就会变得看着像个植物？", L"因为花神之舞就是“化身植物”")
    ,CNJK(L"白马仙人吃了水煮黑背鲈之后辣地呜呜流泪，简称“辣呜马”", NULL)
    ,CNJK(L"为什么欧洛伦当了爸爸的同时也会当哥哥？（提示：“哥哥”用韩语怎么说？）", L"因为欧洛伦当了爸爸，简称“欧爸”（오빠）")
    ,CNJK(L"为什么我一看见瑞希小姐就想笑呢？",L"因为近“貘”者“嘿”")
    ,CNJK(L"你知道为什么瑞希给病人治疗时会让他们躺在枕头上吗？",L"因为她是心理“枕”疗师")
    ,CNJK(L"为什么在菈乌玛的地盘交易石榴，要被砍头？",L"因为那是“鹿”“易”石榴")
    ,CNJK(L"你知道为什么愚人众“队长”坐在了纳塔吗？",L"因为至冬没做（座）")
};

int cynoJoke(void)
{
    int index=0,current;
    
    while(1) {
        const CynoJoke* joke;
        CLS;
        srand((unsigned int)time(NULL));
        index=rand()%jokeCount;
        joke=&CynoJokes[index];
        putws(joke->question);
        ENDL;
        if(joke->answer!=NULL) {
#if 0 // 指定是否需要按任意键才能显示答案，这里设定不需要按就直接显示答案
            puts(
#ifdef _WIN32
                "Press any key to see the answer..."
#else
                "Press ENTER to see the answer..."
#endif
            );
            PAUSE;
#endif
            putws(joke->answer);
            ENDL;
        }
        puts(
#ifdef _WIN32
            "Press SPACE or ENTER to continue, or other thing to go back..."
#else
            "Press ENTER to continue, or type other key and then press ENTER to go back..."
#endif
        );
        ENDL;
        current=getch();
        if(current==EOF||
#ifdef _WIN32
            current==' '||
#endif
            current=='\r'||current=='\n') {
            continue;
        }
        break;
    }
    return 0;
}

void getCynoJokesCount(void) {
    jokeCount=(int)(sizeof(CynoJokes)/sizeof(CynoJokes[0]));
}