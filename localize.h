#pragma once
#define LOCALIZE
#ifdef _WIN32
#include <windows.h>
#endif
size_t localizedLen(const wchar_t*);
char* localize(const wchar_t*);
void printW(const wchar_t*);





size_t localizedLen(const wchar_t* source) {
	// 注意该函数的返回值包括最后的 '\0' 字符，返回0表示出错。
	// 主要是因为 size_t 是 unsigned long long，所以不能返回 -1。
#ifdef _WIN32
	return (size_t) WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)source, -1, NULL, 0, NULL, NULL);
#else
	// On non-Windows platforms, we can use wcstombs to estimate the length
	return wcstombs(NULL, source, 0)+1;
#endif
}

char* localize(const wchar_t* source) {
#ifdef _WIN32
	size_t converted = 0;
	int destSize = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)source, -1, NULL, 0, NULL, NULL);
	if (destSize == 0) return NULL;
	char* localizedStr = (char*)malloc(sizeof(char) * destSize);
	converted = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)source, -1, localizedStr, destSize, NULL, NULL);
	if (converted == 0) {
		free(localizedStr);
		return NULL;
	}
	else return localizedStr;
#else
	// On non-Windows platforms, we can use wcstombs to convert the string
	size_t converted = 0;
	size_t destSize = wcstombs(NULL, source, 0) + 1;
	if (destSize == 0) return NULL;
	char* localizedStr = (char*)malloc(sizeof(char) * destSize);
	converted = wcstombs(localizedStr, source, destSize);
	if (converted == 0) {
		free(localizedStr);
		return NULL;
	}
	else return localizedStr;
#endif
}

void printW(const wchar_t* wstr) {
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written;
	WriteConsoleW(hConsole, wstr, (DWORD)wcslen(wstr), &written, NULL);
#else
	printf("%ls", wstr);
#endif
}