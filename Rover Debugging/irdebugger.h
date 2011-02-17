#pragma once
#include "afxwin.h"


// Cirdebugger dialog

#define IR_READ_TIMER	1101

class Cirdebugger : public CDialog
{	
	DECLARE_DYNAMIC(Cirdebugger)

public:
	Cirdebugger(CWnd* pParent = NULL);   // standard constructor
	virtual ~Cirdebugger();

// dialog members
	CFile f;
	int file_open;

// Dialog Data
	enum { IDD = IDD_IR_DEBUGGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIrCaptureStart();
	afx_msg void OnBnClickedIrCaptureEnd();
	afx_msg void OnClose();
	CButton m_running;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
