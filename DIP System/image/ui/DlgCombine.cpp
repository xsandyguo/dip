#include "DlgCombine.h"

#include "image/image_document.h"
DlgCombine::DlgCombine(CWnd* pParent /*=NULL*/)
	: DialogBase(DlgCombine::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgCombine)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgCombine::DoDataExchange(CDataExchange* pDX)
{
	DialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgCombine)
	DDX_Control(pDX, IDC_COMBO9, m_chs);
	DDX_Control(pDX, IDC_COMBO8, m_cha);
	DDX_Control(pDX, IDC_COMBO7, m_chb);
	DDX_Control(pDX, IDC_COMBO6, m_chg);
	DDX_Control(pDX, IDC_COMBO5, m_chr);
	DDX_Control(pDX, IDC_COMBO4, m_imga);
	DDX_Control(pDX, IDC_COMBO3, m_imgb);
	DDX_Control(pDX, IDC_COMBO2, m_imgg);
	DDX_Control(pDX, IDC_COMBO1, m_imgr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgCombine, DialogBase)
	//{{AFX_MSG_MAP(DlgCombine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgCombine message handlers

BOOL DlgCombine::OnInitDialog() 
{
	DialogBase::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	

	m_imga.AddString(_T("(None)"));
	m_imga.SetItemData(m_imga.GetCount()-1, 0);

	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	while (pos != NULL) {
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(pos);
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
		POSITION pos2 = pTemplate->GetFirstDocPosition();
		while (pos2 != NULL) {
			CImageDocument* pDoc = (CImageDocument*) pTemplate->GetNextDoc(pos2);
			ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CImageDocument)));
			CString title = pDoc->GetTitle();
			m_imgr.AddString(title);
			m_imgr.SetItemData(m_imgr.GetCount()-1, (DWORD)pDoc);
			m_imgg.AddString(title);
			m_imgg.SetItemData(m_imgg.GetCount()-1, (DWORD)pDoc);
			m_imgb.AddString(title);
			m_imgb.SetItemData(m_imgb.GetCount()-1, (DWORD)pDoc);
			m_imga.AddString(title);
			m_imga.SetItemData(m_imga.GetCount()-1, (DWORD)pDoc);
		}
	}
	m_imgr.SetCurSel(0);
	m_imgg.SetCurSel(0);
	m_imgb.SetCurSel(0);
	m_imga.SetCurSel(0);
	m_chr.SetCurSel(1);
	m_chg.SetCurSel(2);
	m_chb.SetCurSel(3);
	m_cha.SetCurSel(0);
	m_chs.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgCombine::OnOK() 
{
	pDocR = (CImageDocument*) m_imgr.GetItemData(m_imgr.GetCurSel());
	pDocG = (CImageDocument*) m_imgg.GetItemData(m_imgg.GetCurSel());
	pDocB = (CImageDocument*) m_imgb.GetItemData(m_imgb.GetCurSel());
	pDocA = (CImageDocument*) m_imga.GetItemData(m_imga.GetCurSel());

	pChR = m_chr.GetCurSel();
	pChG = m_chg.GetCurSel();
	pChB = m_chb.GetCurSel();
	pChA = m_cha.GetCurSel();
	pChS = m_chs.GetCurSel();

	DialogBase::OnOK();
}
