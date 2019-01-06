#include "DlgOpacity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgOpacity dialog


DlgOpacity::DlgOpacity(CWnd* pParent /*=NULL*/)
	: DialogBase(DlgOpacity::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgOpacity)
	m_level = 0;
	//}}AFX_DATA_INIT
}


void DlgOpacity::DoDataExchange(CDataExchange* pDX)
{
	DialogBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_level);
}


BEGIN_MESSAGE_MAP(DlgOpacity, DialogBase)
	//{{AFX_MSG_MAP(DlgOpacity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgOpacity message handlers

BOOL DlgOpacity::OnInitDialog() 
{
	DialogBase::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
