#include "header.h"

//画线
int DrawLine(cairo_t *cr, cairo_surface_t *surface, int x0, int y0, int x1, int y1, int line_width)
{
	cairo_set_line_width(cr, line_width);
	cairo_move_to(cr, x0, y0);
	cairo_line_to(cr, x1, y1);
	cairo_stroke(cr);

	return TRUE;
}

//绘制方框
int DrawRectangle(cairo_t *cr, cairo_surface_t *surface, int x0, int y0, int width, int height, int line_width1, int line_width2, int line_width3, int line_width4)
{
	cairo_set_source_rgb(cr, 0, 0, 0);
	DrawLine(cr, surface, x0 - line_width4 / 2, y0, x0 + width + line_width2 / 2, y0, line_width1);
	DrawLine(cr, surface, x0 + width, y0, x0 + width, y0 + height, line_width2);
	DrawLine(cr, surface, x0 + width + line_width2 / 2, y0 + height, x0 - line_width4 / 2, y0 + height, line_width3);
	DrawLine(cr, surface, x0, y0 + height, x0, y0, line_width4);
	return 0;
}

/*
* 函数名称：WriteWord
* 函数功能：绘制字符串
* 输入参数：起点左上角坐标，行高，RGB,字体大小，字体，斜体，深度，字符串
* 返回值：布尔型
*/
int WriteWord(cairo_t * cr, cairo_surface_t *surface, int x, int y, int line_height, double red, double green, double blue, int font_size, char *font_family, int font_style, int font_weight, char * chars)
//分别为：起点左上角坐标，行高，RGB,字体大小，字体，斜体，深度，字符串
{
	cairo_font_slant_t	slant;
	cairo_font_weight_t	weight;

	if (font_style == 0)
	{
		slant = CAIRO_FONT_SLANT_NORMAL;
	}
	else
	{
		slant = CAIRO_FONT_SLANT_ITALIC;
	}
	if (font_weight == 0)
	{
		weight = CAIRO_FONT_WEIGHT_NORMAL;
	}
	else
	{
		weight = CAIRO_FONT_WEIGHT_BOLD;
	}

	cairo_move_to(cr, x, y + line_height);
	cairo_set_source_rgb(cr, red, green, blue);
	cairo_set_font_size(cr, font_size);
	cairo_select_font_face(cr, font_family, slant, weight);
	cairo_show_text(cr, chars);
	return 0;
}

/*
* 函数名称：outputPNG
* 函数功能：把渲染结果输出到png图片
* 输入参数：html头结点 序号
* 返回值：布尔型
*/
BOOL outputPNG(struct htmlnode *head, char *outfilename)
{
	struct htmlnode *p;
	struct css_attributes *pcss;
	int line_height;

	char filename[1024];
	sprintf(filename, "%s.png", outfilename);


	cairo_surface_t *surface;
	cairo_t *cr;



	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1024, 768);
	cr = cairo_create(surface);



	p = head->next;
	while (p->next != NULL)
	{
		if (p->type == 1)
		{
			DrawRectangle(cr, surface, p->pcss->offset_left + p->pcss->margin[3][1], p->pcss->offset_top + p->pcss->margin[0][1], p->pcss->width[1] + p->pcss->padding[1][1] + p->pcss->padding[3][1] + p->pcss->border[1] + p->pcss->border[3], p->pcss->height[1] + p->pcss->padding[0][1] + p->pcss->padding[2][1] + p->pcss->border[0] + p->pcss->border[2], p->pcss->border[0], p->pcss->border[1], p->pcss->border[2], p->pcss->border[3]);
		}
		else if (p->type == 2)
		{
			pcss = p->pbinnode->parent->phtmlnode->pcss;

			if (pcss->line_height_px == 0)
			{
				line_height = (int)(pcss->double_line_height * pcss->font_size);
			}
			else
			{
				line_height = pcss->line_height;
			}
			WriteWord(cr, surface, pcss->offset_left + pcss->margin[3][1] + pcss->border[3] + pcss->padding[3][1], pcss->offset_top + pcss->margin[0][1] + pcss->border[0] + pcss->padding[0][1] - (line_height - pcss->font_size) / 2, line_height, pcss->color[0] / 15.0, pcss->color[1] / 15.0, pcss->color[2] / 15.0, pcss->font_size, "AdobeHeiti Std", pcss->font_style, pcss->font_weight, p->pinner_text);
		}
		p = p->next;
	}

	cairo_surface_write_to_png(surface, filename);
	cairo_destroy(cr);
	cairo_surface_destroy(surface);

	return TRUE;
}