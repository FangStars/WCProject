//支持读取行数，符号数，单词数，以及stoplist和写文件，不支持目录寻找
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
const int maxn = 200;
int wc_char = 0;
int wc_word = 0;
int wc_line = 0;
int wc_codeLine = 0;
int wc_empLine = 0;
int wc_expLine = 0;
int wc_stopWord = 0;
char getWord[50]="";//用来记录每个单词
char lineWord[maxn] = "";
void notFound(FILE *fp)
{
	if (fp == NULL)
	{
		printf("打开有误！\n");
		printf("请按enter键继续....");
		_getch();
		exit(0);
	}
}//未找到文件
bool isaldig(char ch)//判断是否为数字或字母
{
	if (((ch <= 'z') && (ch >= 'a')) || ((ch <= 'Z') && (ch >= 'A')) || ((ch >= '0') && (ch <= '9')))
		return true;
	else
		return false;
}
bool isnone(int s,int n)//判断空行时判断空格和tab
{
	for (int i = s; i < n; i++)
	{
		if (!isspace(lineWord[i]))
			return false;
	}
	return true;
}
bool isexp(int n)//判断注释行
{
	for (int i = 0; i < n; i++) 
	{
		if (((lineWord[i] == '/') && (lineWord[i + 1] == '*')) || ((lineWord[i] == '*') && (lineWord[i + 1] == '/') && isnone(i + 2, n)) || ((lineWord[i] == '/') && (lineWord[i + 1] == '/')))
			return true;
	}
	return false;
}
void countAll(char ch, FILE *fp)
{
	int i = 0;//用于更新getWord
	int j = 0;//用于更新lineWord
	while ((ch = fgetc(fp)) != EOF)
	{
		if (isaldig(ch))
		{
			wc_char++;
			getWord[i] = ch;
			i++;
			lineWord[j] = ch;
			j++;
		}
		else if ((ch == ' '&&isaldig(lineWord[j-1]))||ch==',')//忽略连续空格,遇到空格或逗号则单词数加一
		{
			if (strlen(lineWord) != 0)
			{
				wc_word++;
				//puts(getWord);
			}
			memset(getWord, 0, sizeof(getWord));
			i = 0;
			lineWord[j] = ch;
			j++;
		}
		else if (ch == '\n')
		{
			wc_line++;
			if (strlen(getWord)!=0)
			{
				wc_word++;//每行最后一个单词，若有则加一
				//puts(getWord);
			}
			memset(getWord, 0, sizeof(getWord));
			i = 0;
			if (isnone(0,j)|| (isnone(1,j) && !isspace(lineWord[0])))
			{
				wc_empLine++;
			}
			if (isexp(j))
				wc_expLine++;
			memset(lineWord, 0, sizeof(lineWord));
			j = 0;
		}
		else
		{
			wc_char++;
			lineWord[j] = ch;
			j++;
		}
	}
	wc_line++;//最后一行再判断一遍
	if (strlen(getWord) != 0)
	{
		wc_word++;
		//puts(getWord);
	}
	memset(getWord, 0, sizeof(getWord));
	i = 0;
	if (isnone(0, j) || (isnone(1, j) && !isspace(lineWord[0])))
	{
		wc_empLine++;
	}
	if (isexp(j))
		wc_expLine++;
	memset(lineWord, 0, sizeof(lineWord));
	j = 0;
	wc_codeLine = wc_line - wc_empLine - wc_expLine;
}//顺序查找文件中字符
void countStop(char ch,FILE *fp, FILE *fps)
{
	int i=0;//计数单词
	char stopWord[5][20] = {};
	while ((ch = fgetc(fps)) != EOF)
	{
		if (isaldig(ch))
		{
			getWord[i] = ch;
			i++;
		}
		else if (ch == ' ')//遇到空格加一
		{
			for (int j = 0; j < 5; j++)
			{
				if (strlen(stopWord[j]) == 0)
				{
					strcpy(stopWord[j], getWord);
					break;
				}
			}
			memset(getWord, 0, sizeof(getWord));
			i = 0;
		}
	}
	for (int j = 0; j < 5; j++)
	{
		if (strlen(stopWord[j]) == 0)
		{
			strcpy(stopWord[j], getWord);
			break;
		}
	}
	memset(getWord, 0, sizeof(getWord));
	i = 0;//此时stopword里保存的为停用词
	int j = 0;//计数行
	while ((ch = fgetc(fp)) != EOF)//此函数中与上函数类似，但不计数
	{
		if (isaldig(ch))
		{
			getWord[i] = ch;
			i++;
			lineWord[j] = ch;
			j++;
		}
		else if ((ch == ' '&&isaldig(lineWord[j - 1])) || ch == ',')//忽略连续空格,遇到空格或逗号则单词数加一
		{
			if (strlen(lineWord) != 0)
			{
				//puts(getWord);
				for (int m = 0; m < 5; m++)
				{
					if(strcmp(getWord, stopWord[m])==0)
						wc_stopWord++;
				}
			}
			memset(getWord, 0, sizeof(getWord));
			i = 0;
			lineWord[j] = ch;
			j++;
		}
		else if (ch == '\n')
		{
			if (strlen(getWord) != 0)
			{
				//puts(getWord);
				for (int m = 0; m < 5; m++)
				{
					if (strcmp(getWord, stopWord[m])==0)
						wc_stopWord++;
				}
			}
			memset(getWord, 0, sizeof(getWord));
			i = 0;
			memset(lineWord, 0, sizeof(lineWord));
			j = 0;
		}
		else
		{
			lineWord[j] = ch;
			j++;
		}
	}
	if (strlen(getWord) != 0)
	{
		//puts(getWord);
		for (int m = 0; m < 5; m++)
		{
			if (strcmp(getWord, stopWord[m])==0)
				wc_stopWord++;
		}
	}
	memset(getWord, 0, sizeof(getWord));
	i = 0;
	memset(lineWord, 0, sizeof(lineWord));
	j = 0;
}
int main(int argc, char * argv[])
{
	char ch=' ';
	char filename[80];
	FILE *fp, *fpw,*fps = NULL;
	if (argc == 1)
	{
		printf("missing file name:\nPlease input file name:");
		gets_s(filename);
		fp = fopen(filename, "r");
		fps= fopen("stopList.txt", "r");
		notFound(fp);
		notFound(fps);
		printf("This is the file :\n");
		countAll(ch, fp);
		fseek(fp, 0, SEEK_SET);//重置指针以再进行从头扫描
		countStop(ch,fp, fps);
		printf("The char count is %d\n", wc_char);
		printf("The word count is %d\n", wc_word);
		printf("The finalWord count is %d\n", wc_stopWord);
		printf("The line count is %d\n", wc_line);
		printf("The emptyline count is %d\n", wc_empLine);
		printf("The explainline count is %d\n", wc_expLine);
		printf("The codeline count is %d\n", wc_codeLine);
		fclose(fp);
		fclose(fps);
		_getch();
		exit(0);
	}
	else if (argc == 2)
	{
		fp = fopen(argv[1], "r");
		notFound(fp);
		countAll(ch, fp);
		printf("文件打成功!\n请输入统计方式如(-c\-w\-l):");
		gets_s(filename);
		if (strcmp(filename, "-c") == 0)
		{
			printf("The char count is %d\n", wc_char);
		}
		else if (strcmp(filename, "-w") == 0)
		{
			printf("The word count is %d\n", wc_word);
		}
		else if (strcmp(filename, "-l") == 0)
		{
			printf("The line count is %d\n", wc_line);
		}
	}
	else if (argc == 3)
	{
		fp = fopen(argv[2], "r");
		notFound(fp);
		countAll(ch, fp);
		if (strcmp(argv[1], "-c") == 0)
		{
			printf("%s,字符数:%d\n", argv[2],wc_char);
		}
		else if (strcmp(argv[1], "-w") == 0)
		{
			printf("%s,单词数:%d\n", argv[2], wc_word);
		}
		else if (strcmp(argv[1], "-l") == 0)
		{
			printf("%s,行数:%d\n", argv[2], wc_line);
		}
		else if (strcmp(argv[1], "-a") == 0)
		{
			printf("%s,代码行/空行/注释行:%d/%d/%d\n", argv[2], wc_codeLine,wc_empLine,wc_expLine);
		}
		else if (strcmp(argv[1], "-o") == 0)
		{
			fpw = fopen("outputFile.txt", "w");
			notFound(fpw);
			fprintf(fpw,"%s,代码行/空行/注释行:%d/%d/%d\n", argv[2], wc_codeLine, wc_empLine, wc_expLine);
			fclose(fpw);
		}
		fclose(fp);
	}
	else if (argc >= 3)
	{
		int flag[7] = { 0 };
		char otext[200];//输出文本
		fp = fopen("file.c", "r");
		notFound(fp);
		countAll(ch, fp);
		for (int i = 0; i < argc; i++)
		{
			if (strcmp(argv[i], "-c") == 0)
			{
				flag[0] = 1;
			}
			else if (strcmp(argv[i], "-w") == 0)
			{
				flag[1] = 1;
			}
			else if (strcmp(argv[i], "-l") == 0)
			{
				flag[2] = 1;
			}
			else if (strcmp(argv[i], "-a") == 0)
			{
				flag[5] = 1;
			}
			else if (strcmp(argv[i], "-o") == 0)
			{
				flag[3] = 1;
			}
		}
		if (flag[3] == 1)
		{
			fpw = fopen("outputFile.txt", "w");
			notFound(fpw);
		}
		if (flag[0] == 1)
		{
			printf("file.c,字符数:%d\n", wc_char);
			if (flag[3] == 1)
			{
				fprintf(fpw, "file.c,字符数:%d\n", wc_char);
			}
		}
		if (flag[1] == 1)
		{
			printf("file.c,单词数:%d\n", wc_word);
			if (flag[3] == 1)
			{
				fprintf(fpw, "file.c,单词数:%d\n", wc_word);
			}
		}
		if (flag[2] == 1)
		{
			printf("file.c,行数数:%d\n", wc_line);
			if (flag[3] == 1)
			{
				fprintf(fpw, "file.c,行数数:%d\n", wc_line);
			}
		}
		if (flag[5] == 1)
		{
			printf("file.c,代码行/空行/注释行:%d/%d/%d\n", wc_codeLine, wc_empLine, wc_expLine);
			if (flag[3] == 1)
			{
				fprintf(fpw, "file.c,代码行/空行/注释行:%d/%d/%d\n", wc_codeLine, wc_empLine, wc_expLine);
			}
		}
		fclose(fp);
		if(flag[3]==1)
			fclose(fpw);
	}
}