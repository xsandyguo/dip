#pragma once
#include "DlgBase.h"

class DlgIncBpp : public DialogBase
{
// Construction
public:
	BYTE m_bit;
	DlgIncBpp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgIncBpp)
	enum { IDD = IDD_INCBPP };
	CButton	m_r4;
	CButton	m_r24;
	CButton	m_r8;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgIncBpp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgIncBpp)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
