#include "header.h"
#include "renderingEngine.h"

int main(void)
{
	int index = 1;  //用于计数第几个文件夹 依次打开test1 test2文件夹等
	char htmlfilename[FILE_NAME_MAX_LENGTH] = "test/test3.html";
	
	//调用RENDERING_ENGINE_H模块依次处理每个html文件 文件不存在时 会返回FALSE 程序结束运行
	renderingEngine(htmlfilename, "test/test3");


	printf("done!\n");

	return 0;

}

//输出一段文本
void myprintf(char * text, int low, int high)
{
	char * buffer;
	if ((buffer = (char *)malloc(sizeof(char)*(high - low + 1))) == NULL)
		exit(OVERFLOW);
	strncpy(buffer, text + low, high - low);
	buffer[high - low] = '\0';
	printf("%s\n", buffer);
	free(buffer);
}