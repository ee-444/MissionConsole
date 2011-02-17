/* This implementation of the three com routines
	is for a WIN32 execution environment.
	Replace as necessary for your particular environment. */

#include "com port.h"

CComport::CComport()
{
	// initialize the memer variables
	hComm = INVALID_HANDLE_VALUE;
	memset(com_name, 0x0, 64);
}

int CComport::IsAvailable(int port)
{
	HANDLE tmp_hCom = INVALID_HANDLE_VALUE;
	wchar_t txt[64] = {NULL};
	wsprintf(txt, L"\\\\.\\COM%d", port);
	tmp_hCom = CreateFile(txt, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL);
	// test the handle for port validity
	if (tmp_hCom == INVALID_HANDLE_VALUE){
		return -1;
	}
	// close the port and delete the handle if present
	DeleteFile(txt);
	CloseHandle(tmp_hCom);
	return 1;
}

int CComport::Close()
{
	// close port, handle and set to invalid
	DeleteFile(com_name);
	CloseHandle(hComm);
	hComm = INVALID_HANDLE_VALUE;
	return 1;
}

int CComport::IsOpen()
{
	if (hComm == INVALID_HANDLE_VALUE){
		return -1;
	}
	return 1;
}

int CComport::Open(int num)
{
	COMMTIMEOUTS timeouts;
	DCB          dcb;
	wchar_t port[64] = {NULL};
	// initialise the comm port
	// was TEXT("COM8")
	wsprintf(port, L"%s%d", L"\\\\.\\COM", num);
	hComm = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL);
	// exit as a failure
	if(hComm == INVALID_HANDLE_VALUE)
	    return -1;
	// store the port name for closure
	wmemcpy(com_name, port, wcslen(port));
	// set com port params
	FillMemory(&dcb, sizeof(dcb), 0);
	dcb.DCBlength = sizeof(dcb);
	// change the port identity and parameters
	if(GetCommState(hComm, &dcb)){
		dcb.BaudRate = (CBR_57600);
		dcb.Parity   = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 8;
		SetCommState(hComm, &dcb);
	}
	// set communication timeouts
	timeouts.ReadIntervalTimeout         = 0;
	timeouts.ReadTotalTimeoutMultiplier  = 0;
	timeouts.ReadTotalTimeoutConstant    = 1;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant   = 0;

	SetCommTimeouts(hComm, &timeouts);

	return 1;
}

/* WincomSendByte(char) sends one byte of data to the com port
*/
void CComport::Write(unsigned char data)
{
	// force a write
	DWORD written;
	WriteFile(hComm, &data, 1, &written, NULL);
}

int CComport::Write(wchar_t* data)
{
	// only write a max amoount of 4096 bytes
	int size = wcslen(data);
	if (size > 4095){
		return -1;
	}
	char* small_data = (char*)calloc(4096, sizeof(char));
	char* base_address = small_data;
	// change the variable size
	while(*data != NULL){
		*small_data = (char)*data;
		small_data++;
		data++;
	}
	DWORD written;
	size = strlen(small_data);
	small_data = base_address;
	if (WriteFile(hComm, small_data, size, &written, NULL) == 0){
		free(base_address);
		return -1;
	}
	free(base_address);
	return 1;
}

int CComport::Write(char* data)
{
	// write of the same data size
	int r_loc = strlen(data);
	DWORD written = 0x0;
	if (WriteFile(hComm, data, r_loc, &written, NULL) == 0){
		return -1;
	}
	// indicate success
	return 1;
}

/* unsigned char WincomRcvByte() receives one byte of data from the com port
   it should time out and return 0x00 after 100ms
*/
unsigned char CComport::ReadSingle()
{
	// pull a single byte from the port
	unsigned char tmpchr=0;
	DWORD read = 0;
	ReadFile(hComm, &tmpchr, 1, &read, NULL);
	return tmpchr;
}

int CComport::Read(char* data)
{
	
	// read of the same size as the port
	DWORD read = 0;
	ReadFile(hComm, data, 4096, &read, NULL);
	// make sure the read was completed
	if (read > 0){
		return 1;
	}
	// indicate failure
	return -1;
}

int CComport::Read(wchar_t* data)
{
	// read a max ammount of 4096 from the port
	char* tmp_data = (char*)calloc(4096, sizeof(char));
	char* base_address = tmp_data;
	DWORD read = 0;
	ReadFile(hComm, tmp_data, 4096, &read, NULL);
	// make sure the read was completed
	if (read > 0){
		// copy the size
		while (*tmp_data != 0x00){
			*data = *tmp_data;
			data++;
			tmp_data++;
		}
		free(base_address);
		return 1;
	}
	free(base_address);
	// indicate failure
	return -1;
}


