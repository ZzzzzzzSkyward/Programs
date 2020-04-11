/*
问题描述：把所有灯熄灭。
输入：一个矩阵(0,1)，这里是5*6的矩阵
输出：一个矩阵(0,1)
操作：1-将上下左右与自己这盏灯的状态改变，0-不变。
思想：深搜剪枝，退化为循环。用一个数（因为int有32位，能够表示2^5种操作）表示一种可能的矩阵，先固定第一行，然后对之后每一行的操作进行分析，发现之后每一行只可能有一种操作余地。于是完成矩阵填充后检查最后一行即可。这种方法无法得到最值（最大/小操作次数），仍然需要遍历。
*/
#include<stdio.h>
char arr[30];
char chg[30];
int main(){
	int i,j,t;
	unsigned int c;
	for(i=0;i<30;i++) scanf("%d",arr+i);
	for(c=0;c<64;c++){
		for(i=0;i<6;i++){
			chg[i]=(c>>i)&1;
		}
		for(i=0;i<4;i++){
			for(j=0;j<6;j++){
				t=arr[i*6+j]^chg[i*6+j];
				if(j) t^=chg[i*6+j-1];
				if(i) t^=chg[i*6-6+j];
				if(j<5) t^=chg[i*6+j+1];
				chg[i*6+6+j]=t;
			}
		}
		j=0;
		while(j<6){
			t=arr[24+j]^chg[24+j]^chg[18+j];
			if(j) t^=chg[24+j-1];
			if(j<5) t^=chg[24+j+1];
			if(t) break;
			else j++;
		}
		if(j==6) break;
	}
	for(i=0;i<5;i++){
		printf("%d %d %d %d %d %d\n",chg[i*6],chg[i*6+1],chg[i*6+2],chg[i*6+3],chg[i*6+4],chg[i*6+5]);
	}
	return 0;
}
