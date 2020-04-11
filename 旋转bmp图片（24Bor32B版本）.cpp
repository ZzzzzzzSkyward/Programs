/*
输入：[空格](0,)[文件名（包括后缀.bmp）][空格](1,)[输出文件名]*(0,)[换行符|文件结束标志]
      文件名不能包括空格，必须属于ASCII字符集；如果要包含空格，请用引号引起来。
      默认是相对路径，可以是绝对路径。
      此文件必须是24位或32位BMP/DIB文件，不能是16位或更少的。
例如：   “my image.bmp”   out.bmp
输出：顺时针旋转后的bmp格式图片
控制：删去第12行将不显示消息；第十四行的1修改为0将逆时针旋转
*/
#include <iostream>
#include<cstring>
#define DEBUG 1
#pragma pack(2)
#define clockwise 1
using namespace std;
int arr[5][3]={{1,2,3},{4,5,6},{7,8,9},{10,11,12},{13,14,15}};
int brr[3][5];
char input[1000]={0};
char output[1000]={0};
typedef unsigned short int bmpType;  //2B
typedef unsigned       int bmpSize;
typedef unsigned       int bmpReserved; //0
typedef unsigned       int bmpHeadOffset;
typedef unsigned       int bmpHeadSize;
typedef                int bmpWidth;
typedef                int bmpHeight;   //可以为负数
typedef unsigned short int bmpPlane;    //1
typedef unsigned short int bmpBit;
typedef unsigned       int bmpCompressionType;
#define RGB                0
#define BLE8               1
#define BIE4               2
#define BITFIELD           3
typedef unsigned       int bmpImageSize;
typedef                int bmpXppm;
typedef                int bmpYppm;
typedef unsigned       int bmpUsedColor;
typedef unsigned       int bmpImportantColor;
struct ColorRGB{
                       char blue;
                       char green;
                       char red;
};
ColorRGB *input24Data,*output24Data;
struct ColorRGBA{
                       char blue;
                       char green;
                       char red;
                       char alpha;
};
ColorRGBA *input32Data,*output32Data;
struct bmpFile{
    bmpType type;
    bmpSize size;
    bmpReserved zero;
    bmpHeadOffset headoffset;
    bmpHeadSize headsize;
    bmpWidth width;
    bmpHeight height;
    bmpPlane one;
    bmpBit depth;
    bmpCompressionType compression;
    bmpImageSize imgSize;
    bmpXppm xppm;
    bmpYppm yppm;
    bmpUsedColor used;
    bmpImportantColor important;
}
    inputFile,outputFile;
bool readIn(char fileLocation[],bmpFile* file){
    FILE *fp=fopen(fileLocation,"rb");
    if(0==fp){
        cout<<"The file "<<fileLocation<<" was not opened"<<endl;
        return 0;
    }
    fread(file,sizeof(*file),1,fp);
    #ifdef DEBUG
    cout<<"----this is the information about thie image----"<<endl;
    cout<<"width="<<file->width<<endl;
    cout<<"height="<<file->height<<endl;
    cout<<"depth="<<file->depth<<endl;
    cout<<"size="<<file->size<<endl;
    cout<<"imageSize="<<file->imgSize<<" as with estimated "<<file->depth*file->width*file->height<<endl;
    cout<<"------------------------------------------------"<<endl;
    #endif
    if(file->depth==24){
	    input24Data=new ColorRGB[file->width*file->height];
	    fread(input24Data,sizeof(char)*3,file->width*file->height,fp);    
	}
    else if(file->depth==32){
	    input32Data=new ColorRGBA[file->width*file->height];
        fread(input32Data,sizeof(char)*4,file->width*file->height,fp);
    }
    fclose(fp);   //关闭文件
    return 1;
}
template<typename t>
bool writeIn(char fileLocation[],bmpFile* file,t* data){
    FILE *fp=fopen(fileLocation,"wb");
    if(fp==0){
		cout<<"The file "<<fileLocation<<" was not created"<<endl;
        return 0;
    }
    fwrite(file,sizeof(*file),1,fp);
    fwrite(data,sizeof(char)*(file->depth/8),file->width*file->height,fp);
    fclose(fp);   //关闭文件
    return 1;
}
bool dataTransfer(bmpFile* input,bmpFile* output){
    #ifdef DEBUG
    cout<<"dataTransfer!"<<endl;
    #endif
    memcpy(output,input,sizeof(*input));
    output->width=input->height;
    output->height=input->width;
    return 1;
}
bool cmd(){
    int i=0,p=0;
    char *command;
    char useComma=0;
    command=new char[1000];
    while(1){
        command[i] = getchar();
        if (command[i] == '\n') break;
        else if(command[i]==EOF) break;
        i++;
    }
    #ifdef DEBUG
    cout<<"your command line is:"<<command<<endl;
    #endif
    for(i=0;i<1000;i++){
        if(command[i]!=' ') break;
    }
    if(command[i]=='"'||command[i]=='\''){useComma=command[i++];}
    for(;i<1000;i++){
        if(command[i]==useComma) break;
        else if((!useComma)&&(command[i]==' ')) break;
        input[p++]=command[i];
    }
    input[p]=0;
    for(i++;i<1000;i++){
        if(command[i]!=' ') break;
    }
    if(command[i]=='"'||command[i]=='\''){useComma=command[i++];}
    else{useComma=0;}
    for(p=0;i<1000;i++){
        if(command[i]==useComma) break;
        else if((!useComma)&&(command[i]==' ')) break;
        else if(command[i]=='\n') break;
        output[p++]=command[i];
    }
    output[p]=0;
    if(0==p){return 0;}
    #ifdef DEBUG
    cout<<"your input  file name is:"<<input<<endl;
    cout<<"your output file name is:"<<output<<endl;
    #endif
    return 1;
}
template<typename t>
void rotate(t *src,t *dest,int width,int height){
    #ifdef DEBUG
    cout<<"rotation!"<<endl;
    cout<<"width:"<<width<<endl;
    cout<<"height:"<<height<<endl;
    cout<<src<<endl;
    cout<<dest<<endl;
    #endif
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
        	if(!clockwise){
        	//cout<<"("<<i<<','<<j<<")->("<<j<<','<<(height-i-1)<<")\n";//逆时针
            		dest[j*height+height-i-1]=src[i*width+j];//逆时针
		}
			dest[(width-j-1)*height+i]=src[i*width+j];//顺时针
        }
    }
}
template<typename t>
void printmap(t *map,int width,int height){
    int max_len=0,max=2;
    for(int i=0;i<width*height;i++){if(max_len<map[i]){max_len=map[i];}}
    while(max_len/=10){max++;}
    cout<<"-----printing map----"<<endl;
    for(int i=0;i<height;i++){
        cout.width(max);
        cout<<map[i*width];
        for(int j=1;j<width;j++){
           // cout<<' ';
            cout.width(max);
            cout<<map[i*width+j];
        }
        cout<<endl;
    }
    
}
int main()
{
	char str[2];
    cmd();//读入输入输出文件名
    readIn(input,&inputFile);//读入bmp文件
    if(inputFile.depth==24){
    	output24Data=new ColorRGB[inputFile.height*inputFile.width];
	}else if(inputFile.depth==32){
		output32Data=new ColorRGBA[inputFile.height*inputFile.width];
	}
    #ifdef DEBUG
    cout<<"rotate will start,press anything to continue"<<endl;
    cin>>str;
    #endif
    dataTransfer(&inputFile,&outputFile);//复制头文件
    if(inputFile.depth==24){
    	rotate(input24Data,output24Data,inputFile.width,inputFile.height);//顺时针旋转矩阵
    	writeIn(output,&outputFile,output24Data);//写入bmp文件
    }
    else if(inputFile.depth==32){
    	rotate(input32Data,output32Data,inputFile.width,inputFile.height);//顺时针旋转矩阵
		writeIn(output,&outputFile,output32Data);//写入bmp文件
	}
	/*
    printmap(&arr[0][0],3,5);
    rotate(&arr[0][0],&brr[0][0],3,5);
    printmap(&brr[0][0],5,3);
    */
	return 0;
}