/*************************************************
*               CSS计算模块                      *
*************************************************/

/*
 * 模块功能： 计算css中width height offsetLeft offsetHeight margin padding
*/
#ifndef CALCULATE_CSS_H
#define CALCULATE_CSS_H

#include "header.h"

//外部函数
BOOL calWidth(struct htmlnode *head);	//计算width
BOOL calInlineWidth1(struct htmlnode *head);	//计算行内元素width
BOOL calHeight(struct htmlnode *head);	//计算高度
BOOL calOffsetleft(struct htmlnode *head);	//计算距离窗口左上角水平距离
BOOL calOffsetTop(struct htmlnode *head);	//计算距离窗口左上角垂直距离

//内部函数
int calWidthOfOneNode(struct htmlnode *head, int parent_width);	//返回p的width
int calHeightOfOneNode(struct htmlnode *head);	//返回该结点高度
double width_of_chars(int font_size, char *chars, char *font_family);	//计算字体宽度

#endif // !CALCULATE_CSS_H