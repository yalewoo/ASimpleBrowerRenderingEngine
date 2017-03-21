/*************************************************
*               输出PNG图片模块                  *
*************************************************/

/*
* 模块功能：将渲染结果输出到web.png图片中
*/

#ifndef OUTPUT_PNG_H
#define OUTPUT_PNG_H

#include "header.h"

//外部函数
BOOL outputPNG(struct htmlnode *head, int index);	//将渲染树输出到png图片

//内部函数
int DrawRectangle(cairo_t *cr, cairo_surface_t *surface, int x0, int y0, int width, int height, int line_width1, int line_width2, int line_width3, int line_width4);	//画矩形
int DrawLine(cairo_t *cr, cairo_surface_t *surface, int x0, int y0, int x1, int y1, int line_width);	//画线

//写字函数 分别为：起点左上角坐标，行高，RGB,字体大小，字体，斜体，深度，字符串
int WriteWord(cairo_t * cr, cairo_surface_t *surface, int x, int y, int line_height, double red, double green, double blue, int font_size, char *font_family, cairo_font_slant_t slant, cairo_font_weight_t weight, char * chars);

#endif // !OUTPUT_PNG_H