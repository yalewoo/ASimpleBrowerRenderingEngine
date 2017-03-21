/*************************************************
*       排版引擎（Rendering Engine）主模块       *
*************************************************/
/*
 * 模块功能：完成排版引擎的所有功能
 * 对外提供了renderingEngine函数，接收两个参数：一个是html文件名 一个是整数index用于生成对应的webN.txt
 * 该模块会调用HTML解析模块、CSS解析模块、CSS计算模块、输出WEB.TXT模块和输出PNG模块
 * 当html文件不存在时，会返回TRUE
*/
#ifndef RENDERING_ENGINE_H
#define RENDERING_ENGINE_H

#include "createDOM.h"
#include "loadCSS.h"
#include "calculateCSS.h"
#include "outputWebTxt.h"
#include "outputPNG.h"

BOOL renderingEngine(char *htmlfilename, char *outdir);	//解析html css并输出webtxt和png图片
void readHtmlFile(char **text, FILE * fphtml);	//读取fphtml文件 读入text 并去掉多余空格

#endif // !RENDERING_ENGINE_H