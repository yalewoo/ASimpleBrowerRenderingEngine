#include "header.h"

extern int createHtmlList(char * text, struct htmlnode * head);
extern BOOL correctUnmatchedHtmlNode(struct htmlnode * head);
extern BOOL createDOMTree(struct bintree * tree, struct htmlnode * head);
extern BOOL loadCSS(char *htmltext, struct bintree * tree);
extern BOOL updateInheritHtmlNodeCssOfAll(struct htmlnode * head);
extern BOOL createOutputTag(struct htmlnode * head);
extern BOOL calBinNodeSub(struct htmlnode * head);
extern BOOL calWidth(struct htmlnode *head);
extern BOOL calInlineWidth1(struct htmlnode *head);
extern BOOL calHeight(struct htmlnode *head);
extern BOOL calOffsetleft(struct htmlnode *head);
extern BOOL calOffsetTop(struct htmlnode *head);
extern BOOL outputWebTxt(struct htmlnode * head, int index);
extern BOOL outputPNG(struct htmlnode *head, int index);
extern BOOL destroyListAndTreeNodes(struct htmlnode * head);


//读取html文件并去掉标签内空格
void readHtmlFile(char **ptext, FILE * fphtml)
{
	int ch;     //用于读取文件内容
	long char_num = 0;  //用于统计index.html长度
	int i;
	char *text;
	int in_tag_flag = 0;
	//统计index.html长度 分配相应大小内存
	for (char_num = 0; (ch = fgetc(fphtml)) != EOF; ++char_num)
	{
		;
	}
	fseek(fphtml, 0, SEEK_SET);
	text = (char *)malloc((char_num + 1)*sizeof(char));

	i = 0;
	while ((ch = fgetc(fphtml)) != EOF)
	{
		if (text[i - 1] == '<' && (ch == ' ' || ch == '\t' || ch == '\n'))
		{
			continue;	//舍弃<  div>中的空白
		}
		if ((text[i - 1] == '/' && text[i - 2] == '<') && (ch == ' ' || ch == '\t' || ch == '\n'))
		{
			continue;	//舍弃</    div>空白
		}
		if (in_tag_flag == 1 && text[i - 1] == ' ' && ch == ' ')
		{	//将标签中连续空格改为1个
			continue;
		}

		if (ch == '<')
			in_tag_flag = 1;
		else if (ch == '>')
			in_tag_flag = 0;

		text[i] = ch;
		++i;

	}

	text[i] = '\0';

	*ptext = text;
}

/*
 * 函数功能：读取html解析并输出
 * 输入参数：一个是html文件名 一个是整数index用于生成对应的webN.txt
 * 返回值：文件不存在返回TRUE 否则返回FALSE
*/
BOOL renderingEngine(char *htmlfilename, char *outdir)
{
	FILE * fphtml;
	char * text;    //用于指向文本字符串 index.html的全部内容
	struct htmlnode head = { 0 };
	struct htmlnode tail = { 0 };

	struct bintree tree = { 0 };

	//初始化双向链表
	head.pred = NULL;
	head.next = &tail;
	tail.pred = &head;
	tail.next = NULL;

	//打开html文件
	if ((fphtml = fopen(htmlfilename, "r")) == NULL)
		return FALSE;

	readHtmlFile(&text, fphtml);	//将html读入text

	fclose(fphtml);

	//printf("test%d\n", index);
	//printf("test%d:\n%s", index, text);

	//建立结点链表
	createHtmlList(text, &head);

	//修正不匹配的标签
	correctUnmatchedHtmlNode(&head);

	//建立DOM树
	createDOMTree(&tree, &head);

	//加载css
	loadCSS(text, &tree);

	//更新需要遗传的属性
	updateInheritHtmlNodeCssOfAll(&head);

	//计算输出时标签名和下标 为输出做准备
	createOutputTag(&head);
	calBinNodeSub(&head);

	//计算宽度
	calWidth(&head);
	calInlineWidth1(&head);

	//计算高度
	calHeight(&head);

	//计算偏移
	calOffsetleft(&head);
	calOffsetTop(&head);

	//输出到webN.txt
	outputWebTxt(&head, outdir);

	//输出到webN.png
	outputPNG(&head, outdir);

	//销毁所用的数据结构 释放内存
	destroyListAndTreeNodes(&head);
		
	free(text);

	return TRUE;
}