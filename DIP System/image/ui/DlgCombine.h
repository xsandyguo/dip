#pragma once


#include "DlgBase.h"

class CImageDocument;
/////////////////////////////////////////////////////////////////////////////
// DlgCombine dialog
class DlgCombine : public DialogBase
{
// Construction
public:
	CImageDocument* pDocR;
	CImageDocument* pDocG;
	CImageDocument* pDocB;
	CImageDocument* pDocA;
	long	pChR;
	long	pChG;
	long	pChB;
	long	pChA;
	long	pChS;

	DlgCombine(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgCombine)
	enum { IDD = IDD_COMBINE };
	CComboBox	m_chs;
	CComboBox	m_cha;
	CComboBox	m_chb;
	CComboBox	m_chg;
	CComboBox	m_chr;
	CComboBox	m_imga;
	CComboBox	m_imgb;
	CComboBox	m_imgg;
	CComboBox	m_imgr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgCombine)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgCombine)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
