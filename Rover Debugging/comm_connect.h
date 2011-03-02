#pragma once
#include "afxwin.h"


// Ccomm_connect dialog

class Ccomm_connect : public CDialog
{
private:
	int com_connected;

public:
	
	DECLARE_DYNAMIC(Ccomm_connect)

public:
	Ccomm_connect(CWnd* pParent = NULL);   // standard constructor
	virtual ~Ccomm_connect();

// Dialog Data
	enum { IDD = IDD_COM_CONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void CloseDialog();
	CComboBox m_com_port_num;
	afx_msg void OnBnClickedComConnect();
	afx_msg void OnBnClickedComDisconnect();
};
