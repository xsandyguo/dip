#pragma once

#include "DlgBase.h"

class DlgResample : public DialogBase
{
// Construction
public:
	BYTE m_mode;
	BYTE m_sizemode;
	float m_ratio;
	DWORD m_w;
	DWORD m_h;
	BYTE m_bKeepRatio;
	DlgResample(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgResample)
	enum { IDD = IDD_RESAMPLE };
	CEdit	m_ed1;
	CButton	m_ch2;
	CComboBox inMethod;
	CButton	m_r2;
	CButton	m_r1;
	float	m_factor;
	DWORD	m_newheight;
	DWORD	m_newwidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgResample)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgResample)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit3();
	afx_msg void OnChangeEdit1();
	afx_msg void OnCheck2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};