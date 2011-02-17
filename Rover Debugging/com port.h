#ifndef COM_PORT_H
#define COM_PORT_H

// Windows Header Files:
#include <windows.h>
#include <commctrl.h>
// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

class CComport
{
private:
	HANDLE hComm;
	wchar_t com_name[64];

public:
	CComport();
	int IsAvailable(int);
	int IsOpen();
	int Open(int);
	int Close();
	void Write(unsigned char);
	int Write(wchar_t*);
	int Write(char*);
	unsigned char ReadSingle();
	int Read(wchar_t*);
	int Read(char*);
};

#endif


