#include<iostream>
#include<string.h>
#include<vector>
#include<map>
#define RED    0
#define BLUE   1

#define SWORD  0
#define BOMB   1
#define ARROW  2

#define DRAGON 0
#define NINJA  1
#define ICEMAN 2
#define LION   3
#define WOLF   4
using namespace std;
struct timerStructure{
	//初始化
	void init();

	//变量
	int minute;       //分钟
	int hour;         //小时
	int max;          //计算的最大时长
	bool isFinished=0;//是否已经达到最大时长 

	//方法
	void executeEvent();//在每个小时里将要发生的事
	void print(){       //时间格式：001:22 这样子的 
		printf("%03d:%02d ",hour,minute);
	}
	bool checkFinished();
	//事件 
	void icemanMelt(int id);    //iceman前进 
	void lionFlee(int id);      //lion逃跑 
	void report(int id);        //武士报告自身情况 
	void dragonCheer(int id,int city);//dragon欢呼 
	void reach(int city);       //到达司令部 
} timer;
void timerStructure::init(){
	hour=0;
	minute=0;
	isFinished=0;
}
struct mapStructure{
	//初始化
	void init();
	//变量存储
	int total;                 //城市数目 
	int placeholder[2]={-1,-1};//为了防止武士占领敌营后无所事事，所以请到这里来玩耍，要求是每次报告情况时出席就行 
	//方法
	void record(int city,int color);           //记录武士战斗历史并改旗易帜 
	void march();                              //行军 
	void removeWarrior(int id);                //注销战士
	void removeWarrior(int cityNumber,int id); //还是注销战士
	void addWarrior(int cityNumber,int id);    //注册战士
} battlefield;
struct cityType{
	int number;              //城市编号 
	int color=0;             //城市的旗帜颜色，red=0,blue=1,none=2 
	int counter=1;           //武士战斗的历史记录，red=0,none=1,blue=2
	int warriors[2]={-1,-1}; //城中武士的id 
	int bonus[2]={0,0};      //奖励暂存
	int health=0;            //生命元数目 
};
vector<cityType> cities;    //城市群 
struct weaponType{
	int id=-1;
	int type=-1;
	int duration=-1;
	int damage=-1;
	int use(){
		if(type==SWORD){
			damage=(damage*4)/5;
			duration=damage;    //响应号召，钝化的武器扔掉 
		}
		else duration--;
		return duration; 
	}
};
struct warriorType{
	int id=-1;
	int number=-1;     //编号 
	bool color=0;      //颜色 
	int type=-1;       //品种 
	int health=-1;     //血量 
	int damage=-1;     //攻击 
	int loyalty=-1;    //忠诚，但是，在iceman上用作计步器 
	double morale=-1.0;//士气
	char information[20];//暂存状态 
	vector<weaponType> weapons;
	void arm(weaponType* oneWeapon){
		if(oneWeapon->duration>0)
		weapons[oneWeapon->type]=(*oneWeapon);
	}
	void unarm(weaponType* oneWeapon){
		oneWeapon->id=-1;
	}
	char* info();//输出自身信息 
};
map<int,warriorType> storage;//武士仓库 
struct weaponStructure{
	//初始化
	void init() ;
	
	//常量存储 
	int initialDamage[3];//初始伤害
	char weaponName[3][6]={"sword","bomb","arrow"};//武器名称
	int id;
	int arrowDamage;
	//方法 
	weaponType* create(int weapon_type,int warrior_type);//制造武器，返回id 
} weapon;
void weaponStructure::init(){
	//初始化
	id=0;
}
struct warriorStructure{
	//初始化 
	void init();
	
	//缩写
	typedef map<int,warriorType>::iterator warritor;//自造词 
	
	//生成与销毁
	int create(int color,int type,int health);//制造一个武士并返回id
	void die(int id);                         //通知处理死亡武士
	
	//战斗 
	int calcDamage(const warriorType*,const weaponType*);  //计算先手伤害 
	int calcBackfire(const warriorType*,const weaponType*);//计算反攻伤害 
	
	//输出 
	char colorName[2][5]={"red","blue"};                             //颜色名称
	char warriorName[5][7]={"dragon","ninja","iceman","lion","wolf"};//战士名称 
	
	//变量存储 
	int warriorCount[2]={-1,-1};//战士计数器 
	int weaponCount;            //武器计数器
	int id;                     //id计数器 
	
	int initialHealth[5];//初始化生命 
	int initialDamage[5];//初始攻击力 
	int loyaltyReduction;//忠诚度减小 

} warrior;
char* warriorType::info(){
		sprintf(information,"%s %s %d",warrior.colorName[color],warrior.warriorName[type],number);
		return information; 
}
weaponType* weaponStructure::create(int weapon_type,int warrior_type){
	weaponType* aWeapon=new weaponType;
	aWeapon->id=id++;
	aWeapon->type=weapon_type;
	     if(weapon_type==SWORD){
			aWeapon->damage=warrior.initialDamage[warrior_type]/5;
	 		aWeapon->duration=aWeapon->damage;
	}
	else if(weapon_type==BOMB ){
			aWeapon->duration=1;
	}
	else if(weapon_type==ARROW){
			aWeapon->duration=3;
			aWeapon->damage=weapon.arrowDamage;
	}
	return aWeapon; 
}
void warriorStructure::init(){
	//变量初始化 
	warriorCount[0]=warriorCount[1]=1;
	weaponCount=id=0;
	
	//清空
	storage.clear();
	
	//读取 
	int i;
	for(i=0;i<5;i++){
		scanf("%d",&initialHealth[i]);
	}
	for(i=0;i<5;i++){
		scanf("%d",&initialDamage[i]);
	}
}
int warriorStructure::create(int colors,int type,int remainingHealth){
	//基础 
	warriorType* aWarrior=new warriorType;
	aWarrior->number=warriorCount[colors]++;
	aWarrior->id=id++;
	aWarrior->type=type;
	aWarrior->damage=initialDamage[type];
	aWarrior->color=colors;
	aWarrior->health=initialHealth[type];
	
	//特性
	     if(type==LION)   aWarrior->loyalty=remainingHealth;
	else if(type==ICEMAN) aWarrior->loyalty=0;
	else if(type==DRAGON) aWarrior->morale =(double)remainingHealth/(double)initialHealth[DRAGON];
	
	//武器
	for(int i=0;i<3;i++){
		weaponType* www=new weaponType;
		aWarrior->weapons.push_back(*www);
	}
	switch(type){
		case DRAGON:
		case ICEMAN:{
			weaponType* oneWeapon=weapon.create(aWarrior->number%3,aWarrior->type);
			aWarrior->arm(oneWeapon);
			break;
		}
		case NINJA:{
			weaponType* oneWeapon=weapon.create(aWarrior->number%3,aWarrior->type);
			weaponType* anotherWeapon=weapon.create((aWarrior->number+1)%3,aWarrior->type);
			aWarrior->arm(oneWeapon);
			aWarrior->arm(anotherWeapon);
			break;
		}
	}
	
	//入库 
	storage[aWarrior->id]=*aWarrior;
	
	//地图上注册
	battlefield.addWarrior(colors==RED?0:(battlefield.total+1),aWarrior->id);
	
	return aWarrior->id;
};
void warriorStructure::die(int id){
	warritor iter=storage.find(id);
	iter->second.weapons.clear();
	storage.erase(iter);
	battlefield.removeWarrior(id);
}
int warriorStructure::calcDamage(const warriorType* theWarrior,const weaponType* theWeapon){
	int basicDamage     =theWarrior->damage,
		additionalDamage=theWeapon ->damage;
	if(theWeapon->id==-1) additionalDamage=0;
	return basicDamage+additionalDamage;
}
int warriorStructure::calcBackfire(const warriorType* theWarrior,const weaponType* theWeapon){
	int basicDamage		=theWarrior->damage,
		additionalDamage=theWeapon ->damage;
	if(theWeapon->id==-1) additionalDamage=0;
	return basicDamage/2+additionalDamage;
}
void mapStructure::init(){
	//清空
	cities.clear();
	placeholder[0]=placeholder[1]=-1;	
	//增加两个大本营
	cityType*  redHeadquarter=new cityType;
	 redHeadquarter->number=0;
	 redHeadquarter ->color=RED;
	cities.push_back(*redHeadquarter);
	//增加城市
	for(int i=1;i<total+1;i++){
		cityType* another=new cityType;
		another->number=i;
		cities.push_back(*another);
	}
	cityType* blueHeadquarter=new cityType;
	blueHeadquarter->number=total+1;
	blueHeadquarter ->color=BLUE;
	cities.push_back(*blueHeadquarter);
}
void mapStructure::march(){
	int i,j,warriorId=-1,
		tempId1=-1,
		tempId2=cities[0].warriors[RED];
	////首先处理红方司令部 
	//恢复司令部空位 
	cities[0].warriors[RED]=-1;
	//拉入蓝武士 
	cities[0].warriors[BLUE]=cities[1].warriors[BLUE];
	//判断蓝武士是否到达司令部
	warriorId=cities[0].warriors[BLUE];
	if(warriorId!=-1){
		//如果是iceman 
		if(storage[warriorId].type==ICEMAN) timer.icemanMelt(warriorId);
		//请进 
		placeholder[RED]=warriorId;
		//通知到达 
		timer.reach(0);
	}
	////其次遍历地图 
	for(i=1;i<total+1;i++){
		//转移 
		tempId1=cities[i].warriors[RED];
		cities[i].warriors[RED]=tempId2;
		tempId2=tempId1;
		
		cities[i].warriors[BLUE]=cities[i+1].warriors[BLUE];
		//判断武士是否移动了
		for(j=0;j<2;j++){
			warriorId=cities[i].warriors[j];
			if(warriorId!=-1){
				//输出信息
				timer.print();
				if(storage[warriorId].type==ICEMAN) timer.icemanMelt(warriorId);
				printf("%s marched to city %d with %d elements and force %d\n",
						storage[warriorId].info(),i,storage[warriorId].health,
						storage[warriorId].damage);
			}
		}
	}
	////最后处理蓝方司令部 
	cities[total+1].warriors[BLUE]=-1;
	cities[total+1].warriors[RED]=tempId2;
	if(cities[total+1].warriors[RED]!=-1){
		if(storage[cities[total+1].warriors[RED]].type==ICEMAN) timer.icemanMelt(cities[total+1].warriors[RED]);
		placeholder[BLUE]=cities[total+1].warriors[RED];
		timer.reach(total+1);
	}
}
void timerStructure::icemanMelt(int id){
	storage[id].loyalty++;
	if(storage[id].loyalty==2){
		storage[id].loyalty=0;
		storage[id].health-=9;
		storage[id].damage+=20;
		if(storage[id].health<=0) storage[id].health=1;
	}
}
void mapStructure::removeWarrior(int id){
	int i,color;
	for(i=0;i<total+2;i++){
	for(color=0;color<2;color++){
		if(cities[i].warriors[color]==id){
			cities[i].warriors[color]=-1;
			return;
		}
	}
	}
}
void mapStructure::addWarrior(int cityNumber,int id){
	cities[cityNumber].warriors[storage[id].color]=id;
}
struct headquarterType{
	//初始化
	void init(bool color);
	
	//变量 
	int color;
	int health=0;
	int index=0;
	bool isConquered=0;//有没有第一位敌人入住 
	int order[5];
	//方法
	void produce();//生产战士 
};
headquarterType  headquarter[2];
void headquarterType::init(bool colors){
	isConquered=0;
	index=0;
	color=colors;
	if(color==RED){
		order[0]=ICEMAN;
		order[1]=LION;
		order[2]=WOLF;
		order[3]=NINJA;
		order[4]=DRAGON;
	}
	else{
		order[0]=LION;
		order[1]=DRAGON;
		order[2]=NINJA;
		order[3]=ICEMAN;
		order[4]=WOLF;
	}
}
void headquarterType::produce(){
	//如果不能生产 
	if(warrior.initialHealth[order[index]]>health) return;
	//否则
	health-=warrior.initialHealth[order[index]];
	int oneWarrior=warrior.create(color,order[index],health);
	//输出信息
	timer.print();
	printf("%s born\n",storage[oneWarrior].info());
	//附加信息 
		 if(order[index]==LION)   printf("Its loyalty is %d\n" ,storage[oneWarrior].loyalty);
	else if(order[index]==DRAGON) printf("Its morale is %.2f\n",storage[oneWarrior].morale );
	//指针后移 
	index++;
	index%=5;
}
struct warStructure{
	//初始化 
	void init(){
		isFinished=0;
	};
	
	//变量 
	int isFinished;//是否终止战争 
	
	//方法 
	bool getFirst(int i){//获取先手颜色 
		if(cities[i].color) return cities[i].color-1;//如果有旗帜 
		else return !(i&1);							 //否则奇偶数 
	}
	void launchWar();//开启全面战争
	void battle(int warriorA,int warriorB,int city);//进行一场战斗,A先手
	void useBomb(int warriorA,int warriorB,int cityname);//进行炸弹投掷测试，A先手 
	void checkBonus();//模拟检测奖励发放 
	void wieldArrow();//使用弓箭 
} war;
bool timerStructure::checkFinished(){//结束了吗
	isFinished=(max<(minute+hour*60));
	return isFinished||war.isFinished;
}
void warStructure::launchWar(){
	int i,color;
	int* warriorsInCity;
	for(i=1;i<battlefield.total+1;i++){
		warriorsInCity=cities[i].warriors;
		if(warriorsInCity[0]!=-1&&warriorsInCity[1]!=-1) {
			color=getFirst(i);//先手颜色 
			battle(warriorsInCity[color],warriorsInCity[!color],i);
		}
	}
	//处理死亡
	for(i=1;i<battlefield.total+1;i++){
		warriorsInCity=cities[i].warriors;
		for(color=0;color<2;color++){
			if(warriorsInCity[color]!=-1&&storage[warriorsInCity[color]].health<=0) warrior.die(warriorsInCity[color]);
		}
	}
};
void warStructure::useBomb(int warriorA,int warriorB,int cityname){
	warriorType* w[2]={&storage[warriorA],&storage[warriorB]};
	//必须双方对战 
	if(w[0]->health<=0||w[1]->health<=0) return;
	
	int damage[2]={warrior.calcDamage(w[0],&(w[0]->weapons[0])),warrior.calcBackfire(w[1],&(w[1]->weapons[0]))};
	//如果满足：有炸弹、伤害不够、被反杀三个条件 
	if(w[0]->weapons[1].id!=-1&&w[1]->health>damage[0]&&w[0]->health<=damage[1]&&w[1]->type!=NINJA){
		//使用bomb
		timer.print();
		printf("%s used a bomb and killed %s\n",
				w[0]->info(),w[1]->info()
		);
	}
	//如果满足：有炸弹、会遇害两个条件 
	else if(w[1]->health<=damage[0]&&w[1]->weapons[1].id!=-1){
		//也使用bomb 
		timer.print();
		printf("%s used a bomb and killed %s\n",
				w[1]->info(),w[0]->info()
		);
	}
	else return;
	//通知死亡 
	warrior.die(w[0]->id);
	warrior.die(w[1]->id);
	
}
void warStructure::checkBonus(){
	warriorType* w[2];
	int damage[2];
	int i,color;//city
	for(i=1;i<battlefield.total+1;i++){
		//清除上次数据
		cities[i].bonus[0]=cities[i].bonus[1]=0;
		//如果双方都有武士
		if(cities[i].warriors[0]!=-1&&cities[i].warriors[1]!=-1){
		 	w[0]=&storage[cities[i].warriors[0]];
		 	w[1]=&storage[cities[i].warriors[1]];
			//模拟战斗
			color=getFirst(i);
			damage[0]=warrior.calcDamage(w[color],&(w[color]->weapons[0]));
			damage[1]=warrior.calcBackfire(w[!color],&(w[!color]->weapons[0]));
			if(w[!color]->type==NINJA) damage[1]=0;//忍者不会反击 
			//如果主动攻击胜利
			if(w[color]->health>0&&w[!color]->health<=damage[0]){
				cities[i].bonus[color]=1;	
			}
			//如果反攻胜利 
			else if((w[!color]->health>0&&w[color]->health<=0)||(w[!color]->health>damage[0]&&w[color]->health<=damage[1])){
				cities[i].bonus[!color]=1; 
			}
		 }
	}
	//第二次遍历，决定奖励
	int& health=headquarter[BLUE].health;
	for(i=1;i<battlefield.total+1;i++){
		if(cities[i].bonus[1]&&health>=8) health-=8;
		else cities[i].bonus[1]=0;
	}
	int& health2=headquarter[RED].health;
	for(i=battlefield.total;i>0;i--){
		if(cities[i].bonus[0]&&health2>=8) health2-=8;
		else cities[i].bonus[0]=0;
	}
}
void warStructure::wieldArrow(){
	warriorType* w[2];
	int damage[2];
	int i,duration,color;
	for(i=1;i<battlefield.total+1;i++){
		//清除之前状态
		cities[i].bonus[0]=cities[i].bonus[1]=0;
	}
	for(i=1;i<battlefield.total;i++){
		if(cities[i].warriors[0]!=-1&&cities[i+1].warriors[1]!=-1){
			//能够攻击了
			w[0]=&storage[cities[i].warriors[0]];
			w[1]=&storage[cities[i+1].warriors[1]];
			if(w[0]->health>0&&w[1]->health>0){
			//互相攻击，如果有弓箭
			for(color=0;color<2;color++){
				if(w[color]->weapons[ARROW].id!=-1){
					//发射
					duration=w[color]->weapons[ARROW].use();
					//造成伤害 
					w[!color]->health-=weapon.arrowDamage;
					//表示使用了弓箭
					cities[i+color].bonus[color]=1;
					//如果报废了 
					if(duration<=0) w[color]->unarm(&w[color]->weapons[ARROW]);
					//判断是否令敌方死亡
					if(w[!color]->health<=0) cities[i+color].bonus[color]=2;//表示敌方阵亡 
				}
			}
			}
		}
	}
	for(i=1;i<battlefield.total+1;i++){
		for(color=0;color<2;color++){
			if(cities[i].bonus[color]>0){
				w[color]=&storage[cities[i].warriors[color]];
				w[!color]=&storage[cities[i-color*2+1].warriors[!color]];
				timer.print();
				printf("%s shot",w[color]->info());
				if(cities[i].bonus[color]==2) printf(" and killed %s",w[!color]->info());
				printf("\n");
			}
		}
	}
}
void warStructure::battle(int warriorA,int warriorB,int city){
	warriorType* w[2]={&storage[warriorA],&storage[warriorB]};
	//计算伤害 
	int damage[2]={warrior.calcDamage(w[0],&(w[0]->weapons[0])),warrior.calcBackfire(w[1],&(w[1]->weapons[0]))};
	//缓存lion的生命值 
	int lionHealth[2]={w[0]->health,w[1]->health};
	//存储死亡情况 
	bool isDead[2]={w[0]->health<=0,w[1]->health<=0};
	//主动攻击
	if(isDead[0]==0&&isDead[1]==0){
		//攻击
		if(w[0]->weapons[0].id!=-1) if(0>=w[0]->weapons[0].use()) w[0]->unarm(&(w[0]->weapons[0]));
		w[1]->health-=damage[0];
		//登记死亡情况
		isDead[1]=w[1]->health<=0;
		//通报情况 
		timer.print();
		printf("%s attacked %s in city %d with %d elements and force %d\n",
				w[0]->info(),w[1]->info(),city,w[0]->health,w[0]->damage
		);
		//战死
		if(isDead[1]){
			timer.print();
			printf("%s was killed in city %d\n",w[1]->info(),city);
			//lion转移 
			if(w[1]->type==LION)w[0]->health+=lionHealth[1];
		}
	}
	//被动反击
	if(w[1]->type!=NINJA&&isDead[1]==0&&isDead[0]==0){
		//攻击
		if(w[1]->weapons[0].id!=-1) if(0>=w[1]->weapons[0].use()) w[1]->unarm(&(w[1]->weapons[0]));
		w[0]->health-=damage[1];
		//登记
		isDead[0]=w[0]->health<=0;
		//通报情况 
		timer.print();
		printf("%s fought back against %s in city %d\n",
				w[1]->info(),w[0]->info(),city
		);
		//战死
		if(isDead[0]){
			timer.print();
			printf("%s was killed in city %d\n",w[0]->info(),city);
			//lion转移
			if(w[0]->type==LION) w[1]->health+=lionHealth[0];
		}
	}
	//战斗结束
	int color=2,j;
	while(color){
		color--;
		if(isDead[color]&&(!isDead[!color])){
			//i死，彼活
			//奖励
			if(cities[city].bonus[w[!color]->color]) w[!color]->health+=8;
			//加士气
			if(w[!color]->type==DRAGON){
				w[!color]->morale+=0.2; 
			//欢呼
				if(color==1) timer.dragonCheer(w[!color]->id,city);
			}
			//wolf缴获武器 
			if(w[!color]->type==WOLF){
				for(j=0;j<3;j++){
					if(w[!color]->weapons[j].id==-1&&w[color]->weapons[j].id!=-1) w[!color]->weapons[j]=w[color]->weapons[j];
				}
			}
			//武士获取生命元
			headquarter[w[!color]->color].health+=cities[city].health;
			timer.print();
			printf("%s earned %d elements for his headquarter\n",w[!color]->info(),cities[city].health);
			cities[city].health=0;
			//改旗易帜
			battlefield.record(city,w[!color]->color);
			break;
		}
	}
	if((!isDead[0])&&(!isDead[1])){
		//无计可施
		color=w[0]->color;
		//减士气
		if(w[color]->type==DRAGON){
			w[color]->morale-=0.2;
			//欢呼
			if(color==0) timer.dragonCheer(w[color]->id,city);
		}
		else if(w[color]->type==LION){
			w[color]->loyalty-=warrior.loyaltyReduction;
		}
		color=!color;
		if(w[color]->type==DRAGON){
			w[color]->morale-=0.2;
			//欢呼
			if(color==0)
			timer.dragonCheer(w[color]->id,city);
		}
		else if(w[color]->type==LION){
			w[color]->loyalty-=warrior.loyaltyReduction;
		}
	//改旗易帜
	battlefield.record(city,2);
	}
}
void timerStructure::dragonCheer(int id,int city){
	warriorType& w=storage[id];
	if(w.morale<=0.8) return;
	timer.print();
	printf("%s yelled in city %d\n",w.info(),city); 
}
void timerStructure::lionFlee(int id){
	timer.print();
	printf("%s ran away\n",storage[id].info());
	//注销 
	warrior.die(id);
}
void timerStructure::executeEvent(){
	int i,j,color;
	warriorType* w;
	int* wid;
	//0分钟，制造
	minute=0;
	if(checkFinished()) return;
	for(color=0;color<2;color++) headquarter[color].produce();
	
	//5分钟，lion逃跑
	minute=5;
	if(checkFinished()) return;
	
	//司令部的两只单独拎出来检测 
	if(cities[RED].warriors[RED]!=-1){
		w=&storage[cities[RED].warriors[RED]];
		//检测忠诚度，如果不合格 
		if(w->type==LION&&w->loyalty<=0) lionFlee(w->id);
	}
	for(i=1;i<battlefield.total+1;i++){
		for(color=0;color<2;color++){
			if(cities[i].warriors[color]!=-1){
				w=&storage[cities[i].warriors[color]];
				//检测忠诚度，如果不合格 
				if(w->type==LION&&w->loyalty<=0) lionFlee(w->id);
			}
		}
	}
	if(cities[battlefield.total+1].warriors[BLUE]!=-1){
		w=&storage[cities[battlefield.total+1].warriors[1]];
		//检测忠诚度，如果不合格 
		if(w->type==LION&&w->loyalty<=0) lionFlee(w->id);
	}
	
	//10分，武士前进
	minute=10;
	if(checkFinished()) return;
	
	battlefield.march();
	 
	//20分，城市产出10个生命元
	minute=20;
	if(checkFinished()) return;
	
	for(i=1;i<battlefield.total+1;i++) cities[i].health+=10;
	
	//30分，武士取走生命元
	minute=30;
	if(checkFinished()) return;
	
	for(i=1;i<battlefield.total+1;i++){
		color=(cities[i].warriors[RED]==-1);
		//如果只有一只武士在城里 
		if(color==(cities[i].warriors[BLUE]!=-1)){
			timer.print();
			w=&storage[cities[i].warriors[color]];
			printf("%s earned %d elements for his headquarter\n",w->info(),cities[i].health);
			headquarter[color].health+=cities[i].health;
			cities[i].health=0;
		}
	}
	
	//35分，射箭 
	minute=35;
	if(checkFinished()) return;
	
	war.wieldArrow();
	
	//38分，用bomb
	minute=38;
	if(checkFinished()) return;
	
	for(i=1;i<battlefield.total+1;i++){
		wid=cities[i].warriors;
		if(wid[0]!=-1&&wid[1]!=-1){
			//如果武士将交战
			color=war.getFirst(i);
			war.useBomb(wid[color],wid[!color],i);
		}
	}
	
	//40分钟，战斗
	minute=40;
	if(checkFinished()) return;
	
	war.checkBonus();
	war.launchWar();
	
	//50分钟，报告生命元
	minute=50;
	if(checkFinished()) return;
	
	for(color=0;color<2;color++){
		timer.print();
		printf("%d elements in %s headquarter\n",headquarter[color].health,warrior.colorName[color]);
	}
	
	//55分钟，武士报告情况
	minute=55;
	if(checkFinished()) return;
	
	for(i=1;i<battlefield.total+1;i++){
		j=cities[i].warriors[0];
		if(j!=-1) timer.report(j);
	}
	if(battlefield.placeholder[1]!=-1) timer.report(battlefield.placeholder[1]);
	if(battlefield.placeholder[0]!=-1) timer.report(battlefield.placeholder[0]);
	for(i=1;i<battlefield.total+1;i++){
		j=cities[i].warriors[1];
		if(j!=-1) timer.report(j);
	}
	
	//60分 
	hour++;
}
void timerStructure::report(int id){
	bool commaNeeded=0;
	warriorType& w=storage[id];
	timer.print();
	printf("%s has ",w.info());
	bool weaponCount[3]={w.weapons[0].id+1,w.weapons[1].id+1,w.weapons[2].id+1};
	//检测是否有武器
	if(weaponCount[0]+weaponCount[1]+weaponCount[2]){
		//有武器
		if(weaponCount[ARROW]){
			printf("arrow(%d)",w.weapons[ARROW].duration);
			commaNeeded=1;
		}
		if(weaponCount[BOMB]){
			if(commaNeeded) printf(",");
			printf("bomb");
			commaNeeded=1;
		}
		if(weaponCount[SWORD]){
			if(commaNeeded) printf(",");
			printf("sword(%d)",w.weapons[SWORD].damage);
		}
	}
	//没有
	else printf("no weapon"); 
	printf("\n");
}
void timerStructure::reach(int city){
	warriorType& w=storage[cities[city].warriors[!city]];
	timer.print();
	printf("%s reached %s headquarter with %d elements and force %d\n",
			w.info(),warrior.colorName[!w.color],w.health,w.damage
	);
	if(headquarter[!!city].isConquered){
		war.isFinished=1;
		timer.print();
		printf("%s headquarter was taken\n",warrior.colorName[!w.color]);
	}
	headquarter[!!city].isConquered=1;
}
void mapStructure::record(int city,int history){
	cityType& c=cities[city];
	int futureColor=c.color;
	//history:red=0,blue=1,none=2
	//color  :red=1,blue=2,none=0
	//counter:red1=0,none=1,blue1=2
	if(history==RED){
		if(c.counter==0) futureColor=1;
		else             c.counter=0;
	}
	else if(history==2)  c.counter=1;
	else{
		if(c.counter==2) futureColor=2;
		else             c.counter=2;
	}
	//改旗易帜 
	if(futureColor!=c.color){
		c.color=futureColor;
		timer.print();
		printf("%s flag raised in city %d\n",warrior.colorName[c.color-1],city);
	}
}
int main(){
	//输入样例数目 
	int n,i=0;
	scanf("%d",&n);
	//循环 
	while(i<n){
		i++;
		//第一行： 
		printf("Case %d:\n",i);
		//输入
		scanf("%d%d%d%d%d",
				&headquarter[RED].health,
				&battlefield.total,
				&weapon.arrowDamage,
				&warrior.loyaltyReduction,
				&timer.max
		);
		headquarter[BLUE].health=headquarter[RED].health;
		//初始化
		headquarter[RED].init(RED);
		headquarter[BLUE].init(BLUE);
		warrior.init();
		weapon.init();
		battlefield.init();
		timer.init();
		war.init();
		//进入循环事件处理
		while(!war.isFinished&&!timer.isFinished) timer.executeEvent();
	}
	return 0;
}