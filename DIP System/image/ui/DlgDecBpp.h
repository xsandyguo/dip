#pragma once

#include "DlgBase.h"

class DlgDecBpp : public DialogBase
{
// Construction
public:
	BYTE m_method;
	BYTE m_bit;
	BYTE m_errordiffusion;
	BYTE m_bLimitColors;
	DlgDecBpp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgDecBpp)
	enum { IDD = IDD_DECBPP };
	CButton	m_limitcolors;
	CButton	m_err;
	CButton	m_opt;
	CButton	m_r8;
	CButton	m_r4;
	CButton	m_r1;
	long	m_maxcolors;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgDecBpp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgDecBpp)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
