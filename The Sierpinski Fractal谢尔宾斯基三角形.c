/*
The Sierpinski Fractal谢尔宾斯基三角形
来源：OpenJudge,Ulm Local 2002
语言：c
输入：迭代次数n
输出：用字符/\与_表示的三角形
*/
#include<stdio.h>
#include<string.h>
char* mp;
void print(int w){
	int i,j;
	for(i=w/2-1;i>=0;i--){
		for(j=w-1;j>=0;j--){
			printf("%c",mp[i*w+j]);
		}
		printf("\n");
	}
}
int main(){
	int n,i,j,w,t,s;
	while(scanf("%d",&n)){
		if(n==0) break;
		t=1;
		w=4;
		while(t++<n) w*=2;
		mp=new char[w*w/2+1];
		memset(mp,' ',w*w/2*sizeof(char));
		t=1;
		s=4;
		strcpy(mp,"\\__/");
		strcpy(mp+w," \\/ ");
		while(t<n){
			for(i=0;i<s;i++){
				for(j=0;j<s/2;j++){
					mp[w*s/2+s/2+i+j*w]=mp[s+i+j*w]=mp[i+j*w];
				}
			}
			s*=2;
			t++;
		}
		print(w);
		delete mp;
	}
	return 0;
}
