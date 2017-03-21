#include "header.h"

/*
* 函数名称：calWidthOfOneNode
* 函数功能：计算一个结点的width 
* 输入参数： 该结点 该结点父元素宽度
* 返回值：该结点宽度
*/
int calWidthOfOneNode(struct htmlnode *p, int parent_width)
{
	struct css_attributes *pcss;
	int sum;
	pcss = p->pcss;

	//if (pcss->position != 2)
	//{
	if (pcss->width[0] == 2)
	{
		pcss->width[0] = 1;
		pcss->width[1] = pcss->width[1] * parent_width / 100;
	}
	if (pcss->margin[0][0] == 2)
	{
		pcss->margin[0][0] = 1;
		pcss->margin[0][1] = pcss->margin[0][1] * parent_width / 100;
	}
	if (pcss->margin[1][0] == 2)
	{
		pcss->margin[1][0] = 1;
		pcss->margin[1][1] = pcss->margin[1][1] * parent_width / 100;
	}
	if (pcss->margin[2][0] == 2)
	{
		pcss->margin[2][0] = 1;
		pcss->margin[2][1] = pcss->margin[2][1] * parent_width / 100;
	}
	if (pcss->margin[3][0] == 2)
	{
		pcss->margin[3][0] = 1;
		pcss->margin[3][1] = pcss->margin[3][1] * parent_width / 100;
	}
	if (pcss->margin[0][0] == 3)
	{
		pcss->margin[0][0] = 1;
		pcss->margin[0][1] = pcss->margin[0][1] * pcss->font_size;
	}
	if (pcss->margin[1][0] == 3)
	{
		pcss->margin[1][0] = 1;
		pcss->margin[1][1] = pcss->margin[1][1] * pcss->font_size;
	}
	if (pcss->margin[2][0] == 3)
	{
		pcss->margin[2][0] = 1;
		pcss->margin[2][1] = pcss->margin[2][1] * pcss->font_size;
	}
	if (pcss->margin[3][0] == 3)
	{
		pcss->margin[3][0] = 1;
		pcss->margin[3][1] = pcss->margin[3][1] * pcss->font_size;
	}

	if (pcss->padding[0][0] == 2)
	{
		pcss->padding[0][0] = 1;
		pcss->padding[0][1] = pcss->padding[0][1] * parent_width / 100;
	}
	if (pcss->padding[1][0] == 2)
	{
		pcss->padding[1][0] = 1;
		pcss->padding[1][1] = pcss->padding[1][1] * parent_width / 100;
	}
	if (pcss->padding[2][0] == 2)
	{
		pcss->padding[2][0] = 1;
		pcss->padding[2][1] = pcss->padding[2][1] * parent_width / 100;
	}
	if (pcss->padding[3][0] == 2)
	{
		pcss->padding[3][0] = 1;
		pcss->padding[3][1] = pcss->padding[3][1] * parent_width / 100;
	}


	if (pcss->width[0] == 0)	//auto
	{
		if (pcss->margin[1][0] == 0)
		{
			pcss->margin[1][0] = 1;
			pcss->margin[1][1] = 0;
		}
		if (pcss->margin[3][0] == 0)
		{
			pcss->margin[3][0] = 1;
			pcss->margin[3][1] = 0;
		}
		pcss->width[0] = 1;
		pcss->width[1] = parent_width - pcss->margin[1][1] - pcss->border[1] - pcss->padding[1][1] - pcss->padding[3][1] - pcss->border[3] - pcss->margin[3][1];
		return pcss->width[1];
	}
	else	//定值
	{
		sum = pcss->width[1] + pcss->padding[1][1] + pcss->padding[3][1] + pcss->border[1] + pcss->border[3];
		if (sum >= parent_width)
		{
			if (pcss->margin[1][0] == 1 && pcss->margin[3][0] == 1)
			{
				pcss->margin[3][0] = 0;
			}

			if (pcss->margin[1][0] == 0)
			{
				pcss->margin[1][0] = 1;
				pcss->margin[1][1] = 0;
			}
			if (pcss->margin[3][0] == 0)
			{
				pcss->margin[3][0] = 1;
				pcss->margin[3][1] = 0;
			}

			return pcss->width[1];

		}
		else
		{
			if (pcss->margin[1][0] == 1 && pcss->margin[3][0] == 1)
			{
				pcss->margin[3][0] = 0;
			}

			if (pcss->margin[1][0] == 0 && pcss->margin[3][0] == 0)
			{
				pcss->margin[1][0] = 1;
				pcss->margin[3][0] = 1;
				pcss->margin[1][1] = (parent_width - sum) / 2;
				pcss->margin[3][1] = pcss->margin[1][1];

			}

			if (pcss->margin[1][0] == 0)
			{
				pcss->margin[1][0] = 1;
				pcss->margin[1][1] = parent_width - sum - pcss->margin[3][1];
			}
			else
			{
				pcss->margin[3][0] = 1;
				pcss->margin[3][1] = parent_width - sum - pcss->margin[1][1];
			}

			return pcss->width[1];
		}
	}
	//}
	//else	//absolute
	//{

	//}
}

/*
* 函数名称：calWidth
* 函数功能：计算所有节点width
* 输入参数：html结点头指针
* 返回值：布尔型
*/
BOOL calWidth(struct htmlnode *head)
{

	struct htmlnode *p;
	struct binnode *pt;

	p = head->next;

	while (p->next != NULL)
	{
		if (p->type == 1)
		{
			if (p->pbinnode->parent == NULL)
			{
				calWidthOfOneNode(p, 1024);
			}
			else
			{
				pt = p->pbinnode->parent;
				if (p->pcss->position == 2)
				{
					while (pt != NULL && pt->phtmlnode->pcss->position == 0)
					{
						pt = pt->parent;
					}
					if (pt == NULL)
					{
						calWidthOfOneNode(p, 1024);
					}
					else
					{
						calWidthOfOneNode(p, pt->phtmlnode->pcss->width[1]);
					}
				}
				else
				{
					calWidthOfOneNode(p, p->pbinnode->parent->phtmlnode->pcss->width[1]);
				}
			}

		}
		p = p->next;
	}

	return TRUE;
}

/*
* 函数名称：calHeightOfOneNode
* 函数功能：计算某节点的高度
* 输入参数：html结点
* 返回值：该结点高度
*/
int calHeightOfOneNode(struct htmlnode *p)
{
	int height = 0;
	struct css_attributes *pcss;
	//struct htmlnode *q;
	int tmp_height = 0;
	struct binnode *pt;

	pcss = p->pcss;

	if (pcss->height[0] == 1)
	{
		return pcss->height[1] + pcss->padding[0][1] + pcss->padding[2][1] + pcss->border[0] + pcss->border[2] + pcss->margin[0][1] + pcss->margin[2][1];
	}
	if (pcss->height[0] == 2)	//是百分数
	{
		if (p->pbinnode->parent != NULL)
		{
			if (p->pbinnode->parent->phtmlnode->pcss->height[0] == 1)
			{
				pcss->height[0] = 1;
				pcss->height[1] = pcss->height[1] * p->pbinnode->parent->phtmlnode->pcss->height[1] / 100;
				return pcss->height[1] + pcss->padding[0][1] + pcss->padding[2][1] + pcss->border[0] + pcss->border[2] + pcss->margin[0][1] + pcss->margin[2][1];
			}
		}
		else
		{
			pcss->height[0] = 1;
			pcss->height[1] = pcss->height[1] * 768 / 100;
			return pcss->height[1] + pcss->padding[0][1] + pcss->padding[2][1] + pcss->border[0] + pcss->border[2] + pcss->margin[0][1] + pcss->margin[2][1];
		}
	}

	if (p->pbinnode->left_child == NULL)
	{
		pcss->height[0] = 1;
		pcss->height[1] = 0;
		return pcss->height[1] + pcss->padding[0][1] + pcss->padding[2][1] + pcss->border[0] + pcss->border[2] + pcss->margin[0][1] + pcss->margin[2][1];
	}
	else if (p->pbinnode->left_child->phtmlnode->type == 2)
	{

		if (pcss->line_height_px == 0)
		{
			pcss->height[0] = 1;
			pcss->height[1] = (int)(pcss->double_line_height * pcss->font_size);
			return pcss->height[1] + pcss->padding[0][1] + pcss->padding[2][1] + pcss->border[0] + pcss->border[2] + pcss->margin[0][1] + pcss->margin[2][1];
		}
		else
		{
			pcss->height[0] = 1;
			pcss->height[1] = pcss->line_height;
			return pcss->height[1] + pcss->padding[0][1] + pcss->padding[2][1] + pcss->border[0] + pcss->border[2] + pcss->margin[0][1] + pcss->margin[2][1];
		}

	}
	else
	{
		pt = p->pbinnode->left_child;
		height = 0;

		while (pt != NULL)
		{
			if (pt->phtmlnode->pcss->position != 2)
			{
				height += calHeightOfOneNode(pt->phtmlnode);
			}
			//height += calHeightOfOneNode(pt->phtmlnode);
			pt = pt->right_sibling;
		}
		pcss->height[0] = 1;
		pcss->height[1] = height;
		return pcss->height[1] + pcss->padding[0][1] + pcss->padding[2][1] + pcss->border[0] + pcss->border[2] + pcss->margin[0][1] + pcss->margin[2][1];
	}


}

/*
* 函数名称：calHeight
* 函数功能：计算所有节点height
* 输入参数：html结点头指针
* 返回值：布尔型
*/
BOOL calHeight(struct htmlnode *head)
{
	struct htmlnode *p;

	p = head->next;

	while (p->next != NULL)
	{
		if (p->type == 1)
		{
			calHeightOfOneNode(p);
		}

		p = p->next;
	}

	return TRUE;
}

/*
* 函数名称：calOffsetleft
* 函数功能：计算所有节点Offsetleft
* 输入参数：html结点头指针
* 返回值：布尔型
*/
BOOL calOffsetleft(struct htmlnode *head)
{
	struct htmlnode *p;
	struct htmlnode *q;
	struct binnode *pt;

	p = head->next;

	while (p->next != NULL)
	{
		if (p->type == 1)
		{
			if (p->pbinnode->parent == NULL)
			{
				p->pcss->offset_left = 0;
			}
			else
			{

				pt = p->pbinnode->parent;
				if (p->pcss->position == 2)
				{
					while (pt != NULL && pt->phtmlnode->pcss->position == 0)
					{
						pt = pt->parent;
					}
					if (pt == NULL)
					{
						p->pcss->offset_left = 0;
					}
					else
					{
						q = pt->phtmlnode;
						p->pcss->offset_left = q->pcss->offset_left + q->pcss->margin[3][1] + q->pcss->border[3] + q->pcss->padding[3][1];
					}
				}
				else
				{
					q = p->pbinnode->parent->phtmlnode;
					p->pcss->offset_left = q->pcss->offset_left + q->pcss->margin[3][1] + q->pcss->border[3] + q->pcss->padding[3][1];
				}


			}

		}
		p = p->next;
	}

	return TRUE;
}

/*
* 函数名称：calOffsetTop
* 函数功能：计算所有节点OffsetTop
* 输入参数：html结点头指针
* 返回值：布尔型
*/
BOOL calOffsetTop(struct htmlnode *head)
{
	struct htmlnode *p;
	struct htmlnode *q;
	struct binnode *pt;

	p = head->next;

	while (p->next != NULL)
	{
		if (p->type == 1)
		{
			if (p->pbinnode->parent == NULL)
			{
				p->pcss->offset_top = 0;
			}
			else
			{
				if (p->pbinnode->rev_right_sibling == NULL)	//第一个结点
				{
					pt = p->pbinnode->parent;
					if (p->pcss->position == 2)
					{
						while (pt != NULL && pt->phtmlnode->pcss->position == 0)
						{
							pt = pt->parent;
						}
						if (pt == NULL)
						{
							p->pcss->offset_top = 0;
						}
						else
						{
							q = pt->phtmlnode;
							p->pcss->offset_top = q->pcss->offset_top + q->pcss->margin[0][1] + q->pcss->border[0] + q->pcss->padding[0][1];
						}
					}
					else
					{
						q = p->pbinnode->parent->phtmlnode;
						p->pcss->offset_top = q->pcss->offset_top + q->pcss->margin[0][1] + q->pcss->border[0] + q->pcss->padding[0][1];
					}
				}
				else	//有上一个兄弟
				{
					pt = p->pbinnode->rev_right_sibling;
					while (pt != NULL && pt->phtmlnode->pcss->position == 2)
					{
						pt = pt->rev_right_sibling;
					}
					if (pt == NULL)
					{
						q = p->pbinnode->parent->phtmlnode;
						p->pcss->offset_top = q->pcss->offset_top + q->pcss->margin[0][1] + q->pcss->border[0] + q->pcss->padding[0][1];
					}
					else
					{
						//上一个兄弟不是absolute
						q = p->pbinnode->rev_right_sibling->phtmlnode;
						p->pcss->offset_top = q->pcss->offset_top + q->pcss->margin[0][1] + q->pcss->margin[2][1] + q->pcss->border[0] + q->pcss->border[2] + q->pcss->padding[0][1] + q->pcss->padding[2][1] + q->pcss->height[1];
					}
					
					
				}


			}

		}
		p = p->next;
	}

	return TRUE;
}

//返回字符串宽度
double width_of_chars(int font_size, char *chars, char *font_family)
//family为字体类型；size为字体大小；chars为字符串；
{
	double x1, y1;
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1024, 768);
	cairo_t *cr = cairo_create(surface);
	cairo_set_font_size(cr, font_size);
	cairo_select_font_face(cr, font_family, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_move_to(cr, 0, 0 + font_size);
	cairo_show_text(cr, chars);
	cairo_get_current_point(cr, &x1, &y1);
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	return x1 + 1;
}

//计算行内元素宽度
BOOL calInlineWidth1(struct htmlnode *head)
{
	struct htmlnode *p;
	struct htmlnode *q;
	//struct binnode *pt;

	p = head->next;

	while (p->next != NULL)
	{
		if (p->type == 2 && p->pbinnode->parent->phtmlnode->pcss->display == 0)
		{
			q = p->pbinnode->parent->phtmlnode;
			q->pcss->width[0] = 1;
			q->pcss->width[1] = (int)width_of_chars(q->pcss->font_size, p->pinner_text, "AdobeHeiti Std");

		}
		p = p->next;
	}

	return TRUE;
}