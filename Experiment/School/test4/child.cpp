#include <stdio.h>
#include <string.h>
#include <windows.h>

using namespace std;

FILE *fp;

BOOL writePipe(HANDLE hWrite) {
	BOOL isSucess = FALSE;

	char *str = "zcyy\n";

	DWORD dwWrite;
	while (1) {
		isSucess = WriteFile(hWrite, str, strlen(str), &dwWrite, NULL);
		if (isSucess) {
			break;
		}
	}
	CloseHandle(hWrite);
	return isSucess;
}

BOOL readPipe(HANDLE hRead) {
	BOOL isSucess = FALSE;
	char str[100];
	DWORD dwRead;
	while (1) {
		isSucess = ReadFile(hRead, str, 43, &dwRead, NULL);
		if (isSucess) {
			break;
		}
	}
	CloseHandle(hRead);
	if (fp) {
		fwrite(str, strlen(str), 1, fp);
	}

	return isSucess;
}

int main() {
	fp = fopen("child.txt", "w");
	char *str = "******子进程开始执行******\n";
	if(fp) {
		fwrite(str, strlen(str), 1, fp);
	}
	HANDLE hRead = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hwrite = GetStdHandle(STD_OUTPUT_HANDLE);

	if (readPipe(hRead)) {
		str = "子程序读取内容结束\n";
		fwrite(str, strlen(str), 1, fp);
	}

	if (writePipe(hwrite)) {
		str = "子进程写入管道成功\n";
		fwrite(str, strlen(str), 1, fp);
	}
	str = "******子进程执行结束******\n";
	fwrite(str, strlen(str), 1, fp);
	fclose(fp);
	return 0;
}

