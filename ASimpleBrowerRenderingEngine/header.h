/*************************************************
*          公用头文件                           *
*************************************************/

#ifndef HEADER_H
#define HEADER_H

#include <cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define OVERFLOW -1

#define DOUBLE_CHAR_LENGTH 20	//line_height小数长度
#define ELEMENT_LENGTH 20 //标签名长度
#define TEXT_LENGTH 1000 	//文本结点内容长度
#define ID_LENGTH 100 	//id名字长度
#define CLASS_LENGTH 100 	//class名字长度
#define CLASS_NUM 100	//一个标签最多属于这么多个class
#define FILE_NAME_MAX_LENGTH 100	//文件名长度
#define TAG_NEST_MAX_NUM 100 	//标签嵌套深度

#define BOOL int
#define TRUE 1
#define FALSE 0

//记录上次更新这些属性时规则的优先级	默认是0
struct css_priority
{
	int offset_left;
	int offset_top;
	int display;
	int position;
	int width;
	int height;
	int padding;
	int border;
	int margin;
	int left;
	int right;
	int top;
	int bottom;
	int color;
	int line_height;
	int font_size;
	int font_style;
	int font_weight;
	int text_align;
	int line_break;
};

//保存每个结点的20个css属性 以及上次更新这些属性时规则的优先级
struct css_attributes
{
	int offset_left;
	int offset_top;

	int display;	//0代表inline 1表示block 2表示none
	int position;	//0表示static 1表示relative 2表示absolute

	int width[3];	//width[0]为0表示auto 1表示单位px 2表示单位% width[1]是实际数值
	int height[3];	//height[0]为0表示auto 1表示单位px 2表示单位% width[1]是实际数值

	int padding[4][3];	//第一维 0 1 2 3分别表示上右下左 
	//第二维下标0有两种：1表示单位px 2表示单位% 
	//第二维下标1是数值 

	int border[4];	//上右下左的px值

	int margin[4][3];	//第一维 0 1 2 3分别表示上右下左 
	//第二维下标0有4种：0表示auto，1表示单位px 2表示单位% 3表示单位em此时数值是double_margin
	//第二维下标1是数值 

	double double_margin[4];

	int left[3];		//第一位表示0 auto 1表示单位px 2表示单位% 第二位是数值 以下三个类似
	int right[3];
	int top[3];
	int bottom[3];

	int color[3];	//分别 R，G，B
	char char_color[4];	//原始字母

	int line_height_px;	//0表示无单位 使用double 1表示有单位 使用int line_height
	int line_height;	//单位是px时的值
	double double_line_height;	//没有单位时候的值
	char char_line_height[DOUBLE_CHAR_LENGTH];	//原始字符串

	int font_size;	//单位px
	int font_style;	//0表示normal 1表示italic
	int font_weight;	//0表示normal 1表示bold

	int text_align;	//0表示left对齐 1表示center 2表示right
	int line_break;	//0表示normal 1表示break

	struct css_priority priority;	//该结构体记录每个css属性的优先级
};

//保存html结点 包括普通标签和文本结点
struct htmlnode
{
	int type;	//1表示开始标签<> 2表示文本结点 3表示闭合标签</>

	int tagtype;	//保存具体标签类型
	/* 1 body
	* 2 div
	* 3 p
	* 4 h
	* 5 span
	* 6 strong
	* 7 em
	*/

	char element[ELEMENT_LENGTH];	//存放标签名
	char *pinner_text;	//存放内容文本

	char id[ID_LENGTH];	//保存id名
	int how_many_class;	//属于多少个class
	char classname[CLASS_NUM][CLASS_LENGTH];	//保存class名

	struct css_attributes * pcss;	//指向该结点css属性的指针

	struct htmlnode * next;	//用于形成一条html链表
	struct htmlnode * pred;

	struct binnode * pbinnode;	//指向在树中该结点的位置

	int tmp_selected;	//临时的。。以后删
};

//二叉树结点
struct binnode
{
	int output_name_length;	//输出时标签名长度
	char * output_name;//输出时的标签名
	int sub;	//输出时的下标值
	struct binnode *left_child;	//指向下一个孩子
	struct binnode *rev_left_child;	//和left_child相反 互相指向

	struct binnode *right_sibling;	//指向下一个兄弟
	struct binnode *rev_right_sibling;	//和right_sibling相反 指向上一个兄弟

	struct binnode *parent;	//指向该结点逻辑上的父节点

	struct htmlnode * phtmlnode;	//指向html结点
	int height;	//逻辑深度


};

//二叉树
struct bintree
{
	int size;	//树的规模
	struct binnode *root;	//根结点指针
};


void myprintf(char * text, int low, int high);	//printf出text字符串的[low,high)区间的字符

#endif // !HEADER_H