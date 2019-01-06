#pragma once

#include "DlgBase.h"

class DlgShadow : public DialogBase
{
// Construction
public:
	DlgShadow(CWnd* pParent = NULL);   // standard constructor

	COLORREF m_shadow;
	COLORREF m_bkg;

// Dialog Data
	//{{AFX_DATA(DlgShadow)
	enum { IDD = IDD_SHADOW };
	CxTargetButton	m_btnBkg;
	CxTargetButton	m_btnShadow;
	long	m_x;
	long	m_y;
	BYTE	m_intensity;
	BOOL	m_relative;
	BYTE	m_radius;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgShadow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgShadow)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
