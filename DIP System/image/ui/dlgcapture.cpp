#include "DlgCapture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct _TASK_LIST {
    DWORD       dwProcessId;
    DWORD       dwInheritedFromProcessId;
    BOOL        flags;
    HWND      hwnd;
    TCHAR        ProcessName[MAX_PATH];
    TCHAR        WindowTitle[MAX_PATH];
} TASK_LIST, *PTASK_LIST;

typedef struct _TASK_LIST_ENUM {
    PTASK_LIST  tlist;
    DWORD       numtasks;
} TASK_LIST_ENUM, *PTASK_LIST_ENUM;



BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
    DWORD           i;
	RECT			r;
    TCHAR           buf[MAX_PATH];
    PTASK_LIST_ENUM te = (PTASK_LIST_ENUM)lParam;
    PTASK_LIST      tlist = te->tlist;
    DWORD           numTasks = te->numtasks;

	i=1+tlist[0].dwProcessId;
	// max 255 windows (I think it's enough)
	if (i<255){
		// filter the windows to capture
		if (IsWindowVisible(hwnd) && !IsIconic(hwnd)){
			GetClientRect(hwnd,&r);
			// discard strange windows
			if (r.right>4 && r.bottom>4){
				// get the window caption
				if (GetWindowText(hwnd, buf, MAX_PATH )) {
					tlist[i].hwnd = hwnd;				 // store hwnd
					_tcscpy( tlist[i].WindowTitle, buf ); // store caption
					tlist[0].dwProcessId++;
				}
			}
		}
	}
	return TRUE;
}

// DlgCapture dialog

DlgCapture::DlgCapture(CWnd* pParent /*=NULL*/)
	: DialogBase(DlgCapture::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgCapture)
	//}}AFX_DATA_INIT
}

void DlgCapture::DoDataExchange(CDataExchange* pDX)
{
	DialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(DlgCapture, DialogBase)
	//{{AFX_MSG_MAP(DlgCapture)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// DlgCapture message handlers
BOOL DlgCapture::OnInitDialog() 
{
	DialogBase::OnInitDialog();
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	//structures to contain the task info
    TASK_LIST_ENUM    te;
	TASK_LIST   tlist[256];
	//reserved slot for task enum
    tlist[0].dwProcessId = 0;
	// task list
    te.tlist = tlist;
	// get the windows
    EnumWindows(EnumWindowsProc,(LPARAM)(&te));

	int numtasks=tlist[0].dwProcessId;
	int idx;
	// always add the desktop
	idx = m_list.InsertString(-1,_T("Desktop"));
	m_list.SetItemData(idx,(DWORD)::GetDesktopWindow());
	// fill the list with the other windows
	for(int i=1;i<numtasks;i++){
		idx = m_list.InsertString(-1,tlist[i].WindowTitle);
		m_list.SetItemData(idx,(DWORD)tlist[i].hwnd);
	}
	// default selection
	m_list.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgCapture::OnOK() 
{
	int idx = m_list.GetCurSel();
	m_SelectedWnd=(HWND)m_list.GetItemData(idx);
	m_list.GetText(idx,m_SelectedTitle);
	DialogBase::OnOK();
}

