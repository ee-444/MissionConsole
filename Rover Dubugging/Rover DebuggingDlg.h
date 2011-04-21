
// Rover DebuggingDlg.h : header file
//

#pragma once
#include "afxwin.h"

// Fixes autoclose in about dialog when not using unicode project
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' "	\
"version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") 

#define PARSE_COM_DATA			10
// time in ms
#define PARSE_COM_DATA_TIMEOUT	50


// CRoverDebuggingDlg dialog
class CRoverDebuggingDlg : public CDialog
{
// Construction
public:
	CRoverDebuggingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ROVERDEBUGGING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	void UpdateComData();
	void StartDialogTimers();
	void StopDialogTimers();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIrDebug();
	CButton m_ir_refresh;
	CButton m_ir_meters;
	CEdit m_lrir_display;
	CEdit m_mrir_display;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
