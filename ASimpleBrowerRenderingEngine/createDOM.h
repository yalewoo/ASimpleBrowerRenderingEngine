/*************************************************
*               HTML解析模块                     *
*************************************************/

/*
 * 模块功能：读取html文件，并生成html结点链表和html DOM树
*/
#ifndef CREATE_DOM_H
#define CREATE_DOM_H

#include "header.h"

//外部函数
int createHtmlList(char * text, struct htmlnode * head);	//创建html结点链表
BOOL correctUnmatchedHtmlNode(struct htmlnode * head);	//修正html中不匹配的标签
BOOL createDOMTree(struct bintree * tree, struct htmlnode * head);	//创建DOM数
BOOL destroyListAndTreeNodes(struct htmlnode * head);	//销毁所用的数据结构 包括链表和树节点 文本内容内存等
int tagType(char * p);	//返回标签类型
int tagWidth(char * ch);	//返回标签名字长度 例如div三个长度

//内部函数
struct binnode * createBinnode(struct bintree *tree, struct htmlnode *phtmlnode, struct binnode *parent);	//新建一个结点 它的父结点指向parent 返回值为新建的结点
struct binnode * insertAsLeftChild(struct bintree *tree, struct htmlnode *phtmlnode, struct binnode *parent);	//将phtmlnode作为parent的左孩子插入
struct binnode * insertAsRightSibling(struct bintree *tree, struct htmlnode *phtmlnode, struct binnode *rev_right_sibling); //将phtmlnode作为rev_right_sibling的右兄弟插入

//将[low,high)的html结点 保存为链表中的一个结点 并返回该节点地址
struct htmlnode * saveToHtmlList(struct htmlnode *p, char * text, int low, int high, int type);
//设置标签可能有的id和class
BOOL setIdAndClass(struct htmlnode * q, char * text, int low, int high);

void cssInit(struct css_attributes * p);	//初始化css属性
void htmlCssInit(int htmltype, struct css_attributes * p);	//初始化一个标签的css属性

#endif // !CREATE_DOM_H