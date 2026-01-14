#pragma once
#ifdef _WIN32
#include <windows.h>
#endif
size_t localizedLen(const wchar_t*);
char* localize(const wchar_t*);
void printW(const wchar_t*);





size_t localizedLen(const wchar_t* source) {
#ifdef _WIN32
	return (size_t) WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)source, -1, NULL, 0, NULL, NULL) - 1;
#else
	// On non-Windows platforms, we can use wcstombs to estimate the length
	return wcstombs(NULL, source, 0);
#endif
}

char* localize(const wchar_t* source) {
#ifdef _WIN32
	int destSize = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)source, -1, NULL, 0, NULL, NULL);
	char* localizedStr = (char*)malloc(sizeof(char) * destSize);
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)source, -1, localizedStr, destSize, NULL, NULL);
	return localizedStr;
#else
	// On non-Windows platforms, we can use wcstombs to convert the string
	size_t destSize = wcstombs(NULL, source, 0) + 1;
	char* localizedStr = (char*)malloc(sizeof(char) * destSize);
	wcstombs(localizedStr, source, destSize);
	return localizedStr;
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