// irdebugger.cpp : implementation file
//

#include "stdafx.h"
#include "Rover Debugging.h"
#include "golab.h"
#include "irdebugger.h"
#include "com port.h"

// global object
extern CComport _com;

// Cirdebugger dialog

IMPLEMENT_DYNAMIC(Cirdebugger, CDialog)

Cirdebugger::Cirdebugger(CWnd* pParent /*=NULL*/)
	: CDialog(Cirdebugger::IDD, pParent)
{
	file_open = -1;
	records_to_capture = -1;
}

Cirdebugger::~Cirdebugger()
{
}

void Cirdebugger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IR_CAPTURE_RUNNING, m_running);
	DDX_Control(pDX, IDC_UNIQUE_FILE_ID, m_uniquefilename);
	DDX_Control(pDX, IDC_RECORD_COUNT, m_records_to_capture);
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
	
	// this is the default directory to hit any users desktop
	CString fname(L"C:\\ProgramData\\Desktop\\a_d_log.csv");
	// dont continue if the file is already open
	if (file_open == 1){
		return;
	}
	// add to the file name if something is there
	if (m_uniquefilename.GetWindowTextLengthW() > 0){
		CString tmp;
		m_uniquefilename.GetWindowTextW(tmp);
		fname.Empty();
		fname.Insert(0, L"C:\\ProgramData\\Desktop\\");
		fname.Append(tmp);
		fname.Append(L" (a_d_log).csv");
	}
	// lets try to open the file
	if ( !f.Open(fname, CFile::modeCreate | CFile::modeWrite, NULL) ){
		MessageBox(L" File creation error.", L"", NULL);
		return;
	}
	// amount of records has been requested
	if (m_records_to_capture.GetWindowTextLengthW() != 0){
		CString tmp_rec;
		m_records_to_capture.GetWindowTextW(tmp_rec);
		records_to_capture = _wtoi(tmp_rec);
	}
	// nothing requested by the user
	else{
		records_to_capture = -1;
	}
	// success - start the read timeout
	SetTimer(IR_READ_TIMER, 100, NULL);
	// indicate that the file is being created
	m_running.SetCheck(1);
	// stop all future requests to open.
	file_open = 1;
}

void Cirdebugger::OnBnClickedIrCaptureEnd()
{
	// TODO: Add your control notification handler code here
	// only proceed if the file is open
	if (file_open != 1){
		return;
	}
	// stop the timer from running
	KillTimer(IR_READ_TIMER);
	// write any remaining data to the file
	//WriteToFile();
	// clear the running check
	m_running.SetCheck(0);
	// invaidate the file_open 
	file_open = -1;
	// close the file
	f.Close();
}

void Cirdebugger::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	// if the file is open - close it
	if (file_open == 1){
		// close the file
		f.Close();
		// invalidate the flag
		file_open = -1;
		// clear the running check
		m_running.SetCheck(0);
		// kill the timer
		KillTimer(IR_READ_TIMER);
	}
	// VS placement
	CDialog::OnClose();
}

void Cirdebugger::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	// Only look at events that we are aware of
	if (nIDEvent == IR_READ_TIMER){
		// stop the timer
		KillTimer(IR_READ_TIMER);
		// read com data and format for writting
		if (WriteToFile()){
			// start up the timer again
			SetTimer(IR_READ_TIMER, 100, NULL);
		}
	}
	// VS placement
	CDialog::OnTimer(nIDEvent);
}

bool Cirdebugger::WriteToFile()
{
	char* buf = (char*)calloc(4096, sizeof(char));
	char* parse_buf = (char*)calloc(1024, sizeof(char));
	char* buf_base = buf;
	char* parse_base = parse_buf;

	int records_written = 0;
	
	// Since there is no implementation of saving context when the last 
	// read is a partial packet - we will drop some data.  Not often but maybe 
	// 1 in 250 packets.  Speeding up the data transfer will help eliminate this
	// otherwise a static buffer needs to be implemented and appended to on re-entry
	if (_com.Read(buf) > 0){
		parse_buf = strtok(buf, ">");
		while (parse_buf != NULL){
			// make sure the correct data is being sent
			while (*parse_buf != '<'){
				parse_buf++;
				if (*parse_buf == NULL){
					break;
				}
			}
			if (strncmp(parse_buf, "<a/d - ", strlen("<a/d - ")) == 0){
				parse_buf += strlen("<a/d - ");
				f.Write(parse_buf, strlen(parse_buf));
				f.Write("\n", strlen("\n"));
				parse_buf -= strlen("<a/d - ");
				records_written++;
			}
			// get another
			parse_buf = strtok(NULL, ">");
		}
	}
	
	// Update the console of what is happening
	buf = buf_base;
	CString s;
	m_records_to_capture.GetWindowTextW(s);
	int res = _wtoi(s.GetString());
	res += records_written;
	itoa(res, buf, 10);
	s.Empty();
	CString ss(buf);
	m_records_to_capture.SetWindowTextW(ss);

	free(buf_base);
	free(parse_base);
	return true;
}
