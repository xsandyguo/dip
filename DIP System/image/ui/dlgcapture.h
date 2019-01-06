#pragma once

#include "DlgBase.h"

/////////////////////////////////////////////////////////////////////////////
// DlgCapture dialog
class DlgCapture : public DialogBase
{
// Construction
public:
	CString m_SelectedTitle;
	HWND m_SelectedWnd;
	DlgCapture(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgCapture)
	enum { IDD = IDD_CAPTURE };
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgCapture)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgCapture)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};