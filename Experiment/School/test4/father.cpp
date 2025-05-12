#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <iostream>

using namespace std;

BOOL writePipe(HANDLE hwrite) {
  BOOL isSucess = FALSE;
  char *str = "这是从父进程写入管道的内容。\n";
  DWORD dwWrite;
  while(1){
    isSucess = WriteFile(hwrite, str, strlen(str), &dwWrite, NULL);
    if(isSucess) {
      break;
    }
  }
  CloseHandle(hwrite) ;
  return isSucess;
}

BOOL readPipe(HANDLE hRead){
  BOOL isSucess = FALSE;
  char str[100];
  DWORD dwRead;
  while(1) {
    isSucess = ReadFile(hRead, str, 43, &dwRead, NULL);
    if(isSucess) {
      break;
    }
  }
  CloseHandle(hRead);
  cout << "从管道中读取的内容如下\n" << str <<endl;
  return isSucess;
}

int main() {
  SECURITY_ATTRIBUTES sa;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle = TRUE;
  sa.lpSecurityDescriptor = NULL;

  STARTUPINFO sui;
  ZeroMemory(&sui, sizeof(STARTUPINFO));
  sui.cb = sizeof(STARTUPINFO);
  sui.dwFlags = STARTF_USESTDHANDLES;

  HANDLE hParentWrite, hParentRead, hChildRead, hChildWrite;
  if(!CreatePipe(&hChildRead, &hParentWrite, &sa, 0)) {
    cout << "创建管道失败\n" << endl;
  }
  if(!CreatePipe(&hParentRead, &hChildWrite, &sa, 0)) {
    cout << "创建管道失败\n" << endl;
  }
  
  sui.hStdOutput = hChildWrite;
  sui.hStdInput = hChildRead;

  PROCESS_INFORMATION pi;
  ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
  BOOL isSucess = FALSE;

  isSucess = CreateProcess(".\\child.exe", NULL, NULL, NULL, TRUE, 0, NULL, NULL, &sui, &pi);
  if(!isSucess) {
    cout << "子进程创建失败\n" << endl;
  } else {
    cout << "子进程创建成功\n" << endl;
    if(writePipe(hParentWrite)) {
      cout << "数据写入管道成功\n" << endl;
    }
    if(readPipe(hParentRead)){
      cout << "从管道中读取数据成功\n" << endl;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(hParentRead);
    CloseHandle(hParentWrite);
    CloseHandle(hChildRead);
    CloseHandle(hChildWrite);
  }

  return 0;
}