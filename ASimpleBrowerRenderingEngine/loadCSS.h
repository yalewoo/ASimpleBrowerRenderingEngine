/*************************************************
*               CSS解析模块                      *
*************************************************/

/*
 * 模块功能：读取css文件并将属性添加到DOM树中对应的结点的css处
*/

#ifndef LOAD_CSS_H
#define LOAD_CSS_H

#include "header.h"

//外部函数
//读取css文件并加载到DOM树
BOOL loadCSS(char *htmltext, struct bintree * tree);
//更新所有节点遗传的css
BOOL updateInheritHtmlNodeCssOfAll(struct htmlnode * head);

//内部函数
int charRangeToInt(char *text, int low, int high);	//将数字字符串转换为int返回
int convert16to10(char * p);	//一个十六进制字符转换为int类型
void readCssFile(char **ptext, FILE * fpcss);	//将fpcss指向的文件内容读入内存 并将*ptext指向该内存区域
void addNewCssRule(struct bintree * tree, char *text, int i, int j, int left, int right);	//添加一个新css规则 一个 选择器{}
BOOL matchAllRule(char * text, int start, int end, struct binnode * pt);	//判断某结点是否和css选择器匹配
BOOL matchAndRule(char *text, int start, int end, struct htmlnode *p);	//某结点是不是和一个与关系的选择器匹配 例如div#id.class
BOOL matchARule(char *text, int low, int high, struct htmlnode * p);	//某结点是否和一个单选择器匹配 例如#id
int calPriority(char *text, int start, int end);	//计算css优先级并返回
struct css_attributes * createCSSRuleNode(int priority, char *text, int start, int end);	//将css规则新建一个规则结点
int ReadACssRule(struct css_attributes *p, int priority, char *text, int start);	//读取一条css 的 属性：取值 返回下一条的下标 例如position:absolute返回e的右边下标
BOOL updateHtmlNodeCss(struct htmlnode * p, struct css_attributes * pcss);	//按照优先级更新一个结点的css规则
BOOL updateInheritHtmlNodeCssOfOne(struct htmlnode *head, struct htmlnode * parent);	//更新某个结点所有后代的遗传css属性
BOOL isParent(struct binnode *pchild, struct binnode *parent);	//判断某个结点p是否有某个父辈parent

#endif // !LOAD_CSS_H