/*************************************************
*               输出渲染树模块                   *
*************************************************/

/*
* 模块功能：将所有结点和它的css属性输出到web.txt中
*/

#ifndef OUTPUT_WEBTXT_H
#define OUTPUT_WEBTXT_H

#include "header.h"

//外部函数
BOOL outputWebTxt(struct htmlnode * head, char *filename);

//内部函数
BOOL createOutputTag(struct htmlnode * head);	//将输出时该结点的标签名写入对应树节点
BOOL calBinNodeSub(struct htmlnode * head);	//计算该结点所属的数组下标
BOOL outputWebTxtCss(struct htmlnode * phtml, FILE * fp);	//输出一个css规则

#endif // !OUTPUT_WEBTXT_H