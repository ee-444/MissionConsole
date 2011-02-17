// irdebugger.cpp : implementation file
//

#include "stdafx.h"
#include "Rover Debugging.h"
#include "golab.h"
#include "irdebugger.h"


// Cirdebugger dialog

IMPLEMENT_DYNAMIC(Cirdebugger, CDialog)

Cirdebugger::Cirdebugger(CWnd* pParent /*=NULL*/)
	: CDialog(Cirdebugger::IDD, pParent)
{
	file_open = -1;
}

Cirdebugger::~Cirdebugger()
{
}

void Cirdebugger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IR_CAPTURE_RUNNING, m_running);
}


BEGIN_MESSAGE_MAP(Cirdebugger, CDialog)
	ON_BN_CLICKED(IDC_IR_CAPTURE_START, &Cirdebugger::OnBnClickedIrCaptureStart)
	ON_BN_CLICKED(IDC_IR_CAPTURE_END, &Cirdebugger::OnBnClickedIrCaptureEnd)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Cirdebugger message handlers

void Cirdebugger::OnBnClickedIrCaptureStart()
{
	// TODO: Add your control notification handler code here
	
	CString fname;
	wchar_t* tmp_buf = (wchar_t*)calloc(236, sizeof(wchar_t));
	wchar_t* base_address = tmp_buf;

	if (tmp_buf == NULL){
		// calloc failed
		MessageBox(L" Memory allocation error", L"", NULL);
		return;
	}
	if (GetTime(tmp_buf) == 0){
		// calloc failed
		MessageBox(L" Memory allocation error", L"", NULL);
		return;
	}

	if (file_open == 1){
		return;
	}
	
	if ( !f.Open(L"a_d_log.csv", CFile::modeCreate | CFile::modeWrite, NULL) ){
		MessageBox(L" File creation error.", L"", NULL);
		return;
	}
	SetTimer(IR_READ_TIMER, 100, NULL);
	m_running.SetCheck(1);
	file_open = 1;
}

void Cirdebugger::OnBnClickedIrCaptureEnd()
{
	// TODO: Add your control notification handler code here
	if (file_open != 1){
		return;
	}
	KillTimer(IR_READ_TIMER);
	m_running.SetCheck(0);
	file_open = -1;
	f.Close();
}

void Cirdebugger::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	if (file_open == 1){
		f.Close();
		file_open = -1;
		m_running.SetCheck(0);
		KillTimer(IR_READ_TIMER);
	}
	CDialog::OnClose();
}

void Cirdebugger::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == IR_READ_TIMER){
	
		// read com data and format for writting
	
	}

	CDialog::OnTimer(nIDEvent);
}
