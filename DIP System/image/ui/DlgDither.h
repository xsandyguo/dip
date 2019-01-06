#pragma once

#include "image/ui/DlgBase.h"

class DlgDither : public DialogBase
{
// Construction
public:
	BYTE m_method;
	DlgDither(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgDither)
	enum { IDD = IDD_DITHER };
	CButton	m_r8;
	CButton	m_r7;
	CButton	m_r6;
	CButton	m_r5;
	CButton	m_r4;
	CButton	m_r3;
	CButton	m_r2;
	CButton	m_r1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgDither)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgDither)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
