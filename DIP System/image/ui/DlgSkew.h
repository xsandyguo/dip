#pragma once

#include "DlgBase.h"

class DlgSkew : public DialogBase
{
// Construction
public:
	DlgSkew(CWnd* pParent = NULL);   // standard constructor

	DWORD m_w;
	DWORD m_h;

// Dialog Data
	//{{AFX_DATA(DlgSkew)
	enum { IDD = IDD_SKEW };
	BOOL	m_bEnableInterpolation;
	long	m_pivotx;
	long	m_pivoty;
	float	m_skewx;
	float	m_skewy;
	float	m_anglex;
	float	m_angley;
	float	m_slopex;
	float	m_slopey;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgSkew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateMyData();

	// Generated message map functions
	//{{AFX_MSG(DlgSkew)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit4();
	afx_msg void OnChangeEdit5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
