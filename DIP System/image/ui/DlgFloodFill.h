#pragma once

#include <afxwin.h>

#include "resource.h"
#include "xTargetButton.h"

class CMainFrame;


// DlgFloodFill dialog

class DlgFloodFill : public CDialog
{
// Construction
public:
	BOOL Create();
	DlgFloodFill(CMainFrame* pParent);
	DlgFloodFill(CWnd* pParent = NULL);   // standard constructor

	COLORREF m_color;

// Dialog Data
	//{{AFX_DATA(DlgFloodFill)
	enum { IDD = IDD_FLOOD };
	CxTargetButton	m_btnSelColor;
	BYTE	m_tol;
	BOOL	m_select;
	BYTE	m_opacity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgFloodFill)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMainFrame* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(DlgFloodFill)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
