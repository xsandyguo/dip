#pragma once

#include "DlgBase.h"

class DlgExpand : public DialogBase
{
// Construction
public:
	DlgExpand(CWnd* pParent = NULL);   // standard constructor

	COLORREF m_color;
	BYTE	m_Mode;
	BYTE	m_CenterH;
	BYTE	m_CenterV;
	BYTE	m_KeepRatio;
	BYTE	m_UseImageBkg;
	float m_ratio;

// Dialog Data
	//{{AFX_DATA(DlgExpand)
	enum { IDD = IDD_EXPAND };
	CxTargetButton	m_btnSelColor;
	CButton	m_ckKeepRatio;
	CButton	m_ckUseImageBkg;
	CButton	m_ckCenterV;
	CButton	m_ckCenterH;
	long	m_newwidth;
	long	m_newheight;
	long	m_left;
	long	m_right;
	long	m_top;
	long	m_bottom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgExpand)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgExpand)
	afx_msg void OnColorsel();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeEditW();
	afx_msg void OnChangeEditH();
	afx_msg void OnChangeEditL();
	afx_msg void OnChangeEditR();
	afx_msg void OnChangeEditT();
	afx_msg void OnChangeEditB();
	afx_msg void OnCheck8();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
