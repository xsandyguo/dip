#pragma once

#include "DlgBase.h"

/////////////////////////////////////////////////////////////////////////////
// DlgGamma dialog

class DlgGamma : public DialogBase
{
// Construction
public:
	DlgGamma(CWnd* pParent = NULL);   // standard constructor

	BYTE	m_bGammaMode;

// Dialog Data
	//{{AFX_DATA(DlgGamma)
	enum { IDD = IDD_GAMMA };
	CButton	m_rbIndep;
	CButton	m_rbLink;
	float	m_gamma;
	float	m_gammaR;
	float	m_gammaG;
	float	m_gammaB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgGamma)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgGamma)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
