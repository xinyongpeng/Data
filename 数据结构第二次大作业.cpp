// 数据结构第二次大作业.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <stdio.h>
#include<iostream>
#define Max 50

//兄弟孩子链存储结构
typedef struct tnode
{
	int data=0;
	struct tnode* brothers;
	struct tnode* sons;
	int length;//存储之前节点的个数
	bool flag;
}TSBnode;

//链队节点类型
typedef struct qnode
{
	TSBnode * pointer;
	struct qnode *next;
}Queue;          

//头结点类型
typedef struct
{
	Queue *front;
	Queue *rear;
}LinkQuNode;        

//链队的基本运算

//初始化队列
void InitQueue(LinkQuNode * &q)    
{
	q = (LinkQuNode *)malloc(sizeof(LinkQuNode));
	q->front = NULL;
	q->rear = NULL;
}

//销毁队列
void DestroyQueue(LinkQuNode * &q)  
{
	Queue *pre = q->front, *p;
	if (pre != NULL) {
		p = pre->next;
		while (p != NULL) {
			free(pre);
			pre = p;
			p = p->next;
		}
		free(pre);
	}
	free(q);
}

//判断队列是否为空
bool QueueEmpty(LinkQuNode *q)  
{
	return (q->rear == NULL);
}

//进队列
void enQueue(LinkQuNode * &q, TSBnode *e)   
{
	Queue *p;
	p = (Queue *)malloc(sizeof(Queue));
	p->pointer = e;
	p->next = NULL;
	if (q->rear == NULL) {
		q->front = p;
		q->rear = p;
	}
	else
	{
		q->rear->next = p;
		q->rear = p;
	}
}

//出队列
bool deQueue(LinkQuNode * &q, TSBnode * &e)  
{
	Queue *t;
	if (q->rear == NULL) {
		return false;
	}
	t = q->front;
	if (q->front == q->rear) {
		q->front = q->rear = NULL;
	}
	else
		q->front = q->front->next;
	e = t->pointer;
	free(t);
	return true;
}

//取队头元素
bool GetQueueFront(LinkQuNode *q, TSBnode * &e)  
{
	if (q->rear == NULL) {
		return false;
	}
	e = q->front->pointer;
	return true;
}



	
//栈的相关操作
typedef struct linknode
{
	TSBnode *pointer;
	struct linknode *next;
}Stack;

//栈的基本运算
void InitStack(Stack * &s)     //初始化栈
{
	s = (Stack *)malloc(sizeof(Stack));
	s->next = NULL;
}

//销毁栈
void DestroyStack(Stack * &s)      
{
	Stack * pre = s, *p = s->next;
	while (p != NULL) {
		free(pre);
		pre = p;
		p = pre->next;
	}
	free(pre);
}

//判断栈是否为空
bool StackEmpty(Stack * s)     
{
	return (s->next == NULL);
}

//进栈
void Push(Stack * &s, TSBnode *e)      
{
	Stack *p;
	p = (Stack *)malloc(sizeof(Stack));
	p->pointer = e;
	p->next = s->next;
	s->next = p;
}

//出栈
bool Pop(Stack * &s, TSBnode * &e)       
{
	Stack *p;
	if (s->next == NULL) {
		return false;
	}
	p = s->next;
	e = p->pointer;
	s->next = p->next;
	free(p);
	return true;
}

//取栈顶元素
bool GetTop(Stack *s, TSBnode * &e)    
{
	if (s->next == NULL) {
		return false;
	}
	e = s->next->pointer;
	return true;
}




//输入函数
void input(TSBnode *&head,Stack *&s) {
	std::cout<<"请用广义表的格式输入一棵树"<<std::endl;//动态给广义表分配内存。采用非递归算法，需要一个栈来读取写入。
	char symbol[Max];
	//scanf_s("%s", &symbol);不好用，不容易得到长度。反手选择iostream。（但是用getchar()也是一个好办法啊）
	head = (TSBnode*)malloc(sizeof(TSBnode));
	TSBnode *p;
	head->sons = NULL;
	head->brothers = NULL;
	head->data = 0;
	head->flag = true;
	head->length = 0;
	int i,n;
	std::cin.getline(symbol, Max);
	n = std::cin.gcount() - 1;

	//判断括号是否匹配
	int count = 0;
	for (i = 0; i < n; i++)
	{
		if (count < 0)
		{
			std::cout << "括号不匹配哦";
			exit(1)	;
		}
		if (symbol[i] == '(')
			count++;
		else if (symbol[i] == ')')
			count--;
		else
			;
	}
	if (count != 0)
	{
		std::cout << "括号不匹配哦";
		exit(1);
	}
	//end

	//非递归动态建立兄弟孩子表
	p = head;
	int leng=0;
	for (i = 0; i < n; i++)
	{
		if (symbol[i] == '(')
		{
			TSBnode *newone = (TSBnode*)malloc(sizeof(TSBnode));
			newone->brothers = NULL;
			newone->sons = NULL;
			newone->length = leng;
			p->sons = newone;
			Push(s, p);
			p = newone;
		}
		else if (symbol[i] == ')')
		{
			Pop(s, p);
		}
		else if (symbol[i] == ',')
		{
			leng++;
			TSBnode *newone = (TSBnode*)malloc(sizeof(TSBnode));
			newone->brothers = NULL;
			newone->sons = NULL;
			p->brothers = newone;
			newone->length = leng;
			p = newone;
		}
		else
		{
			p->data = symbol[i]-48;//symbol[i]是字符，ascii码-48是对应的数(节点里直接用char是不是会更好一点啊)(反正题里也只有0-9)
		}
	}
	DestroyStack(s);
}

//后序遍历整个兄弟孩子表，计算每个节点的长度。         
/*bool calculate(TSBnode*& head) {
	这里不好输出     {   1   }
	                 {2 3 4 5}
					   {6}
	这样的，这个思路有点麻瓜。
	TSBnode *p, *q;
	q = head;
	Stack *d;
	InitStack(d);
	p = head->sons;
	while (head->sons->length == 0) {
		if (NULL == p->sons&&p->flag == true) {//没有孩子的长度为0,将他的flag置false
			p->length = 1;
			p->flag = false;
		}
		
		if (NULL != p->sons&&p->flag == true)//有孩子的先看孩子，将他的flag置false
		{
			p->flag = false;
			Push(d, q);
			q = p;
			p->length = 0;
			p = p->sons;
		}
		else if (NULL != p->brothers)//如果没有孩子有兄弟再看兄弟
		{
			p = p->brothers;
		}
		else {
			p = q->sons;
			while (NULL != p->brothers) {
				q->length += 2*p->length;
				p = p->brothers;
			}
			Pop(d, q);
		}
	}

	return true;
}
*/

//输出函数
/*void DisplayNode(TSBnode *s) {
	int i;
	std::cout << "{";
	for (i = 0; i < s->length / 2; i++)
		std::cout << "\t";

	std::cout << s->data;

	for (i = 0; i < (s->length / 2); i++)
	{
		std::cout << "\t";
	}
	std::cout << "}";
}
*/

//广度优先遍历输出,此时，为了不用递归，我们需要一个队列。
void DisplayAll(TSBnode *s) {
	TSBnode *p,*q;
	LinkQuNode* d;
	bool flag = 0;
	int n=0;
	InitQueue(d);
	enQueue(d, s);
	std::cout << "这棵树的输出是："<<std::endl;
	while (!QueueEmpty(d)) {
		deQueue(d,q );
		if (q->sons != NULL) {
			enQueue(d, q->sons);
		}
		if (flag) {
			for (n = q->length; n > 0; n--) {
				printf("\t\t");
			}
			flag = 0;
		}
		printf("{\t%d", q->data);
		while (q->brothers != NULL) {
			p = q->brothers;
			if (p->sons != NULL) {
				enQueue(d, p->sons);
			}
			else for (n = p->length - q->length; n > 1; n--) {
				printf("\t\t");
			}	
			printf("\t");
			printf("%-4d", p->data);
			q = p;
		}
		putchar('}');
		if (GetQueueFront(d, p)) {
			if (p->length <= q->length) {
				putchar('\n');
				flag = 1;
			}
			else    for (n = p->length - q->length; n > 0; n--) {
				printf("\t");
			}
		}
	}
	DestroyQueue(d);
}


//主函数
int main() {
	Stack *s;
	InitStack(s);
	TSBnode *head;
	input(head, s);
	DisplayAll(head);
	std::cin.get();
	return 0;
}