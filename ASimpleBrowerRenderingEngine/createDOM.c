#include "header.h"

extern struct htmlnode * saveToHtmlList(struct htmlnode *p, char * text, int low, int high, int type);

/*
* 函数名称：tagType
* 函数功能：返回一个标签的类型
* 输入参数：字符指针指向标签的第一个字母
* 返回值：整数 具体编码如下
* 1 body
* 2 div
* 3 p
* 4 h
* 5 span
* 6 strong
* 7 em
*/
int tagType(char * p)
{
	switch (*p)
	{
	case 'b': return 1;
	case 'd': return 2;
	case 'p': return 3;
	case 'h': return 4;
	case 's': {
		if (*(p + 1) == 'p')
			return 5;
		else
			return 6;
	}
	case 'e': return 7;
	default: return -1;
	}
}

/*
* 函数名称：tagWidth
* 函数功能：返回一个标签的长度
* 输入参数：字符指针指向标签的第一个字母
* 返回值：标签长度（字符数）
*/
int tagWidth(char * ch)
{
	int width;

	switch (tagType(ch))
	{
	case 1:;
	case 5:{
		width = 4;
		break;
	}
	case 2: {
		width = 3;
		break;
	}
	case 3:;
	case 4: {
		width = 1;
		break;
	}
	case 6: {
		width = 6;
		break;
	}
	case 7: {
		width = 2;
		break;
	}
	}

	return width;
}

/*
* 输入参数：tree二叉树 phtmlnode指向需要插入的具体信息 parent指向以哪个结点作为逻辑父结点
* 返回值： 新插入的结点指针
* 功能： 新建结点，设置phtmlnode指向的具体信息 并指向逻辑父结点parent
*/
struct binnode * createBinnode(struct bintree *tree, struct htmlnode *phtmlnode, struct binnode *parent)
{
	struct binnode *p;

	if ((p = (struct binnode *)malloc(sizeof(struct binnode))) == NULL)
	{
		exit(OVERFLOW);
	}
	memset(p, 0, sizeof(struct binnode));

	p->parent = parent;
	p->phtmlnode = phtmlnode;

	phtmlnode->pbinnode = p;

	++tree->size;

	return p;
}

/*
* 输入参数：tree二叉树 phtmlnode指向需要插入的具体信息 parent表示在哪个结点上插入
* 返回值： 新插入的结点指针
* 功能： 新建结点，设置phtmlnode指向对应html结点 并和父结点互相指向
* 父结点left_child指向新建结点 tree的size增加1
*/
struct binnode * insertAsLeftChild(struct bintree *tree, struct htmlnode *phtmlnode, struct binnode *parent)
{
	struct binnode * p;

	p = createBinnode(tree, phtmlnode, parent);

	parent->left_child = p;
	p->rev_left_child = parent;

	return p;
}

/*
* 输入参数：tree二叉树 phtmlnode指向需要插入的结点指针,rev_right_sibling在哪个节点上插入
* 返回值： 新插入的结点指针
* 功能： 新建结点，设置phtmlnode指向的具体信息 与rev_right_sibling相互指向 另外设置新建结点的逻辑父结点指针
* 父结点right_sibling指向新建结点 tree的size增加1
*/
struct binnode * insertAsRightSibling(struct bintree *tree, struct htmlnode *phtmlnode, struct binnode *rev_right_sibling)
{
	struct binnode * p;

	p = createBinnode(tree, phtmlnode, rev_right_sibling->parent);

	p->rev_right_sibling = rev_right_sibling;
	rev_right_sibling->right_sibling = p;

	return p;
}

/*
* 函数名称：setIdAndClass
* 函数功能：将id和class信息写入结点 设置htmlnode的tagtype element id how_many_class和classname
* 输入参数：html结点 一串字符串 用下标表示 例如<div id = "abc" class = "def gh" >
* 返回值：BOOL类型
*/
BOOL setIdAndClass(struct htmlnode * q, char * text, int low, int high)
{
	int i;
	int j;
	int loop_two_times = 2;

	++low;
	i = low;
	j = low;

	while (text[j] != ' ' && text[j] != '\t' && text[j] != '\n' && text[j] != '>')
		++j;
	//现在[i, j)是标签名
	strncpy(q->element, text + i, j - i);
	q->element[j - i + 1] = '\0';

	q->tagtype = tagType(text + i);

	i = j;
	if (text[i] == '>')
		return TRUE;
	if (text[i] == ' ')
	{
		++i;
		++j;
	}
	if (text[i] == '>')
		return TRUE;

	while (1)
	{

		if (text[i] == ' ')
		{
			++i;
			continue;
		}
		else if (text[i] == '>')
		{
			return TRUE;
		}
		else if (text[i] == 'i')
		{
			while (text[i] != '\"')
			{
				++i;
			}
			++i;
			j = i;	//指向id名
			while (text[j] != '\"')
			{
				++j;
			}
			strncpy(q->id, text + i, j - i);
			q->id[j - i + 1] = '\0';
			++j;
			i = j;
		}
		else
		{
			while (text[i] != '\"')
			{
				++i;
			}
			++i;
			j = i;
			while (text[j] != '\"')
			{
				++j;
				if (text[j] == ' ' || text[j] == '\"')
				{
					++q->how_many_class;
					strncpy(q->classname[q->how_many_class], text + i, j - i);
					q->classname[q->how_many_class][j - i + 1] = '\0';
					i = j + 1;
				}

			}
			j = i;
		}
	}

}

/*
* 函数名称：createHtmlList
* 函数功能：读取一串字符串 变为html结点存入链表
* 输入参数：字符串和链表头结点
* 返回值：整数 TRUE表示成功
*/
int createHtmlList(char * text, struct htmlnode * head)
{
	int low = 0;
	int high = 0;
	struct htmlnode *p = head;
	int type;

	while ((strncmp(text + low, "<body", 5)) != 0 && text[low] != '\0')
	{
		++low;
	}
	if (text[low] == '\0')	//没有<body>的html
	{
		return 0;	//没有body错误
	}
	//这时候low指向<body>中的< 。
	high = low;

	while (text[high] != '\0')
	{
		if (text[low] == '<')	//普通标签
		{
			if (text[low + 1] == '\0')
			{
				return -1;	//<标签错误
			}

			if (text[low + 1] == '/')	//结束标签
			{
				type = 3;
				while (text[high] != '\0' && text[high] != '>')
				{
					++high;
				}
				if (text[high] == '\0')
				{
					return -1;	//标签错误
				}
				//现在high指向</div>的>处

			}
			else	//开始标签的情况<div id= "abc" class="ab cd efg">
			{
				type = 1;
				while (text[high] != '\0' && text[high] != '>')
				{
					if (text[high] == '\"')	//遇见引号寻找对应右引号
					{
						++high;
						while (text[high] != '\0' && text[high] != '\"')
						{
							++high;
						}
						if (text[high] == '\0')
						{
							return -2;	//引号错误
						}
						++high;	//现在high指向对应右引号的下一个
					}
					else
					{
						++high;
					}
				}
				if (text[high] == '\0')
				{
					return -1;	//标签错误
				}
				//现在high指向<xxx>的>
			}

			++high;
			//[low, high)是<div id="ab">或</div   >
		}
		else if (text[low] == ' ' || text[low] == '\t' || text[low] == '\n')	//跳过所有空白符
		{
			++low;
			high = low;
			continue;
		}

		else	//文本结点开始
		{
			type = 2;
			while (text[high] != '\0' && text[high] != '<')
			{
				++high;
			}
			if (text[high] == '\0')
			{
				return -3;	//以文本结束 不是</body>结束
			}
			//high指向<
			--high;
			while (text[high] == ' ' || text[high] == '\t' || text[high] == '\n')
			{
				--high;
			}
			++high;
			//现在[low, high)是文本内容
		}


		p = saveToHtmlList(p, text, low, high, type);
		//printf("%s****\n", text+low);
		low = high;
	}
	return TRUE;
}

/*
* 函数名称：destroyListAndTreeNodes
* 函数功能：销毁用到的数据结构 释放内存
* 输入参数：链表头结点
* 返回值：BOOL类型
*/
BOOL destroyListAndTreeNodes(struct htmlnode * head)
{
	struct htmlnode * p = head->next;
	struct htmlnode * q = p;
	struct htmlnode * tail;
	while (p->next != NULL)
	{
		q = p;
		p = p->next;
		if (q->type == 2)
		{
			free(q->pinner_text);
		}
		if (q->type == 1)
		{
			free(q->pcss);
		}
		if (q->pbinnode != NULL)
		{
			if (q->pbinnode->output_name != NULL)
				free(q->pbinnode->output_name);
			free(q->pbinnode);
		}
		free(q);
	}
	tail = p;
	head->next = p;

	return TRUE;
}

/*
* 函数名称：cssInit
* 函数功能：初始化css属性
* 输入参数：css属性指针
* 返回值：无
*/
void cssInit(struct css_attributes * p)
{
	p->padding[0][0] = 1;
	p->padding[1][0] = 1;
	p->padding[2][0] = 1;
	p->padding[3][0] = 1;

	p->border[0] = 1;
	p->border[1] = 1;
	p->border[2] = 1;
	p->border[3] = 1;

	p->margin[0][0] = 1;
	p->margin[1][0] = 1;
	p->margin[2][0] = 1;
	p->margin[3][0] = 1;
	p->margin[0][1] = 0;
	p->margin[1][1] = 0;
	p->margin[2][1] = 0;
	p->margin[3][1] = 0;

	p->color[0] = 0;
	p->color[1] = 0;
	p->color[2] = 0;
	p->char_color[0] = '0';
	p->char_color[1] = '0';
	p->char_color[2] = '0';
	p->char_color[3] = '\0';

	p->font_size = 16;

	p->double_line_height = 1.2;
	p->char_line_height[0] = '1';
	p->char_line_height[1] = '.';
	p->char_line_height[2] = '2';
	p->char_line_height[3] = '\0';

}

/*
* 函数名称：htmlCssInit
* 函数功能：初始化一个html结点特有的css属性
* 输入参数：标签类型和html结点
* 返回值：无
*/
void htmlCssInit(int htmltype, struct css_attributes * p)
{
	cssInit(p);

	switch (htmltype)
	{
		/* 1 body
		* 2 div
		* 3 p
		* 4 h
		* 5 span
		* 6 strong
		* 7 em
		*/
	case 1:{	//body
		p->display = 1;
		p->margin[0][1] = 8;
		p->margin[1][1] = 8;
		p->margin[2][1] = 8;
		p->margin[3][1] = 8;
		break;
	}
	case 2:{	//div
		p->display = 1;
		break;
	}
	case 3:{	//p
		p->display = 1;
		p->margin[0][0] = 3;
		p->margin[2][0] = 3;
		p->double_margin[0] = 1;
		p->double_margin[2] = 1;
		break;
	}
	case 4:{	//h
		p->margin[0][0] = 3;
		p->margin[2][0] = 3;
		p->double_margin[0] = 0.67;
		p->double_margin[2] = 0.67;
		p->display = 1;
		p->font_size = 32;
		p->font_weight = 1;

		break;
	}
	case 5:{	//span
		break;
	}
	case 6:{	//strong
		p->font_weight = 1;
		break;
	}
	case 7:{	//em
		p->font_style = 1;
		break;
	}
	default: break;
	}
}

/*
* 函数名称：correctUnmatchedHtmlNode
* 函数功能：修正html中不匹配的标签
* 输入参数：htmlnode链表头结点
* 返回值：布尔型
*/
BOOL correctUnmatchedHtmlNode(struct htmlnode * head)
{
	struct htmlnode * match_stack[TAG_NEST_MAX_NUM];
	int unmached_stack[8] = { 0 };

	int top_match = 0;


	struct htmlnode * p = head->next;
	struct htmlnode * q;

	while (p->next != NULL)
	{
		if (p->type == 1)
		{
			//push p
			++top_match;
			match_stack[top_match] = p;
			p = p->next;
			continue;
		}
		else if (p->type == 3)
		{
			if (p->tagtype != match_stack[top_match]->tagtype)
			{
				if (unmached_stack[p->tagtype] > 0)
				{
					--unmached_stack[p->tagtype];
					//链表中删去p
					q = p;
					p->pred->next = p->next;
					p->next->pred = p->pred;
					p = p->next;
					free(q);
					continue;
				}
				else
				{
					//补上结束标签
					if ((q = (struct htmlnode *)malloc(sizeof(struct htmlnode))) == NULL)
						exit(OVERFLOW);
					memset(q, 0, sizeof(struct htmlnode));

					//构成双向链表
					q->next = match_stack[top_match]->next;
					match_stack[top_match]->next->pred = q;
					match_stack[top_match]->next = q;
					q->pred = match_stack[top_match];

					//初始化
					q->type = 3;
					q->tagtype = q->pred->tagtype;
					strcpy(q->element, q->pred->element);

					++unmached_stack[q->tagtype];
					top_match--;
					continue;

				}
			}
			else
			{
				p = p->next;
				--top_match;
				continue;
			}
		}
		else	//text结点
		{
			p = p->next;
			continue;
		}
	}

	return TRUE;
}

/*
* 函数名称：createDOMTree
* 函数功能：生成DOM树
* 输入参数：树指针tree和html结点链表头结点
* 返回值：布尔型
*/
BOOL createDOMTree(struct bintree * tree, struct htmlnode * head)
{
	int treestatus = -1;	//-1插入左孩子 1插入右孩子
	struct binnode *pt;
	struct htmlnode *p;
	int height = 1;	//逻辑深度
	//初始化树
	if ((tree->root = (struct binnode *)malloc(sizeof(struct binnode))) == NULL)
	{
		exit(OVERFLOW);
	}
	memset(tree->root, 0, sizeof(struct binnode));
	tree->root->phtmlnode = head->next;
	head->next->pbinnode = tree->root;
	//构建树的准备工作
	p = head->next->next;
	pt = tree->root;
	++tree->size;
	pt->height = 1;
	treestatus = -1;
	//构造树
	while (p->next != NULL)
	{
		if (treestatus == -1)
		{
			if (p->type == 1)	//开始标签
			{
				pt = insertAsLeftChild(tree, p, pt);
				pt->height = ++height;
				p = p->next;
				continue;
			}
			else if (p->type == 2)	//文本结点
			{
				pt = insertAsLeftChild(tree, p, pt);
				pt->height = ++height;
				p = p->next;
				treestatus = 1;
			}
			else	//结束标签
			{
				if (pt->phtmlnode->type == 2)
				{
					pt = pt->parent;
					--height;
				}
				p = p->next;
				treestatus = 1;
			}
		}
		else
		{
			if (p->type == 3)	//结束标签
			{
				pt = pt->parent;
				--height;
				p = p->next;
				continue;
			}
			else	//开始标签
			{
				pt = insertAsRightSibling(tree, p, pt);
				pt->height = height;
				p = p->next;
				treestatus = -1;
				continue;
			}
		}
	}

	return TRUE;
}

/*
* 函数名称：saveToHtmlList
* 函数功能：将单位的字符串转换为html结点
* 输入参数：链表结点 文本指针 下标[low, high) 结点类型是标签还是文本
* 返回值：新插入的链表结点指针
*/
struct htmlnode * saveToHtmlList(struct htmlnode *p, char * text, int low, int high, int type)
{
	struct htmlnode *q;
	int tagtype;
	int width;
	if ((q = (struct htmlnode *)malloc(sizeof(struct htmlnode))) == NULL)
		exit(OVERFLOW);
	memset(q, 0, sizeof(struct htmlnode));

	//构成双向链表
	q->next = p->next;
	p->next->pred = q;
	p->next = q;
	q->pred = p;

	//初始化
	q->type = type;

	//处理文本节点
	if (type == 2)
	{
		if ((q->pinner_text = (char *)malloc((high - low + 1)*sizeof(char))) == NULL)
		{
			exit(OVERFLOW);
		}
		memset(q->pinner_text, 0, (high - low + 1)*sizeof(char));
		strncpy(q->pinner_text, text + low, high - low);
		q->pinner_text[high - low] = '\0';
		//printf("%s\n", q->pinner_text);
		return q;
	}

	//处理闭合标签
	if (type == 3)
	{
		tagtype = tagType(text + low + 2);
		q->tagtype = tagtype;
		width = tagWidth(text + low + 2);

		strncpy(q->element, text + low + 2, width);
		q->element[width + 1] = '\0';
	}
	else	//处理开始标签
	{
		setIdAndClass(q, text, low, high);

		if ((q->pcss = (struct css_attributes *)malloc(sizeof(struct css_attributes))) == NULL)
		{
			exit(OVERFLOW);
		}
		memset(q->pcss, 0, sizeof(struct css_attributes));

		htmlCssInit(q->tagtype, q->pcss);

	}

	return q;

}