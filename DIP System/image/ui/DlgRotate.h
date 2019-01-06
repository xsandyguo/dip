#pragma once

#include "DlgBase.h"

class DlgRotate : public DialogBase
{
// Construction
public:
	long m_method;
	long m_overflow;
	BYTE m_keepsize;
	DlgRotate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgRotate)
	enum { IDD = IDD_ROTATE };
	CButton	m_ch1;
	float	m_angle;
	CComboBox inMethod;
	CComboBox ofMethod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgRotate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgRotate)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};