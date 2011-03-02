// comm_connect.cpp : implementation file
//

#include "stdafx.h"
#include "Rover Debugging.h"
#include "comm_connect.h"
#include "com port.h"

// global object
extern CComport _com;


// Ccomm_connect dialog

IMPLEMENT_DYNAMIC(Ccomm_connect, CDialog)

Ccomm_connect::Ccomm_connect(CWnd* pParent /*=NULL*/)
	: CDialog(Ccomm_connect::IDD, pParent)
{
}

Ccomm_connect::~Ccomm_connect()
{
}

void Ccomm_connect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_NUM, m_com_port_num);


	if (_com.IsOpen() != true){
		char s[24] = {""};
		int write_loc = 0;
		// populate the available system ports
		for(int cnt = 0; cnt<=255; cnt++){
			if (_com.IsAvailable(cnt+1) == 1){
				_itoa_s((cnt+1), s, 10);
				CString ss(s);
				m_com_port_num.InsertString(write_loc++, ss);
			}
		}
	}
	else {
		m_com_port_num.SetWindowTextW(L"In Use!");
	}
}


BEGIN_MESSAGE_MAP(Ccomm_connect, CDialog)
	ON_BN_CLICKED(IDC_COM_CONNECT, &Ccomm_connect::OnBnClickedComConnect)
	ON_BN_CLICKED(IDC_COM_DISCONNECT, &Ccomm_connect::OnBnClickedComDisconnect)
END_MESSAGE_MAP()


// Ccomm_connect message handlers

void Ccomm_connect::OnBnClickedComConnect()
{
	// TODO: Add your control notification handler code here
	wchar_t s[24] = {L""};
	m_com_port_num.GetWindowTextW(s, 10);
	int num = _wtoi(s);
	// if the port is open - dont reopen it
	if (_com.IsOpen() == 1){
		MessageBox(L"COM port already open.", NULL, NULL);
		return;
	}
	// if it isnt open try to open it
	else if (_com.Open(num) == -1){
		MessageBox(L"Didnt open COM port.", NULL, NULL);
	}
	// the open was successful
	else{
		MessageBox(L"Connection to COM successful.", NULL, NULL);
		CloseDialog();
	}
}

void Ccomm_connect::OnBnClickedComDisconnect()
{
	// TODO: Add your control notification handler code here
	if (_com.IsOpen() == 1){
		_com.Close();
		CloseDialog();
	}
}

void Ccomm_connect::CloseDialog()
{
	CWnd* pWnd = FindWindow(NULL, L"COM Connect");
	if(pWnd != NULL){
		// Send close command to the message box window
		::PostMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0);
	}
}
