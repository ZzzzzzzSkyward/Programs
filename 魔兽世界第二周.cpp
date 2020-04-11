/*
语言：c++
题目：魔兽世界的西面是红魔军的司令部，东面是蓝魔军的司令部。两个司令部之间是依次排列的若干城市。 
红司令部，City 1，City 2，……，City n，蓝司令部
两军的司令部都会制造武士。武士一共有 dragon 、ninja、iceman、lion、wolf 五种。每种武士都有编号、生命值这两种属性。 
有的武士可以拥有武器。武器有三种，sword, bomb,和arrow，编号分别为0,1,2。
双方的武士编号都是从1开始计算。红方制造出来的第 n 个武士，编号就是n。同样，蓝方制造出来的第 n 个武士，编号也是n。 
不同的武士有不同的特点。
dragon 可以拥有一件武器。编号为n的dragon降生时即获得编号为 n%3 的武器。dragon还有“士气”这个属性，是个浮点数，其值为它降生后其司令部剩余生命元的数量除以造dragon所需的生命元数量。
ninja可以拥有两件武器。编号为n的ninja降生时即获得编号为 n%3 和 (n+1)%3的武器。
iceman有一件武器。编号为n的iceman降生时即获得编号为 n%3 的武器。
lion 有“忠诚度”这个属性，其值等于它降生后其司令部剩余生命元的数目。
wolf没特点。
请注意，在以后的题目里，武士的士气，生命值，忠诚度在其生存期间都可能发生变化，都有作用，武士手中的武器随着使用攻击力也会发生变化。
武士在刚降生的时候有一个生命值。 
在每个整点，双方的司令部中各有一个武士降生。 
红方司令部按照 iceman、lion、wolf、ninja、dragon 的顺序循环制造武士。 
蓝方司令部按照 lion、dragon、ninja、iceman、wolf 的顺序循环制造武士。 
制造武士需要生命元。 
制造一个初始生命值为 m 的武士，司令部中的生命元就要减少 m 个。 
如果司令部中的生命元不足以制造某个按顺序应该制造的武士，那么司令部就试图制造下一个。如果所有武士都不能制造了，则司令部停止制造武士。
给定一个时间，和双方司令部的初始生命元数目，要求你将从0点0分开始到双方司令部停止制造武士为止的所有事件按顺序输出。
一共有两种事件，其对应的输出样例如下： 
1) 武士降生 
输出样例： 004 blue lion 5 born with strength 5,2 lion in red headquarter
表示在 4点整，编号为5的蓝魔lion武士降生，它降生时生命值为5,降生后蓝魔司令部里共有2个lion武士。(为简单起见，不考虑单词的复数形式)注意，每制造出一个新的武士，都要输出此时司令部里共有多少个该种武士。
如果造出的是dragon，那么还要输出一行，例：
It has a arrow,and it's morale is 23.34
表示该dragon降生时得到了arrow,其士气是23.34（为简单起见，本题中arrow前面的冠词用a,不用an，士气精确到小数点后面2位，四舍五入）
如果造出的是ninja，那么还要输出一行，例：
It has a bomb and a arrow
表示该ninja降生时得到了bomb和arrow。
如果造出的是iceman，那么还要输出一行，例：
It has a sword
表示该iceman降生时得到了sword。
如果造出的是lion，那么还要输出一行，例：
It's loyalty is 24
表示该lion降生时的忠诚度是24。
2) 司令部停止制造武士
输出样例： 010 red headquarter stops making warriors
表示在 10点整，红方司令部停止制造武士
输出事件时： 
首先按时间顺序输出； 
同一时间发生的事件，先输出红司令部的，再输出蓝司令部的。
输入
第一行是一个整数,代表测试数据组数。
每组测试数据共两行。 
第一行，一个整数M。其含义为： 每个司令部一开始都有M个生命元( 1 <= M <= 10000) 
第二行：五个整数，依次是 dragon 、ninja、iceman、lion、wolf 的初始生命值。它们都大于0小于等于10000
输出
对每组测试数据，要求输出从0时0分开始，到双方司令部都停止制造武士为止的所有事件。
对每组测试数据，首先输出“Case:n" n是测试数据的编号，从1开始 
接下来按恰当的顺序和格式输出所有事件。每个事件都以事件发生的时间开头，时间以小时为单位，有三位。
*/
#include<string.h>
#include<stdio.h>
#define NINJA 0
#define WOLF 1
#define LION 2
#define DRAGON 3
#define ICEMAN 4
#define SWORD 0
#define BOMB 1
#define ARROW 2
#define RED 0
#define BLUE 1
#define MAX 100
#define TOTAL 5
int timeElapse=0;   
char nameOfWarriors[TOTAL][7]={"ninja","wolf","lion","dragon","iceman"};
char nameOfWeapons[3][6]={"sword","bomb","arrow"};
char nameOfHeadquarter[2][5]={"red","blue"};
int initialLife[TOTAL];
void printFinish(int);
void printInfo(const int headquarterName,const int warriorName,const int number,const int sum);
class warrior{
    public:
    //int life=0;
    char *name;
    warrior(const char *p){name=new char[strlen(p)+1];strcpy(name,p);}
    static warrior* produce(int x,int y,int z);
};
class ninja:public warrior{
    public:
    static int life;
    int weapons[2];
    void arm(const int n){weapons[0]=n%3;weapons[1]=(n+1)%3;}
    ninja(const int n,const int lifes):warrior(nameOfWarriors[NINJA]){birth(n,lifes);}
    void birth(const int n,const int lifes){
        arm(n);
        printf("It has a %s and a %s\n",nameOfWeapons[weapons[0]],nameOfWeapons[weapons[1]]);
    }
};
class wolf:public warrior{
    public:
    //int weapons[2];
    static int life;
    //void arm(int n){weapons[0]=n%3;weapons[1]=(n+1)%3;}
    wolf(const int n,const int lifes):warrior(nameOfWarriors[WOLF]){birth(n,lifes);}
    void birth(int n,int lifes){}
};
class lion:public warrior{
    public:
    int loyalty;
    static int life;
    void update(){loyalty=this->life;}
    lion(const int n,const int lifes):warrior(nameOfWarriors[LION]){birth(n,lifes);}
    void birth(int n,const int lifes){
        loyalty=lifes;
        printf("It's loyalty is %d\n",loyalty);
    }
};
class dragon:public warrior{
    public:
    int weapon;
    static int life;
    double morale=0;
    void arm(const int n,const int lifes){weapon=n%3;morale=(double)lifes/(double)life;}
    dragon(const int n,const int lifes):warrior(nameOfWarriors[DRAGON]){birth(n,lifes);}
    void birth(const int n,const int lifes){
        arm(n,lifes);
        printf("It has a %s,and it's morale is %.2f\n",nameOfWeapons[weapon],morale);
    }
};
class iceman:public warrior{
    public:
    int weapon;
    static int life;
    void arm(const int n){weapon=n%3;}
    iceman(const int n,const int lifes):warrior(nameOfWarriors[ICEMAN]){birth(n,lifes);}
    void birth(int n,int lifes){
        arm(n);
        printf("It has a %s\n",nameOfWeapons[weapon]);
    }
};
warrior* warrior::produce(int x,int n,int lifes){
    warrior* temp;
    switch(x){
        case NINJA:temp=new ninja(n,lifes);break;
        case DRAGON:temp=new dragon(n,lifes);break;
        case WOLF:temp=new wolf(n,lifes);break;
        case LION:temp=new lion(n,lifes);break;
        case ICEMAN:temp=new iceman(n,lifes);break;
    }
    return temp;
}
class headquarter{
    public:
    int color;
    int isFinished=0;
    int life=0;
    int next;
    warrior* storage[TOTAL][MAX];
    int length[TOTAL];
    int list[TOTAL];
    headquarter(const int Color,const int l):color(Color),isFinished(0),life(l){//l==life
        int i=0;
        for(;i<TOTAL;i++){
            length[i]=0;
            storage[i][0]=NULL;
        }
        next=0;
        if(color==RED){
            list[0]=ICEMAN;
            list[1]=LION;
            list[2]=WOLF;
            list[3]=NINJA;
            list[4]=DRAGON;
        }
        else if(color==BLUE){
            list[0]=LION;
            list[1]=DRAGON;
            list[2]=NINJA;
            list[3]=ICEMAN;
            list[4]=WOLF;
        }
    }
    void finish(){
        isFinished=1;
        printFinish(color);
    }
    void execCommand(){
        if(isFinished) return;
        if(produce()) finish();
    }
    bool produce(){
        int i=0;
        while(
            (i<5)&&(initialLife[list[(i+next)%TOTAL]]>life)
             ) i++;
        if(i==5) return 1;//fail
        else{
            next+=i;
            next%=TOTAL;
            life-=initialLife[list[next]];
            printInfo(color,list[next],timeElapse+1,length[next]+1);
            storage[next][length[next]]=warrior::produce(list[next],timeElapse+1,life);
            length[next]++;
            next++;
            return 0;//success
            }
    }
};
void init(){
    static int n=1;
    timeElapse=0;
    printf("Case:%d\n",n);
    n++;
}
void printInfo(const int headquarterName,const int warriorName,const int number,const int sum){
    printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",
            timeElapse,
            nameOfHeadquarter[headquarterName],
            nameOfWarriors[warriorName],
            number,
            initialLife[warriorName],
            sum,
            nameOfWarriors[warriorName],
            nameOfHeadquarter[headquarterName]
          );//000 red iceman 1 born with strength 5,1 iceman in red headquarter
}
void printFinish(int headquarterName){
    printf("%03d %s headquarter stops making warriors\n",
           timeElapse,
           nameOfHeadquarter[headquarterName]
          );
}
void readIn(){
    int x;
    scanf("%d",&x);
    dragon::life=x;
    initialLife[DRAGON]=x;
    scanf("%d",&x);
    ninja::life=x;
    initialLife[NINJA]=x;
    scanf("%d",&x);
    iceman::life=x;
    initialLife[ICEMAN]=x;
    scanf("%d",&x);
    lion::life=x;
    initialLife[LION]=x;
    scanf("%d",&x);
    wolf::life=x;
    initialLife[WOLF]=x;
}
int dragon::life=0;
int wolf::life=0;
int iceman::life=0;
int lion::life=0;
int ninja::life=0;
int main(){
    int number;
    scanf("%d",&number);
    int tempLife;
    while(number-->0){
    scanf("%d",&tempLife);
    headquarter red(RED,tempLife);
    headquarter blue(BLUE,tempLife);
        readIn();
        init();
    while(1){
        red.execCommand();
        blue.execCommand();
        if(red.isFinished&&blue.isFinished) break;
        timeElapse++;
    }
    }
    return 0;
}