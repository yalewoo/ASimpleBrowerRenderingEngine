#include "header.h"

extern int tagType(char * p);

//传入‘1’‘2’‘3’返回整数123
int charRangeToInt(char *text, int low, int high)
{
	char *buffer;
	int a;
	if ((buffer = (char *)malloc((high - low + 1)*sizeof(char))) == NULL)
		exit(OVERFLOW);

	strncpy(buffer, text + low, high - low);
	buffer[high - low] = '\0';
	a = atoi(buffer);
	return a;
}

//一个十六进制字符转换为int类型
int convert16to10(char * p)
{
	if (*p >= '0' && *p <= '9')
		return *p - '0';
	if (*p >= 'a' && *p <= 'f')
		return *p - 'a' + 10;
	if (*p >= 'A' && *p <= 'F')
		return *p - 'A' + 10;
	return -1;
}

/*
* 函数名称：readCssFile
* 函数功能：读入css文件内容 去掉多余空格和分号
* 输入参数：字符指针的地址 文件指针指向css文件
* 返回值：无
*/
void readCssFile(char **ptext, FILE * fpcss)
{
	int ch;     //用于读取文件内容
	long char_num = 0;  //用于统计index.html长度
	int i;
	char *text;
	int in_tag_flag = 0;
	//统计css文件长度 分配相应大小内存
	for (char_num = 0; (ch = fgetc(fpcss)) != EOF; ++char_num)
	{
		;
	}
	fseek(fpcss, 0, SEEK_SET);
	text = (char *)malloc((char_num + 1)*sizeof(char));

	i = 0;
	while ((ch = fgetc(fpcss)) != EOF)
	{
		if (ch == '\n' || ch == '\t' || ch == ';')
		{
			continue;	//去掉所有换行和分号
		}
		if (in_tag_flag == 1 && (ch == ' '))
		{
			continue;	//删去{}中全部空白
		}

		if (in_tag_flag == 0 && text[i - 1] == ' ' && ch == ' ')
		{	//将选择器部分连续两个空格改为1个
			continue;
		}
		if (in_tag_flag == 0 && text[i - 1] == ' ' && ch == '>')
		{	//去掉>前面的空格
			text[i - 1] = ch;
			continue;
		}
		if (in_tag_flag == 0 && text[i - 1] == '>' && ch == ' ')
		{	//去掉>后面的空格
			continue;
		}
		if (in_tag_flag == 0 && text[i - 1] == '}' && ch == ' ')
		{	//去掉}后面的空格
			continue;
		}
		if (in_tag_flag == 0 && text[i - 1] == ' ' && ch == '{')
		{	//去掉{前面的空格
			in_tag_flag = 1;
			text[i - 1] = ch;
			continue;
		}

		if (in_tag_flag == 0 && text[i - 1] == ' ' && ch == ',')
		{	//去掉,前面的空格
			text[i - 1] = ch;
			continue;
		}
		if (in_tag_flag == 0 && text[i - 1] == ',' && ch == ' ')
		{	//去掉,后面的空格
			continue;
		}


		if (ch == '{')
			in_tag_flag = 1;
		else if (ch == '}')
			in_tag_flag = 0;

		text[i] = ch;
		++i;

	}

	text[i] = '\0';

	*ptext = text;
}

/*
* 函数名称：matchARule
* 函数功能：判断一个结点是否和单一的css选择器匹配 例如div或 .class
* 输入参数：字符串首地址 文本下标前闭后开 html结点指针
* 返回值：布尔型 匹配返回TRUE 不匹配返回FALSE
*/
BOOL matchARule(char *text, int low, int high, struct htmlnode * p)
{
	int i;

	if (text[low] == '#')
	{
		++low;
		if (strncmp(p->id, text + low, high - low) == 0)
			return TRUE;
	}
	else if (text[low] == '.')
	{
		++low;
		if (p->how_many_class == 0)
			return FALSE;
		for (i = 1; i <= p->how_many_class; ++i)
		{
			if (strncmp(p->classname[i], text + low, high - low) == 0)
				return TRUE;
		}
	}
	else
	{
		if (p->tagtype == tagType(text + low))
			return TRUE;
	}
	return FALSE;
}

/*
* 函数名称：matchARule
* 函数功能：判断一个结点是否和 与关系的css选择器匹配 例如div.class
* 输入参数：字符串首地址 文本下标前闭后开 html结点指针
* 返回值：布尔型 匹配返回TRUE 不匹配返回FALSE
*/
BOOL matchAndRule(char *text, int start, int end, struct htmlnode *p)
{
	int i;
	int j;
	i = start;
	j = i;

	while (j != end)
	{
		++j;
		if (text[j] == '.' || text[j] == '#' || j == end)
		{
			if (matchARule(text, i, j, p) == FALSE)
				return FALSE;
			i = j;
		}
	}
	return TRUE;
}

/*
* 函数名称：matchARule
* 函数功能：判断一个结点是否和 整个css选择器匹配 例如div p>div.class span
* 输入参数：字符串首地址 文本下标前闭后开 html结点指针
* 返回值：布尔型 匹配返回TRUE 不匹配返回FALSE
*/
BOOL matchAllRule(char * text, int start, int end, struct binnode * pt)
{
	int i;
	int j;

	j = end - 1;
	i = j;

	while (i != start - 1)
	{
		--i;
		if (text[i] == ' ' || text[i] == '>' || i == start - 1)
		{
			if (j == end - 1)
			{
				if (matchAndRule(text, i + 1, j + 1, pt->phtmlnode) == FALSE)
					return FALSE;

			}
			else if (text[j] == ' ')
			{
				pt = pt->parent;
				while (pt != NULL)
				{
					if (matchAndRule(text, i + 1, j + 1, pt->phtmlnode) == FALSE)
					{
						pt = pt->parent;
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				if (pt->parent == NULL)
					return FALSE;
				if (matchAndRule(text, i + 1, j + 1, pt->phtmlnode) == FALSE)
					return FALSE;
			}
			j = i - 1;
			continue;
		}
	}
	return TRUE;
}

/*
* 函数名称：calPriority
* 函数功能：计算一个css选择器的优先级
* 输入参数：字符串首地址 文本下标前闭后开
* 返回值：该选择器的优先级 采用的是一个整数 abc按照64进制计算
*/
int calPriority(char *text, int start, int end)
{
	int i;
	//int j;
	int a = 0;
	int b = 0;
	int c = 0;

	for (i = start; i < end; ++i)
	{
		if ((i == start || text[i - 1] == '>' || text[i - 1] == ' ') && (text[i] != '.' && text[i] != '#'))
		{
			++c;
		}
		if (text[i] == '.')
		{
			++b;
		}
		if (text[i] == '#')
		{
			++a;
		}
	}
	//printf("calPriority:%d %d %d\n", a, b, c);
	return (64 * 64 * a + 64 * b + c);
}

/*
* 函数名称：ReadACssRule
* 函数功能：读取一条css的 属性：取值
* 输入参数：css结点， 优先级， 字符串首地址 文本下标开始位置
* 返回值：返回文本下标结束位置 例如position:absolute返回e的右边下标
*/
int ReadACssRule(struct css_attributes *p, int priority, char *text, int start)
{
	int i, j;
	int tmp;
	int n;
	i = start;
	while (text[i] != ':')
		++i;
	++i;
	j = i;
	switch (text[start])
	{
	case 'b':{
		if (text[start + 2] == 'r')
		{
			p->priority.border = priority;

			for (tmp = 0; tmp < 4; ++tmp)
			{
				j = i;
				if (text[i] == '0')
				{
					p->border[tmp] = 0;

					if (text[i + 1] == 'p' && text[i + 2] == 'x')
						i = i + 3;
					if (text[i + 1] == '%')
						i = i + 2;
					else
						++i;
				}
				else
				{
					while (text[j] != 'p')
						++j;
					p->border[tmp] = charRangeToInt(text, i, j);
					i = j + 2;
				}
			}

			return i;

		}
		else if (text[start + 2] == 't')	//bottom:3px 23% auto
		{
			p->priority.bottom = priority;

			if (text[i] == 'a')//auto
			{
				p->bottom[0] = 0;
				return (i + 4);
			}
			else if (text[i] == '0')
			{
				p->bottom[0] = 1;
				p->bottom[1] = 0;
				if (text[i + 1] == 'p' && text[i + 2] == 'x')
					return i + 3;
				if (text[i + 1] == '%')
					return i + 2;
				return i + 1;
			}
			else
			{
				while (text[j] != 'p' && text[j] != '%')
					++j;
				n = charRangeToInt(text, i, j);
				if (text[j] == 'p')
				{
					p->bottom[0] = 1;
					p->bottom[1] = n;
					return j + 2;
				}
				else
				{
					p->bottom[0] = 2;
					p->bottom[1] = n;
					return j + 1;
				}
			}

		}
		else
			return -1;
	}
	case 'c':{
		p->priority.color = priority;
		++i;
		p->color[0] = convert16to10(text + i);
		p->color[1] = convert16to10(text + i + 1);
		p->color[2] = convert16to10(text + i + 2);
		p->char_color[0] = text[i];
		p->char_color[1] = text[i + 1];
		p->char_color[2] = text[i + 2];
		return i + 3;
	}
	case 'd':{
		p->priority.display = priority;

		if (text[i] == 'i')	//inline
		{
			p->display = 0;
			return i + 6;
		}
		else if (text[i] == 'b')//block
		{
			p->display = 1;
			return i + 5;
		}
		else	//none
		{
			p->display = 2;
			return i + 4;
		}
	}
	case 'f':{
		if (text[start + 6] == 'i')	//font-size
		{
			p->priority.font_size = priority;
			while (text[j] != 'p')
				++j;
			p->font_size = charRangeToInt(text, i, j);
			return j + 2;
		}
		else if (text[start + 6] == 't')	//font-style
		{
			p->priority.font_style = priority;
			if (text[i] == 'n')
			{
				p->font_style = 0;
				return i + 6;
			}
			else
			{
				p->font_style = 1;
				return i + 6;
			}
		}
		else	//font-weight
		{
			p->priority.font_weight = priority;
			if (text[i] == 'n')
			{
				p->font_weight = 0;
				return i + 6;
			}
			else
			{
				p->font_weight = 1;
				return i + 4;
			}
		}
	}
	case 'h':{
		p->priority.height = priority;

		if (text[i] == 'a')//auto
		{
			p->height[0] = 0;
			return (i + 4);
		}
		else if (text[i] == '0')
		{
			p->height[0] = 1;
			p->height[1] = 0;
			if (text[i + 1] == 'p' && text[i + 2] == 'x')
				return i + 3;
			if (text[i + 1] == '%')
				return i + 2;
			return i + 1;
		}
		else
		{
			while (text[j] != 'p' && text[j] != '%')
				++j;
			n = charRangeToInt(text, i, j);
			if (text[j] == 'p')
			{
				p->height[0] = 1;
				p->height[1] = n;
				return j + 2;
			}
			else
			{
				p->height[0] = 2;
				p->height[1] = n;
				return j + 1;
			}
		}

	}
	case 'l':{
		if (text[start + 1] == 'e')
		{
			p->priority.left = priority;

			if (text[i] == 'a')//auto
			{
				p->left[0] = 0;
				return (i + 4);
			}
			else if (text[i] == '0')
			{
				p->left[0] = 1;
				p->left[1] = 0;
				if (text[i + 1] == 'p' && text[i + 2] == 'x')
					return i + 3;
				if (text[i + 1] == '%')
					return i + 2;
				return i + 1;
			}
			else
			{
				while (text[j] != 'p' && text[j] != '%')
					++j;
				n = charRangeToInt(text, i, j);
				if (text[j] == 'p')
				{
					p->left[0] = 1;
					p->left[1] = n;
					return j + 2;
				}
				else
				{
					p->left[0] = 2;
					p->left[1] = n;
					return j + 1;
				}
			}
		}
		else if (text[start + 5] == 'b')
		{
			p->priority.line_break = priority;
			if (text[i] == 'n')
			{
				p->line_break = 0;
				return i + 6;
			}
			else
			{
				p->line_break = 1;
				return i + 5;
			}
		}
		else
		{
			p->priority.line_height = priority;

			while ((text[j] >= '0' && text[j] <= '9') || text[j] == '.')
				++j;
			if (text[j + 1] == 'x')
			{
				p->line_height_px = 1;
				p->line_height = charRangeToInt(text, i, j);
				return j + 2;
			}
			else
			{
				p->line_height_px = 0;
				p->double_line_height = atof(text + i);
				strncpy(p->char_line_height, text + i, j - i);
				p->char_line_height[j - i] = '\0';
				return j;
			}
		}
	}
	case 'm':{
		p->priority.margin = priority;

		for (tmp = 0; tmp < 4; ++tmp)
		{
			j = i;
			if (text[i] == 'a')//auto
			{
				p->margin[tmp][0] = 0;
				i = (i + 4);
			}
			else if (text[i] == '0')
			{
				p->margin[tmp][0] = 1;
				p->margin[tmp][1] = 0;
				if (text[i + 1] == 'p' && text[i + 2] == 'x')
					i = i + 3;
				else if (text[i + 1] == 'e')
					i = i + 3;
				else if (text[i + 1] == '%')
					i = i + 2;
				else
					i = i + 1;
			}
			else
			{
				while (text[j] != 'p' && text[j] != '%' && text[j] != 'e')
					++j;
				if (text[j] == 'e')
				{
					p->margin[tmp][0] = 3;
					p->double_margin[tmp] = atof(text + i);
					i = j + 2;
				}
				else
				{
					n = charRangeToInt(text, i, j);
					if (text[j] == 'p')
					{
						p->margin[tmp][0] = 1;
						p->margin[tmp][1] = n;
						i = j + 2;
					}
					else
					{
						p->margin[tmp][0] = 2;
						p->margin[tmp][1] = n;
						i = j + 1;
					}
				}
			}
		}

		return i;

	}
	case 'p':{
		if (text[start + 1] == 'a')
		{
			p->priority.padding = priority;

			for (tmp = 0; tmp < 4; ++tmp)
			{
				j = i;
				if (text[i] == '0')
				{
					p->padding[tmp][0] = 1;
					p->padding[tmp][1] = 0;
					if (text[i + 1] == 'p' && text[i + 2] == 'x')
						i = i + 3;
					else if (text[i + 1] == '%')
						i = i + 2;
					else
						i = i + 1;
				}
				else
				{
					while (text[j] != 'p' && text[j] != '%')
						++j;

					n = charRangeToInt(text, i, j);
					if (text[j] == 'p')
					{
						p->padding[tmp][0] = 1;
						p->padding[tmp][1] = n;
						i = j + 2;
					}
					else
					{
						p->padding[tmp][0] = 2;
						p->padding[tmp][1] = n;
						i = j + 1;
					}

				}
			}

			return i;

		}
		else
		{
			p->priority.position = priority;

			if (text[i] == 's')
			{
				p->position = 0;
				return i + 6;
			}
			else if (text[i] == 'r')
			{
				p->position = 1;
				return i + 8;
			}
			else
			{
				p->position = 2;
				return i + 8;
			}
		}
	}
	case 'r':{
		p->priority.right = priority;

		if (text[i] == 'a')//auto
		{
			p->right[0] = 0;
			return (i + 4);
		}
		else if (text[i] == '0')
		{
			p->right[0] = 1;
			p->right[1] = 0;
			if (text[i + 1] == 'p' && text[i + 2] == 'x')
				return i + 3;
			if (text[i + 1] == '%')
				return i + 2;
			return i + 1;
		}
		else
		{
			while (text[j] != 'p' && text[j] != '%')
				++j;
			n = charRangeToInt(text, i, j);
			if (text[j] == 'p')
			{
				p->right[0] = 1;
				p->right[1] = n;
				return j + 2;
			}
			else
			{
				p->right[0] = 2;
				p->right[1] = n;
				return j + 1;
			}
		}
	}
	case 't':{
		if (text[start + 1] == 'e')
		{
			p->priority.text_align = priority;
			if (text[i] == 'l')
			{
				p->position = 0;
				return i + 4;
			}
			else if (text[i] == 'c')
			{
				p->position = 1;
				return i + 6;
			}
			else
			{
				p->position = 2;
				return i + 5;
			}
		}
		else
		{
			p->priority.top = priority;

			if (text[i] == 'a')//auto
			{
				p->top[0] = 0;
				return (i + 4);
			}
			else if (text[i] == '0')
			{
				p->top[0] = 1;
				p->top[1] = 0;
				if (text[i + 1] == 'p' && text[i + 2] == 'x')
					return i + 3;
				if (text[i + 1] == '%')
					return i + 2;
				return i + 1;
			}
			else
			{
				while (text[j] != 'p' && text[j] != '%')
					++j;
				n = charRangeToInt(text, i, j);
				if (text[j] == 'p')
				{
					p->top[0] = 1;
					p->top[1] = n;
					return j + 2;
				}
				else
				{
					p->top[0] = 2;
					p->top[1] = n;
					return j + 1;
				}
			}

		}
	}
	case 'w':{
		p->priority.width = priority;

		if (text[i] == 'a')//auto
		{
			p->width[0] = 0;
			return (i + 4);
		}
		else if (text[i] == '0')
		{
			p->width[0] = 1;
			p->width[1] = 0;
			if (text[i + 1] == 'p' && text[i + 2] == 'x')
				return i + 3;
			if (text[i + 1] == '%')
				return i + 2;
			return i + 1;
		}
		else
		{
			while (text[j] != 'p' && text[j] != '%')
				++j;
			n = charRangeToInt(text, i, j);
			if (text[j] == 'p')
			{
				p->width[0] = 1;
				p->width[1] = n;
				return j + 2;
			}
			else
			{
				p->width[0] = 2;
				p->width[1] = n;
				return j + 1;
			}
		}
	}
	default:{
		return -1;
	}
	}
}

/*
* 函数名称：createCSSRuleNode
* 函数功能：读取一个 选择器{} 生成一个css规则结点
* 输入参数： 优先级， 字符串首地址 前闭后开的两个下标
* 返回值：新生成的css结点的地址
*/
struct css_attributes * createCSSRuleNode(int priority, char *text, int start, int end)
{
	struct css_attributes *p;
	int i;

	if ((p = (struct css_attributes *)malloc(sizeof(struct css_attributes))) == NULL)
		exit(OVERFLOW);
	memset(p, 0, sizeof(struct css_attributes));

	p->priority.offset_left = -1;
	p->priority.offset_top = -1;
	p->priority.display = -1;
	p->priority.position = -1;
	p->priority.width = -1;
	p->priority.height = -1;
	p->priority.padding = -1;
	p->priority.border = -1;
	p->priority.margin = -1;
	p->priority.left = -1;
	p->priority.right = -1;
	p->priority.top = -1;
	p->priority.bottom = -1;
	p->priority.color = -1;
	p->priority.line_height = -1;
	p->priority.font_size = -1;
	p->priority.font_style = -1;
	p->priority.font_weight = -1;
	p->priority.text_align = -1;
	p->priority.line_break = -1;

	i = start;

	while (i != end)
	{
		i = ReadACssRule(p, priority, text, i);
		if (i == -1)
			return NULL;
	}

	return p;
}

/*
* 函数名称：updateHtmlNodeCss
* 函数功能：将html结点中css规则和css规则进行更新 若css优先级大 则更新
* 输入参数： html结点 css结点
* 返回值：布尔型
*/
BOOL updateHtmlNodeCss(struct htmlnode * p, struct css_attributes * pcss)
{
	if (pcss->priority.display >= p->pcss->priority.display)
	{
		p->pcss->display = pcss->display;
		p->pcss->priority.display = pcss->priority.display;
	}
	if (pcss->priority.position >= p->pcss->priority.position)
	{
		p->pcss->position = pcss->position;
		p->pcss->priority.position = pcss->priority.position;
	}
	if (pcss->priority.width >= p->pcss->priority.width)
	{
		p->pcss->width[0] = pcss->width[0];
		p->pcss->width[1] = pcss->width[1];
		p->pcss->priority.width = pcss->priority.width;
	}
	if (pcss->priority.height >= p->pcss->priority.height)
	{
		p->pcss->height[0] = pcss->height[0];
		p->pcss->height[1] = pcss->height[1];
		p->pcss->priority.height = pcss->priority.height;
	}
	if (pcss->priority.padding >= p->pcss->priority.padding)
	{
		p->pcss->padding[0][0] = pcss->padding[0][0];
		p->pcss->padding[0][1] = pcss->padding[0][1];
		p->pcss->padding[1][0] = pcss->padding[1][0];
		p->pcss->padding[1][1] = pcss->padding[1][1];
		p->pcss->padding[2][0] = pcss->padding[2][0];
		p->pcss->padding[2][1] = pcss->padding[2][1];
		p->pcss->padding[3][0] = pcss->padding[3][0];
		p->pcss->padding[3][1] = pcss->padding[3][1];
		p->pcss->priority.padding = pcss->priority.padding;
	}
	if (pcss->priority.border >= p->pcss->priority.border)
	{
		p->pcss->border[0] = pcss->border[0];
		p->pcss->border[1] = pcss->border[1];
		p->pcss->border[2] = pcss->border[2];
		p->pcss->border[3] = pcss->border[3];
		p->pcss->priority.border = pcss->priority.border;
	}
	if (pcss->priority.margin >= p->pcss->priority.margin)
	{
		p->pcss->margin[0][0] = pcss->margin[0][0];
		p->pcss->margin[0][1] = pcss->margin[0][1];
		p->pcss->margin[1][0] = pcss->margin[1][0];
		p->pcss->margin[1][1] = pcss->margin[1][1];
		p->pcss->margin[2][0] = pcss->margin[2][0];
		p->pcss->margin[2][1] = pcss->margin[2][1];
		p->pcss->margin[3][0] = pcss->margin[3][0];
		p->pcss->margin[3][1] = pcss->margin[3][1];
		p->pcss->double_margin[0] = pcss->double_margin[0];
		p->pcss->double_margin[1] = pcss->double_margin[1];
		p->pcss->double_margin[2] = pcss->double_margin[2];
		p->pcss->double_margin[3] = pcss->double_margin[3];
		p->pcss->priority.margin = pcss->priority.margin;

	}
	if (pcss->priority.left >= p->pcss->priority.left)
	{
		p->pcss->left[0] = pcss->left[0];
		p->pcss->left[1] = pcss->left[1];
		p->pcss->left[2] = pcss->left[2];
		p->pcss->left[3] = pcss->left[3];
		p->pcss->priority.left = pcss->priority.left;

	}
	if (pcss->priority.right >= p->pcss->priority.right)
	{
		p->pcss->right[0] = pcss->right[0];
		p->pcss->right[1] = pcss->right[1];
		p->pcss->right[2] = pcss->right[2];
		p->pcss->right[3] = pcss->right[3];
		p->pcss->priority.right = pcss->priority.right;

	}
	if (pcss->priority.top >= p->pcss->priority.top)
	{
		p->pcss->top[0] = pcss->top[0];
		p->pcss->top[1] = pcss->top[1];
		p->pcss->top[2] = pcss->top[2];
		p->pcss->top[3] = pcss->top[3];
		p->pcss->priority.top = pcss->priority.top;

	}
	if (pcss->priority.bottom >= p->pcss->priority.bottom)
	{
		p->pcss->bottom[0] = pcss->bottom[0];
		p->pcss->bottom[1] = pcss->bottom[1];
		p->pcss->bottom[2] = pcss->bottom[2];
		p->pcss->bottom[3] = pcss->bottom[3];
		p->pcss->priority.bottom = pcss->priority.bottom;

	}
	if (pcss->priority.color >= p->pcss->priority.color)
	{
		p->pcss->color[0] = pcss->color[0];
		p->pcss->color[1] = pcss->color[1];
		p->pcss->color[2] = pcss->color[2];
		p->pcss->char_color[0] = pcss->char_color[0];
		p->pcss->char_color[1] = pcss->char_color[1];
		p->pcss->char_color[2] = pcss->char_color[2];
		p->pcss->priority.color = pcss->priority.color;

	}
	if (pcss->priority.line_height >= p->pcss->priority.line_height)
	{
		p->pcss->line_height_px = pcss->line_height_px;
		p->pcss->line_height = pcss->line_height;
		p->pcss->double_line_height = pcss->double_line_height;
		p->pcss->priority.line_height = pcss->priority.line_height;

	}
	if (pcss->priority.font_size >= p->pcss->priority.font_size)
	{
		p->pcss->font_size = pcss->font_size;
		p->pcss->priority.font_size = pcss->priority.font_size;

	}
	if (pcss->priority.font_style >= p->pcss->priority.font_style)
	{
		p->pcss->font_style = pcss->font_style;
		p->pcss->priority.font_style = pcss->priority.font_style;

	}
	if (pcss->priority.font_weight >= p->pcss->priority.font_weight)
	{
		p->pcss->font_weight = pcss->font_weight;
		p->pcss->priority.font_weight = pcss->priority.font_weight;

	}
	if (pcss->priority.text_align >= p->pcss->priority.text_align)
	{
		p->pcss->text_align = pcss->text_align;
		p->pcss->priority.text_align = pcss->priority.text_align;

	}
	if (pcss->priority.line_break >= p->pcss->priority.line_break)
	{
		p->pcss->line_break = pcss->line_break;
		p->pcss->priority.line_break = pcss->priority.line_break;

	}
	return TRUE;
}

/*
* 函数名称：isParent
* 函数功能：判断某个结点p是否有某个父辈parent
* 输入参数： 孩子结点 树中任意结点
* 返回值：所孩子结点拥有该父辈 返回TRUE
*/
BOOL isParent(struct binnode *pchild, struct binnode *parent)
{

	struct binnode *p;

	p = pchild;

	while (p != NULL)
	{
		if (p->parent == parent)
			return TRUE;
		else
			p = p->parent;
	}
	return FALSE;
}

/*
* 函数名称：updateInheritHtmlNodeCssOfOne
* 函数功能：更新某个结点所有后代的遗传css属性
* 输入参数： html头结点 需要更新后代css规则的结点
* 返回值：布尔型
*/
BOOL updateInheritHtmlNodeCssOfOne(struct htmlnode *head, struct htmlnode * parent)
{
	struct htmlnode * p;
	p = head->next;

	while (p->next != NULL)
	{
		if (p->type == 2 || p->type == 3)
		{
			p = p->next;
			continue;
		}
		else
		{
			if (isParent(p->pbinnode, parent->pbinnode) == TRUE)
			{
				if (p->pcss->priority.color == 0)
				{
					p->pcss->color[0] = parent->pcss->color[0];
					p->pcss->color[1] = parent->pcss->color[1];
					p->pcss->color[2] = parent->pcss->color[2];
					p->pcss->char_color[0] = parent->pcss->char_color[0];
					p->pcss->char_color[1] = parent->pcss->char_color[1];
					p->pcss->char_color[2] = parent->pcss->char_color[2];
				}
				if (p->pcss->priority.line_height == 0)
				{
					p->pcss->line_height_px = parent->pcss->line_height_px;
					p->pcss->line_height = parent->pcss->line_height;
					p->pcss->double_line_height = parent->pcss->double_line_height;
				}
				if (p->pcss->priority.font_size == 0)
				{
					p->pcss->font_size = parent->pcss->font_size;
				}
				if (p->pcss->priority.font_style == 0)
				{
					p->pcss->font_style = parent->pcss->font_style;
				}
				if (p->pcss->priority.font_weight == 0)
				{
					p->pcss->font_weight = parent->pcss->font_weight;
				}
				if (p->pcss->priority.text_align == 0)
				{
					p->pcss->text_align = parent->pcss->text_align;
				}
				if (p->pcss->priority.line_break == 0)
				{
					p->pcss->line_break = parent->pcss->line_break;
				}
			}

		}
		p = p->next;
	}

	return TRUE;
}

/*
* 函数名称：updateInheritHtmlNodeCssOfAll
* 函数功能：更新所有节点遗传的css
* 输入参数： html头结点
* 返回值：布尔型
*/
BOOL updateInheritHtmlNodeCssOfAll(struct htmlnode * head)
{
	struct htmlnode * p;
	p = head->next;

	while (p->next != NULL)
	{
		if (p->type == 2 || p->type == 3)
		{
			p = p->next;
		}
		else
		{
			updateInheritHtmlNodeCssOfOne(head, p);
			p = p->next;
		}
	}
	return TRUE;
}

/*
* 函数名称：addNewCssRule
* 函数功能：将字符串形式的css规则写入DOM树
* 输入参数： DOM树 字符串指针 选择器文本区间[i,j) 属性:取值 文本区间[left,high)
* 返回值：无
*/
void addNewCssRule(struct bintree * tree, char *text, int i, int j, int left, int right)
{
	struct htmlnode * p;
	int priority;
	struct css_attributes *pcssnode;

	p = tree->root->phtmlnode;
	while (p->next != NULL)
	{
		if (p->type == 1)
		{
			if (matchAllRule(text, i, j, p->pbinnode) == TRUE)
			{
				//update css
				priority = calPriority(text, i, j);
				p->tmp_selected = 1;
				pcssnode = createCSSRuleNode(priority, text, left, right);
				if (pcssnode == NULL)
				{
					printf("wrong css:");
					myprintf(text, left, right);
					return;
				}
				updateHtmlNodeCss(p, pcssnode);


				free(pcssnode);

			}
		}
		else
		{
			p = p->next;
			continue;
		}
		p = p->next;
	}
}

/*
* 函数名称：loadCSS
* 函数功能：读取css文件所有内容并加载到DOM树
* 输入参数： css内容字符串指针 DOM树 css所在文件夹testN序号
* 返回值：布尔类型
*/
BOOL loadCSS(char *htmltext, struct bintree * tree)
{
	char cssfilename[FILE_NAME_MAX_LENGTH];
	char cssfile[FILE_NAME_MAX_LENGTH];
	int low = 0;
	int high = 0;
	FILE * fpcss;
	char *text;

	int i;
	int j;

	int left;
	int right;


	while (1)
	{
		while (htmltext[low] != '\"')
		{
			if (strncmp(htmltext + low, "</head", 6) == 0)
			{
				goto end;
			}
			++low;

		}
		++low;
		high = low;
		while (htmltext[high] != '\"')
		{
			++high;
		}
		//现在[low, high)是abc.css
		strncpy(cssfile, htmltext + low, high - low);
		cssfile[high - low] = '\0';
		sprintf(cssfilename, "%s", cssfile);
		//printf("%s\n", cssfilename);

		if ((fpcss = fopen(cssfilename, "r")) != NULL)
		{
			readCssFile(&text, fpcss);
			fclose(fpcss);



			//printf("%s\n", text);


			//依次读取div p span#id,xxx,xxx{} 
			i = 0;
			j = 0;

			while (text[i] != '\0')
			{
				j = i;
				while (text[j] != '{')
				{
					++j;
					if (text[j] == ',' || text[j] == '{')
					{
						//[i,j)是一个选择器
						left = j;
						while (text[left] != '{')
							++left;
						right = ++left;
						while (text[right] != '}')
							++right;
						//[left,right)是{}的内容(不含{} )

						//myprintf(text, i, j);

						//myprintf(text, left, right);
						//printf("\n");

						addNewCssRule(tree, text, i, j, left, right);


						i = j + 1;


					}
				}
				i = right + 1;


			}

			free(text);
		}

		low = high + 1;
	}
end:
	return TRUE;
}