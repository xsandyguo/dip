#pragma once

#include "DlgBase.h"

/////////////////////////////////////////////////////////////////////////////
// DlgOpacity dialog
class DlgOpacity : public DialogBase
{
// Construction
public:
	DlgOpacity(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgOpacity)
	enum { IDD = IDD_OPACITY };
	BYTE	m_level;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgOpacity)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgOpacity)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
