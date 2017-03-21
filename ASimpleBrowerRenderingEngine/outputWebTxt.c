#include "header.h"

/*
* 函数名称：createOutputTag
* 函数功能：写入标签名字 例如div.class#id
* 输入参数：html结点头指针
* 返回值：布尔型
*/
BOOL createOutputTag(struct htmlnode * head)
{
	struct htmlnode * p;
	int i, j;
	int length;

	p = head->next;

	while (p->next != NULL)
	{
		if (p->type == 1)
		{
			length = 0;
			length += strlen(p->element);
			if (p->id[0] != '\0')
			{
				length += strlen(p->id);
				++length;
			}
			for (j = p->how_many_class; j > 0; --j)
			{
				length = length + strlen(p->classname[j]) + 1;
			}
			length += 1;


			if ((p->pbinnode->output_name = (char *)malloc(length * sizeof(char))) == NULL)
				exit(OVERFLOW);

			p->pbinnode->output_name_length = length;
			i = sprintf(p->pbinnode->output_name, "%s", p->element);

			if (p->id[0] != '\0')
				i += sprintf(p->pbinnode->output_name + i, "#%s", p->id);

			for (j = p->how_many_class; j > 0; --j)
			{
				i += sprintf(p->pbinnode->output_name + i, ".%s", p->classname[j]);
			}
			p->pbinnode->output_name[i] = '\0';


		}

		p = p->next;

	}

	return TRUE;
}

/*
* 函数名称：calBinNodeSub
* 函数功能：写入标签下标值
* 输入参数：html结点头指针
* 返回值：布尔型
*/
BOOL calBinNodeSub(struct htmlnode * head)
{
	struct htmlnode * p;
	struct binnode * pt;
	//int i, j;

	p = head->next;


	while (p->next != NULL)
	{
		if (p->type == 1)
		{
			pt = p->pbinnode->rev_right_sibling;
			while (pt != NULL)
			{
				if (strcmp(p->pbinnode->output_name, pt->output_name) == 0)
				{
					break;
				}
				pt = pt->rev_right_sibling;

			}
			if (pt != NULL)
			{
				p->pbinnode->sub = pt->sub + 1;
			}

		}

		p = p->next;

	}

	return TRUE;
}

/*
* 函数名称：outputWebTxtCss
* 函数功能：把一个结点的css属性输出到web.txt
* 输入参数：html结点 文件指针fp
* 返回值：布尔型
*/
BOOL outputWebTxtCss(struct htmlnode * phtml, FILE * fp)
{
	int tmp;
	struct css_attributes * p = phtml->pcss;

	if (p->offset_left == 0)
		fprintf(fp, "\n    offsetLeft: 0;");
	else
		fprintf(fp, "\n    offsetLeft: %dpx;", p->offset_left);

	if (p->offset_top == 0)
		fprintf(fp, "\n    offsetTop: 0;");
	else
		fprintf(fp, "\n    offsetTop: %dpx;", p->offset_top);

	if (p->display == 0)
		fprintf(fp, "\n    display: inline;");
	else if (p->display == 1)
		fprintf(fp, "\n    display: block;");
	else
		fprintf(fp, "\n    display: none;");

	if (p->position == 0)
		fprintf(fp, "\n    position: static;");
	else if (p->position == 1)
		fprintf(fp, "\n    position: relative;");
	else
		fprintf(fp, "\n    position: absolute;");

	if (p->width[1] == 0)
		fprintf(fp, "\n    width: 0;");
	else
		fprintf(fp, "\n    width: %dpx;", p->width[1]);

	if (p->height[1] == 0)
		fprintf(fp, "\n    height: 0;");
	else
		fprintf(fp, "\n    height: %dpx;", p->height[1]);

	fprintf(fp, "\n    padding:");
	for (tmp = 0; tmp < 4; ++tmp)
	{
		if (p->padding[tmp][1] == 0)
			fprintf(fp, " 0");
		else
			fprintf(fp, " %dpx", p->padding[tmp][1]);
	}
	fprintf(fp, ";");

	fprintf(fp, "\n    border:");
	for (tmp = 0; tmp < 4; ++tmp)
	{
		if (p->border[tmp] == 0)
			fprintf(fp, " 0");
		else
			fprintf(fp, " %dpx", p->border[tmp]);
	}
	fprintf(fp, ";");

	fprintf(fp, "\n    margin:");
	if (phtml->pcss->display != 0)
	{
		for (tmp = 0; tmp < 4; ++tmp)
		{
			if (p->margin[tmp][1] == 0)
				fprintf(fp, " 0");
			else
				fprintf(fp, " %dpx", p->margin[tmp][1]);
		}
	}
	else
	{

		fprintf(fp, " 0");

		if (p->margin[1][1] == 0)
			fprintf(fp, " 0");
		else
			fprintf(fp, " %dpx", p->margin[1][1]);

		fprintf(fp, " 0");

		if (p->margin[3][1] == 0)
			fprintf(fp, " 0");
		else
			fprintf(fp, " %dpx", p->margin[3][1]);
	}
	fprintf(fp, ";");

	if (p->position == 0)
	{
		fprintf(fp, "\n    left: auto;");
		fprintf(fp, "\n    right: auto;");
		fprintf(fp, "\n    top: auto;");
		fprintf(fp, "\n    bottom: auto;");
	}
	else
	{
		if (p->left[1] == 0)
			fprintf(fp, "\n    left: 0;");
		else
			fprintf(fp, "\n    left: %dpx;", p->left[1]);

		if (p->right[1] == 0)
			fprintf(fp, "\n    right: 0;");
		else
			fprintf(fp, "\n    right: %dpx;", p->right[1]);

		if (p->top[1] == 0)
			fprintf(fp, "\n    top: 0;");
		else
			fprintf(fp, "\n    top: %dpx;", p->top[1]);

		if (p->bottom[1] == 0)
			fprintf(fp, "\n    bottom: 0;");
		else
			fprintf(fp, "\n    bottom: %dpx;", p->bottom[1]);
	}

	fprintf(fp, "\n    color: #%s;", p->char_color);

	if (p->line_height_px == 0)
		fprintf(fp, "\n    line-height: %s;", p->char_line_height);
	else
		fprintf(fp, "\n    line-height: %dpx;", p->line_height);

	fprintf(fp, "\n    font-size: %dpx;", p->font_size);

	if (p->font_style == 0)
		fprintf(fp, "\n    font-style: normal;");
	else
		fprintf(fp, "\n    font-style: italic;");

	if (p->font_weight == 0)
		fprintf(fp, "\n    font-weight: normal;");
	else
		fprintf(fp, "\n    font-weight: bold;");

	if (p->text_align == 0)
		fprintf(fp, "\n    text-align: left;");
	else if (p->text_align == 1)
		fprintf(fp, "\n    text-align: center;");
	else
		fprintf(fp, "\n    text-align: right;");

	if (p->line_break == 0)
		fprintf(fp, "\n    line-break: normal;");
	else
		fprintf(fp, "\n    line-break: break;");


	return TRUE;
}

/*
* 函数名称：outputWebTxt
* 函数功能：把所有结点的css属性输出到web.txt
* 输入参数：html头结点 序号
* 返回值：布尔型
*/
BOOL outputWebTxt(struct htmlnode * head, char *filename)
{
	//char * buffer;
	//int buffer_length;
	struct htmlnode * p;
	FILE * fp;

	struct binnode * pt;
	struct binnode * stack[TAG_NEST_MAX_NUM];
	int top = 0;

	char outfile[1024];
	sprintf(outfile, "%s.txt", filename);
	if ((fp = fopen(outfile, "w")) == NULL)
		return FALSE;

	p = head->next;

	if (p->next != NULL)
	{
		fprintf(fp, "body{");
		outputWebTxtCss(p, fp);
	}
	fprintf(fp, "\n}");

	p = p->next;
	while (p->next != NULL)
	{
		if (p->type == 1)
		{
			fprintf(fp, "\n\nbody");
			pt = p->pbinnode;
			while (pt->parent != NULL)
			{
				++top;
				stack[top] = pt;
				pt = pt->parent;
			}
			while (top > 0)
			{
				pt = stack[top];
				fprintf(fp, ">%s[%d]", pt->output_name, pt->sub);
				--top;
			}
			fprintf(fp, "{");
			outputWebTxtCss(p, fp);
			fprintf(fp, "\n}");


		}
		p = p->next;
	}

	fclose(fp);

	return TRUE;
}