#include <cstdio>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include  <vector>
#define MaxSize 20
using namespace std;
//int tape[N]={0};//纸带
//cout<<s<<end;

typedef struct usual{
	char *data;
	struct usual *next;
}usual_struct;



//typedef struct usual_struct *UsualHashTable;
typedef struct transfer_function{
	char *now_state;
	char  *now_value;
	char *next_state;
	char *change_value;
	char *direction;
	struct transfer_function*next;
}tran_fun; 

/*typedef struct TapeNode{
	int data;
	struct TapeNode *next;
}TapeNode,*TapList;

UsualHashTable InitializeTable(int TableSize);
void DestroyTable(UsualHashTable H);
*/
/*A[]是非空元素集合

R[]包含空元素的集合
Q[]是状态集合
*/
char tape[20];
int sum=0;
class Turing{
private:
	usual_struct A[MaxSize],R[MaxSize],Q[MaxSize];//是非空字符集集合，R是字符集集合，Q是状态集合
	tran_fun T[MaxSize];//状态转移函数集合
	char *start_state;
	char *accept_state;

public:
	void init();
	void readTape();
	void checkTape();
	void readSeventuple();
	void calculate(char *nowstate,int index);

	void print();
	void printHash();
	//2个hash函数
	int hash(char *key);//对普通的状态、字符集合做hash
	void insert_usualhash(char x,usual_struct hashtable[MaxSize]);
	void insert_tranfun(char nowstate,char nowvalue,char nextstate,char changevalue,char direction,tran_fun T[MaxSize]);
	bool search_usualhash(char x,usual_struct hashtable[MaxSize]);
	tran_fun *search_tranfun(char *nowstate,char *nowvalue,tran_fun T[MaxSize]);
	char *getPrivate(){return start_state;}
	//int transfer_hash(char *key1,char *key2);//对转移函数（具体来说应该是当前状态和当前值）做hash
};

void Turing::init()
{
	for(int i=0;i<MaxSize;i++)
	{
		R[i].data=NULL;
		R[i].next=NULL;
		A[i].data=NULL;
		A[i].next=NULL;
		Q[i].data=NULL;
		Q[i].next=NULL;
		T[i].now_state=NULL;
		T[i].next_state=NULL;
		T[i].now_value=NULL;
		T[i].change_value=NULL;
		T[i].direction=NULL;
		T[i].next=NULL;
	//	cout<<" i init finished"<<endl;
	}

}
int Turing::hash(char key[100])
{
//	char s[20]={'\0'};
	//sprintf(s,"%c%c",'r','p');
	//cout<<"SSS"<<s<<endl;
	unsigned long h=0,g=0;
	//cout<<"*key:"<<key<<endl;
	while(*key)
	{
		h=(h<<4)+*key;//h左移4位，当前字符ASCII存入h的低四位
		key++;
		g=h&0xF0000000L;
		if(g) //如果最高位不为0，则说明字符多余7个，如果不处理，再加第九个字符时，第一个字符会被移出
		//因此要有如下处理
		 //最高位为0，就会仅仅影响5-8位，否则会影响5-31位，因为C语言使用的算数移位
                //因为1-4位刚刚存储了新加入到字符，所以不能>>28
		{
		h^=(g>>24);
		//清空28~31f位
		h&=~g;
		}
	}
	
	h=h%MaxSize;
	return (h);
}
//usual_struct *temp1=new usual_struct;//申请了内存没释放
usual_struct *mytemp1,*mytemp2,pt;
void Turing::insert_usualhash(char x,usual_struct hashtable[MaxSize])//
{
	char s[20]={'\0'};
	sprintf(s,"%c",x);
	int pos=hash(s);
	
	//p=hashtable;
	//cout<<"pos:"<<pos<<endl;
	mytemp1=(usual_struct *)malloc(sizeof(usual_struct)); //创建新结点
	mytemp1->data=(char *)malloc(10);
	strcpy(mytemp1->data,s);
	//cout<<"temp1->data:"<<(*mytemp1->data)<<endl;
	mytemp1->next=NULL;
	if(hashtable[pos].data==NULL)	{
		hashtable[pos]=*mytemp1;
	}//如果头结点为空，则直接为头结点赋值 
	else
	{
		for(mytemp2=(hashtable+pos);mytemp2->next;mytemp2=mytemp2->next)
			mytemp2->next=mytemp1;
	}
	//temp1->next=hashtable[pos].next;
	//hashtable[pos].next=temp1;//hashtable[pos].next为头指针,将新结点插入表中
	//cout<<"out insert_usualhash:"<<endl;
	//delete []temp;
}

bool Turing::search_usualhash(char x,usual_struct hashtable[MaxSize])
{

	char s[5]={'\0'};
	sprintf(s,"%c",x);
	int pos=hash(s);
	//cout<<"pos:"<<pos<<endl;
	usual_struct *temp=(R+pos);
	while(temp->data!=NULL)
	{
		if(strcmp(temp->data,s)==0)
		{
	//		cout<<"Has been searched in usualhash table"<<endl;
			return true; //在hash表中找到了
		}		
		temp=temp->next;
	}
	return false;//在hash表中没找到
}
//tran_fun *tmp=new tran_fun;

void Turing::insert_tranfun(char nowstate,char nowvalue,char nextstate,char changevalue,char direction,tran_fun T[MaxSize])//
{
	char ns[20]={'\0'};
	char nv[20]={'\0'};
	char nes[20]={'\0'};
	char cv[20]={'\0'};
	char di[20]={'\0'};
	char *str1;
	char s1[100]={'\0'};
	sprintf(s1,"%c%c", nowstate,nowvalue);
	//strcpy(str1,s1);
	//cout<<"str:"<<str1<<endl;
	//cout<<"insert s:"<<s1<<endl;
	int pos=hash(s1);
	tran_fun *tmp1,*tmp2;
	tmp1=(tran_fun *)malloc(sizeof(tran_fun));
	//tmp=&tempstruct;
	sprintf(ns,"%c",nowstate);
	sprintf(nv,"%c",nowvalue);
	sprintf(nes,"%c",nextstate);
	sprintf(cv,"%c",changevalue);
	sprintf(di,"%c",direction);
	tmp1->now_state=(char *)malloc(10);

	strcpy(tmp1->now_state,ns);

	tmp1->now_value=(char *)malloc(10);
	strcpy(tmp1->now_value,nv);
	tmp1->next_state=(char *)malloc(10);
	strcpy(tmp1->next_state,nes);
	tmp1->change_value=(char *)malloc(10);
	strcpy(tmp1->change_value,cv);
	tmp1->direction=(char *)malloc(10);
	strcpy(tmp1->direction,di);
	tmp1->next=NULL;
	if(T[pos].now_state==NULL) 
	{	//cout<<"NULL"<<endl;
		T[pos]=*tmp1;
		//cout<<"T["<<pos<<"]:"<<T[pos].now_state<<T[pos].now_value<<T[pos].next_state<<T[pos].change_value<<T[pos].direction<<endl;
	}
	else
	{
		//cout<<"NOT NULL"<<endl;
		for(tmp2=(T+pos);tmp2->next;tmp2=tmp2->next)
			tmp2->next=tmp1;
		//T[pos].next=tmp; //将新结点插入表中
	}
}

tran_fun *Turing::search_tranfun(char *nowstate,char *nowvalue,tran_fun T[MaxSize])
{

	char s[20]={'\0'};
	sprintf(s,"%c%c",*nowstate,*nowvalue);
	int pos=hash(s);
	tran_fun *tmp=(T+pos);
	//tmp=T[pos].next;
	/*if(tmp==NULL)
		cout<<"is null"<<endl;*/
	//cout<<"tmp->now_state:"<<tmp->now_state<<endl;
	while(tmp->now_state!=NULL)
	{
		if(strcmp(tmp->now_state,nowstate)==0&&strcmp(tmp->now_value,nowvalue)==0)
		{
/*			cout<<"Has searched in tran_fun hashtable"<<endl;
			cout<<"next_state:"<<*tmp->next_state<<"change_value:"<<*tmp->change_value<<"direction:"<<*tmp->direction<<endl;
*/			
			return tmp;
		}
		tmp=tmp->next;
	}
	return NULL;//在hash表中没找到
}
void Turing::printHash()//(tran_fun T[MaxSize])
{
	usual_struct *node,*node1,*node2;
	tran_fun *node3;
	for(int i=0;i<MaxSize;i++)
	{
		node=(R+i);
		if(node->data==NULL)
			cout<<"head is NULL"<<endl;
		else
		{
			cout<<"node value:"<<i<<endl;
			cout<<(*node->data)<<endl;
			while(node->next!=NULL)
			{
				node=node->next;
			}
		}

	}
	
	for(int i=0;i<MaxSize;i++)
	{
		cout<<"Q:"<<endl;
		node1=(Q+i);
		if(node1->data==NULL)
			cout<<"head is NULL"<<endl;
		else
		{
			cout<<"node value:"<<i<<endl;
			cout<<(*node1->data)<<endl;
			while(node1->next!=NULL)
			{
				node1=node1->next;
				//cout<<"node:"<<(*node->data)<<endl;
			}
		}

	}
	for(int i=0;i<MaxSize;i++)
	{
		cout<<"A";
		node2=(A+i);
		if(node2->data==NULL)
			cout<<"head is NULL"<<endl;
		else
		{
			cout<<"node value:"<<i<<endl;
			cout<<(*node2->data)<<endl;
			while(node2->next!=NULL)
			{
				node2=node2->next;
				//cout<<"node:"<<(*node->data)<<endl;
			}
		}

	}
	
	for(int i=0;i<MaxSize;i++)
	{
		node3=(T+i);
		if(node3->now_state==NULL)
			cout<<"head is NULL"<<endl;
		else
		{
			cout<<"node value:"<<i<<endl;
			cout<<(*node3->now_state)<<" ";
			cout<<(*node3->now_value)<<" ";
			cout<<(*node3->next_state)<<" ";
			cout<<(*node3->change_value)<<" ";
			cout<<(*node3->direction)<<endl;
			while(node3->next!=NULL)
			{
				node3=node3->next;
				//cout<<"node:"<<(*node->data)<<endl;
			}
		}

	}

	//cout<<"start_state:"<<start_state<<endl;
	//cout<<"accept_state:"<<accept_state<<endl;


}
char temp;//temp是char所以ifstream是按插入读入的
char str[20][100]={'\0'};
char nowvalue,nowstate,nextstate,changevalue,direction;
char start[20]={'\0'};
void Turing::readSeventuple()
{	
	tran_fun tempstruct;
	ifstream myfile;
	int flag=0;
	myfile.open("Tuple.txt");
	if(!myfile)
	{
		cout<<"无法打开该文件"<<endl;
		exit(1);
	}
	myfile>>temp;
	while(!myfile.eof())
	{
		if(temp=='Q')
		{
			myfile>>temp;//在这儿读到的是：
			myfile>>temp;//
			while(temp!='A') //说明还没读到下一行
			{
				
				if(temp!=',')//说明这儿读到了一个词  是“且”的关系!
				{
					//insert_usualhash(Turing &turing,char *x,usual_struct (&hashtable)[MaxSize]);
					//sprintf(str[flag],"%c",temp);
					//cout<<"str:"<<str[flag][]<<" &temp:"<<&temp<<endl;
					insert_usualhash(temp,Q);
					myfile>>temp;
					flag++;
				}
				else
				{
					myfile>>temp;
				}
																
			}
		}
		else if(temp=='A')
		{
			myfile>>temp;
			myfile>>temp;
			while(temp!='R')
			{
				if(temp!=',')
				{
					//sprintf(str[flag].c_str(),"%c",temp);
					//cout<<"str:"<<str[flag].c_str()<<endl;
					insert_usualhash(temp,A);
					myfile>>temp;
					flag++;

				}
				else
					myfile>>temp;
			}
		}
		else if(temp=='R')
		{	
			myfile>>temp;
			myfile>>temp;
			while(temp!='T')
			{
				if(temp!=',')
				{
					//sprintf(str[flag].c_str(),"%c",temp);
					//cout<<"str:"<<str[flag].c_str()<<endl;
					insert_usualhash(temp,R);
					myfile>>temp;
					flag++;
				}
				else
					myfile>>temp;
			}
		}
		else if(temp=='T')
		{
			myfile>>temp;
			myfile>>temp;
			while(temp!='T'&&temp!='S')
			{
				if(temp!='\n')
				{
					//strcpy(tempstruct.now_value,&temp); //f   this always is wrong
					//tempstruct.now_state=&temp;
					//*tempstruct.now_state=temp;
					//sprintf(s1,"%c",temp);
					//cout<<"str:"<<str[flag]<<endl;
					nowstate=temp;
					myfile>>temp;  //,
					myfile>>temp;  //1
					//cout<<"temp:"<<temp<<endl;
					//strcpy(tempstruct.now_value,&temp);
					//sprintf(str[flag],"%c",temp);
					//cout<<"str:"<<str[flag]<<endl;
					nowvalue=temp;
					
					//cout<<"tempstruct.now_value"<<nowvalue<<endl;
					myfile>>temp;  //,
					myfile>>temp;  //f
					//cout<<"temp:"<<temp<<endl;
					//strcpy(tempstruct.next_state,&temp);
					//sprintf(str[flag],"%c",temp);
					//cout<<"str:"<<str[flag]<<endl;
					nextstate=temp;
					
					//cout<<"tempstruct.next_state"<<nextstate<<endl;
					myfile>>temp;  //,
					myfile>>temp;  //1
					//strcpy(tempstruct.change_value,&temp);
					//sprintf(str[flag],"%c",temp);
					//cout<<"str:"<<str[flag]<<endl;
					changevalue=temp;
					
					//cout<<"tempstruct.change_value"<<changevalue<<endl;
					myfile>>temp; //,
					myfile>>temp; //R
					direction=temp;
					//cout<<"tempstruct.direction"<<direction<<endl;
					//cout<<"tempstruct.now_value2"<<nowvalue<<endl;//前面值被后面值覆盖了
					insert_tranfun(nowstate,nowvalue,nextstate,changevalue,direction,T);
					myfile>>temp; // \n
				}
				else
				{
					myfile>>temp;
				}
			}

		}
		else if(temp=='S')
		{
			myfile>>temp;
			myfile>>temp; //temp=q0
			while(temp!='F')
			{
				if(temp!='\n')
				{
					
					sprintf(start,"%c",temp);
					start_state=start;
					myfile>>temp;  // \n
				}
				else
					myfile>>temp;

			}

		}
		else if(temp=='F')
		{
			myfile>>temp;
			myfile>>temp; //q1
			while(!myfile.eof())
			{
				if(temp!='\n')
				{
					accept_state=&temp;
					myfile>>temp;
				}
				else
					myfile>>temp;
			}
		}
	}
	myfile.close();

}
void Turing::readTape()
{
	char temp;
	int index=0;
	ifstream mytape;
	mytape.open("tape.txt");
	if(!mytape)
	{
		cout<<"无法打开该文件"<<endl;
		exit(1);
	}
	mytape>>temp;
	while(!mytape.eof())
	{
		tape[index]=temp;
		index++;
		mytape>>temp;
	}

}
void Turing::checkTape()
{
	bool ret;
	char *t;
	for(int i=0;i<20;i++)//判断纸带元素是否在字符集合中
	{
		
		if(tape[i])
		{
			//cout<<"tape["<<i<<"]:"<<endl;
			ret=search_usualhash(tape[i],R);
			//cout<<"ret:"<<ret<<endl;
			if(!ret)
			{	
				cout<<"纸带输入有误"<<endl;
				break;
			}
		}
	}
	cout<<"纸带输入无误"<<endl;
}

char st[20];
char *nowValue=(char *)malloc(10);
void Turing::calculate(char *nowState,int index) //计算要根据当前状态，当前值（其实是跳转移动之后那个位置的值）char *nowvalue
{
	tran_fun *retStruct;
	char *next_value;
	sprintf(st,"%c",tape[index]);
	strcpy(nowValue,st);
	retStruct=search_tranfun(nowState,nowValue,T);
	while(strcmp(retStruct->next_state,accept_state)!=0)
	{		
		cout<<"now_state:"<<(*nowState)<<endl;
		cout<<"now_value:"<<tape[index]<<endl;
		cout<<"next_state:"<<(*retStruct->next_state)<<endl;
		cout<<"change_value:"<<(*retStruct->change_value)<<endl;
		cout<<"direction:"<<(*retStruct->direction)<<endl;
		//改变纸带当前这一格的值
		tape[index]=(*retStruct->change_value);//这个当前值将被修改
		nowState=retStruct->next_state;
	
		if((*retStruct->direction)=='R')
			index=index+1;
		else if((*retStruct->direction)=='-')
			index=index;
		else if((*retStruct->direction)=='L')
			index=index-1;
		*nowValue=tape[index];
		retStruct=search_tranfun(nowState,nowValue,T);

	}
	
	for(int i=0;i<20;i++)
	{
		if(tape[i]=='1')
			sum=sum+1;
	}
	//return sum;
}

void Turing::print()
{
	cout<<"End result:"<<endl;
	cout<<sum<<endl;

}


int main()
{

	Turing turing;
	char *startState;
	turing.init();
	turing.readTape();
	cout<<"readTape():"<<endl;
	turing.readSeventuple();
	turing.checkTape();
	startState=turing.getPrivate();
//	cout<<"startState:"<<startState<<endl;
	turing.calculate(startState,0);
	turing.print();
	//delete []temp1;
	//delete []tmp;
	return 0;
}





































