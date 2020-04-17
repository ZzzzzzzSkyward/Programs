/*
题目：河宽l,河里有n块石头，有一定间距。现移除至多m块石头，求所能达到的最大最小距离d。
定义【最大最小距离】为【所有石头间最小的距离】的最大值。
输入：l n m 数列
输出：d
解答：以d为变量，运用二分查找。下界为所有石头间最小的距离（不妨设为0，不影响结果）；上界为l（假设m==n,全部移除）。用一个变量ans存储找到的可行解。
每次查找时判断是否可行：遍历所有石头，如果间距大于等于查找值，则可以跳过，否则应当移除当前石头直到距离足够，或者已经到达对岸（此时也不应该增加计数）。判断计数是否符合搬走的石头的最大值要求。
*/
#include <stdio.h>
#include <stdlib.h>
int* arr;
int d,n,i,k,lo,hi,ans,mid,j;
int main()
{
	scanf("%d%d%d",&hi,&d,&n);
	d+=2;
	arr=(int*)malloc(sizeof(int)*d);
	arr[d-1]=hi;
	arr[0]=0;
	for(i=1;i<=d;i++) scanf("%d",arr+i);
	lo=0;
	mid=(lo+hi)>>1;
	while(lo<=hi){
	//printf("%d %d %d\n",lo,mid,hi);
		j=k=0;
		i=1;
		while(i<=d){
			if(arr[i]-arr[j]>=mid){
				k++;
				j=i;
			}
			i++;
		}
		if(k>(d-n-2)){
			lo=mid+1;
			ans=mid;
		}
		else hi=mid-1;
		mid=(hi+lo)>>1;
	}
	printf("%d",ans);
	return 0;
}
