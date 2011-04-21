
// Rover DebuggingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Rover Debugging.h"
#include "Rover DebuggingDlg.h"
#include "comm_connect.h"
#include "com port.h"

// global object
extern CComport _com;

// Dialogs
#include "irdebugger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickLaunchHelp(NMHDR *pNMHDR, LRESULT *pResult);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LAUNCH_HELP, &CAboutDlg::OnNMClickLaunchHelp)
END_MESSAGE_MAP()


// CRoverDebuggingDlg dialog




CRoverDebuggingDlg::CRoverDebuggingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRoverDebuggingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRoverDebuggingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IR_REFRESH, m_ir_refresh);
	DDX_Control(pDX, IDC_IR_METERS, m_ir_meters);
	DDX_Control(pDX, IDC_LRIR_VALUE, m_lrir_display);
	DDX_Control(pDX, IDC_MRIR_VALUE, m_mrir_display);
}

BEGIN_MESSAGE_MAP(CRoverDebuggingDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_IR_DEBUG, &CRoverDebuggingDlg::OnBnClickedIrDebug)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRoverDebuggingDlg message handlers

BOOL CRoverDebuggingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_COM_CONNECT, L"COM connect");
			
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	StartDialogTimers();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRoverDebuggingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == IDM_COM_CONNECT){
		// com connection dialog
		Ccomm_connect dlg;
		// open the dialog if a port is available on the system
		dlg.DoModal();
	}	
	else if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRoverDebuggingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRoverDebuggingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRoverDebuggingDlg::OnBnClickedIrDebug()
{
	// TODO: Add your control notification handler code here
	Cirdebugger dlg;
	
	StopDialogTimers();
	
	// build the dialog
	dlg.DoModal();

	StartDialogTimers();
}

void CRoverDebuggingDlg::StopDialogTimers()
{
	KillTimer(PARSE_COM_DATA);
}

void CRoverDebuggingDlg::StartDialogTimers()
{
	SetTimer(PARSE_COM_DATA, PARSE_COM_DATA_TIMEOUT, NULL);
}

void CRoverDebuggingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == PARSE_COM_DATA){
		// stop the timer in case of a backlog of data
		KillTimer(PARSE_COM_DATA);
		// only parse com data if the port is connected
		if (_com.IsOpen() == 1){
			
			// Update all the dialog windows
			UpdateComData();

		}
		SetTimer(PARSE_COM_DATA, PARSE_COM_DATA_TIMEOUT, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}

void CRoverDebuggingDlg::UpdateComData()
{
	// stored on the stack
	static char left_over[4096] = {NULL};
	// stored on the heap
	char* tmp = (char*)calloc(4096, sizeof(char));
	char* tmp_base = tmp;

	if (tmp == NULL){
		// indicate heap failure
		MessageBeep(0xffffffff);
		return;
	}

	if (strlen(left_over) > 0){
		// copy the leftovers
		memcpy(tmp, left_over, strlen(left_over));
		// offset so the read doesnt clobber the data
		tmp += strlen(left_over);
		// erase the old left overs
		memset(left_over, 0, 4096);
	}

	if (_com.Read(tmp) > 0){
		// left justify the result
		int shift_cnt = 1;
		char* packet = (char*)calloc(128, sizeof(char));
		char* packet_base = packet;

		if (packet == NULL){
			// indicate heap failure
			MessageBeep(0xffffffff);
			// clear the heap
			free(tmp_base);
			return;
		}
		
		// restore tmp to the base address
		tmp = tmp_base;

		while (*tmp != '<'){
			// nothing is here
			if (*tmp == NULL){
				// clear the heap
				free(tmp_base);
				free(packet_base);
				return;
			}
			// shift the contents
			tmp = (tmp+shift_cnt++);
		}
		// process the packets
		do{
			// build a single packet
			while (*tmp != '>'){
				if (*tmp != '\n'){
					*packet = *tmp;
					packet++;
				}

				tmp++;

				if (*tmp == NULL){
					// restore the base pointer address
					packet -= strlen(packet);
					memcpy(left_over, packet, strlen(packet));
					// clear the heap
					free(tmp_base);
					free(packet_base);
					return;
				}
			}

			tmp++;
			packet = packet_base;


			// parse the known complete packets
			if (strncmp(packet, "<l.r.i.r. - ", strlen("<l.r.i.r. - ")) == 0){
				packet += strlen("<l.r.i.r. - ");
				CString s_disp(packet);
				if (m_ir_refresh.GetCheck() > 0){
					m_lrir_display.SetWindowTextW(s_disp);
				}
			}
			else if (strncmp(packet, "<m.r.i.r. - ", strlen("<m.r.i.r. - ")) == 0){
				packet += strlen("<m.r.i.r. - ");
				CString s_disp(packet);
				if (m_ir_refresh.GetCheck() > 0){
					m_mrir_display.SetWindowTextW(s_disp);
				}
			}

			packet = packet_base;
			memset(packet, 0, 128);

		}while(*tmp != NULL);

		free(packet_base);

	}
	if (strlen(tmp) > 0){
		// copy the leftovers
		memcpy(left_over, tmp, strlen(tmp));
	}
	// clear the heap
	free(tmp_base);
}


void CAboutDlg::OnNMClickLaunchHelp(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	PNMLINK pNMLINK = (PNMLINK)pNMHDR;		// create a typecasted object
	// open the link in the OS default browser
	::ShellExecute(m_hWnd, _T("open"), pNMLINK->item.szUrl, NULL, NULL, SW_NORMAL);
	*pResult = 0;	// return result OK
}
